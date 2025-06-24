#include "ih/random/algorithm/utils/attributes_setter.hpp"

#include <gtest/gtest.h>

#include "core/domain/enumerators.hpp"
#include "ih/adaptation/generated_message.hpp"

namespace Simulator::Generator::Random {
namespace {

struct GeneratorRandomAttributesSetter : public ::testing::Test {
  GeneratedMessage message;
};

TEST_F(GeneratorRandomAttributesSetter, SetsMessageType) {
  constexpr auto message_type = MessageType::OrderCancelReplaceRequest;
  AttributesSetter::set(message, message_type);
  EXPECT_EQ(message.message_type, message_type);
}

TEST_F(GeneratorRandomAttributesSetter, SetsOrderType) {
  constexpr auto order_type = simulator::OrderType::Option::Market;
  AttributesSetter::set(message, order_type);
  EXPECT_EQ(message.order_type, order_type);
}

TEST_F(GeneratorRandomAttributesSetter, SetsSide) {
  constexpr auto side = simulator::Side::Option::Buy;
  AttributesSetter::set(message, side);
  EXPECT_EQ(message.side, side);
}

TEST_F(GeneratorRandomAttributesSetter, SetsTimeInForce) {
  constexpr auto tif = simulator::TimeInForce::Option::ImmediateOrCancel;
  AttributesSetter::set(message, tif);
  EXPECT_EQ(message.time_in_force, tif);
}

}  // namespace
}  // namespace Simulator::Generator::Random