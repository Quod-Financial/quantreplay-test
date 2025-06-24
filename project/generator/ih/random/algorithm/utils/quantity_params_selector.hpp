#ifndef SIMULATOR_GENERATOR_SRC_ALGORITHM_UTILS_QUANTITY_PARAMS_SELECTOR_HPP_
#define SIMULATOR_GENERATOR_SRC_ALGORITHM_UTILS_QUANTITY_PARAMS_SELECTOR_HPP_

#include <string_view>

#include "data_layer/api/models/listing.hpp"
#include "ih/random/values/event.hpp"
#include "ih/random/values/quantity_generation_params.hpp"
#include "ih/tracing/tracing.hpp"

namespace Simulator::Generator::Random {

class QuantityParamsSelector {
 public:
  explicit QuantityParamsSelector(
      const DataLayer::Listing& instrument) noexcept;

  QuantityParamsSelector(const QuantityParamsSelector&) = delete;
  auto operator=(const QuantityParamsSelector&)
      -> QuantityParamsSelector& = delete;
  QuantityParamsSelector(QuantityParamsSelector&&) = delete;
  auto operator=(QuantityParamsSelector&&) -> QuantityParamsSelector& = delete;

  template <typename Tracer = Trace::NullTracer>
  auto select(double price, Event event, Tracer&& tracer = Tracer{})
      -> QuantityGenerationParams;

 private:
  struct Quantity {
    enum class Source : std::uint8_t { Instrument_Qty, Random_Qty };

    double value;
    Source source;
  };

  auto init_order_qty_min(Quantity& order_qty_min,
                          const std::optional<double>& random_qty_min) -> void;
  auto init_order_qty_max(Quantity& order_qty_max,
                          const std::optional<double>& random_qty_max) -> void;

  template <typename StepTracer>
  auto selectQtyMultiplier(StepTracer&& step_tracer) -> double;

  template <typename StepTracer>
  auto selectMinQty(double price, Event event, StepTracer&& step_tracer)
      -> double;

  template <typename StepTracer>
  auto selectRestingMinQty(double price, StepTracer&& step_tracer) -> double;

  template <typename StepTracer>
  auto selectAggressiveMinQty(double price, StepTracer&& step_tracer) -> double;

  template <typename StepTracer>
  auto selectMaxQty(double price, Event event, StepTracer&& step_tracer)
      -> double;

  template <typename StepTracer>
  auto selectRestingMaxQty(double price, StepTracer&& step_tracer) -> double;

  template <typename StepTracer>
  auto selectAggressiveMaxQty(double price, StepTracer&& step_tracer) -> double;

  static auto isZero(double value) noexcept -> bool;

  std::string symbol_;
  const double qty_multiplier_;
  const double instrument_qty_min_;
  const double instrument_qty_max_;

  bool at_least_one_random_aggressive_min_parameter_present_;
  bool at_least_one_random_aggressive_max_parameter_present_;
  Quantity aggressive_qty_min_;
  Quantity aggressive_qty_max_;
  const std::optional<double> aggressive_amt_min_;
  const std::optional<double> aggressive_amt_max_;

  Quantity resting_qty_min_;
  Quantity resting_qty_max_;
  const std::optional<double> resting_amt_minimum_;
  const std::optional<double> resting_amt_maximum_;
};

template <typename Tracer>
auto QuantityParamsSelector::select(double price, Event event, Tracer&& tracer)
    -> QuantityGenerationParams {
  using namespace Trace;

  QuantityGenerationParams::Builder paramsBuilder;
  auto step = makeStep(tracer, "selecting params for quantity generation");
  traceInput(step, makeValue("instrument_symbol", symbol_));

  paramsBuilder.setQuantityMultiplier(selectQtyMultiplier(step));
  paramsBuilder.setMinimalQuantity(selectMinQty(price, event, step));
  paramsBuilder.setMaximalQuantity(selectMaxQty(price, event, step));

  traceStep(std::move(step), tracer);
  return QuantityGenerationParams{paramsBuilder};
}

template <typename StepTracer>
auto QuantityParamsSelector::selectQtyMultiplier(StepTracer&& step_tracer)
    -> double {
  Trace::traceOutput(
      step_tracer,
      Trace::makeCommentedValue(
          "qty_multiple",
          qty_multiplier_,
          "quantity has been selected as instrument minimal quantity"));
  return qty_multiplier_;
}

template <typename StepTracer>
auto QuantityParamsSelector::selectMinQty(const double price,
                                          Event event,
                                          StepTracer&& step_tracer) -> double {
  if (event.isAggressiveOrderEvent() &&
      at_least_one_random_aggressive_min_parameter_present_) {
    return selectAggressiveMinQty(price, step_tracer);
  }
  return selectRestingMinQty(price, step_tracer);
}

template <typename StepTracer>
auto QuantityParamsSelector::selectAggressiveMinQty(double price,
                                                    StepTracer&& step_tracer)
    -> double {
  using namespace Trace;

  const bool selected_random_qty =
      aggressive_qty_min_.source == Quantity::Source::Random_Qty;
  double min_qty = aggressive_qty_min_.value;

  bool selected_random_amount = false;
  if (aggressive_amt_min_.has_value() && !isZero(price)) {
    const double min_amt_qty = *aggressive_amt_min_ / price;
    selected_random_amount = min_amt_qty >= min_qty;
    min_qty = selected_random_amount ? min_amt_qty : min_qty;
  }

  std::string_view traceComment{
      "quantity has been selected as instrument minimal quantity"};
  if (selected_random_amount) {
    traceComment = std::string_view{
        "quantity has been selected "
        "based on RandomAggressiveAmountMinimum value"};
  } else if (selected_random_qty) {
    traceComment = std::string_view{
        "quantity has been selected "
        "based on RandomAggressiveQtyMinimum value"};
  }

  traceOutput(step_tracer,
              makeCommentedValue("min_qty", min_qty, traceComment));

  return min_qty;
}

template <typename StepTracer>
auto QuantityParamsSelector::selectRestingMinQty(double price,
                                                 StepTracer&& step_tracer)
    -> double {
  using namespace Trace;

  traceInput(step_tracer, makeValue("instrument_qty_min", instrument_qty_min_));

  double min_qty = resting_qty_min_.value;

  bool selected_random_amount = false;
  if (resting_amt_minimum_ && !isZero(price)) {
    const double min_amt_qty = *resting_amt_minimum_ / price;
    selected_random_amount = min_amt_qty >= min_qty;
    min_qty = selected_random_amount ? min_amt_qty : min_qty;
  }

  std::string_view traceComment{
      "quantity has been selected as instrument minimal quantity"};
  if (selected_random_amount) {
    // This has to be checked firstly as
    // min amount qty can override min random qty
    traceComment = std::string_view{
        "quantity has been selected "
        "based on RandomOrdersAmountMinimum value"};
  } else if (resting_qty_min_.source == Quantity::Source::Random_Qty) {
    traceComment = std::string_view{
        "quantity has been selected "
        "based on RandomQtyMinimum value"};
  }

  traceOutput(step_tracer,
              makeCommentedValue("min_qty", min_qty, traceComment));
  return min_qty;
}

template <typename StepTracer>
auto QuantityParamsSelector::selectMaxQty(double price,
                                          Event event,
                                          StepTracer&& step_tracer) -> double {
  if (event.isAggressiveOrderEvent() &&
      at_least_one_random_aggressive_max_parameter_present_) {
    return selectAggressiveMaxQty(price, step_tracer);
  }
  return selectRestingMaxQty(price, step_tracer);
}

template <typename StepTracer>
auto QuantityParamsSelector::selectAggressiveMaxQty(double price,
                                                    StepTracer&& step_tracer)
    -> double {
  using namespace Trace;

  const bool selected_random_qty =
      aggressive_qty_max_.source == Quantity::Source::Random_Qty;
  double max_qty = aggressive_qty_max_.value;

  bool selected_random_amount = false;
  if (aggressive_amt_max_.has_value() && !isZero(price)) {
    const double max_amt_qty = *aggressive_amt_max_ / price;
    selected_random_amount = max_amt_qty <= max_qty;
    max_qty = selected_random_amount ? max_amt_qty : max_qty;
  }

  std::string_view traceComment{
      "quantity has been selected as instrument maximal quantity"};
  if (selected_random_amount) {
    traceComment = std::string_view{
        "quantity has been selected "
        "based on RandomAggressiveAmountMaximum value"};
  } else if (selected_random_qty) {
    traceComment = std::string_view{
        "quantity has been selected "
        "based on RandomAggressiveQtyMaximum value"};
  }

  traceOutput(step_tracer,
              makeCommentedValue("max_qty", max_qty, traceComment));

  return max_qty;
}

template <typename StepTracer>
auto QuantityParamsSelector::selectRestingMaxQty(double price,
                                                 StepTracer&& step_tracer)
    -> double {
  using namespace Trace;

  traceInput(step_tracer, makeValue("instrument_qty_max", instrument_qty_max_));

  double max_qty = resting_qty_max_.value;

  bool selected_random_amount = false;
  if (resting_amt_maximum_ && !isZero(price)) {
    const double maxAmtQty = *resting_amt_maximum_ / price;
    selected_random_amount = maxAmtQty <= max_qty;
    max_qty = selected_random_amount ? maxAmtQty : max_qty;
  }

  std::string_view traceComment{
      "quantity has been selected as instrument maximal quantity"};

  if (selected_random_amount) {
    // This has to be checked firstly as max amount qty
    // can override max random qty
    traceComment = std::string_view{
        "quantity has been selected "
        "based on RandomOrdersAmountMaximum value"};
  } else if (resting_qty_max_.source == Quantity::Source::Random_Qty) {
    traceComment = std::string_view{
        "quantity has been selected "
        "based on RandomQtyMaximum value"};
  }

  traceOutput(step_tracer,
              makeCommentedValue("max_qty", max_qty, traceComment));
  return max_qty;
}

}  // namespace Simulator::Generator::Random

#endif  // SIMULATOR_GENERATOR_SRC_ALGORITHM_UTILS_QUANTITY_PARAMS_SELECTOR_HPP_
