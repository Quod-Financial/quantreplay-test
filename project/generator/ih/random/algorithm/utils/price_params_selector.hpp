#ifndef SIMULATOR_GENERATOR_SRC_ALGORITHM_UTILS_PRICE_PARAMS_SELECTOR_HPP_
#define SIMULATOR_GENERATOR_SRC_ALGORITHM_UTILS_PRICE_PARAMS_SELECTOR_HPP_

#include "data_layer/api/models/listing.hpp"

#include "ih/tracing/tracing.hpp"
#include "ih/random/values/price_generation_params.hpp"
#include "ih/constants.hpp"

namespace Simulator::Generator::Random {

class PriceParamsSelector
{
public:

    template<typename Tracer = Trace::NullTracer>
    static PriceGenerationParams select(
            DataLayer::Listing const & _instrument
        ,   Tracer && _tracer = Tracer {}
    );
};


template<typename Tracer>
PriceGenerationParams PriceParamsSelector::select(
        DataLayer::Listing const & _instrument
    ,   Tracer && _tracer
)
{
    using namespace Trace;

    PriceGenerationParams::Builder paramsBuilder {};
    auto step = makeStep(_tracer, "selecting params for price generation");

    auto const & instrumentSymbol = _instrument.getSymbol();
    traceInput(
        step, makeValue("instrumentSymbol", instrumentSymbol.value_or("none")));

    auto const pxTickRange = _instrument.getRandomTickRange().value_or(
        Constant::DefaultListingTickRange);
    paramsBuilder.setPriceTickRange(pxTickRange);
    traceOutput(step, makeValue("randomTickRange", pxTickRange));

    auto const pxTickSize = _instrument.getPriceTickSize().value_or(
        Constant::DefaultListingPriceTickSize);
    paramsBuilder.setPriceTickSize(pxTickSize);
    traceOutput(step, makeValue("priceTickSize", pxTickSize));

    auto const pxSpread = _instrument.getRandomOrdersSpread().value_or(pxTickSize);
    paramsBuilder.setPriceSpread(pxSpread);

    traceOutput(step, makeValue("priceSpread", pxSpread));
    return PriceGenerationParams { paramsBuilder };
}

} // namespace Simulator::Generator

#endif // SIMULATOR_GENERATOR_SRC_ALGORITHM_UTILS_PRICE_PARAMS_SELECTOR_HPP_
