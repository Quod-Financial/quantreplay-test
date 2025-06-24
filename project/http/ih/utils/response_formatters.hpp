#ifndef SIMULATOR_HTTP_IH_UTILS_RESPONSE_FORMATTERS_HPP_
#define SIMULATOR_HTTP_IH_UTILS_RESPONSE_FORMATTERS_HPP_

#include <string>
#include <string_view>

#include <boost/date_time.hpp>
#include <fmt/format.h>
#include <rapidjson/document.h>

#include "cfg/api/cfg.hpp"
#include "core/version.hpp"
#include "data_layer/api/models/venue.hpp"
#include "ih/marshalling/json/detail/utils.hpp"

namespace Simulator::Http {

[[nodiscard]]
inline auto formatResponse(std::string_view _key, std::string_view _msg)
    -> std::string
{
    return fmt::format(R"({{"{}":"{}"}})", _key, _msg);
}

[[nodiscard]]
inline auto formatResultResponse(std::string_view _resultMessage) -> std::string
{
    return formatResponse("result", _resultMessage);
}

[[nodiscard]]
inline auto formatVenueStatus(
    DataLayer::Venue const& _venue,
    int _responseCode
) -> std::string
{
    rapidjson::Document document{};
    rapidjson::Value value{};

    auto& allocator = document.GetAllocator();
    document.SetObject();

    value.SetString(_venue.getVenueId().data(), allocator);
    document.AddMember("id", value, allocator);

    if (auto const& venueName = _venue.getName()) {
        value.SetString(venueName->data(), allocator);
        document.AddMember("name", value, allocator);
    }

    auto timeStr = boost::posix_time::to_simple_string(Cfg::venue().startTime);
    value.SetString(timeStr.data(), allocator);
    document.AddMember("startTime", value, allocator);

    value.SetString(simulator::core::version().data(), allocator);
    document.AddMember("version", value, allocator);

    if (_responseCode != 0) {
        value.SetInt(_responseCode);
        document.AddMember("statusCode", value, allocator);
    }

    return Json::encode(document);
}

} // namespace Simulator::Http

#endif // SIMULATOR_HTTP_IH_UTILS_RESPONSE_FORMATTERS_HPP_
