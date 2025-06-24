#include "ih/context/order_generation_context_impl.hpp"

#include <cassert>
#include <memory>
#include <utility>

#include "ih/registry/generated_orders_registry_impl.hpp"

namespace Simulator::Generator {

OrderInstrumentContextImpl::OrderInstrumentContextImpl(
        DataLayer::Listing _instrument
    ,   simulator::InstrumentDescriptor _descriptor
    ,   std::shared_ptr<ComponentContext> _pGlobalContext
    ,   std::unique_ptr<GeneratedOrdersRegistry> _pGeneratedOrdersRegistry
) noexcept
    :   m_instrument { std::move(_instrument) }
    ,   m_instrumentDescriptor { std::move(_descriptor) }
    ,   m_pGlobalContext { std::move(_pGlobalContext) }
    ,   m_pInstrumentOrdersRegistry { std::move(_pGeneratedOrdersRegistry) }
{
    assert(m_pGlobalContext);
    assert(m_pInstrumentOrdersRegistry);
}


std::shared_ptr<OrderInstrumentContext> OrderInstrumentContextImpl::create(
        DataLayer::Listing const & _instrument
    ,   simulator::InstrumentDescriptor const & _descriptor
    ,   std::shared_ptr<ComponentContext> _pGlobalContext
)
{
    return std::make_shared<OrderInstrumentContextImpl>(
            _instrument
        ,   _descriptor
        ,   std::move(_pGlobalContext)
        ,   std::make_unique<GeneratedOrdersRegistryImpl>()
    );
}



std::string OrderInstrumentContextImpl::getSyntheticIdentifier() noexcept
{
    return m_pGlobalContext->generateIdentifier();
}


DataLayer::Venue const & OrderInstrumentContextImpl::getVenue() const noexcept
{
    return m_pGlobalContext->getVenue();
}


DataLayer::Listing const &
OrderInstrumentContextImpl::getInstrument() const noexcept
{
    return m_instrument;
}


simulator::InstrumentDescriptor const &
OrderInstrumentContextImpl::getInstrumentDescriptor() const noexcept
{
  return m_instrumentDescriptor;
}


std::size_t OrderInstrumentContextImpl::nextMessageNumber() noexcept
{
    return m_pGlobalContext->nextGeneratedOrderMessageNumber();
}


GeneratedOrdersRegistry &
OrderInstrumentContextImpl::takeRegistry() noexcept
{
    return *m_pInstrumentOrdersRegistry;
}


OrderGenerationContextImpl::OrderGenerationContextImpl(
        std::shared_ptr<OrderInstrumentContext> _pInstrumentCtx
    ,   DataLayer::PriceSeed _configuredPrices
) noexcept
    :   m_configuredPrices { std::move(_configuredPrices) }
    ,   m_pInstrumentContext { std::move(_pInstrumentCtx) }
{
    assert(m_pInstrumentContext);
    m_pMarketDataProvider = std::make_unique<OrderMarketDataProvider>(
        m_pInstrumentContext->getInstrumentDescriptor()
    );
}


std::shared_ptr<OrderGenerationContextImpl> OrderGenerationContextImpl::create(
        std::shared_ptr<OrderInstrumentContext> _pInstrumentCtx
    ,   DataLayer::PriceSeed const & _configuredPrices
)
{
    return std::make_shared<OrderGenerationContextImpl>(
            std::move(_pInstrumentCtx)
        ,   _configuredPrices
    );
}


std::string OrderGenerationContextImpl::getSyntheticIdentifier() noexcept
{
    return m_pInstrumentContext->getSyntheticIdentifier();
}


DataLayer::Venue const & OrderGenerationContextImpl::getVenue() const noexcept
{
    return m_pInstrumentContext->getVenue();
}


DataLayer::Listing const &
OrderGenerationContextImpl::getInstrument() const noexcept
{
    return m_pInstrumentContext->getInstrument();
}


simulator::InstrumentDescriptor const &
OrderGenerationContextImpl::getInstrumentDescriptor() const noexcept
{
  return m_pInstrumentContext->getInstrumentDescriptor();
}


std::size_t OrderGenerationContextImpl::nextMessageNumber() noexcept
{
    return m_pInstrumentContext->nextMessageNumber();
}


GeneratedOrdersRegistry & OrderGenerationContextImpl::takeRegistry() noexcept
{
    return m_pInstrumentContext->takeRegistry();
}


DataLayer::PriceSeed const &
OrderGenerationContextImpl::getPriceSeed() const noexcept
{
    return m_configuredPrices;
}


Generator::MarketState OrderGenerationContextImpl::getCurrentMarketState() const
{
    return m_pMarketDataProvider->getMarketState();
}

} // namespace Simulator::Generator
