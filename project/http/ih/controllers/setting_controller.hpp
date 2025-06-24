#ifndef SIMULATOR_HTTP_IH_CONTROLLERS_SETTING_CONTROLLER_HPP_
#define SIMULATOR_HTTP_IH_CONTROLLERS_SETTING_CONTROLLER_HPP_

#include <functional>
#include <string>
#include <utility>

#include <pistache/http_defs.h>

#include "ih/data_bridge/setting_accessor.hpp"

namespace Simulator::Http {

class SettingController {
  public:
    using Result = std::pair<Pistache::Http::Code, std::string>;

    explicit SettingController(
        DataBridge::SettingAccessor& _dataAccessor
    ) noexcept;


    [[nodiscard]]
    auto selectAllSettings() const -> Result;

    [[nodiscard]]
    auto updateSettings(std::string const& _body) const -> Result;

  private:
    [[nodiscard]]
    auto settingAccessor() const noexcept -> DataBridge::SettingAccessor const&;

    static auto formatErrorResponse(DataBridge::Failure _failure)
        -> std::string;

    static auto isUpdatableSetting(std::string_view _settingKey) -> bool;

    static auto isReadonlySetting(std::string_view _settingKey) -> bool;


    std::reference_wrapper<DataBridge::SettingAccessor const> mSettingsAccessor;

    static const std::string msDisplayNameSetting;
    static const std::string msExternalPriceSeedConnectionSetting;
    static const std::string msPriceSeedsLastUpdateTimeSetting;
};

} // namespace Simulator::Http

#endif // SIMULATOR_HTTP_IH_CONTROLLERS_SETTING_CONTROLLER_HPP_
