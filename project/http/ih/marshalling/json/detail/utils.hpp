#ifndef SIMULATOR_HTTP_IH_MARSHALLING_JSON_DETAIL_UTILS_HPP_
#define SIMULATOR_HTTP_IH_MARSHALLING_JSON_DETAIL_UTILS_HPP_

#include <string>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace Simulator::Http::Json {

inline auto encode(rapidjson::Document const& _document) -> std::string
{
    rapidjson::StringBuffer buffer{};
    rapidjson::Writer writer{buffer};

    _document.Accept(writer);

    return {buffer.GetString(), buffer.GetSize()};
}

inline auto makeKey(std::string_view _key)
{
    return rapidjson::StringRef(_key.data(), _key.size());
}

} // namespace Simulator::Http::Json

#endif // SIMULATOR_HTTP_IH_MARSHALLING_JSON_DETAIL_UTILS_HPP_
