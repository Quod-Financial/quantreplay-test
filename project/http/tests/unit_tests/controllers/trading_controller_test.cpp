#include "ih/controllers/trading_controller.hpp"

#include <gmock/gmock.h>

#include "core/common/return_code.hpp"
#include "ih/utils/response_formatters.hpp"
#include "mocks/trading_admin_receiver_mock.hpp"

namespace Simulator::Http {
namespace {

using namespace testing;  // NOLINT

struct HttpTradingControllerTest : Test {
  auto bind_channel() -> void {
    std::shared_ptr<simulator::middleware::TradingAdminRequestReceiver>
        receiver_pointer{std::addressof(receiver_), [](auto* /*pointer*/) {}};
    simulator::middleware::bind_trading_admin_channel(receiver_pointer);
  }

  TradingController controller;

 protected:
  NiceMock<Mock::TradingAdminRequestReceiver> receiver_;

  auto TearDown() -> void override {
    simulator::middleware::release_trading_admin_channel();
  }
};

struct HttpTradingControllerHaltTest : HttpTradingControllerTest {
  auto set_halt_request_reply(
      simulator::protocol::HaltPhaseReply::Result result) -> void {
    ON_CALL(receiver_,
            process(A<const simulator::protocol::HaltPhaseRequest&>(),
                    A<simulator::protocol::HaltPhaseReply&>()))
        .WillByDefault(
            Invoke([result]([[maybe_unused]] const auto& request, auto& reply) {
              reply.result = result;
            }));
  }
};

TEST_F(HttpTradingControllerHaltTest,
       RepliesInternalServerErrorOnHaltRequestWithoutJsonBody) {
  const auto [code, body] = TradingController{}.halt("body");

  ASSERT_EQ(code, Pistache::Http::Code::Internal_Server_Error);
  ASSERT_EQ(body,
            formatResultResponse("Failed to unmarshall request body: failed to "
                                 "parse Halt JSON object."));
}

TEST_F(HttpTradingControllerHaltTest,
       RepliesInternalServerErrorOnHaltRequestWithoutAllowCancelsField) {
  const auto [code, body] = TradingController{}.halt("{}");

  ASSERT_EQ(code, Pistache::Http::Code::Internal_Server_Error);
  ASSERT_EQ(
      body,
      formatResultResponse(
          R"(Failed to unmarshall request body: missing required field "allowCancels".)"));
}

TEST_F(HttpTradingControllerHaltTest,
       RepliesInternalServerErrorOnHaltIfReceiverIsNotBound) {
  const auto [code, body] = controller.halt(R"({"allowCancels": true})");

  ASSERT_EQ(code, Pistache::Http::Code::Internal_Server_Error);
  ASSERT_EQ(body, formatResultResponse("Failed to process the request."));
}

TEST_F(HttpTradingControllerHaltTest, RepliesOkIfHaltRequestIsApplied) {
  bind_channel();
  set_halt_request_reply(simulator::protocol::HaltPhaseReply::Result::Halted);

  const auto [code, body] = controller.halt(R"({"allowCancels": true})");

  ASSERT_EQ(code, Pistache::Http::Code::Ok);
  ASSERT_EQ(body, formatResultResponse("The market was successfully halted."));
}

TEST_F(HttpTradingControllerHaltTest, RepliesConflictIfMarketIsAlreadyHalted) {
  bind_channel();
  set_halt_request_reply(
      simulator::protocol::HaltPhaseReply::Result::AlreadyHaltedByRequest);

  const auto [code, body] = controller.halt(R"({"allowCancels": true})");

  ASSERT_EQ(code, Pistache::Http::Code::Conflict);
  ASSERT_EQ(body, formatResultResponse("The market is already halted."));
}

TEST_F(HttpTradingControllerHaltTest, RepliesNotFoundIfNoActivePhase) {
  bind_channel();
  set_halt_request_reply(
      simulator::protocol::HaltPhaseReply::Result::NoActivePhase);

  const auto [code, body] = controller.halt(R"({"allowCancels": true})");

  ASSERT_EQ(code, Pistache::Http::Code::Not_Found);
  ASSERT_EQ(body, formatResultResponse("There is no phase to halt."));
}

TEST_F(HttpTradingControllerHaltTest, RepliesConflictIfUnableToHalt) {
  bind_channel();
  set_halt_request_reply(
      simulator::protocol::HaltPhaseReply::Result::UnableToHalt);

  const auto [code, body] = controller.halt(R"({"allowCancels": true})");

  ASSERT_EQ(code, Pistache::Http::Code::Conflict);
  ASSERT_EQ(body, formatResultResponse("Unable to halt the phase."));
}

struct HttpTradingControllerResumeTest : HttpTradingControllerTest {
  auto set_resume_request_reply(
      simulator::protocol::ResumePhaseReply::Result result) -> void {
    ON_CALL(receiver_,
            process(A<const simulator::protocol::ResumePhaseRequest&>(),
                    A<simulator::protocol::ResumePhaseReply&>()))
        .WillByDefault(
            Invoke([result]([[maybe_unused]] const auto& request, auto& reply) {
              reply.result = result;
            }));
  }
};

TEST_F(HttpTradingControllerResumeTest,
       RepliesInternalServerErrorOnResumeRequestIfReceiverIsNotBound) {
  const auto [code, body] = controller.resume();

  ASSERT_EQ(code, Pistache::Http::Code::Internal_Server_Error);
  ASSERT_EQ(body, formatResultResponse("Failed to process the request."));
}

TEST_F(HttpTradingControllerResumeTest, RepliesOkIfResumeRequestIsApplied) {
  bind_channel();
  set_resume_request_reply(
      simulator::protocol::ResumePhaseReply::Result::Resumed);

  const auto [code, body] = controller.resume();

  ASSERT_EQ(code, Pistache::Http::Code::Ok);
  ASSERT_EQ(body, formatResultResponse("The market was successfully resumed."));
}

TEST_F(HttpTradingControllerResumeTest, RepliesConflictIfNoRequestedHalt) {
  bind_channel();
  set_resume_request_reply(
      simulator::protocol::ResumePhaseReply::Result::NoRequestedHalt);

  const auto [code, body] = controller.resume();

  ASSERT_EQ(code, Pistache::Http::Code::Conflict);
  ASSERT_EQ(body,
            formatResultResponse("There is no halt request to terminate."));
}

struct HttpTradingControllerStoreMarketStateTest : HttpTradingControllerTest {
  auto set_store_request_reply(simulator::core::code::StoreMarketState result)
      -> void {
    ON_CALL(receiver_,
            process(A<const simulator::protocol::StoreMarketStateRequest&>(),
                    A<simulator::protocol::StoreMarketStateReply&>()))
        .WillByDefault(
            Invoke([result]([[maybe_unused]] const auto& request, auto& reply) {
              reply.result = result;
            }));
  }
};

TEST_F(
    HttpTradingControllerStoreMarketStateTest,
    RepliesInternalServerErrorOnStoreMarketStateRequestIfReceiverIsNotBound) {
  const auto [code, body] = controller.store_market_state();

  ASSERT_EQ(code, Pistache::Http::Code::Internal_Server_Error);
  ASSERT_EQ(body, formatResultResponse("Failed to process the request."));
}

TEST_F(HttpTradingControllerStoreMarketStateTest,
       RepliesCreatedIfMarketStateWasStored) {
  bind_channel();
  set_store_request_reply(simulator::core::code::StoreMarketState::Stored);

  const auto [code, body] = controller.store_market_state();

  ASSERT_EQ(code, Pistache::Http::Code::Created);
  ASSERT_EQ(body,
            formatResultResponse(
                "Matching engine state has been successfully persisted."));
}

TEST_F(HttpTradingControllerStoreMarketStateTest,
       RepliesForbiddenIfPersistenceDisabled) {
  bind_channel();
  set_store_request_reply(
      simulator::core::code::StoreMarketState::PersistenceDisabled);

  const auto [code, body] = controller.store_market_state();

  ASSERT_EQ(code, Pistache::Http::Code::Forbidden);
  ASSERT_EQ(body, formatResultResponse("Persistence is disabled."));
}

TEST_F(HttpTradingControllerStoreMarketStateTest,
       RepliesConflictIfPersistenceFilePathIsEmpty) {
  bind_channel();
  set_store_request_reply(
      simulator::core::code::StoreMarketState::PersistenceFilePathIsEmpty);

  const auto [code, body] = controller.store_market_state();

  ASSERT_EQ(code, Pistache::Http::Code::Conflict);
  ASSERT_EQ(body, formatResultResponse("The persistence file path is empty."));
}

TEST_F(HttpTradingControllerStoreMarketStateTest,
       RepliesConflictIfPersistenceFilePathUnreachable) {
  bind_channel();
  set_store_request_reply(simulator::core::code::StoreMarketState::
                              PersistenceFilePathIsUnreachable);

  const auto [code, body] = controller.store_market_state();

  ASSERT_EQ(code, Pistache::Http::Code::Conflict);
  ASSERT_EQ(body,
            formatResultResponse("The persistence file path is unreachable."));
}

TEST_F(HttpTradingControllerStoreMarketStateTest,
       RepliesConflictIfErrorWhenOpeningPersistenceFile) {
  bind_channel();
  set_store_request_reply(
      simulator::core::code::StoreMarketState::ErrorWhenOpeningPersistenceFile);

  const auto [code, body] = controller.store_market_state();

  ASSERT_EQ(code, Pistache::Http::Code::Conflict);
  ASSERT_EQ(body,
            formatResultResponse(
                "An error occurs when opening the persistence file."));
}

TEST_F(HttpTradingControllerStoreMarketStateTest,
       RepliesConflictIfErrorWhenWritingToPersistenceFile) {
  bind_channel();
  set_store_request_reply(simulator::core::code::StoreMarketState::
                              ErrorWhenWritingToPersistenceFile);

  const auto [code, body] = controller.store_market_state();

  ASSERT_EQ(code, Pistache::Http::Code::Conflict);
  ASSERT_EQ(body,
            formatResultResponse(
                "An error occurs when writing to the persistence file."));
}

struct HttpTradingControllerRecoverMarketStateTest : HttpTradingControllerTest {
  auto set_recover_request_reply(
      simulator::protocol::RecoverMarketStateReply recover_reply) -> void {
    ON_CALL(receiver_,
            process(A<const simulator::protocol::RecoverMarketStateRequest&>(),
                    A<simulator::protocol::RecoverMarketStateReply&>()))
        .WillByDefault(Invoke(
            [recover_reply = std::move(recover_reply)](
                [[maybe_unused]] const auto& request, auto& reply) mutable {
              reply = std::move(recover_reply);
            }));
  }
};

TEST_F(
    HttpTradingControllerRecoverMarketStateTest,
    RepliesInternalServerErrorOnRecoverMarketStateRequestIfReceiverIsNotBound) {
  const auto [code, body] = controller.recover_market_state();

  ASSERT_EQ(code, Pistache::Http::Code::Internal_Server_Error);
  ASSERT_EQ(body, formatResultResponse("Failed to process the request."));
}

TEST_F(HttpTradingControllerRecoverMarketStateTest,
       RepliesCreatedIfMarketStateWasRecovered) {
  bind_channel();
  set_recover_request_reply(
      {simulator::core::code::RecoverMarketState::Recovered, {}});

  const auto [code, body] = controller.recover_market_state();

  ASSERT_EQ(code, Pistache::Http::Code::Created);
  ASSERT_EQ(body,
            formatResultResponse(
                "Matching engine state has been successfully recovered."));
}

TEST_F(HttpTradingControllerRecoverMarketStateTest,
       RepliesForbiddenIfPersistenceDisabled) {
  bind_channel();
  set_recover_request_reply(
      {simulator::core::code::RecoverMarketState::PersistenceDisabled, {}});

  const auto [code, body] = controller.recover_market_state();

  ASSERT_EQ(code, Pistache::Http::Code::Forbidden);
  ASSERT_EQ(body, formatResultResponse("Persistence is disabled."));
}

TEST_F(HttpTradingControllerRecoverMarketStateTest,
       RepliesConflictIfPersistenceFilePathIsEmpty) {
  bind_channel();
  set_recover_request_reply(
      {simulator::core::code::RecoverMarketState::PersistenceFilePathIsEmpty,
       {}});

  const auto [code, body] = controller.recover_market_state();

  ASSERT_EQ(code, Pistache::Http::Code::Conflict);
  ASSERT_EQ(body, formatResultResponse("The persistence file path is empty."));
}

TEST_F(HttpTradingControllerRecoverMarketStateTest,
       RepliesConflictIfPersistenceFilePathUnreachable) {
  bind_channel();
  set_recover_request_reply({simulator::core::code::RecoverMarketState::
                                 PersistenceFilePathIsUnreachable,
                             {}});

  const auto [code, body] = controller.recover_market_state();

  ASSERT_EQ(code, Pistache::Http::Code::Conflict);
  ASSERT_EQ(body,
            formatResultResponse("The persistence file path is unreachable."));
}

TEST_F(HttpTradingControllerRecoverMarketStateTest,
       RepliesConflictIfErrorWhenOpeningPersistenceFile) {
  bind_channel();
  set_recover_request_reply({simulator::core::code::RecoverMarketState::
                                 ErrorWhenOpeningPersistenceFile,
                             {}});

  const auto [code, body] = controller.recover_market_state();

  ASSERT_EQ(code, Pistache::Http::Code::Conflict);
  ASSERT_EQ(body,
            formatResultResponse(
                "An error occurs when opening the persistence file."));
}

TEST_F(HttpTradingControllerRecoverMarketStateTest,
       RepliesInternalServerErrorIfPersistenceFileIsMalformed) {
  bind_channel();
  set_recover_request_reply(
      {simulator::core::code::RecoverMarketState::PersistenceFileIsMalformed,
       {}});

  const auto [code, body] = controller.recover_market_state();

  ASSERT_EQ(code, Pistache::Http::Code::Internal_Server_Error);
  ASSERT_EQ(body, formatResultResponse("The persistence file is malformed."));
}

TEST_F(HttpTradingControllerRecoverMarketStateTest,
       RepliesErrorMessageIfPersistenceFileIsMalformed) {
  bind_channel();
  set_recover_request_reply(
      {simulator::core::code::RecoverMarketState::PersistenceFileIsMalformed,
       {"Error message"}});

  const auto [code, body] = controller.recover_market_state();

  ASSERT_EQ(code, Pistache::Http::Code::Internal_Server_Error);
  ASSERT_EQ(body,
            formatResultResponse(
                "The persistence file is malformed: Error message."));
}

}  // namespace
}  // namespace Simulator::Http