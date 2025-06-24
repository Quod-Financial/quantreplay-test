#ifndef SIMULATOR_DATA_LAYER_TESTS_TEST_UTILS_MATCHERS_HPP_
#define SIMULATOR_DATA_LAYER_TESTS_TEST_UTILS_MATCHERS_HPP_

#include <gmock/gmock.h>

namespace Simulator::DataLayer {

using namespace ::testing;

MATCHER_P(IsUnexpected, error_message, "") {
  return ExplainMatchResult(IsFalse(), arg.has_value(), result_listener) &&
         ExplainMatchResult(StrEq(error_message), arg.error(), result_listener);
}

}  // namespace Simulator::DataLayer

#endif  // SIMULATOR_DATA_LAYER_TESTS_TEST_UTILS_MATCHERS_HPP_
