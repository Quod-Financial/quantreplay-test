#ifndef SIMULATOR_DATA_LAYER_INCLUDE_DATA_LAYER_API_VALIDATIONS_COLUMN_MAPPING_HPP_
#define SIMULATOR_DATA_LAYER_INCLUDE_DATA_LAYER_API_VALIDATIONS_COLUMN_MAPPING_HPP_

#include "data_layer/api/models/column_mapping.hpp"

namespace Simulator::DataLayer::Validation {

auto valid_variable_depth(const ColumnMapping& column_mapping) -> bool;
auto valid_variable_depth(const ColumnMapping::Patch& column_mapping) -> bool;

}  // namespace Simulator::DataLayer::Validation

#endif  // SIMULATOR_DATA_LAYER_INCLUDE_DATA_LAYER_API_VALIDATIONS_COLUMN_MAPPING_HPP_
