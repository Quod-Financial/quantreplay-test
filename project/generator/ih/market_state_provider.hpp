#ifndef SIMULATOR_GENERATOR_IH_INSTRUMENT_STATE_PROVIDER_HPP_
#define SIMULATOR_GENERATOR_IH_INSTRUMENT_STATE_PROVIDER_HPP_

#include <cstddef>

#include "common/api/enum/side.hpp"
#include "common/api/message.hpp"

namespace Simulator::Generator {

class MarketStateProvider
{
public:

    virtual ~MarketStateProvider() = default;

    virtual std::size_t getMarketDepth(Common::Side _depthSide) = 0;

    virtual Common::MarketState getCurrentState() = 0;
};

} // namespace Simulator::Generator

#endif // SIMULATOR_GENERATOR_IH_INSTRUMENT_STATE_PROVIDER_HPP_
