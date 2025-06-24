#ifndef SIMULATOR_GENERATOR_TESTS_MOCKS_ORDER_GENERATION_CONTEXT_HPP_
#define SIMULATOR_GENERATOR_TESTS_MOCKS_ORDER_GENERATION_CONTEXT_HPP_

#include <string>

#include "data_layer/api/models/listing.hpp"
#include "data_layer/api/models/price_seed.hpp"
#include "data_layer/api/models/venue.hpp"

#include "ih/context/instrument_context.hpp"
#include "ih/context/order_generation_context.hpp"
#include "mocks/registry/generated_orders_registry.hpp"

namespace Simulator::Generator::Mock {

class OrderInstrumentContext
    :   public Generator::OrderInstrumentContext
{
public:

    MOCK_METHOD(std::string, getSyntheticIdentifier, (), (noexcept, override));

    MOCK_METHOD(DataLayer::Venue const &, getVenue, (), (const, noexcept, override));

    MOCK_METHOD(DataLayer::Listing const &, getInstrument, (), (const, noexcept, override));

    MOCK_METHOD(simulator::InstrumentDescriptor const &, getInstrumentDescriptor, (), (const, noexcept, override));

    MOCK_METHOD(std::size_t, nextMessageNumber, (), (noexcept, override));

    MOCK_METHOD(Generator::GeneratedOrdersRegistry &, takeRegistry, (), (noexcept, override));
};

class OrderGenerationContext
    :   public Generator::OrderGenerationContext
{
public:

    MOCK_METHOD(std::string, getSyntheticIdentifier, (), (noexcept, override));

    MOCK_METHOD(DataLayer::Venue const &, getVenue, (), (const, noexcept, override));

    MOCK_METHOD(DataLayer::Listing const &, getInstrument, (), (const, noexcept, override));

    MOCK_METHOD(simulator::InstrumentDescriptor const &, getInstrumentDescriptor, (), (const, noexcept, override));

    MOCK_METHOD(std::size_t, nextMessageNumber, (), (noexcept, override));

    MOCK_METHOD(Generator::GeneratedOrdersRegistry &, takeRegistry, (), (noexcept, override));

    MOCK_METHOD(DataLayer::PriceSeed const &, getPriceSeed, (), (const, noexcept, override));

    MOCK_METHOD(Generator::MarketState, getCurrentMarketState, (), (const, override));
};

} // namespace Simulator::Generator::Mock

#endif // SIMULATOR_GENERATOR_TESTS_MOCKS_ORDER_GENERATION_CONTEXT_HPP_

