#ifndef SIMULATOR_GENERATOR_IH_HISTORICAL_MAPPING_COLUMN_MAPPING_FILTER_HPP_
#define SIMULATOR_GENERATOR_IH_HISTORICAL_MAPPING_COLUMN_MAPPING_FILTER_HPP_

#include <cstdint>
#include <vector>

#include "data_layer/api/converters/column_mapping.hpp"
#include "data_layer/api/models/column_mapping.hpp"

namespace Simulator::Generator::Historical::Mapping {

auto filter(const std::vector<DataLayer::ColumnMapping>& mapping_configs,
            std::uint32_t depth_to_parse)
    -> std::vector<DataLayer::Converter::ColumnConfig>;

}  // namespace Simulator::Generator::Historical::Mapping

#endif  // SIMULATOR_GENERATOR_IH_HISTORICAL_MAPPING_COLUMN_MAPPING_FILTER_HPP_
