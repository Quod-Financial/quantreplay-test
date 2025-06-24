#ifndef SIMULATOR_HTTP_IH_MARSHALLING_JSON_DETAIL_MARSHALLER_HPP_
#define SIMULATOR_HTTP_IH_MARSHALLING_JSON_DETAIL_MARSHALLER_HPP_

#include <cstdint>
#include <functional>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

#include <rapidjson/document.h>

#include "ih/marshalling/json/detail/enumeration_resolver.hpp"
#include "ih/marshalling/json/detail/key_resolver.hpp"
#include "ih/marshalling/json/detail/traits.hpp"

namespace Simulator::Http::Json {

class Marshaller {
    auto document() -> rapidjson::Document&;
    auto allocator() -> decltype(auto);

    template<typename Attribute>
    static auto resolveKey(Attribute _attribute);

  public:
    explicit Marshaller(rapidjson::Document& _jsonDocument);

    template<typename Attribute>
    auto operator()(Attribute _attribute, bool _value) -> void;

    template<typename Attribute>
    auto operator()(Attribute _attribute, char _value) -> void;

    template<typename Attribute>
    auto operator()(Attribute _attribute, double _value) -> void;

    template<typename Attribute>
    auto operator()(Attribute _attribute, std::string const& _value) -> void;

    template<typename Attribute, typename T>
    auto operator()(Attribute _attribute, T _value)
        -> std::enable_if_t<is_marshallable_int_v<T>>;

    template<typename Attribute, typename T>
    auto operator()(Attribute _attribute, T _value)
        -> std::enable_if_t<is_marshallable_uint_v<T>>;

    template<typename Attribute, typename T>
    auto operator()(Attribute _attribute, T _value)
        -> std::enable_if_t<std::is_enum_v<T>>;

  private:
    std::reference_wrapper<rapidjson::Document> mDocument;
};


inline auto Marshaller::document() -> rapidjson::Document&
{
    return mDocument.get();
}

inline auto Marshaller::allocator() -> decltype(auto)
{
    return document().GetAllocator();
}

template<typename Attribute>
inline auto Marshaller::resolveKey(Attribute _attribute)
{
    static_assert(
        std::is_enum_v<Attribute>,
        "Given Attribute type is not an enumeration type, thus could not "
        "be resolved as JSON document key"
    );

    std::string_view const key = Json::KeyResolver::resolve_key(_attribute);
    return rapidjson::StringRef(key.data(), key.size());
}

inline Marshaller::Marshaller(rapidjson::Document& _jsonDocument) :
    mDocument(_jsonDocument)
{
    if (!document().IsObject()) {
        document().SetObject();
    }
}

template<typename Attribute>
inline auto Marshaller::operator()(Attribute _attribute, bool _value) -> void
{
    auto const key = resolveKey(_attribute);
    rapidjson::Value value{};
    value.SetBool(_value);
    document().AddMember(key, value, allocator());
}

template<typename Attribute>
inline auto Marshaller::operator()(Attribute _attribute, char _value) -> void
{
    auto const key = resolveKey(_attribute);
    rapidjson::Value value{};
    value.SetString(std::addressof(_value), 1, allocator());
    document().AddMember(key, value, allocator());
}

template<typename Attribute>
inline auto Marshaller::operator()(Attribute _attribute, double _value) -> void
{
    auto const key = resolveKey(_attribute);
    rapidjson::Value value{};
    value.SetDouble(_value);
    document().AddMember(key, value, allocator());
}

template<typename Attribute>
inline auto Marshaller::operator()(
    Attribute _attribute,
    std::string const& _value
) -> void
{
    auto const size = static_cast<rapidjson::SizeType>(_value.size());
    auto const key = resolveKey(_attribute);
    rapidjson::Value value{};
    value.SetString(_value.data(), size, allocator());
    document().AddMember(key, value, allocator());
}

template<typename Attribute, typename T>
inline auto Marshaller::operator()(Attribute _attribute, T _value)
    -> std::enable_if_t<is_marshallable_int_v<T>>
{
    auto const key = resolveKey(_attribute);
    rapidjson::Value value{};
    value.SetInt64(static_cast<std::int64_t>(_value));
    document().AddMember(key, value, allocator());
}

template<typename Attribute, typename T>
inline auto Marshaller::operator()(Attribute _attribute, T _value)
    -> std::enable_if_t<is_marshallable_uint_v<T>>
{
    auto const key = resolveKey(_attribute);
    rapidjson::Value value{};
    value.SetUint64(static_cast<std::uint64_t>(_value));
    document().AddMember(key, value, allocator());
}

template<typename Attribute, typename T>
inline auto Marshaller::operator()(Attribute _attribute, T _value)
    -> std::enable_if_t<std::is_enum_v<T>>
{
    std::string_view const encoded = Json::EnumerationResolver::resolve(_value);
    (*this)(_attribute, std::string{encoded});
}

} // namespace Simulator::Http::Json

#endif // SIMULATOR_HTTP_IH_MARSHALLING_JSON_DETAIL_MARSHALLER_HPP_
