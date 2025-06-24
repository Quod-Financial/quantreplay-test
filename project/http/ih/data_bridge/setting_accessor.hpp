#ifndef SIMULATOR_HTTP_IH_DATA_BRIDGE_SETTING_ACCESSOR_HPP_
#define SIMULATOR_HTTP_IH_DATA_BRIDGE_SETTING_ACCESSOR_HPP_

#include <tl/expected.hpp>
#include <vector>

#include "data_layer/api/database/context.hpp"
#include "data_layer/api/models/setting.hpp"
#include "ih/data_bridge/operation_failure.hpp"

namespace Simulator::Http::DataBridge {

class SettingAccessor {
 public:
  SettingAccessor() = default;
  SettingAccessor(SettingAccessor const&) = default;
  SettingAccessor(SettingAccessor&&) noexcept = default;

  auto operator=(SettingAccessor const&) -> SettingAccessor& = default;
  auto operator=(SettingAccessor&&) noexcept -> SettingAccessor& = default;

  virtual ~SettingAccessor() = default;

  [[nodiscard]]
  virtual auto selectSingle(std::string const& _key) const noexcept
      -> tl::expected<DataLayer::Setting, Failure> = 0;

  [[nodiscard]]
  virtual auto selectAll() const noexcept
      -> tl::expected<std::vector<DataLayer::Setting>, Failure> = 0;

  [[nodiscard]]
  virtual auto add(DataLayer::Setting::Patch _snapshot) const noexcept
      -> tl::expected<void, Failure> = 0;

  [[nodiscard]]
  virtual auto update(DataLayer::Setting::Patch _update,
                      std::string const& _key) const noexcept
      -> tl::expected<void, Failure> = 0;
};

class DataLayerSettingAccessor final : public SettingAccessor {
 public:
  using DbContext = DataLayer::Database::Context;

  DataLayerSettingAccessor() = delete;

  explicit DataLayerSettingAccessor(DbContext _context) noexcept;

  [[nodiscard]]
  auto selectSingle(std::string const& _key) const noexcept
      -> tl::expected<DataLayer::Setting, Failure> override;

  [[nodiscard]]
  auto selectAll() const noexcept
      -> tl::expected<std::vector<DataLayer::Setting>, Failure> override;

  [[nodiscard]]
  auto add(DataLayer::Setting::Patch _snapshot) const noexcept
      -> tl::expected<void, Failure> override;

  [[nodiscard]]
  auto update(DataLayer::Setting::Patch _update,
              std::string const& _key) const noexcept
      -> tl::expected<void, Failure> override;

 private:
  DataLayer::Database::Context context_;
};

}  // namespace Simulator::Http::DataBridge

#endif  // SIMULATOR_HTTP_IH_DATA_BRIDGE_SETTING_ACCESSOR_HPP_
