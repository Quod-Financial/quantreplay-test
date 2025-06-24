#include "ih/context/order_market_data_provider.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "mocks/trading_request_channel.hpp"

using namespace Simulator;
using namespace Simulator::Generator;

using ::testing::A;
using ::testing::DoAll;
using ::testing::Eq;
using ::testing::Optional;
using ::testing::Return;
using ::testing::SaveArg;

class Generator_OrderMarketDataProvider : public testing::Test {
 public:

  auto bind_trading_request_channel() -> void {
    std::shared_ptr<Mock::TradingRequestReceiver> receiver_pointer{
        std::addressof(request_receiver), [](auto* /*pointer*/) {}};
    simulator::middleware::bind_trading_request_channel(receiver_pointer);
  }

  Mock::TradingRequestReceiver request_receiver;
  OrderMarketDataProvider market_data_provider;

 private:
  auto TearDown() -> void override {
    simulator::middleware::release_trading_request_channel();
  }
};