#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <utility>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "core/domain/attributes.hpp"

#include "ih/adaptation/generated_message.hpp"
#include "ih/random/algorithm/order_generation_algorithm.hpp"
#include "ih/random/values/event.hpp"
#include "ih/random/values/resting_order_action.hpp"
#include "ih/registry/generated_order_data.hpp"
#include "mocks/context/order_context.hpp"
#include "mocks/random/counterparty_generator.hpp"
#include "mocks/random/price_generator.hpp"
#include "mocks/random/quantity_generator.hpp"
#include "mocks/random/random_event_generator.hpp"
#include "mocks/random/resting_order_action_generator.hpp"
#include "mocks/registry/generated_orders_registry.hpp"

namespace Simulator::Generator::Random {
namespace {

using ::testing::DoAll;
using ::testing::Eq;
using ::testing::Return;
using ::testing::ReturnRef;

class Generator_Random_OrderGenerationAlgorithm
    :   public testing::Test
{
    template<typename T>
    using Captured = std::optional<std::reference_wrapper<T>>;

public:

    using EventType = Random::Event::Type;
    using ActionType = Random::RestingOrderAction::Type;

    inline static const std::string symbol{"TestListingSymbol"};
    inline static const std::string venue_id{"TestVenue"};

    constexpr static auto toInteger(Random::Event::Type _event)
        -> Random::Event::RandomInteger
    {
        return static_cast<Random::Event::RandomInteger>(_event);
    }

    constexpr static auto toInteger(Random::RestingOrderAction::Type _action)
        -> Random::RestingOrderAction::RandomInteger
    {
        return static_cast<Random::RestingOrderAction::RandomInteger>(_action);
    }

    auto getContextPointer() -> std::shared_ptr<Mock::OrderGenerationContext>
    {
        return m_pContext;
    }

    auto context() -> Mock::OrderGenerationContext & { return *m_pContext; }

    auto registry() -> Mock::GeneratedOrdersRegistry & { return m_registry; }

    auto counterpartyGenerator() -> Mock::CounterpartyGenerator &
    {
        return m_cpGenerator->get();
    }

    auto restingOrderActionGenerator() -> Mock::RestingOrderActionGenerator &
    {
        return m_actionGenerator->get();
    }

    auto priceGenerator() -> Mock::PriceGenerator &
    {
        return m_priceGenerator->get();
    }

    auto quantityGenerator() -> Mock::QuantityGenerator &
    {
        return m_quantityGenerator->get();
    }

    auto eventGenerator() -> Mock::EventGenerator &
    {
        return m_eventGenerator->get();
    }

    auto generate() -> std::optional<GeneratedMessage>
    {
        GeneratedMessage generatedMessage{};

        bool completed = false;
        EXPECT_NO_THROW(completed = m_pAlgorithm->generate(generatedMessage));

        return completed ? std::make_optional(generatedMessage) : std::nullopt;
    }

    static auto makeMarketState(
            std::optional<double> _bidPx = std::nullopt
        ,   std::optional<std::uint32_t> _bidDepth = std::nullopt
        ,   std::optional<double> _offerPx = std::nullopt
        ,   std::optional<std::uint32_t> _offerDepth = std::nullopt
    ) -> Generator::MarketState
    {
        Generator::MarketState state{};
        state.bestBidPrice = _bidPx;
        state.bidDepthLevels = _bidDepth;
        state.bestOfferPrice = _offerPx;
        state.offerDepthLevels = _offerDepth;
        return state;
    }

    static auto makeRegisteredOrder(
            simulator::ClientOrderId const &_orderID
        ,   simulator::OrderPrice _price
        ,   simulator::Side _side
        ,   simulator::Quantity _quantity
        ,   simulator::PartyId const &_counterparty
    ) -> GeneratedOrderData
    {
        GeneratedOrderData::Builder builder{ _counterparty, _orderID, _side };
        builder
            .setPrice(_price)
            .setQuantity(_quantity);
        return GeneratedOrderData{ std::move(builder) };
    }

protected:
    static auto create_listing(
        const std::string& smbl,
        const std::string& venue,
        std::optional<std::uint32_t> random_depth_levels = std::nullopt)
        -> DataLayer::Listing {
        DataLayer::Listing::Patch listing_patch;
        listing_patch.withSymbol(smbl).withVenueId(venue);
        if (random_depth_levels.has_value()) {
          listing_patch.withRandomDepthLevels(*random_depth_levels);
        }
        return DataLayer::Listing::create(std::move(listing_patch), 1);
    }

    static auto create_venue(const std::string& venue) -> DataLayer::Venue
    {
        DataLayer::Venue::Patch venue_patch;
        venue_patch.withVenueId(venue);
        return DataLayer::Venue::create(std::move(venue_patch));
    }

    static auto create_price_seed(const std::string& symbol)
        -> DataLayer::PriceSeed {
        DataLayer::PriceSeed::Patch price_seed_patch;
        price_seed_patch.withSymbol(symbol);
        return DataLayer::PriceSeed::create(std::move(price_seed_patch), 1);
    }

    Generator_Random_OrderGenerationAlgorithm()
      : listing{create_listing(symbol, venue_id)},
        m_testVenue{create_venue(venue_id)},
        m_testPriceSeed{create_price_seed(symbol)}
    {
        setupTestEntities();
        setupAlgorithm();
    }

    void setupTestEntities()
    {
        m_pContext = std::make_shared<Mock::OrderGenerationContext>();

        EXPECT_CALL(context(), getVenue)
            .WillRepeatedly(ReturnRef(m_testVenue));

        EXPECT_CALL(context(), getInstrument)
            .WillRepeatedly(ReturnRef(listing));

        EXPECT_CALL(context(), getPriceSeed)
            .WillRepeatedly(ReturnRef(m_testPriceSeed));

        EXPECT_CALL(context(), takeRegistry)
            .WillRepeatedly(ReturnRef(m_registry));
    }

    void setupAlgorithm()
    {
        auto pCpGen = std::make_unique<Mock::CounterpartyGenerator>();
        auto pActionGen = std::make_unique<Mock::RestingOrderActionGenerator>();
        auto pPriceGen = std::make_unique<Mock::PriceGenerator>();
        auto pQtyGen = std::make_unique<Mock::QuantityGenerator>();
        auto pEventGen = std::make_unique<Mock::EventGenerator>();

        m_cpGenerator = std::ref(*pCpGen);
        m_actionGenerator = std::ref(*pActionGen);
        m_priceGenerator = std::ref(*pPriceGen);
        m_quantityGenerator = std::ref(*pQtyGen);
        m_eventGenerator = std::ref(*pEventGen);

        m_pAlgorithm = OrderGenerationAlgorithm::create(
                m_pContext
            ,   std::move(pEventGen)
            ,   std::move(pCpGen)
            ,   std::move(pActionGen)
            ,   std::move(pPriceGen)
            ,   std::move(pQtyGen)
        );
    }

    DataLayer::Listing listing;

private:

    DataLayer::Venue m_testVenue;
    DataLayer::PriceSeed m_testPriceSeed;

    Mock::GeneratedOrdersRegistry m_registry;

    std::shared_ptr<Mock::OrderGenerationContext> m_pContext;
    std::unique_ptr<OrderGenerationAlgorithm> m_pAlgorithm;

    Captured<Mock::CounterpartyGenerator> m_cpGenerator;
    Captured<Mock::RestingOrderActionGenerator> m_actionGenerator;
    Captured<Mock::PriceGenerator> m_priceGenerator;
    Captured<Mock::QuantityGenerator> m_quantityGenerator;
    Captured<Mock::EventGenerator> m_eventGenerator;
};

TEST_F(Generator_Random_OrderGenerationAlgorithm, Construct_WithContextOnly)
{
    EXPECT_NO_THROW(OrderGenerationAlgorithm::create(getContextPointer()));
}

TEST_F(Generator_Random_OrderGenerationAlgorithm, Generate_NoopEvent)
{
    constexpr auto event = EventType::NoOperation;

    EXPECT_CALL(eventGenerator(), generateChoiceInteger)
        .Times(1)
        .WillOnce(Return(toInteger(event)));

    std::optional<GeneratedMessage> const generated = generate();

    EXPECT_FALSE(generated.has_value());
}

TEST_F(Generator_Random_OrderGenerationAlgorithm, Generate_Aggressive_OppositeSideEmpty)
{
    constexpr auto event = EventType::AggressiveBuy;
    constexpr std::uint32_t cpNumber = 3;

    // Sell side has no prices
    MarketState const state = makeMarketState(120.1, 2);

    EXPECT_CALL(eventGenerator(), generateChoiceInteger)
        .Times(1)
        .WillOnce(Return(toInteger(event)));

    EXPECT_CALL(counterpartyGenerator(), generateCounterpartyNumber)
        .Times(1)
        .WillOnce(Return(cpNumber));

    EXPECT_CALL(context(), getCurrentMarketState)
        .Times(1)
        .WillOnce(Return(state));

    std::optional<GeneratedMessage> const generated = generate();

    EXPECT_FALSE(generated.has_value());
}

TEST_F(Generator_Random_OrderGenerationAlgorithm, Generate_Aggressive_OppositeSideHasPrices)
{
    constexpr auto event = EventType::AggressiveSell;
    constexpr std::uint32_t cpNumber = 3;
    constexpr double price = 149;
    constexpr double quantity = 170;
    std::string const orderID{ "AggressiveSellOrderID" };
    std::string const party{ "CP3" }; // Depends on cpNumber

    // Sell side has no prices
    MarketState const state = makeMarketState(150, 12);

    EXPECT_CALL(eventGenerator(), generateChoiceInteger)
        .Times(1)
        .WillOnce(Return(toInteger(event)));

    EXPECT_CALL(counterpartyGenerator(), generateCounterpartyNumber)
        .Times(1)
        .WillOnce(Return(cpNumber));

    EXPECT_CALL(context(), getCurrentMarketState)
        .Times(1)
        .WillRepeatedly(Return(state));

    EXPECT_CALL(context(), getSyntheticIdentifier)
        .Times(1)
        .WillOnce(Return(orderID));

    EXPECT_CALL(priceGenerator(), generatePx)
        .Times(1)
        .WillOnce(Return(Mock::PriceGenerator::makeOutput(price)));

    EXPECT_CALL(quantityGenerator(), generateRandomQty)
        .Times(1)
        .WillOnce(Return(Mock::QuantityGenerator::makeOutput(quantity)));

    std::optional<GeneratedMessage> const generated = generate();

    ASSERT_TRUE(generated.has_value());
    auto const & message = generated.value();

    EXPECT_EQ(message.message_type, MessageType::NewOrderSingle);
    EXPECT_EQ(message.order_type, simulator::OrderType::Option::Limit);
    EXPECT_EQ(message.side, simulator::Side::Option::Sell);
    EXPECT_EQ(message.time_in_force, simulator::TimeInForce::Option::ImmediateOrCancel);
    EXPECT_DOUBLE_EQ(message.order_price->value(), price);
    EXPECT_DOUBLE_EQ(message.quantity->value(), quantity);
    EXPECT_EQ(message.client_order_id->value(), orderID);
    EXPECT_EQ(message.party_id->value(), party);
}

TEST_F(Generator_Random_OrderGenerationAlgorithm, Generate_Resting_PartyHasNoOrder_NoMaxMktDepth)
{
    constexpr auto event = EventType::RestingBuy;
    constexpr int cpNumber = 5;
    constexpr double price = 34.54;
    constexpr double quantity = 20.05;
    std::string const party{ "CP5" };
    std::string const orderID{ "NewOrderID" };

    EXPECT_CALL(eventGenerator(), generateChoiceInteger)
        .Times(1)
        .WillOnce(Return(toInteger(event)));

    EXPECT_CALL(counterpartyGenerator(), generateCounterpartyNumber)
        .Times(1)
        .WillOnce(Return(cpNumber));

    EXPECT_CALL(registry(), findByOwner(Eq(party)))
        .Times(1)
        .WillOnce(Return(std::nullopt));

    EXPECT_CALL(context(), getSyntheticIdentifier)
        .Times(1)
        .WillOnce(Return(orderID));

    EXPECT_CALL(context(), getCurrentMarketState)
        .Times(1)
        .WillOnce(Return(makeMarketState()));

    EXPECT_CALL(priceGenerator(), generatePx)
        .Times(1)
        .WillOnce(Return(Mock::PriceGenerator::makeOutput(price)));

    EXPECT_CALL(quantityGenerator(), generateRandomQty)
        .Times(1)
        .WillOnce(Return(Mock::QuantityGenerator::makeOutput(quantity)));

    EXPECT_CALL(registry(), add)
        .Times(1)
        .WillOnce(Return(true));

    std::optional<GeneratedMessage> const generated = generate();

    ASSERT_TRUE(generated.has_value());
    auto const & message = generated.value();

    EXPECT_EQ(message.message_type, MessageType::NewOrderSingle);
    EXPECT_EQ(message.order_type, simulator::OrderType::Option::Limit);
    EXPECT_EQ(message.side, simulator::Side::Option::Buy);
    EXPECT_EQ(message.time_in_force, simulator::TimeInForce::Option::Day);
    EXPECT_DOUBLE_EQ(message.order_price->value(), price);
    EXPECT_DOUBLE_EQ(message.quantity->value(), quantity);
    EXPECT_EQ(message.client_order_id->value(), orderID);
    EXPECT_EQ(message.party_id->value(), party);
}

TEST_F(Generator_Random_OrderGenerationAlgorithm, Generate_Resting_PartyHasNoOrder_MaxMktDepthReached)
{
    constexpr auto event = EventType::RestingBuy;
    constexpr int cpNumber = 5;
    constexpr int maxMktDepth = 14;
    constexpr std::size_t currentMktDepth = 14;
    std::string const party{ "CP5" };
    std::string const orderID{ "NewOrderID" };

    listing = create_listing(symbol, venue_id, maxMktDepth);

    EXPECT_CALL(eventGenerator(), generateChoiceInteger)
        .Times(1)
        .WillOnce(Return(toInteger(event)));

    EXPECT_CALL(counterpartyGenerator(), generateCounterpartyNumber)
        .Times(1)
        .WillOnce(Return(cpNumber));

    EXPECT_CALL(registry(), findByOwner(Eq(party)))
        .Times(1)
        .WillOnce(Return(std::nullopt));

    EXPECT_CALL(context(), getCurrentMarketState)
        .Times(1)
        .WillOnce(Return(makeMarketState(std::nullopt, currentMktDepth)));

    std::optional<GeneratedMessage> const generated = generate();

    EXPECT_FALSE(generated.has_value());
}

TEST_F(Generator_Random_OrderGenerationAlgorithm, Generate_Resting_PartyHasOrder_PriceModificationAction)
{
    constexpr auto event = EventType::RestingSell;
    constexpr auto action = ActionType::PriceModification;
    constexpr int cpNumber = 5;
    constexpr simulator::OrderPrice oldPrice{120.3};
    constexpr double newPrice = 121.4;
    constexpr simulator::Quantity quantity{121.4};
    const simulator::PartyId party{"CP5"};

    const simulator::ClientOrderId orderID{ "RegisteredOrderID" };

    // Note: registered order side is SELL, we have a resting sell event
    GeneratedOrderData const registeredOrder = makeRegisteredOrder(
            orderID
        ,   oldPrice
        ,   simulator::Side::Option::Buy
        ,   quantity
        ,   party
    );

    EXPECT_CALL(eventGenerator(), generateChoiceInteger)
        .Times(1)
        .WillOnce(Return(toInteger(event)));

    EXPECT_CALL(counterpartyGenerator(), generateCounterpartyNumber)
        .Times(1)
        .WillOnce(Return(cpNumber));

    EXPECT_CALL(registry(), findByOwner(Eq(party.value())))
        .Times(1)
        .WillOnce(Return(registeredOrder));

    EXPECT_CALL(restingOrderActionGenerator(), generateInteger)
        .Times(1)
        .WillOnce(Return(toInteger(action)));

    EXPECT_CALL(context(), getCurrentMarketState)
        .Times(1)
        .WillOnce(Return(makeMarketState()));

    EXPECT_CALL(priceGenerator(), generatePx)
        .Times(1)
        .WillOnce(Return(Mock::PriceGenerator::makeOutput(newPrice)));

    EXPECT_CALL(quantityGenerator(), generateRandomQty)
        .Times(0);

    EXPECT_CALL(registry(), updateByOwner(Eq(party.value()), testing::_))
        .Times(1)
        .WillOnce(Return(true));

    std::optional<GeneratedMessage> const generated = generate();

    ASSERT_TRUE(generated.has_value());
    auto const & message = generated.value();

    EXPECT_EQ(message.message_type, MessageType::OrderCancelReplaceRequest);
    EXPECT_EQ(message.order_type, simulator::OrderType::Option::Limit);
    EXPECT_EQ(message.side, simulator::Side::Option::Buy); // Side is same as in stored order
    EXPECT_EQ(message.time_in_force, simulator::TimeInForce::Option::Day);
    EXPECT_DOUBLE_EQ(message.order_price->value(), newPrice);
    EXPECT_DOUBLE_EQ(message.quantity->value(), quantity.value());
    EXPECT_EQ(message.client_order_id, orderID);
    EXPECT_EQ(message.orig_client_order_id->value(), orderID.value());
    EXPECT_EQ(message.party_id, party);
}

TEST_F(Generator_Random_OrderGenerationAlgorithm, Generate_Resting_PartyHasOrder_QtyModificationAction)
{
    constexpr auto event = EventType::RestingSell;
    constexpr auto action = ActionType::QuantityModification;
    constexpr int cpNumber = 5;
    constexpr simulator::OrderPrice price{121.4};
    constexpr simulator::Quantity oldQuantity{121.4};
    constexpr double newQuantity = 121.5;
    const simulator::PartyId party{ "CP5" };

    const simulator::ClientOrderId orderID{ "RegisteredOrderID" };

    GeneratedOrderData const registeredOrder = makeRegisteredOrder(
            orderID
        ,   price
        ,   simulator::Side::Option::Sell
        ,   oldQuantity
        ,   party
    );

    EXPECT_CALL(eventGenerator(), generateChoiceInteger)
        .Times(1)
        .WillOnce(Return(toInteger(event)));

    EXPECT_CALL(counterpartyGenerator(), generateCounterpartyNumber)
        .Times(1)
        .WillOnce(Return(cpNumber));

    EXPECT_CALL(registry(), findByOwner(Eq(party.value())))
        .Times(1)
        .WillOnce(Return(registeredOrder));

    EXPECT_CALL(restingOrderActionGenerator(), generateInteger)
        .Times(1)
        .WillOnce(Return(toInteger(action)));

    EXPECT_CALL(context(), getCurrentMarketState)
        .Times(1)
        .WillOnce(Return(makeMarketState()));

    EXPECT_CALL(priceGenerator(), generatePx)
        .Times(0);

    EXPECT_CALL(quantityGenerator(), generateRandomQty)
        .Times(1)
        .WillOnce(Return(Mock::QuantityGenerator::makeOutput(newQuantity)));

    EXPECT_CALL(registry(), updateByOwner(Eq(party.value()), testing::_))
        .Times(1)
        .WillOnce(Return(true));

    std::optional<GeneratedMessage> const generated = generate();

    ASSERT_TRUE(generated.has_value());
    auto const & message = generated.value();

    EXPECT_EQ(message.message_type, MessageType::OrderCancelReplaceRequest);
    EXPECT_EQ(message.order_type, simulator::OrderType::Option::Limit);
    EXPECT_EQ(message.side, simulator::Side::Option::Sell);
    EXPECT_EQ(message.time_in_force, simulator::TimeInForce::Option::Day);
    EXPECT_DOUBLE_EQ(message.order_price->value(), price.value());
    EXPECT_DOUBLE_EQ(message.quantity->value(), newQuantity);
    EXPECT_EQ(message.client_order_id, orderID);
    EXPECT_EQ(message.orig_client_order_id->value(), orderID.value());
    EXPECT_EQ(message.party_id, party);
}

TEST_F(Generator_Random_OrderGenerationAlgorithm, Generate_Resting_PartyHasOrder_Cancellation)
{
    constexpr auto event = EventType::RestingBuy;
    constexpr auto action = ActionType::Cancellation;
    constexpr int cpNumber = 5;
    constexpr simulator::OrderPrice price{121.4};
    constexpr simulator::Quantity quantity{121.5};
    const simulator::PartyId party{ "CP5" };

    const simulator::ClientOrderId orderID{ "RegisteredOrderID" };

    GeneratedOrderData const registeredOrder = makeRegisteredOrder(
            orderID
        ,   price
        ,   simulator::Side::Option::Buy
        ,   quantity
        ,   party
    );

    EXPECT_CALL(eventGenerator(), generateChoiceInteger)
        .Times(1)
        .WillOnce(Return(toInteger(event)));

    EXPECT_CALL(counterpartyGenerator(), generateCounterpartyNumber)
        .Times(1)
        .WillOnce(Return(cpNumber));

    EXPECT_CALL(registry(), findByOwner(Eq(party.value())))
        .Times(1)
        .WillOnce(Return(registeredOrder));

    EXPECT_CALL(restingOrderActionGenerator(), generateInteger)
        .Times(1)
        .WillOnce(Return(toInteger(action)));

    EXPECT_CALL(context(), getCurrentMarketState)
        .Times(1)
        .WillOnce(Return(makeMarketState()));

    EXPECT_CALL(priceGenerator(), generatePx)
        .Times(0);

    EXPECT_CALL(quantityGenerator(), generateRandomQty)
        .Times(0);

    EXPECT_CALL(registry(), removeByOwner(Eq(party.value())))
        .Times(1)
        .WillOnce(Return(true));

    std::optional<GeneratedMessage> const generated = generate();

    ASSERT_TRUE(generated.has_value());
    auto const & message = generated.value();

    EXPECT_EQ(message.message_type, MessageType::OrderCancelRequest);
    EXPECT_EQ(message.order_type, simulator::OrderType::Option::Limit);
    EXPECT_EQ(message.side, simulator::Side::Option::Buy);
    EXPECT_EQ(message.time_in_force, simulator::TimeInForce::Option::Day);
    EXPECT_EQ(message.client_order_id, orderID);
    EXPECT_EQ(message.orig_client_order_id->value(), orderID.value());
    EXPECT_EQ(message.party_id, party);
}

}  // namespace
}  // namespace Simulator::Generator::Random