#ifndef SIMULATOR_HTTP_IH_CONTROLLERS_PRICE_SEED_CONTROLLER_HPP_
#define SIMULATOR_HTTP_IH_CONTROLLERS_PRICE_SEED_CONTROLLER_HPP_

#include <cstdint>
#include <functional>
#include <string>
#include <utility>

#include <pistache/http_defs.h>

#include "ih/data_bridge/price_seed_accessor.hpp"
#include "ih/data_bridge/setting_accessor.hpp"

namespace Simulator::Http {

class PriceSeedController {
  public:
    using Result = std::pair<Pistache::Http::Code, std::string>;

    PriceSeedController(
        DataBridge::PriceSeedAccessor const& _seedAccessor,
        DataBridge::SettingAccessor const& _settingAccessor
    ) noexcept;

    [[nodiscard]]
    auto selectPriceSeed(std::uint64_t _seedId) const -> Result;

    [[nodiscard]]
    auto selectAllPriceSeeds() const -> Result;


    [[nodiscard]]
    auto insertPriceSeed(std::string const& _body) const -> Result;

    [[nodiscard]]
    auto updatePriceSeed(std::uint64_t _seedId, std::string const& _body) const
        -> Result;

    [[nodiscard]]
    auto deletePriceSeed(std::uint64_t _seedId) const -> Result;

    [[nodiscard]]
    auto syncPriceSeeds() const -> Result;


    static auto formatErrorResponse(DataBridge::Failure _failure)
        -> std::string;

    std::reference_wrapper<DataBridge::PriceSeedAccessor const> mSeedAccessor;
    std::reference_wrapper<DataBridge::SettingAccessor const> mSettingAccessor;


    static std::string const msConnectionSetting;
    static std::string const msSyncTimeSetting;
};

} // namespace Simulator::Http

#endif // SIMULATOR_HTTP_IH_CONTROLLERS_PRICE_SEED_CONTROLLER_HPP_
