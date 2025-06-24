#include "ih/marshalling/json/setting.hpp"

#include <algorithm>
#include <iterator>

#include <fmt/format.h>
#include <rapidjson/document.h>

#include "data_layer/api/inspectors/setting.hpp"

#include "ih/marshalling/json/detail/keys.hpp"
#include "ih/marshalling/json/detail/marshaller.hpp"
#include "ih/marshalling/json/detail/unmarshaller.hpp"
#include "ih/marshalling/json/detail/utils.hpp"

namespace Simulator::Http::Json {

namespace {

auto marshallSetting(
    DataLayer::Setting const& _setting,
    rapidjson::Document& _dest
) -> void
{
    Marshaller marshaller{_dest};
    DataLayer::SettingReader reader{marshaller};
    reader.read(_setting);
}

[[nodiscard]]
auto unmarshallSetting(rapidjson::Value const& _value)
    -> DataLayer::Setting::Patch
{
    DataLayer::Setting::Patch patch{};

    Json::Unmarshaller unmarshaller{_value};
    DataLayer::SettingPatchWriter writer{unmarshaller};
    writer.write(patch);

    return patch;
}

} // namespace

auto SettingMarshaller::marshall(
    std::vector<DataLayer::Setting> const& _settings
) -> std::string
{
    rapidjson::Document root{};
    root.SetObject();
    auto& allocator = root.GetAllocator();

    rapidjson::Document settingsList{std::addressof(allocator)};
    settingsList.SetObject().SetArray();

    for (auto const& setting : _settings) {
        rapidjson::Document settingDoc{std::addressof(allocator)};
        marshallSetting(setting, settingDoc);
        settingsList.PushBack(settingDoc, allocator);
    }

    constexpr auto key = SettingKey::Settings;
    root.AddMember(Json::makeKey(key), settingsList, allocator);

    return Json::encode(root);
}

auto SettingUnmarshaller::unmarshall(
    std::string_view _json,
    std::vector<DataLayer::Setting::Patch>& _dest
) -> void
{
    rapidjson::Document document{};
    document.Parse(_json.data());

    if (!document.IsObject()) {
        throw std::runtime_error{"failed to parse settings JSON"};
    }

    if (!document.HasMember(Json::makeKey(SettingKey::Settings))) {
        throw std::runtime_error{fmt::format(
            "settings JSON does not contain required `{}' key",
            SettingKey::Settings
        )};
    }

    auto const& settings = document[SettingKey::Settings.data()];
    if (!settings.IsArray()) {
        throw std::runtime_error{fmt::format(
            "`{}' key is expected to point on a JSON array",
            SettingKey::Settings
        )};
    }

    auto const settingsArray = settings.GetArray();
    _dest.reserve(settingsArray.Size());
    std::transform(
        std::begin(settingsArray),
        std::end(settingsArray),
        std::back_inserter(_dest),
        [](auto const& _settingJson) -> DataLayer::Setting::Patch {
            return unmarshallSetting(_settingJson);
        }
    );
}

} // namespace Simulator::Http::Json
