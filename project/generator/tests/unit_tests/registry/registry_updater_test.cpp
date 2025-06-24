#include "ih/registry/registry_updater.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <optional>
#include <stdexcept>
#include <string>

#include "ih/constants.hpp"
#include "ih/registry/generated_order_data.hpp"
#include "mocks/registry/generated_orders_registry.hpp"

namespace Simulator::Generator {
namespace {

// NOLINTBEGIN(*magic-numbers*)

using namespace testing;  // NOLINT

struct GeneratorOrderRegistryUpdater : public Test {
  static constexpr auto side = simulator::Side::Option::Buy;
  const simulator::ClientOrderId order_id{"OrderID"};
  const simulator::PartyId owner_id{"OwnerID"};

  static auto makeMessage(MessageType _messageType) -> GeneratedMessage {
    GeneratedMessage message;
    message.message_type = _messageType;
    return message;
  }

  // Need to use StrictMock to ensure that no unexpected calls are made
  StrictMock<Mock::GeneratedOrdersRegistry> registry;
};

struct GeneratorOrderRegistryUpdaterRestingNewOrderSingle
    : public GeneratorOrderRegistryUpdater {
  GeneratedMessage resting_new_order_single =
      makeMessage(MessageType::NewOrderSingle);

  GeneratorOrderRegistryUpdaterRestingNewOrderSingle() {
    resting_new_order_single.order_type = Constant::RestingOrderType;
    resting_new_order_single.time_in_force = Constant::RestingTimeInForce;
    resting_new_order_single.client_order_id = order_id;
    resting_new_order_single.party_id = owner_id;
    resting_new_order_single.side = side;
  }
};

ACTION_P5(CheckGeneratedOrderData, Owner, OrderID, Price, Side, Qty) {
  const GeneratedOrderData& data = arg0;
  EXPECT_EQ(data.getOwnerID(), Owner);
  EXPECT_EQ(data.getOrderID(), OrderID);
  EXPECT_EQ(data.getOrderPx(), Price);
  EXPECT_EQ(data.getOrderSide(), Side);
  EXPECT_EQ(data.getOrderQty(), Qty);
}

TEST_F(GeneratorOrderRegistryUpdaterRestingNewOrderSingle,
       DoesNotAddAggressiveToRegistry) {
  auto message = resting_new_order_single;
  message.order_type = Constant::AggressiveOrderType;
  message.time_in_force = Constant::AggressiveTimeInForce;

  EXPECT_CALL(registry, add).Times(0);

  EXPECT_NO_THROW(OrderRegistryUpdater::update(registry, message));
}

TEST_F(GeneratorOrderRegistryUpdaterRestingNewOrderSingle,
       ThrowsExceptionOnAbsentClOrdID) {
  resting_new_order_single.client_order_id = std::nullopt;

  EXPECT_CALL(registry, add).Times(0);

  EXPECT_THROW(OrderRegistryUpdater::update(registry, resting_new_order_single),
               std::invalid_argument);
}

TEST_F(GeneratorOrderRegistryUpdaterRestingNewOrderSingle,
       ThrowsExceptionOnEmptyClOrdID) {
  resting_new_order_single.client_order_id = simulator::ClientOrderId{{}};

  EXPECT_CALL(registry, add).Times(0);

  EXPECT_THROW(OrderRegistryUpdater::update(registry, resting_new_order_single),
               std::invalid_argument);
}

TEST_F(GeneratorOrderRegistryUpdaterRestingNewOrderSingle,
       ThrowsExceptionOnAbsentPartyId) {
  resting_new_order_single.party_id = std::nullopt;

  EXPECT_CALL(registry, add).Times(0);

  EXPECT_THROW(OrderRegistryUpdater::update(registry, resting_new_order_single),
               std::invalid_argument);
}

TEST_F(GeneratorOrderRegistryUpdaterRestingNewOrderSingle,
       ThrowsExceptionOnEmptyPartyId) {
  resting_new_order_single.party_id = simulator::PartyId{{}};

  EXPECT_CALL(registry, add).Times(0);

  EXPECT_THROW(OrderRegistryUpdater::update(registry, resting_new_order_single),
               std::invalid_argument);
}

TEST_F(GeneratorOrderRegistryUpdaterRestingNewOrderSingle,
       ThrowsExceptionOnAbsentSide) {
  resting_new_order_single.side = std::nullopt;

  EXPECT_CALL(registry, add).Times(0);

  EXPECT_THROW(OrderRegistryUpdater::update(registry, resting_new_order_single),
               std::invalid_argument);
}

TEST_F(GeneratorOrderRegistryUpdaterRestingNewOrderSingle, CallsAddOnUpdate) {
  constexpr simulator::OrderPrice price{123.432};
  constexpr simulator::Quantity quantity{334.34};

  resting_new_order_single.order_price = price;
  resting_new_order_single.quantity = quantity;

  EXPECT_CALL(registry, add(testing::_))
      .Times(1)
      .WillOnce(DoAll(
          CheckGeneratedOrderData(owner_id, order_id, price, side, quantity),
          Return(true)));

  EXPECT_NO_THROW(
      OrderRegistryUpdater::update(registry, resting_new_order_single));
}

struct GeneratorOrderRegistryUpdaterRestingOrderCancelReplaceRequest
    : public GeneratorOrderRegistryUpdater {
  GeneratedMessage resting_order_cancel_replace_request =
      makeMessage(MessageType::OrderCancelReplaceRequest);

  GeneratorOrderRegistryUpdaterRestingOrderCancelReplaceRequest() {
    resting_order_cancel_replace_request.order_type =
        Constant::RestingOrderType;
    resting_order_cancel_replace_request.time_in_force =
        Constant::RestingTimeInForce;
    resting_order_cancel_replace_request.client_order_id = order_id;
    resting_order_cancel_replace_request.party_id = owner_id;
  }
};

TEST_F(GeneratorOrderRegistryUpdaterRestingOrderCancelReplaceRequest,
       DoesNotUpdateAggressiveToRegistry) {
  auto message = resting_order_cancel_replace_request;
  message.order_type = Constant::AggressiveOrderType;
  message.time_in_force = Constant::AggressiveTimeInForce;

  EXPECT_CALL(registry, add).Times(0);

  EXPECT_NO_THROW(OrderRegistryUpdater::update(registry, message));
}

TEST_F(GeneratorOrderRegistryUpdaterRestingOrderCancelReplaceRequest,
       ThrowsExceptionOnAbsentClOrdID) {
  resting_order_cancel_replace_request.client_order_id = std::nullopt;

  EXPECT_CALL(registry, add).Times(0);

  EXPECT_THROW(OrderRegistryUpdater::update(
                   registry, resting_order_cancel_replace_request),
               std::invalid_argument);
}

TEST_F(GeneratorOrderRegistryUpdaterRestingOrderCancelReplaceRequest,
       ThrowsExceptionOnEmptyClOrdID) {
  resting_order_cancel_replace_request.client_order_id =
      simulator::ClientOrderId{{}};

  EXPECT_CALL(registry, add).Times(0);

  EXPECT_THROW(OrderRegistryUpdater::update(
                   registry, resting_order_cancel_replace_request),
               std::invalid_argument);
}

TEST_F(GeneratorOrderRegistryUpdaterRestingOrderCancelReplaceRequest,
       ThrowsExceptionOnAbsentPartyId) {
  resting_order_cancel_replace_request.party_id = std::nullopt;

  EXPECT_CALL(registry, add).Times(0);

  EXPECT_THROW(OrderRegistryUpdater::update(
                   registry, resting_order_cancel_replace_request),
               std::invalid_argument);
}

TEST_F(GeneratorOrderRegistryUpdaterRestingOrderCancelReplaceRequest,
       ThrowsExceptionOnEmptyPartyId) {
  resting_order_cancel_replace_request.party_id = simulator::PartyId{{}};

  EXPECT_CALL(registry, add).Times(0);

  EXPECT_THROW(OrderRegistryUpdater::update(
                   registry, resting_order_cancel_replace_request),
               std::invalid_argument);
}

TEST_F(GeneratorOrderRegistryUpdaterRestingOrderCancelReplaceRequest,
       UpdatesByOwnerOnUpdate) {
  constexpr simulator::OrderPrice price{123.432};
  constexpr simulator::Quantity quantity{334.34};

  resting_order_cancel_replace_request.order_price = price;
  resting_order_cancel_replace_request.quantity = quantity;

  EXPECT_CALL(registry, updateByOwner(Eq(owner_id.value()), testing::_))
      .Times(1);

  EXPECT_NO_THROW(OrderRegistryUpdater::update(
      registry, resting_order_cancel_replace_request));
}

struct GeneratorOrderRegistryUpdaterOrderCancelRequest
    : public GeneratorOrderRegistryUpdater {
  GeneratedMessage resting_order_cancel_request =
      makeMessage(MessageType::OrderCancelRequest);

  GeneratorOrderRegistryUpdaterOrderCancelRequest() {
    resting_order_cancel_request.order_type = Constant::RestingOrderType;
    resting_order_cancel_request.time_in_force = Constant::RestingTimeInForce;
    resting_order_cancel_request.party_id = owner_id;
  }
};

TEST_F(GeneratorOrderRegistryUpdaterOrderCancelRequest,
       DoesNotRemoveAggressiveFromRegistry) {
  auto message = resting_order_cancel_request;
  message.order_type = Constant::AggressiveOrderType;
  message.time_in_force = Constant::AggressiveTimeInForce;

  EXPECT_CALL(registry, add).Times(0);

  EXPECT_NO_THROW(OrderRegistryUpdater::update(registry, message));
}

TEST_F(GeneratorOrderRegistryUpdaterOrderCancelRequest,
       ThrowsExceptionOnAbsentPartyId) {
  resting_order_cancel_request.party_id = std::nullopt;

  EXPECT_CALL(registry, add).Times(0);

  EXPECT_THROW(
      OrderRegistryUpdater::update(registry, resting_order_cancel_request),
      std::invalid_argument);
}

TEST_F(GeneratorOrderRegistryUpdaterOrderCancelRequest,
       ThrowsExceptionOnEmptyPartyId) {
  resting_order_cancel_request.party_id = simulator::PartyId{{}};

  EXPECT_CALL(registry, add).Times(0);

  EXPECT_THROW(
      OrderRegistryUpdater::update(registry, resting_order_cancel_request),
      std::invalid_argument);
}

TEST_F(GeneratorOrderRegistryUpdaterOrderCancelRequest,
       CallsRemoveByOwnerOnUpdate) {
  EXPECT_CALL(registry, removeByOwner(Eq(owner_id.value()))).Times(1);

  EXPECT_NO_THROW(
      OrderRegistryUpdater::update(registry, resting_order_cancel_request));
}

struct GeneratorOrderRegistryUpdaterExecutionReport
    : public GeneratorOrderRegistryUpdater {
  GeneratedMessage execution_report = makeMessage(MessageType::ExecutionReport);

  GeneratorOrderRegistryUpdaterExecutionReport() {
    execution_report.client_order_id = order_id;
    execution_report.order_status = simulator::OrderStatus::Option::New;
  }
};

TEST_F(GeneratorOrderRegistryUpdaterExecutionReport,
       ThrowsExceptionOnAbsentClOrdID) {
  execution_report.client_order_id = std::nullopt;

  EXPECT_THROW(OrderRegistryUpdater::update(registry, execution_report),
               std::invalid_argument);
}

TEST_F(GeneratorOrderRegistryUpdaterExecutionReport,
       ThrowsExceptionOnEmptyClOrdID) {
  execution_report.client_order_id = simulator::ClientOrderId{{}};

  EXPECT_THROW(OrderRegistryUpdater::update(registry, execution_report),
               std::invalid_argument);
}

TEST_F(GeneratorOrderRegistryUpdaterExecutionReport,
       ThrowsExceptionOnAbsentOrderStatus) {
  execution_report.order_status = std::nullopt;

  EXPECT_THROW(OrderRegistryUpdater::update(registry, execution_report),
               std::invalid_argument);
}

TEST_F(GeneratorOrderRegistryUpdaterExecutionReport,
       DoesNotCallUpdateByIdentifierIfParitallyFilledDoesNotHaveQuantity) {
  execution_report.order_status =
      simulator::OrderStatus::Option::PartiallyFilled;
  execution_report.quantity = std::nullopt;

  EXPECT_CALL(registry, updateByIdentifier(Eq(order_id.value()), testing::_))
      .Times(0);

  EXPECT_NO_THROW(OrderRegistryUpdater::update(registry, execution_report));
}

TEST_F(GeneratorOrderRegistryUpdaterExecutionReport,
       CallsUpdateByIdentifierIfParitallyFilledHasQuantity) {
  execution_report.order_status =
      simulator::OrderStatus::Option::PartiallyFilled;
  execution_report.quantity = simulator::Quantity{334.34};

  EXPECT_CALL(registry, updateByIdentifier(Eq(order_id.value()), testing::_))
      .Times(1);

  EXPECT_NO_THROW(OrderRegistryUpdater::update(registry, execution_report));
}

TEST_F(GeneratorOrderRegistryUpdaterExecutionReport,
       CallsRemoveByIdentifierIfOrderStatusIsFilled) {
  execution_report.order_status = simulator::OrderStatus::Option::Filled;

  EXPECT_CALL(registry, removeByIdentifier(Eq(order_id.value()))).Times(1);

  EXPECT_NO_THROW(OrderRegistryUpdater::update(registry, execution_report));
}

TEST_F(GeneratorOrderRegistryUpdaterExecutionReport,
       CallsRemoveByIdentifierIfOrderStatusIsCancelled) {
  execution_report.order_status = simulator::OrderStatus::Option::Cancelled;

  EXPECT_CALL(registry, removeByIdentifier(Eq(order_id.value()))).Times(1);

  EXPECT_NO_THROW(OrderRegistryUpdater::update(registry, execution_report));
}

TEST_F(GeneratorOrderRegistryUpdaterExecutionReport,
       CallsRemoveByIdentifierIfOrderStatusIsRejected) {
  execution_report.order_status = simulator::OrderStatus::Option::Rejected;

  EXPECT_CALL(registry, removeByIdentifier(Eq(order_id.value()))).Times(1);

  EXPECT_NO_THROW(OrderRegistryUpdater::update(registry, execution_report));
}

// The test actually checks only when `registry` is StrictMock
TEST_F(GeneratorOrderRegistryUpdaterExecutionReport,
       RegistryMethodsAreNotCalledIfOrderStatusIsNew) {
  execution_report.order_status = simulator::OrderStatus::Option::New;
  EXPECT_NO_THROW(OrderRegistryUpdater::update(registry, execution_report));
}

// The test actually checks only when `registry` is StrictMock
TEST_F(GeneratorOrderRegistryUpdaterExecutionReport,
       RegistryMethodsAreNotCalledIfOrderStatusIsModified) {
  execution_report.order_status = simulator::OrderStatus::Option::Modified;
  EXPECT_NO_THROW(OrderRegistryUpdater::update(registry, execution_report));
}

// NOLINTEND(*magic-numbers*)

}  // namespace
}  // namespace Simulator::Generator
