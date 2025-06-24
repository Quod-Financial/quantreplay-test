#ifndef SIMULATOR_GENERATOR_IH_HISTORICAL_MAPPING_PARAMS_HPP_
#define SIMULATOR_GENERATOR_IH_HISTORICAL_MAPPING_PARAMS_HPP_

#include <string>
#include <vector>

#include "data_layer/api/models/column_mapping.hpp"
#include "data_layer/api/models/datasource.hpp"
#include "ih/historical/mapping/datasource_params.hpp"
#include "ih/historical/mapping/depth_config.hpp"
#include "ih/historical/mapping/specification.hpp"

namespace Simulator::Generator::Historical {

class MappingParams {
 public:
  using ColumnNames = std::vector<std::string>;
  using ColumnMappings = std::vector<DataLayer::ColumnMapping>;

  explicit MappingParams(ColumnMappings mapping_configs,
                         DatasourceParams datasource_params) noexcept;

  auto initialize(Mapping::DepthConfig depth_config) -> void;

  auto initialize(ColumnNames column_names, Mapping::DepthConfig depth_config)
      -> void;

  [[nodiscard]]
  auto column_idx(DataLayer::Converter::ColumnFrom column) const
      -> std::optional<std::uint32_t>;

 private:
  Mapping::Specification spec_;
  ColumnMappings column_mappings_;
  DatasourceParams datasource_params_;
};

auto make_mapping_params(const DataLayer::Datasource& datasource)
    -> MappingParams;

}  // namespace Simulator::Generator::Historical

#endif  // SIMULATOR_GENERATOR_IH_HISTORICAL_MAPPING_PARAMS_HPP_
