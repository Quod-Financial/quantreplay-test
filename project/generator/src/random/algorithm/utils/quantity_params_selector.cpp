#include "ih/random/algorithm/utils/quantity_params_selector.hpp"

#include "core/tools/numeric.hpp"
#include "ih/constants.hpp"

namespace Simulator::Generator::Random {

QuantityParamsSelector::QuantityParamsSelector(
    const DataLayer::Listing& instrument) noexcept
    : symbol_{instrument.getSymbol().value_or("none")},
      qty_multiplier_{instrument.getQtyMultiple().value_or(
          Constant::DefaultListingQtyMultiple)},
      instrument_qty_min_{instrument.getQtyMinimum().value_or(
          Constant::DefaultListingQtyMinimum)},
      instrument_qty_max_{instrument.getQtyMaximum().value_or(
          Constant::DefaultListingQtyMaximum)},
      at_least_one_random_aggressive_min_parameter_present_{
          instrument.getRandomAggressiveQtyMinimum().has_value() ||
          instrument.getRandomAggressiveAmtMinimum().has_value()},
      at_least_one_random_aggressive_max_parameter_present_{
          instrument.getRandomAggressiveQtyMaximum().has_value() ||
          instrument.getRandomAggressiveAmtMaximum().has_value()},
      aggressive_amt_min_{instrument.getRandomAggressiveAmtMinimum()},
      aggressive_amt_max_{instrument.getRandomAggressiveAmtMaximum()},
      resting_amt_minimum_{instrument.getRandomAmtMinimum()},
      resting_amt_maximum_{instrument.getRandomAmtMaximum()} {
  init_order_qty_min(resting_qty_min_, instrument.getRandomQtyMinimum());
  init_order_qty_min(aggressive_qty_min_,
                     instrument.getRandomAggressiveQtyMinimum());
  init_order_qty_max(resting_qty_max_, instrument.getRandomQtyMaximum());
  init_order_qty_max(aggressive_qty_max_,
                     instrument.getRandomAggressiveQtyMaximum());
}

auto QuantityParamsSelector::init_order_qty_min(
    Quantity& order_qty_min, const std::optional<double>& random_qty_min)
    -> void {
  if (random_qty_min.has_value() && *random_qty_min >= instrument_qty_min_) {
    order_qty_min = Quantity{.value = *random_qty_min,
                             .source = Quantity::Source::Random_Qty};
  } else {
    order_qty_min = Quantity{.value = instrument_qty_min_,
                             .source = Quantity::Source::Instrument_Qty};
  }
}

auto QuantityParamsSelector::init_order_qty_max(
    Quantity& order_qty_max, const std::optional<double>& random_qty_max)
    -> void {
  if (random_qty_max.has_value() && *random_qty_max <= instrument_qty_max_) {
    order_qty_max = Quantity{.value = *random_qty_max,
                             .source = Quantity::Source::Random_Qty};
  } else {
    order_qty_max = Quantity{.value = instrument_qty_max_,
                             .source = Quantity::Source::Instrument_Qty};
  }
}

auto QuantityParamsSelector::isZero(double value) noexcept -> bool {
  return simulator::core::equal(value, 0.);
}

}  // namespace Simulator::Generator::Random