#include <gmock/gmock.h>

#include <optional>

#include "ih/adaptation/protocol_conversion.hpp"

namespace Simulator::Generator {
namespace {

// NOLINTBEGIN(*magic-numbers*)

using namespace testing;  // NOLINT

struct GeneratorOrderPlacementRequestConversion : Test {
  GeneratedMessage message;
  simulator::InstrumentDescriptor instrument;
};

TEST_F(GeneratorOrderPlacementRequestConversion, ConvertsGeneratorSession) {
  const auto request = convert_to_order_placement_request(message, instrument);

  ASSERT_THAT(request.session.value,
              VariantWith<simulator::protocol::generator::Session>(_));
}

TEST_F(GeneratorOrderPlacementRequestConversion, ConvertsOrderType) {
  message.order_type = simulator::OrderType::Option::Limit;

  const auto request = convert_to_order_placement_request(message, instrument);

  ASSERT_THAT(request.order_type,
              Optional(Eq(simulator::OrderType::Option::Limit)));
}

TEST_F(GeneratorOrderPlacementRequestConversion, ConvertsPriceForLimitOrder) {
  message.order_type = simulator::OrderType::Option::Limit;
  message.order_price = simulator::OrderPrice{42.42};

  const auto request = convert_to_order_placement_request(message, instrument);

  ASSERT_THAT(request.order_price, Optional(Eq(simulator::Price{42.42})));
}

TEST_F(GeneratorOrderPlacementRequestConversion, IgnoresPriceForMarketOrders) {
  message.order_type = simulator::OrderType::Option::Market;
  message.order_price = simulator::OrderPrice{42.42};

  const auto request = convert_to_order_placement_request(message, instrument);

  ASSERT_THAT(request.order_price, Eq(std::nullopt));
}

TEST_F(GeneratorOrderPlacementRequestConversion, ConvertsQuantity) {
  message.quantity = simulator::Quantity{42.42};

  const auto request = convert_to_order_placement_request(message, instrument);

  ASSERT_THAT(request.order_quantity,
              Optional(Eq(simulator::OrderQuantity{42.42})));
}

TEST_F(GeneratorOrderPlacementRequestConversion, ConvertsTimeInForce) {
  message.time_in_force = simulator::TimeInForce::Option::Day;

  const auto request = convert_to_order_placement_request(message, instrument);

  ASSERT_THAT(request.time_in_force,
              Optional(Eq(simulator::TimeInForce::Option::Day)));
}

TEST_F(GeneratorOrderPlacementRequestConversion, ConvertsSide) {
  message.side = simulator::Side::Option::Sell;

  const auto request = convert_to_order_placement_request(message, instrument);

  ASSERT_THAT(request.side, Optional(Eq(simulator::Side::Option::Sell)));
}

TEST_F(GeneratorOrderPlacementRequestConversion, ConvertsClientOrderId) {
  message.client_order_id = simulator::ClientOrderId{"GeneratorOrdID"};

  const auto request = convert_to_order_placement_request(message, instrument);

  ASSERT_THAT(request.client_order_id,
              Optional(Eq(simulator::ClientOrderId{"GeneratorOrdID"})));
}

TEST_F(GeneratorOrderPlacementRequestConversion, ConvertsParties) {
  message.party_id = simulator::PartyId{"CP1"};

  const auto request = convert_to_order_placement_request(message, instrument);

  ASSERT_THAT(
      request.parties,
      ElementsAre(AllOf(
          Property(&simulator::Party::party_id, Eq(simulator::PartyId{"CP1"})),
          Property(&simulator::Party::source,
                   Eq(simulator::PartyIdSource::Option::Proprietary)),
          Property(&simulator::Party::role,
                   Eq(simulator::PartyRole::Option::ExecutingFirm)))));
}

struct GeneratorOrderModificationRequestConversion : Test {
  GeneratedMessage message;
  simulator::InstrumentDescriptor instrument;
};

TEST_F(GeneratorOrderModificationRequestConversion, ConvertsGeneratorSession) {
  const auto request =
      convert_to_order_modification_request(message, instrument);

  ASSERT_THAT(request.session.value,
              VariantWith<simulator::protocol::generator::Session>(_));
}

TEST_F(GeneratorOrderModificationRequestConversion, ConvertsOrderType) {
  message.order_type = simulator::OrderType::Option::Limit;

  const auto request =
      convert_to_order_modification_request(message, instrument);

  ASSERT_THAT(request.order_type,
              Optional(Eq(simulator::OrderType::Option::Limit)));
}

TEST_F(GeneratorOrderModificationRequestConversion,
       ConvertsPriceForLimitOrder) {
  message.order_type = simulator::OrderType::Option::Limit;
  message.order_price = simulator::OrderPrice{42.42};

  const auto request = convert_to_order_placement_request(message, instrument);

  ASSERT_THAT(request.order_price, Optional(Eq(simulator::Price{42.42})));
}

TEST_F(GeneratorOrderModificationRequestConversion,
       IgnoresPriceForMarketOrders) {
  message.order_type = simulator::OrderType::Option::Market;
  message.order_price = simulator::OrderPrice{42.42};

  const auto request =
      convert_to_order_modification_request(message, instrument);

  ASSERT_THAT(request.order_price, Eq(std::nullopt));
}

TEST_F(GeneratorOrderModificationRequestConversion, ConvertsQuantity) {
  message.quantity = simulator::Quantity{42.42};

  const auto request =
      convert_to_order_modification_request(message, instrument);

  ASSERT_THAT(request.order_quantity,
              Optional(Eq(simulator::OrderQuantity{42.42})));
}

TEST_F(GeneratorOrderModificationRequestConversion, ConvertsTimeInForce) {
  message.time_in_force = simulator::TimeInForce::Option::Day;

  const auto request =
      convert_to_order_modification_request(message, instrument);

  ASSERT_THAT(request.time_in_force,
              Optional(Eq(simulator::TimeInForce::Option::Day)));
}

TEST_F(GeneratorOrderModificationRequestConversion, ConvertsSide) {
  message.side = simulator::Side::Option::Sell;

  const auto request =
      convert_to_order_modification_request(message, instrument);

  ASSERT_THAT(request.side, Optional(Eq(simulator::Side::Option::Sell)));
}

TEST_F(GeneratorOrderModificationRequestConversion, ConvertsClientOrderId) {
  message.client_order_id = simulator::ClientOrderId{"GeneratorOrdID"};

  const auto request =
      convert_to_order_modification_request(message, instrument);

  ASSERT_THAT(request.client_order_id,
              Optional(Eq(simulator::ClientOrderId{"GeneratorOrdID"})));
}

TEST_F(GeneratorOrderModificationRequestConversion, ConvertsOrigClientOrderId) {
  message.orig_client_order_id = simulator::OrigClientOrderId{"GeneratorOrdID"};

  const auto request =
      convert_to_order_modification_request(message, instrument);

  ASSERT_THAT(request.orig_client_order_id,
              Optional(Eq(simulator::OrigClientOrderId{"GeneratorOrdID"})));
}

TEST_F(GeneratorOrderModificationRequestConversion, ConvertsParties) {
  message.party_id = simulator::PartyId{"CP1"};

  const auto request =
      convert_to_order_modification_request(message, instrument);

  ASSERT_THAT(
      request.parties,
      ElementsAre(AllOf(
          Property(&simulator::Party::party_id, Eq(simulator::PartyId{"CP1"})),
          Property(&simulator::Party::source,
                   Eq(simulator::PartyIdSource::Option::Proprietary)),
          Property(&simulator::Party::role,
                   Eq(simulator::PartyRole::Option::ExecutingFirm)))));
}

struct GeneratorOrderCancellationRequestConversion : Test {
  GeneratedMessage message;
  simulator::InstrumentDescriptor instrument;
};

TEST_F(GeneratorOrderCancellationRequestConversion, ConvertsGeneratorSession) {
  const auto request =
      convert_to_order_cancellation_request(message, instrument);

  ASSERT_THAT(request.session.value,
              VariantWith<simulator::protocol::generator::Session>(_));
}

TEST_F(GeneratorOrderCancellationRequestConversion, ConvertsSide) {
  message.side = simulator::Side::Option::Buy;

  const auto request =
      convert_to_order_cancellation_request(message, instrument);

  ASSERT_THAT(request.side, Optional(Eq(simulator::Side::Option::Buy)));
}

TEST_F(GeneratorOrderCancellationRequestConversion, ConvertsClientOrderId) {
  message.client_order_id = simulator::ClientOrderId{"GeneratorOrdID"};

  const auto request =
      convert_to_order_cancellation_request(message, instrument);

  ASSERT_THAT(request.client_order_id,
              Optional(Eq(simulator::ClientOrderId{"GeneratorOrdID"})));
}

TEST_F(GeneratorOrderCancellationRequestConversion, ConvertsOrigClientOrderId) {
  message.orig_client_order_id = simulator::OrigClientOrderId{"GeneratorOrdID"};

  const auto request =
      convert_to_order_cancellation_request(message, instrument);

  ASSERT_THAT(request.orig_client_order_id,
              Optional(Eq(simulator::OrigClientOrderId{"GeneratorOrdID"})));
}

// NOLINTEND(*magic-numbers*)

}  // namespace
}  // namespace Simulator::Generator
