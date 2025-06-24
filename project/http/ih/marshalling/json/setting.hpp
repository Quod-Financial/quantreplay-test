#ifndef SIMULATOR_HTTP_IH_MARSHALLING_JSON_SETTING_HPP_
#define SIMULATOR_HTTP_IH_MARSHALLING_JSON_SETTING_HPP_

#include <string>
#include <string_view>
#include <vector>

#include <rapidjson/document.h>

#include "data_layer/api/models/setting.hpp"

namespace Simulator::Http::Json {

class SettingMarshaller {
  public:
    static auto marshall(std::vector<DataLayer::Setting> const& _settings)
        -> std::string;
};


class SettingUnmarshaller {
  public:
    static auto unmarshall(
        std::string_view _json,
        std::vector<DataLayer::Setting::Patch>& _dest
    ) -> void;
};

} // namespace Simulator::Http::Json

#endif // SIMULATOR_HTTP_IH_MARSHALLING_JSON_SETTING_HPP_
