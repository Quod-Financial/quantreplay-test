#include "ih/registry/generated_orders_registry.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "core/domain/attributes.hpp"
#include "ih/registry/generated_order_data.hpp"
#include "ih/registry/generated_orders_registry_impl.hpp"

namespace Simulator::Generator {
namespace {

class NoopGeneratedOrderCallback {
 public:
  operator GeneratedOrdersRegistry::Visitor() {
    return [this](GeneratedOrderData const& _order) { process(_order); };
  }

  MOCK_METHOD(void, process, (GeneratedOrderData const&));
};

class Generator_GeneratedOrdersRegistry : public testing::Test {
 public:
  static constexpr auto buy_side = simulator::Side::Option::Buy;
  static constexpr auto sell_side = simulator::Side::Option::Sell;
  const simulator::PartyId owner_id{"OwnerID"};
  const simulator::ClientOrderId order_id{"OrderID"};

  auto registry() -> GeneratedOrdersRegistry& { return m_registry; }

  void addRegisteredOrder(
      simulator::PartyId _ownerID,
      simulator::ClientOrderId _orderID,
      simulator::Side _side,
      std::optional<simulator::OrderPrice> _optPrice = std::nullopt,
      std::optional<simulator::Quantity> _optQuantity = std::nullopt) {
    ASSERT_TRUE(m_registry.add(makeOrderData(std::move(_ownerID),
                                             std::move(_orderID),
                                             _side,
                                             _optPrice,
                                             _optQuantity)));
  }

  static auto makeOrderData(
      simulator::PartyId _ownerID,
      simulator::ClientOrderId _orderID,
      simulator::Side _side,
      std::optional<simulator::OrderPrice> _optPrice = std::nullopt,
      std::optional<simulator::Quantity> _optQuantity = std::nullopt)
      -> GeneratedOrderData {
    GeneratedOrderData::Builder builder{
        std::move(_ownerID), std::move(_orderID), _side};
    if (_optPrice.has_value()) {
      builder.setPrice(*_optPrice);
    }
    if (_optQuantity.has_value()) {
      builder.setQuantity(*_optQuantity);
    }
    return GeneratedOrderData{std::move(builder)};
  }

  static auto createOrderDataUpdate(
      simulator::ClientOrderId _updatedOrdID,
      std::optional<simulator::OrderPrice> _optPrice = std::nullopt,
      std::optional<simulator::Quantity> _optQuantity = std::nullopt)
      -> GeneratedOrderData::Patch {
    GeneratedOrderData::Patch patch{};
    patch.setUpdatedID(std::move(_updatedOrdID));
    if (_optPrice.has_value()) {
      patch.setUpdatedPrice(*_optPrice);
    }
    if (_optQuantity.has_value()) {
      patch.setUpdatedQuantity(*_optQuantity);
    }
    return patch;
  }

 private:
  GeneratedOrdersRegistryImpl m_registry;
};

TEST_F(Generator_GeneratedOrdersRegistry, AddsOrderData) {
  GeneratedOrderData orderData = makeOrderData(owner_id, order_id, buy_side);
  EXPECT_TRUE(registry().add(std::move(orderData)));
}

TEST_F(Generator_GeneratedOrdersRegistry, DoesNotAddDuplicatedOwnerID) {
  addRegisteredOrder(owner_id, order_id, sell_side);

  const simulator::ClientOrderId new_order_id{"NewOrderID"};
  GeneratedOrderData orderData =
      makeOrderData(owner_id, new_order_id, buy_side);

  EXPECT_FALSE(registry().add(std::move(orderData)));
}

TEST_F(Generator_GeneratedOrdersRegistry, DoesNotAddDuplicatedOrderID) {
  addRegisteredOrder(owner_id, order_id, sell_side);

  const simulator::PartyId new_owner_id{"NewOwnerID"};
  GeneratedOrderData orderData =
      makeOrderData(new_owner_id, order_id, buy_side);

  EXPECT_FALSE(registry().add(std::move(orderData)));
}

TEST_F(Generator_GeneratedOrdersRegistry, DoesNotFindByNonexistentOwnerID) {
  addRegisteredOrder(owner_id, order_id, buy_side);

  const std::string search_order_owner{"AnotherOrderOwner"};
  auto optFoundOrder = registry().findByOwner(search_order_owner);
  EXPECT_FALSE(optFoundOrder.has_value());
}

TEST_F(Generator_GeneratedOrdersRegistry, FindsByOwnerID) {
  addRegisteredOrder(owner_id, order_id, sell_side);

  auto optFoundOrder = registry().findByOwner(owner_id.value());
  ASSERT_TRUE(optFoundOrder.has_value());

  EXPECT_EQ(optFoundOrder->getOwnerID(), owner_id);
  EXPECT_EQ(optFoundOrder->getOrigOrderID().value(), order_id.value());
  EXPECT_EQ(optFoundOrder->getOrderSide(), sell_side);
}

TEST_F(Generator_GeneratedOrdersRegistry, DoesNotFindByNonexistentOrderID) {
  addRegisteredOrder(owner_id, order_id, sell_side);

  const std::string search_order_id{"AnotherOrderID"};
  auto optFoundOrder = registry().findByIdentifier(search_order_id);
  EXPECT_FALSE(optFoundOrder.has_value());
}

TEST_F(Generator_GeneratedOrdersRegistry, FindsByOrderID) {
  addRegisteredOrder(owner_id, order_id, buy_side);

  auto optFoundOrder = registry().findByIdentifier(order_id.value());
  ASSERT_TRUE(optFoundOrder.has_value());

  EXPECT_EQ(optFoundOrder->getOwnerID(), owner_id);
  EXPECT_EQ(optFoundOrder->getOrigOrderID().value(), order_id.value());
  EXPECT_EQ(optFoundOrder->getOrderSide(), buy_side);
}

TEST_F(Generator_GeneratedOrdersRegistry, DoesNotUpdateByNonexistentOwnerID) {
  const simulator::ClientOrderId updatedOrdID{"UpdatedOrdID"};

  GeneratedOrderData::Patch patch = createOrderDataUpdate(updatedOrdID);

  EXPECT_FALSE(registry().updateByOwner(owner_id.value(), std::move(patch)));
}

TEST_F(Generator_GeneratedOrdersRegistry, UpdatesByOwner) {
  const simulator::ClientOrderId orig_ord_id{"OrigOrdID"};
  addRegisteredOrder(owner_id, orig_ord_id, buy_side);

  const simulator::ClientOrderId updated_ord_id{"UpdatedOrdID"};
  GeneratedOrderData::Patch patch = createOrderDataUpdate(updated_ord_id);
  ASSERT_TRUE(registry().updateByOwner(owner_id.value(), std::move(patch)));

  auto optUpdatedData = registry().findByOwner(owner_id.value());
  ASSERT_TRUE(optUpdatedData.has_value());

  EXPECT_EQ(optUpdatedData->getOwnerID(), owner_id);
  EXPECT_EQ(optUpdatedData->getOrderID(), updated_ord_id);
  EXPECT_EQ(optUpdatedData->getOrigOrderID().value(), orig_ord_id.value());
  EXPECT_EQ(optUpdatedData->getOrderSide(), buy_side);
}

TEST_F(Generator_GeneratedOrdersRegistry, DoesNotUpdateByNonexistentOrderID) {
  const simulator::ClientOrderId updatedOrdID{"UpdatedOrdID"};

  GeneratedOrderData::Patch patch = createOrderDataUpdate(updatedOrdID);

  EXPECT_FALSE(
      registry().updateByIdentifier(order_id.value(), std::move(patch)));
}

TEST_F(Generator_GeneratedOrdersRegistry, UpdatesByOrderID) {
  const simulator::ClientOrderId origOrdID{"OrigOrdID"};
  const simulator::ClientOrderId updatedOrdID{"UpdatedOrdID"};

  addRegisteredOrder(owner_id, origOrdID, sell_side);

  GeneratedOrderData::Patch patch = createOrderDataUpdate(updatedOrdID);
  ASSERT_TRUE(
      registry().updateByIdentifier(origOrdID.value(), std::move(patch)));

  auto optUpdatedData = registry().findByIdentifier(updatedOrdID.value());
  ASSERT_TRUE(optUpdatedData.has_value());

  EXPECT_EQ(optUpdatedData->getOwnerID(), owner_id);
  EXPECT_EQ(optUpdatedData->getOrderID(), updatedOrdID);
  EXPECT_EQ(optUpdatedData->getOrigOrderID().value(), origOrdID.value());
  EXPECT_EQ(optUpdatedData->getOrderSide(), sell_side);
}

TEST_F(Generator_GeneratedOrdersRegistry, DoesNotRemoveByNonexistentOwnerID) {
  EXPECT_FALSE(registry().removeByOwner(owner_id.value()));
}

TEST_F(Generator_GeneratedOrdersRegistry, RemovesByOwnerID) {
  addRegisteredOrder(owner_id, order_id, buy_side);

  EXPECT_TRUE(registry().removeByOwner(owner_id.value()));

  EXPECT_FALSE(registry().findByOwner(owner_id.value()).has_value());
  EXPECT_FALSE(registry().findByIdentifier(order_id.value()).has_value());
}

TEST_F(Generator_GeneratedOrdersRegistry, DoesNotRemoveByNonexistentOrderID) {
  EXPECT_FALSE(registry().removeByIdentifier(order_id.value()));
}

TEST_F(Generator_GeneratedOrdersRegistry, RemovesByOrderID) {
  addRegisteredOrder(owner_id, order_id, sell_side);

  EXPECT_TRUE(registry().removeByIdentifier(order_id.value()));

  EXPECT_FALSE(registry().findByOwner(owner_id.value()).has_value());
  EXPECT_FALSE(registry().findByIdentifier(order_id.value()).has_value());
}

TEST_F(Generator_GeneratedOrdersRegistry, InvokesVisitorForEach) {
  addRegisteredOrder(simulator::PartyId{"Owner1"},
                     simulator::ClientOrderId{"OrderID1"},
                     sell_side);
  addRegisteredOrder(simulator::PartyId{"Owner2"},
                     simulator::ClientOrderId{"OrderID2"},
                     sell_side);
  addRegisteredOrder(simulator::PartyId{"Owner3"},
                     simulator::ClientOrderId{"OrderID3"},
                     buy_side);

  NoopGeneratedOrderCallback visitor;
  EXPECT_CALL(visitor, process).Times(3);

  registry().forEach(visitor);
}

TEST_F(Generator_GeneratedOrdersRegistry, SelectsByPredicate) {
  addRegisteredOrder(simulator::PartyId{"Owner1"},
                     simulator::ClientOrderId{"OrderID1"},
                     sell_side);
  addRegisteredOrder(simulator::PartyId{"Owner2"},
                     simulator::ClientOrderId{"OrderID2"},
                     sell_side);
  addRegisteredOrder(owner_id, order_id, buy_side);

  auto const pred = [](GeneratedOrderData const& _order) {
    return _order.getOrderSide() == buy_side;
  };

  const auto selected = registry().selectBy(pred);
  ASSERT_EQ(selected.size(), 1);

  EXPECT_EQ(selected.front().getOwnerID(), owner_id);
  EXPECT_EQ(selected.front().getOrderID(), order_id);
  EXPECT_EQ(selected.front().getOrderSide(), buy_side);
}
}  // namespace
}  // namespace Simulator::Generator