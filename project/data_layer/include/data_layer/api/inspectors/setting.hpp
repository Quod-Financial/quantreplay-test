#ifndef SIMULATOR_DATA_LAYER_API_COMMON_INSPECTORS_SETTING_HPP_
#define SIMULATOR_DATA_LAYER_API_COMMON_INSPECTORS_SETTING_HPP_

#include <cstdint>
#include <string>
#include <type_traits>
#include <utility>

#include "data_layer/api/models/setting.hpp"

namespace Simulator::DataLayer {

template<typename Marshaller>
class SettingReader final {
    using Attribute = Setting::Attribute;

    template<typename T>
    constexpr inline static bool can_marshall_v = std::is_invocable_v<
        Marshaller,
        Attribute,
        std::add_const_t<std::add_lvalue_reference_t<T>>>;

  public:
    explicit SettingReader(Marshaller& _marshaller) noexcept :
        mMarshaller(_marshaller)
    {}

    auto read(Setting const& _setting) -> void;

  private:
    std::reference_wrapper<Marshaller> mMarshaller;
};


template<typename Marshaller>
class SettingPatchReader final {
    using Attribute = Setting::Attribute;

    template<typename T>
    constexpr inline static bool can_marshall_v = std::is_invocable_v<
        Marshaller,
        Attribute,
        std::add_const_t<std::add_lvalue_reference_t<T>>>;

  public:
    explicit SettingPatchReader(Marshaller& _marshaller) noexcept :
        mMarshaller(_marshaller)
    {}

    auto read(Setting::Patch const& _patch) -> void;

  private:
    std::reference_wrapper<Marshaller> mMarshaller;
};


template<typename Unmarshaller>
class SettingPatchWriter final {
    using Attribute = Setting::Attribute;

    template<typename T>
    constexpr inline static bool can_unmarshall_v = std::is_invocable_r_v<
        bool,
        Unmarshaller,
        Attribute,
        std::add_lvalue_reference_t<T>>;

  public:
    explicit SettingPatchWriter(Unmarshaller& _unmarshaller) noexcept :
        mUnmarshaller(_unmarshaller)
    {}

    auto write(Setting::Patch& _patch) -> void;

  private:
    std::reference_wrapper<Unmarshaller> mUnmarshaller;
};


template<typename Marshaller>
auto SettingReader<Marshaller>::read(Setting const& _setting) -> void
{
    std::string const& key = _setting.getKey();
    static_assert(can_marshall_v<decltype(key)>);
    mMarshaller(Attribute::Key, key);

    if (auto const& value = _setting.getValue()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::Value, *value);
    }
}


template<typename Marshaller>
inline auto SettingPatchReader<Marshaller>::read(Setting::Patch const& _patch)
    -> void
{
    if (auto const& value = _patch.getKey()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::Key, *value);
    }

    if (auto const& value = _patch.getValue()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::Value, *value);
    }
}


template<typename Unmarshaller>
inline auto SettingPatchWriter<Unmarshaller>::write(Setting::Patch& _patch)
    -> void
{
    std::string key{};
    static_assert(can_unmarshall_v<decltype(key)>);
    if (mUnmarshaller(Attribute::Key, key)) {
        _patch.withKey(std::move(key));
    }

    std::string value{};
    static_assert(can_unmarshall_v<decltype(value)>);
    if (mUnmarshaller(Attribute::Value, value)) {
        _patch.withValue(std::move(value));
    }
}

} // namespace Simulator::DataLayer

#endif // SIMULATOR_DATA_LAYER_API_COMMON_INSPECTORS_SETTING_HPP_
