#ifndef SIMULATOR_GENERATOR_IH_CONTEXT_ORDER_GENERATION_CONTEXT_HPP_
#define SIMULATOR_GENERATOR_IH_CONTEXT_ORDER_GENERATION_CONTEXT_HPP_

#include "data_layer/api/models/price_seed.hpp"

#include "ih/context/instrument_context.hpp"
#include "ih/context/order_market_data_provider.hpp"

namespace Simulator::Generator {

class OrderGenerationContext
    :   public OrderInstrumentContext
{
public:

    [[nodiscard]]
    virtual DataLayer::PriceSeed const & getPriceSeed() const noexcept = 0;

    [[nodiscard]]
    virtual Generator::MarketState getCurrentMarketState() const = 0;
};

} // namespace Simulator::Generator

#endif // SIMULATOR_GENERATOR_IH_CONTEXT_ORDER_GENERATION_CONTEXT_HPP_

