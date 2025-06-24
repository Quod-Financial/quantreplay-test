#ifndef SIMULATOR_GENERATOR_SRC_ALGORITHM_UTILS_MAX_MKTDEPTH_SELECTOR_HPP_
#define SIMULATOR_GENERATOR_SRC_ALGORITHM_UTILS_MAX_MKTDEPTH_SELECTOR_HPP_

#include <cstdint>
#include <optional>

#include "data_layer/api/models/listing.hpp"

#include "ih/tracing/tracing.hpp"

namespace Simulator::Generator::Random {

class MaxMktDepthSelector
{
public:

    template<typename Tracer = Trace::NullTracer>
    static std::optional<std::uint32_t> select(
            DataLayer::Listing const & _instrument
        ,   std::uint32_t _partiesCount
        ,   Tracer && _tracer = Tracer {}
    );
};


template<typename Tracer>
std::optional<std::uint32_t> MaxMktDepthSelector::select(
        DataLayer::Listing const & _instrument
    ,   std::uint32_t _partiesCount
    ,   Tracer && _tracer
)
{
    using namespace Trace;
    auto step = makeStep(_tracer, "selecting maximal instrument depth value");

    auto const & instrumentSymbol = _instrument.getSymbol().value_or(std::string{});
    traceInput(step, makeValue("instrumentSymbol", instrumentSymbol));
    traceInput(step, makeValue("maxPartiesCount", _partiesCount));

    std::optional<std::uint32_t> optMaxLevels;
    if (const auto instrumentLevels = _instrument.getRandomDepthLevels())
    {
        traceInput(step, makeValue("randomDepthLevels", *instrumentLevels));

        if (*instrumentLevels <= _partiesCount)
        {
            optMaxLevels = *instrumentLevels;
            traceOutput(step, makeCommentedValue(
                    "maxDepthLevels"
                ,   *optMaxLevels
                ,   "randomDepthLevels is used as maximal market depth value"
            ));
        }
        else
        {
            optMaxLevels = _partiesCount;
            traceOutput(step, makeCommentedValue(
                    "maxDepthLevels"
                ,   *optMaxLevels
                ,   "a random parties count is used as max depth value "
                    "(maxInstrumentDepth > randomPartiesCount)"
            ));
        }
    }
    else {
        traceInput(step,
            makeValue("randomDepthLevels", std::string_view { "none" })
        );

        traceOutput(step, makeCommentedValue(
                "maxDepthLevels"
            ,   std::string_view { "none" }
            ,   "this instrument does not have max depth levels configured")
        );
    }

    traceStep(std::move(step), _tracer);
    return optMaxLevels;
}

} // namespace Simulator::Generator::Random

#endif // SIMULATOR_GENERATOR_SRC_ALGORITHM_UTILS_MAX_MKTDEPTH_SELECTOR_HPP_
