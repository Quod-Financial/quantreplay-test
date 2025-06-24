#include "api/validations/column_mapping.hpp"

#include "api/converters/column_mapping.hpp"

namespace Simulator::DataLayer::Validation {

namespace {

template <typename T>
auto variable_depth_validation(const T& column_mapping) -> bool {
  const auto column_from = Converter::extract_column_name_from_variable_depth(
      column_mapping.column_from());
  const auto column_to = Converter::extract_column_name_from_variable_depth(
      column_mapping.column_to());
  return column_from.has_value() && column_to.has_value();
}

}  // namespace

auto valid_variable_depth(const ColumnMapping& column_mapping) -> bool {
  return variable_depth_validation(column_mapping);
}

auto valid_variable_depth(const ColumnMapping::Patch& column_mapping) -> bool {
  return variable_depth_validation(column_mapping);
}

}  // namespace Simulator::DataLayer::Validation
