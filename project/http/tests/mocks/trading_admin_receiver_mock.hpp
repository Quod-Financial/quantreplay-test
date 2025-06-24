#ifndef SIMULATOR_HTTP_TESTS_MOCKS_TRADING_PHASE_ADMIN_RECEIVER_MOCK_HPP_
#define SIMULATOR_HTTP_TESTS_MOCKS_TRADING_PHASE_ADMIN_RECEIVER_MOCK_HPP_

#include <gmock/gmock.h>

#include "middleware/channels/trading_admin_channel.hpp"

namespace Simulator::Http::Mock {

struct TradingAdminRequestReceiver
    : public simulator::middleware::TradingAdminRequestReceiver {
  MOCK_METHOD(void,
              process,
              (const simulator::protocol::HaltPhaseRequest& request,
               simulator::protocol::HaltPhaseReply& reply),
              (override));

  MOCK_METHOD(void,
              process,
              (const simulator::protocol::ResumePhaseRequest& request,
               simulator::protocol::ResumePhaseReply& reply),
              (override));

  MOCK_METHOD(void,
              process,
              (const simulator::protocol::StoreMarketStateRequest& request,
               simulator::protocol::StoreMarketStateReply& reply),
              (override));

  MOCK_METHOD(void,
              process,
              (const simulator::protocol::RecoverMarketStateRequest& request,
               simulator::protocol::RecoverMarketStateReply& reply),
              (override));
};

}  // namespace Simulator::Http::Mock

#endif  // SIMULATOR_HTTP_TESTS_MOCKS_TRADING_PHASE_ADMIN_RECEIVER_MOCK_HPP_
