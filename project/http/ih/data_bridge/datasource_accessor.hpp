#ifndef SIMULATOR_HTTP_IH_DATA_BRIDGE_DATASOURCE_ACCESSOR_HPP_
#define SIMULATOR_HTTP_IH_DATA_BRIDGE_DATASOURCE_ACCESSOR_HPP_

#include <cstdint>
#include <tl/expected.hpp>
#include <vector>

#include "data_layer/api/database/context.hpp"
#include "data_layer/api/models/datasource.hpp"
#include "ih/data_bridge/operation_failure.hpp"

namespace Simulator::Http::DataBridge {

class DatasourceAccessor {
 public:
  DatasourceAccessor() = default;
  DatasourceAccessor(DatasourceAccessor const&) = default;
  DatasourceAccessor(DatasourceAccessor&&) noexcept = default;

  auto operator=(DatasourceAccessor const&) -> DatasourceAccessor& = default;
  auto operator=(DatasourceAccessor&&) noexcept
      -> DatasourceAccessor& = default;

  virtual ~DatasourceAccessor() = default;

  [[nodiscard]]
  virtual auto selectSingle(std::uint64_t _datasourceId) const noexcept
      -> tl::expected<DataLayer::Datasource, FailureInfo> = 0;

  [[nodiscard]]
  virtual auto selectAll() const noexcept
      -> tl::expected<std::vector<DataLayer::Datasource>, FailureInfo> = 0;

  [[nodiscard]]
  virtual auto add(DataLayer::Datasource::Patch _snapshot) const noexcept
      -> tl::expected<void, FailureInfo> = 0;

  [[nodiscard]]
  virtual auto update(DataLayer::Datasource::Patch _update,
                      std::uint64_t _datasourceId) const noexcept
      -> tl::expected<void, FailureInfo> = 0;
};

class DataLayerDatasourceAccessor final : public DatasourceAccessor {
 public:
  using DbContext = DataLayer::Database::Context;

  DataLayerDatasourceAccessor() = delete;

  explicit DataLayerDatasourceAccessor(DbContext _context) noexcept;

  [[nodiscard]]
  auto selectSingle(std::uint64_t _datasourceId) const noexcept
      -> tl::expected<DataLayer::Datasource, FailureInfo> override;

  [[nodiscard]]
  auto selectAll() const noexcept
      -> tl::expected<std::vector<DataLayer::Datasource>, FailureInfo> override;

  [[nodiscard]]
  auto add(DataLayer::Datasource::Patch _snapshot) const noexcept
      -> tl::expected<void, FailureInfo> override;

  [[nodiscard]]
  auto update(DataLayer::Datasource::Patch _update,
              std::uint64_t _datasourceId) const noexcept
      -> tl::expected<void, FailureInfo> override;

 private:
  DataLayer::Database::Context context_;
};

}  // namespace Simulator::Http::DataBridge

#endif  // SIMULATOR_HTTP_IH_DATA_BRIDGE_DATASOURCE_ACCESSOR_HPP_
