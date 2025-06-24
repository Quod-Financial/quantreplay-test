#include <gtest/gtest.h>

#include "ih/marshalling/json/halt.hpp"
#include "protocol/admin/trading_phase.hpp"

namespace Simulator::Http::Json {
namespace {

TEST(HttpMarshallingJsonHalt, ThrowsExceptionOnAllowCancelsNotSet) {
  const std::string json = R"({"notAllowCancels": true})";
  simulator::protocol::HaltPhaseRequest request;

  EXPECT_ANY_THROW(HaltUnmarshaller::unmarshall(json, request));
}

TEST(HttpMarshallingJsonHalt, UnmarshallsAllowCancels) {
  const std::string json = R"({"allowCancels": true})";
  simulator::protocol::HaltPhaseRequest request;

  HaltUnmarshaller::unmarshall(json, request);
  EXPECT_TRUE(request.allow_cancels);
}

}  // namespace
}  // namespace Simulator::Http::Json