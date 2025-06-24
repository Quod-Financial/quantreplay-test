#ifndef SIMULATOR_HTTP_IH_CONTROLLERS_DATASOURCE_CONTROLLER_HPP_
#define SIMULATOR_HTTP_IH_CONTROLLERS_DATASOURCE_CONTROLLER_HPP_

#include <cstdint>
#include <functional>
#include <string>
#include <utility>

#include <pistache/http_defs.h>

#include "ih/data_bridge/datasource_accessor.hpp"

namespace Simulator::Http {

class DatasourceController {
  public:
    using Result = std::pair<Pistache::Http::Code, std::string>;

    explicit DatasourceController(
        DataBridge::DatasourceAccessor& _dataAccessor
    ) noexcept;


    [[nodiscard]]
    auto selectDatasource(std::uint64_t _datasourceId) const -> Result;

    [[nodiscard]]
    auto selectAllDatasources() const -> Result;


    [[nodiscard]]
    auto insertDatasource(std::string const& _body) const -> Result;

    [[nodiscard]]
    auto updateDatasource(
        std::uint64_t _datasourceId,
        std::string const& _body
    ) const -> Result;

  private:
    static auto formatErrorResponse(DataBridge::FailureInfo failure_info)
        -> std::string;

    std::reference_wrapper<DataBridge::DatasourceAccessor const> mDataAccessor;
};

} // namespace Simulator::Http

#endif // SIMULATOR_HTTP_IH_CONTROLLERS_DATASOURCE_CONTROLLER_HPP_
