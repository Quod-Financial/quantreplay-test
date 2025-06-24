#ifndef SIMULATOR_DATA_LAYER_INCLUDE_DATA_LAYER_API_VALIDATIONS_DATASOURCE_HPP_
#define SIMULATOR_DATA_LAYER_INCLUDE_DATA_LAYER_API_VALIDATIONS_DATASOURCE_HPP_

#include <tl/expected.hpp>

#include "data_layer/api/models/datasource.hpp"

namespace Simulator::DataLayer::Validation {

[[nodiscard]]
auto valid(const Datasource& datasource) -> tl::expected<void, std::string>;

[[nodiscard]]
auto valid(const Datasource::Patch& datasource)
    -> tl::expected<void, std::string>;

}  // namespace Simulator::DataLayer::Validation

#endif  // SIMULATOR_DATA_LAYER_INCLUDE_DATA_LAYER_API_VALIDATIONS_DATASOURCE_HPP_
