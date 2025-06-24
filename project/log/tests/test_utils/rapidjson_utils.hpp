#ifndef SIMULATOR_LOG_TESTS_TEST_UTILS_RAPIDJSON_PRINTER_HPP_
#define SIMULATOR_LOG_TESTS_TEST_UTILS_RAPIDJSON_PRINTER_HPP_

#include <gmock/gmock.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace rapidjson {

inline auto PrintTo(const Document& document, std::ostream* os) -> void {
  StringBuffer buffer;
  Writer<StringBuffer> writer(buffer);
  document.Accept(writer);
  *os << buffer.GetString();
}

}  // namespace rapidjson

namespace simulator::log {

using namespace ::testing;

MATCHER_P(HasMember, key, "") {
  if (!arg.HasMember(key)) {
    *result_listener << fmt::format("does not contain key \"{}\"", key);
    return false;
  }
  return true;
}

MATCHER(ValueIsString, "") {
  if (!arg.IsString()) {
    *result_listener << "value is not a string";
    return false;
  }
  return true;
}

MATCHER_P(ContainsStringValueByKey, key, "") {
  return ExplainMatchResult(HasMember(key), arg, result_listener) &&
         ExplainMatchResult(ValueIsString(), arg[key], result_listener);
}

}  // namespace simulator::log

#endif  // SIMULATOR_LOG_TESTS_TEST_UTILS_RAPIDJSON_PRINTER_HPP_
