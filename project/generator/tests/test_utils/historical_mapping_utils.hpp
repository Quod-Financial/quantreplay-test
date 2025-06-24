#ifndef SIMULATOR_GENERATOR_TESTS_TEST_UTILS_HISTORICAL_MAPPING_UTILS_HPP_
#define SIMULATOR_GENERATOR_TESTS_TEST_UTILS_HISTORICAL_MAPPING_UTILS_HPP_

#include <gmock/gmock.h>

#include "data_layer/api/models/column_mapping.hpp"

namespace Simulator::Generator::Historical::Mapping {

using namespace ::testing;

inline auto make_column_config(DataLayer::Converter::ColumnFrom column_from,
                               std::string source)
    -> DataLayer::Converter::ColumnConfig {
  return DataLayer::Converter::ColumnConfig{
      std::move(column_from), std::move(source), 0};
}

MATCHER_P(Resolves, column_from, "") {
  return arg.resolve_by(column_from).has_value();
}

MATCHER_P2(ResolvesColumnFromAndIndexIs, column_from, index, "") {
  const auto resolved = arg.resolve_by(column_from);
  return ExplainMatchResult(Ne(std::nullopt), resolved, result_listener) &&
         ExplainMatchResult(Eq(index), resolved->index(), result_listener);
}

}  // namespace Simulator::Generator::Historical::Mapping

#endif  // SIMULATOR_GENERATOR_TESTS_TEST_UTILS_HISTORICAL_MAPPING_UTILS_HPP_
