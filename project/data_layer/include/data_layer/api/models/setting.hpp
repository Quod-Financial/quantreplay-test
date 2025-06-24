#ifndef SIMULATOR_DATA_LAYER_INCLUDE_DATA_LAYER_API_MODELS_SETTING_HPP_
#define SIMULATOR_DATA_LAYER_INCLUDE_DATA_LAYER_API_MODELS_SETTING_HPP_

#include <optional>
#include <string>

#include "data_layer/api/predicate/definitions.hpp"

namespace Simulator::DataLayer {

class Setting {
  public:
    class Patch;

    using Predicate = Predicate::Expression<Setting>;

    enum class Attribute { Key, Value };

    static auto create(Patch _snapshot) -> Setting;

    [[nodiscard]]
    auto getKey() const noexcept -> std::string const&;

    [[nodiscard]]
    auto getValue() const noexcept -> std::optional<std::string> const&;

  private:
    Setting() = default;


    std::optional<std::string> mValue;

    std::string mKey;
};

class Setting::Patch {
    friend class Setting;

  public:
    using Attribute = Setting::Attribute;

    [[nodiscard]]
    auto getKey() const noexcept -> std::optional<std::string> const&;
    auto withKey(std::string _key) noexcept -> Patch&;

    [[nodiscard]]
    auto getValue() const noexcept -> std::optional<std::string> const&;
    auto withValue(std::string _value) noexcept -> Patch&;

  private:
    std::optional<std::string> mNewKey;
    std::optional<std::string> mNewValue;
};

} // namespace Simulator::DataLayer

#endif // SIMULATOR_DATA_LAYER_INCLUDE_DATA_LAYER_API_MODELS_SETTING_HPP_
