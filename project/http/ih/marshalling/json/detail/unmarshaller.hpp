#ifndef SIMULATOR_HTTP_IH_MARSHALLING_JSON_DETAIL_UNMARSHALLER_HPP_
#define SIMULATOR_HTTP_IH_MARSHALLING_JSON_DETAIL_UNMARSHALLER_HPP_

#include <cstdint>
#include <functional>
#include <limits>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

#include <fmt/format.h>
#include <rapidjson/document.h>

#include "ih/marshalling/json/detail/enumeration_resolver.hpp"
#include "ih/marshalling/json/detail/key_resolver.hpp"
#include "ih/marshalling/json/detail/traits.hpp"

namespace Simulator::Http::Json {

class Unmarshaller {
  public:
    explicit Unmarshaller(rapidjson::Value const& _value);

    auto operator()(std::string_view _key, bool& _value) -> bool;

    template<typename Attribute>
      requires std::is_enum_v<Attribute>
    auto operator()(Attribute _attribute, bool& _value) -> bool;

    template<typename Attribute>
      requires std::is_enum_v<Attribute>
    auto operator()(Attribute _attribute, char& _value) -> bool;

    template<typename Attribute>
      requires std::is_enum_v<Attribute>
    auto operator()(Attribute _attribute, double& _value) -> bool;

    template<typename Attribute>
      requires std::is_enum_v<Attribute>
    auto operator()(Attribute _attribute, std::string& _value) -> bool;

    template<typename Attribute, typename T>
      requires std::is_enum_v<Attribute> && is_marshallable_int_v<T>
    auto operator()(Attribute _attribute, T& _value)
        -> bool;

    template<typename Attribute, typename T>
      requires std::is_enum_v<Attribute> && is_marshallable_uint_v<T>
    auto operator()(Attribute _attribute, T& _value)
        -> bool;

    template<typename Attribute, typename T>
      requires std::is_enum_v<Attribute> && std::is_enum_v<T>
    auto operator()(Attribute _attribute, T& _value)
        -> bool;

  private:
    auto getBoolean(std::string_view _key) -> std::optional<bool>;

    auto getInteger(std::string_view _key) -> std::optional<std::int64_t>;

    auto getUnsInteger(std::string_view _key) -> std::optional<std::uint64_t>;

    auto getDouble(std::string_view _key) -> std::optional<double>;

    auto getString(std::string_view _key) -> std::optional<std::string>;

    auto getCharacter(std::string_view _key) -> std::optional<char>;

    auto jsonObject() -> rapidjson::Value const&;

    template<typename Target, typename Source>
    static auto castInteger(std::string_view _key, Source _integer)
        -> std::remove_cv_t<std::remove_reference_t<Target>>;

    template<typename Attribute>
    static auto resolveKey(Attribute _attribute) -> std::string_view;

    [[noreturn]]
    static auto reportInvalidType(
        std::string_view _key,
        std::string_view _expectedType
    ) -> void;


    std::reference_wrapper<rapidjson::Value const> mJsonObject;
};


inline Unmarshaller::Unmarshaller(rapidjson::Value const& _value) :
    mJsonObject(_value)
{
    if (!_value.IsObject()) {
        throw std::logic_error{
            "trying to initialize an unmarshaller with a JSON value which is "
            "not an object"};
    }
}

inline auto Unmarshaller::operator()(std::string_view _key, bool& _value) -> bool {
  if (const auto value = getBoolean(_key)) {
    _value = *value;
    return true;
  }
  return false;
}

template<typename Attribute>
  requires std::is_enum_v<Attribute>
inline auto Unmarshaller::operator()(Attribute _attribute, bool& _value) -> bool
{
    std::string_view const key = resolveKey(_attribute);
    return operator()(key, _value);
}

template<typename Attribute>
  requires std::is_enum_v<Attribute>
inline auto Unmarshaller::operator()(Attribute _attribute, char& _value) -> bool
{
    std::string_view const key = resolveKey(_attribute);
    if (auto const value = getCharacter(key)) {
        _value = *value;
        return true;
    }
    return false;
}

template<typename Attribute>
  requires std::is_enum_v<Attribute>
inline auto Unmarshaller::operator()(Attribute _attribute, double& _value)
    -> bool
{
    std::string_view const key = resolveKey(_attribute);
    if (auto const value = getDouble(key)) {
        _value = *value;
        return true;
    }
    return false;
}

template<typename Attribute>
  requires std::is_enum_v<Attribute>
inline auto Unmarshaller::operator()(Attribute _attribute, std::string& _value)
    -> bool
{
    std::string_view const key = resolveKey(_attribute);
    if (auto value = getString(key)) {
        _value = std::move(*value);
        return true;
    }
    return false;
}

template<typename Attribute, typename T>
  requires std::is_enum_v<Attribute> && is_marshallable_int_v<T>
inline auto Unmarshaller::operator()(Attribute _attribute, T& _value)
    -> bool
{
    std::string_view const key = resolveKey(_attribute);
    if (auto value = getInteger(key)) {
        _value = castInteger<T>(key, *value);
        return true;
    }
    return false;
}

template<typename Attribute, typename T>
  requires std::is_enum_v<Attribute> && is_marshallable_uint_v<T>
inline auto Unmarshaller::operator()(Attribute _attribute, T& _value)
    -> bool
{
    std::string_view const key = resolveKey(_attribute);
    if (auto value = getUnsInteger(key)) {
        _value = castInteger<T>(key, *value);
        return true;
    }
    return false;
}

template<typename Attribute, typename T>
  requires std::is_enum_v<Attribute> && std::is_enum_v<T>
inline auto Unmarshaller::operator()(Attribute _attribute, T& _value)
    -> bool
{
    std::string decodedString{};
    if ((*this)(_attribute, decodedString)) {
        Json::EnumerationResolver::resolve(decodedString, _value);
        return true;
    }
    return false;
}

inline auto Unmarshaller::getBoolean(std::string_view _key)
    -> std::optional<bool>
{
    if (!jsonObject().HasMember(_key.data())) {
        return std::nullopt;
    }

    auto const& value = jsonObject()[_key.data()];
    if (value.IsBool()) {
        return std::make_optional<bool>(value.GetBool());
    }

    reportInvalidType(_key, "boolean");
}

inline auto Unmarshaller::getInteger(std::string_view _key)
    -> std::optional<std::int64_t>
{
    if (!jsonObject().HasMember(_key.data())) {
        return std::nullopt;
    }

    auto const& value = jsonObject()[_key.data()];
    if (value.IsInt64()) {
        return std::make_optional<std::int64_t>(value.GetInt64());
    }

    reportInvalidType(_key, "integer");
}

inline auto Unmarshaller::getUnsInteger(std::string_view _key)
    -> std::optional<std::uint64_t>
{
    if (!jsonObject().HasMember(_key.data())) {
        return std::nullopt;
    }

    auto const& value = jsonObject()[_key.data()];
    if (value.IsUint64()) {
        return std::make_optional<std::uint64_t>(value.GetUint64());
    }

    reportInvalidType(_key, "unsigned integer");
}

inline auto Unmarshaller::getDouble(std::string_view _key)
    -> std::optional<double>
{
    if (!jsonObject().HasMember(_key.data())) {
        return std::nullopt;
    }

    auto const& value = jsonObject()[_key.data()];
    std::optional<double> decodedValue{};
    if (value.IsDouble()) {
        decodedValue = std::make_optional<double>(value.GetDouble());
    } else if (value.IsInt64()) {
        decodedValue = std::make_optional<double>(value.GetInt64());
    } else if (value.IsUint64()) {
        decodedValue = std::make_optional<double>(value.GetUint64());
    }

    if (decodedValue.has_value()) {
        return decodedValue;
    }
    reportInvalidType(_key, "double-precision floating point");
}

inline auto Unmarshaller::getString(std::string_view _key)
    -> std::optional<std::string>
{
    if (!jsonObject().HasMember(_key.data())) {
        return std::nullopt;
    }

    auto const& value = jsonObject()[_key.data()];
    if (value.IsString()) {
        return std::make_optional<std::string>(
            value.GetString(),
            value.GetStringLength()
        );
    }

    reportInvalidType(_key, "string");
}

inline auto Unmarshaller::getCharacter(std::string_view _key)
    -> std::optional<char>
{
    std::optional<std::string> const decodedAsString = getString(_key);
    if (!decodedAsString.has_value()) {
        return std::nullopt;
    }

    if (decodedAsString->size() == 1) {
        return std::make_optional<char>(decodedAsString->at(0));
    }

    throw std::runtime_error{fmt::format(
        "invalid string received for `{}', a string with exactly 1 character "
        "is expected",
        _key
    )};
}

inline auto Unmarshaller::jsonObject() -> rapidjson::Value const&
{
    return mJsonObject.get();
}

template<typename Target, typename Source>
inline auto Unmarshaller::castInteger(std::string_view _key, Source _integer)
    -> std::remove_cv_t<std::remove_reference_t<Target>>
{
    using TargetType = std::remove_cv_t<std::remove_reference_t<Target>>;
    constexpr auto min = std::numeric_limits<TargetType>::min();
    constexpr auto max = std::numeric_limits<TargetType>::max();

    if (min <= _integer && _integer <= max) {
        return static_cast<TargetType>(_integer);
    }

    if (_integer < min) {
        throw std::runtime_error{fmt::format(
            "invalid integer received for `{}', value is less than "
            "supported minimal value {}",
            _key,
            min
        )};
    }

    throw std::runtime_error{fmt::format(
        "invalid integer received for `{}', value is greater that "
        "supported maximal value {}",
        _key,
        min
    )};
}

template<typename Attribute>
inline auto Unmarshaller::resolveKey(Attribute _attribute) -> std::string_view
{
    static_assert(
        std::is_enum_v<Attribute>,
        "Given Attribute type is not an enumeration type, thus could not "
        "be resolved as JSON document key"
    );

    return Json::KeyResolver::resolve_key(_attribute);
}

inline auto Unmarshaller::reportInvalidType(
    std::string_view _key,
    std::string_view _expectedType
) -> void
{
    throw std::runtime_error{fmt::format(
        "unexpected data type received for `{}', {} is expected",
        _key,
        _expectedType
    )};
}

} // namespace Simulator::Http::Json

#endif // SIMULATOR_HTTP_IH_MARSHALLING_JSON_DETAIL_UNMARSHALLER_HPP_
