#include "ih/registry/generated_order_data.hpp"

#include <gtest/gtest.h>

#include <stdexcept>

#include "core/domain/attributes.hpp"

namespace Simulator::Generator {
namespace {

struct GeneratedOrderDataBuilder : public ::testing::Test {
  static constexpr auto side = simulator::Side::Option::Buy;
  const simulator::ClientOrderId order_id{"OrderID"};
  const simulator::PartyId owner_id{"CounterpartyID"};

  GeneratedOrderData::Builder builder{owner_id, order_id, side};
};

TEST_F(GeneratedOrderDataBuilder, ThrowsExceptionOnEmptyPartyId) {
  const simulator::PartyId empty_owner_id{{}};

  auto createBuilder = [&] {
    return GeneratedOrderData::Builder{empty_owner_id, order_id, side};
  };

  EXPECT_THROW(createBuilder(), std::invalid_argument);
}

TEST_F(GeneratedOrderDataBuilder, ThrowsExceptionOnEmptyClientOrderId) {
  const simulator::ClientOrderId empty_order_id{{}};

  auto createBuilder = [&] {
    return GeneratedOrderData::Builder{owner_id, empty_order_id, side};
  };

  EXPECT_THROW(createBuilder(), std::invalid_argument);
}

struct GeneratedOrderDataPatch : public ::testing::Test {
  GeneratedOrderData::Patch patch;
};

TEST_F(GeneratedOrderDataPatch, ThrowsExceptionOnEmptyClientOrderId) {
  const simulator::ClientOrderId updatedID{{}};
  EXPECT_THROW(patch.setUpdatedID(updatedID), std::invalid_argument);
}

struct GeneratedOrderDataTest : public GeneratedOrderDataBuilder {
  GeneratedOrderData::Patch patch;
};

TEST_F(GeneratedOrderDataTest, InitializedFromDefaultBuilderAttribute) {
  const GeneratedOrderData data{std::move(builder)};

  EXPECT_EQ(data.getOwnerID(), owner_id);
  EXPECT_EQ(data.getOrderID(), order_id);
  EXPECT_EQ(data.getOrigOrderID().value(), order_id.value());
  EXPECT_EQ(data.getOrderSide(), side);

  EXPECT_DOUBLE_EQ(data.getOrderPx().value(), 0.);
  EXPECT_DOUBLE_EQ(data.getOrderQty().value(), 0.);
}

TEST_F(GeneratedOrderDataTest, SetsPriceFromBuilder) {
  constexpr simulator::OrderPrice price{124.53};

  builder.setPrice(price);
  const GeneratedOrderData data{std::move(builder)};

  EXPECT_DOUBLE_EQ(data.getOrderPx().value(), price.value());
}

TEST_F(GeneratedOrderDataTest, SetsQuantityFromBuilder) {
  constexpr simulator::Quantity quantity{124.53};

  builder.setQuantity(quantity);
  const GeneratedOrderData data{std::move(builder)};

  EXPECT_DOUBLE_EQ(data.getOrderQty().value(), quantity.value());
}

TEST_F(GeneratedOrderDataTest, DoesNotApplyEmptyPatch) {
  GeneratedOrderData data{std::move(builder)};
  data.apply(std::move(patch));

  EXPECT_EQ(data.getOwnerID(), owner_id);
  EXPECT_EQ(data.getOrderID(), order_id);
  EXPECT_EQ(data.getOrigOrderID().value(), order_id.value());
  EXPECT_EQ(data.getOrderSide(), side);

  EXPECT_DOUBLE_EQ(data.getOrderPx().value(), 0.);
  EXPECT_DOUBLE_EQ(data.getOrderQty().value(), 0.);
}

TEST_F(GeneratedOrderDataTest, AppliesClientOrderIdFromPatch) {
  const simulator::ClientOrderId updatedOrderID{"updated-OrderID"};
  patch.setUpdatedID(updatedOrderID);

  GeneratedOrderData data{std::move(builder)};
  data.apply(std::move(patch));

  EXPECT_EQ(data.getOrderID(), updatedOrderID);
  EXPECT_EQ(data.getOrigOrderID().value(), order_id.value());
}

TEST_F(GeneratedOrderDataTest, AppliesPriceFromPatch) {
  constexpr simulator::OrderPrice old_price{123.43};
  constexpr simulator::OrderPrice new_price{123.44};

  builder.setPrice(old_price);
  GeneratedOrderData data{std::move(builder)};

  patch.setUpdatedPrice(new_price);
  data.apply(std::move(patch));

  EXPECT_DOUBLE_EQ(data.getOrderPx().value(), new_price.value());
}

TEST_F(GeneratedOrderDataTest, AppliesQuantityFromPatch) {
  constexpr simulator::Quantity old_quantity{123.43};
  constexpr simulator::Quantity new_quantity{123.44};

  builder.setQuantity(old_quantity);
  GeneratedOrderData data{std::move(builder)};

  patch.setUpdatedQuantity(new_quantity);
  data.apply(std::move(patch));

  EXPECT_DOUBLE_EQ(data.getOrderQty().value(), new_quantity.value());
}

}  // namespace
}  // namespace Simulator::Generator