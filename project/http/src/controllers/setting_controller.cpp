#include "ih/controllers/setting_controller.hpp"

#include <fmt/format.h>

#include <string>
#include <utility>

#include "data_layer/api/models/setting.hpp"
#include "ih/marshalling/json/setting.hpp"
#include "ih/utils/response_formatters.hpp"
#include "log/logging.hpp"

namespace Simulator::Http {
namespace {

[[nodiscard]] [[maybe_unused]]
auto unmarshallRequestBody(std::string_view _body,
                           std::vector<DataLayer::Setting::Patch>& _dest,
                           Pistache::Http::Code& _code,
                           std::string& _content) noexcept -> bool {
  try {
    Json::SettingUnmarshaller::unmarshall(_body, _dest);
    return true;
  } catch (std::runtime_error const& _ex) {
    _code = Pistache::Http::Code::Bad_Request;
    _content = formatResultResponse(fmt::format("Bad request: {}", _ex.what()));
  } catch (...) {
    simulator::log::err(
        "failed to unmarshall settings, unexpected error raised");
    _code = Pistache::Http::Code::Internal_Server_Error;
    _content = formatResultResponse("Failed to unmarshall request body");
  }

  return false;
}

}  // namespace

SettingController::SettingController(
    DataBridge::SettingAccessor& _dataAccessor) noexcept
    : mSettingsAccessor(_dataAccessor) {}

auto SettingController::selectAllSettings() const -> Result {
  Pistache::Http::Code code{};
  std::string content{};

  auto opResult = settingAccessor().selectAll();
  if (opResult) {
    try {
      const std::vector<DataLayer::Setting>& settings = opResult.value();
      content = Json::SettingMarshaller::marshall(settings);
      code = Pistache::Http::Code::Ok;
      simulator::log::info("selected {} setting records", settings.size());
    } catch (std::exception const& _ex) {
      simulator::log::warn("failed to marshall settings list to JSON: {}",
                           _ex.what());
      code = Pistache::Http::Code::Internal_Server_Error;
      content = formatResultResponse("failed to marshall venues list");
    }
  } else {
    const auto failure = opResult.error();
    content = formatErrorResponse(failure);
    code = Pistache::Http::Code::Internal_Server_Error;
  }

  return std::make_pair(code, std::move(content));
}

auto SettingController::updateSettings(std::string const& _body) const
    -> Result {
  Pistache::Http::Code code{};
  std::string content{};

  std::vector<DataLayer::Setting::Patch> settings{};
  if (!unmarshallRequestBody(_body, settings, code, content)) {
    return std::make_pair(code, std::move(content));
  }

  code = Pistache::Http::Code::Ok;
  content = formatResultResponse("General settings updated successfully");

  for (auto const& setting : settings) {
    std::optional<std::string> const& key = setting.getKey();
    std::optional<std::string> const& value = setting.getValue();
    if (!key.has_value() || !value.has_value() || isReadonlySetting(*key)) {
      continue;
    }

    if (!isUpdatableSetting(*key)) {
      code = Pistache::Http::Code::Bad_Request;
      content = formatResultResponse(
          fmt::format("Unknown `{}' setting update requested", *key));
      break;
    }

    auto const opResult = settingAccessor().update(setting, *key);
    if (!opResult) {
      code = Pistache::Http::Code::Internal_Server_Error;
      content = formatErrorResponse(opResult.error());
      break;
    }
  }

  return std::make_pair(code, std::move(content));
}

auto SettingController::settingAccessor() const noexcept
    -> DataBridge::SettingAccessor const& {
  return mSettingsAccessor.get();
}

auto SettingController::formatErrorResponse(DataBridge::Failure _failure)
    -> std::string {
  std::string message{};
  switch (_failure) {
    case DataBridge::Failure::MalformedInput:
      message = "Required fields missing for Setting model in request";
      break;
    case DataBridge::Failure::DatabaseConnectionError:
      message = "Database connection failed";
      break;
    case DataBridge::Failure::ResponseDecodingError:
      message = "Failed to decode a database response";
      break;
    case DataBridge::Failure::ResponseCardinalityError:
      message = "Can not resolve a single Setting by a given key";
      break;
    case DataBridge::Failure::DataIntegrityViolationError:
      message = "Requested operation violates data integrity constraints";
      break;
    case DataBridge::Failure::UnknownError:
      message = "Unknown server error occurred";
      break;
  }
  return formatResultResponse(message);
}

auto SettingController::isUpdatableSetting(std::string_view _settingKey)
    -> bool {
  return _settingKey == msDisplayNameSetting ||
         _settingKey == msExternalPriceSeedConnectionSetting;
}

auto SettingController::isReadonlySetting(std::string_view _settingKey)
    -> bool {
  return _settingKey == msPriceSeedsLastUpdateTimeSetting;
}

std::string const SettingController::msDisplayNameSetting{"DisplayName"};

std::string const SettingController::msExternalPriceSeedConnectionSetting{
    "SeedPriceDatabaseConnection"};

std::string const SettingController::msPriceSeedsLastUpdateTimeSetting{
    "SeedPricesLastUpdated"};

}  // namespace Simulator::Http
