#include "api/models/setting.hpp"

#include "ih/common/exceptions.hpp"

namespace Simulator::DataLayer {

auto Setting::create(Setting::Patch _snapshot) -> Setting
{
    if (!_snapshot.mNewKey.has_value()) {
        throw RequiredAttributeMissing("Setting", "Key");
    }

    Setting setting{};
    setting.mKey = std::move(*_snapshot.mNewKey);
    setting.mValue = std::move(_snapshot.mNewValue);
    return setting;
}

auto Setting::getKey() const noexcept -> std::string const& { return mKey; }

auto Setting::getValue() const noexcept -> std::optional<std::string> const&
{
    return mValue;
}

auto Setting::Patch::getKey() const noexcept
    -> std::optional<std::string> const&
{
    return mNewKey;
}

auto Setting::Patch::withKey(std::string _key) noexcept -> Patch&
{
    mNewKey = std::move(_key);
    return *this;
}

auto Setting::Patch::getValue() const noexcept
    -> std::optional<std::string> const&
{
    return mNewValue;
}

auto Setting::Patch::withValue(std::string _value) noexcept -> Patch&
{
    mNewValue = std::move(_value);
    return *this;
}

} // namespace Simulator::DataLayer
