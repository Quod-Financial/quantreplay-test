#ifndef SIMULATOR_GENERATOR_SRC_CONTEXT_ORDER_GENERATION_CONTEXT_IMPL_HPP_
#define SIMULATOR_GENERATOR_SRC_CONTEXT_ORDER_GENERATION_CONTEXT_IMPL_HPP_

#include <cstddef>
#include <memory>

#include "ih/context/component_context.hpp"
#include "ih/context/instrument_context.hpp"
#include "ih/context/order_generation_context.hpp"
#include "ih/context/order_market_data_provider.hpp"
#include "ih/registry/generated_orders_registry.hpp"
#include "ih/registry/generated_orders_registry_impl.hpp"

namespace Simulator::Generator {

class OrderInstrumentContextImpl
    :   public OrderInstrumentContext
{
public:

    OrderInstrumentContextImpl() = delete;

    OrderInstrumentContextImpl(
            DataLayer::Listing _instrument
        ,   simulator::InstrumentDescriptor descriptor
        ,   std::shared_ptr<ComponentContext> _pGlobalContext
        ,   std::unique_ptr<GeneratedOrdersRegistry> _pGeneratedOrdersRegistry
    ) noexcept;

    [[nodiscard]]
    static std::shared_ptr<OrderInstrumentContext> create(
            DataLayer::Listing const & _instrument
        ,   simulator::InstrumentDescriptor const & descriptor
        ,   std::shared_ptr<ComponentContext> _pGlobalContext
    );


    std::string getSyntheticIdentifier() noexcept override;

    [[nodiscard]]
    DataLayer::Venue const & getVenue() const noexcept override;

    [[nodiscard]]
    DataLayer::Listing const & getInstrument() const noexcept override;

    [[nodiscard]]
    simulator::InstrumentDescriptor const &
    getInstrumentDescriptor() const noexcept override;

    [[nodiscard]]
    std::size_t nextMessageNumber() noexcept override;

    GeneratedOrdersRegistry & takeRegistry() noexcept override;

private:

    DataLayer::Listing m_instrument;

    simulator::InstrumentDescriptor m_instrumentDescriptor;

    std::shared_ptr<ComponentContext> m_pGlobalContext;

    std::unique_ptr<GeneratedOrdersRegistry> m_pInstrumentOrdersRegistry;
};


class OrderGenerationContextImpl
    :   public OrderGenerationContext
{
public:

    OrderGenerationContextImpl() = delete;

    OrderGenerationContextImpl(
            std::shared_ptr<OrderInstrumentContext> _pInstrumentCtx
        ,   DataLayer::PriceSeed _configuredPrices
    ) noexcept;

    static std::shared_ptr<OrderGenerationContextImpl> create(
            std::shared_ptr<OrderInstrumentContext> _pInstrumentCtx
        ,   DataLayer::PriceSeed const & _configuredPrices
    );


    [[nodiscard]]
    std::string getSyntheticIdentifier() noexcept override;

    [[nodiscard]]
    DataLayer::Venue const & getVenue() const noexcept override;

    [[nodiscard]]
    DataLayer::Listing const & getInstrument() const noexcept override;

    [[nodiscard]]
    simulator::InstrumentDescriptor const &
    getInstrumentDescriptor() const noexcept override;

    [[nodiscard]]
    std::size_t nextMessageNumber() noexcept override;

    [[nodiscard]]
    GeneratedOrdersRegistry & takeRegistry() noexcept override;


    [[nodiscard]]
    DataLayer::PriceSeed const & getPriceSeed() const noexcept override;

    [[nodiscard]]
    Generator::MarketState getCurrentMarketState() const override;

private:

    DataLayer::PriceSeed m_configuredPrices;

    std::shared_ptr<OrderInstrumentContext> m_pInstrumentContext;
    std::shared_ptr<OrderMarketDataProvider> m_pMarketDataProvider;
};

} // namespace Simulator::Generator

#endif // SIMULATOR_GENERATOR_SRC_CONTEXT_ORDER_GENERATION_CONTEXT_IMPL_HPP_
