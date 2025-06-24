#include "ih/controllers/price_seed_controller.hpp"

#include <fmt/chrono.h>

#include <ctime>
#include <string>
#include <utility>

#include "data_layer/api/models/price_seed.hpp"
#include "data_layer/api/models/setting.hpp"
#include "ih/marshalling/json/price_seed.hpp"
#include "ih/utils/response_formatters.hpp"
#include "log/logging.hpp"

namespace Simulator::Http {

namespace {

[[nodiscard]]
auto unmarshallRequestBody(std::string_view _body,
                           DataLayer::PriceSeed::Patch& _dest,
                           Pistache::Http::Code& _code,
                           std::string& _content) noexcept -> bool {
  try {
    Json::PriceSeedUnmarshaller::unmarshall(_body, _dest);
    return true;
  } catch (std::runtime_error const& _ex) {
    _code = Pistache::Http::Code::Bad_Request;
    _content = formatResultResponse(fmt::format("Bad request: {}", _ex.what()));
  } catch (...) {
    simulator::log::err(
        "failed to unmarshall PriceSeed model, unexpected error raised");
    _code = Pistache::Http::Code::Internal_Server_Error;
    _content = formatResultResponse("Failed to unmarshall request body");
  }

  return false;
}

[[nodiscard]]
auto makeSeedUpdateTimePatch() -> DataLayer::Setting::Patch {
  using std::chrono::system_clock;

  std::time_t const now = system_clock::to_time_t(system_clock::now());
  std::tm utcTime{};
  gmtime_r(&now, &utcTime);

  std::string updateTime = fmt::format("{:%F %T}", utcTime);
  return DataLayer::Setting::Patch{}.withValue(std::move(updateTime));
}

}  // namespace

PriceSeedController::PriceSeedController(
    DataBridge::PriceSeedAccessor const& _seedAccessor,
    DataBridge::SettingAccessor const& _settingAccessor) noexcept
    : mSeedAccessor(_seedAccessor), mSettingAccessor(_settingAccessor) {}

auto PriceSeedController::selectPriceSeed(std::uint64_t _seedId) const
    -> Result {
  Pistache::Http::Code code{};
  std::string content{};

  auto opResult = mSeedAccessor.get().selectSingle(_seedId);
  if (opResult) {
    const DataLayer::PriceSeed& selected = opResult.value();
    try {
      content = Json::PriceSeedMarshaller::marshall(selected);
      code = Pistache::Http::Code::Ok;
    } catch (std::exception const& _ex) {
      simulator::log::warn("failed to marshall price seed to JSON: {}",
                           _ex.what());
      code = Pistache::Http::Code::Internal_Server_Error;
      content = formatResultResponse("failed to marshall price seed");
    }
  } else {
    auto const failure = opResult.error();
    content = formatErrorResponse(failure);
    switch (failure) {
      case DataBridge::Failure::ResponseCardinalityError:
        code = Pistache::Http::Code::Not_Found;
        break;
      default:
        code = Pistache::Http::Code::Internal_Server_Error;
        break;
    }
  }

  return std::make_pair(code, std::move(content));
}

auto PriceSeedController::selectAllPriceSeeds() const -> Result {
  Pistache::Http::Code code{};
  std::string content{};

  auto opResult = mSeedAccessor.get().selectAll();
  if (opResult) {
    try {
      const std::vector<DataLayer::PriceSeed>& seeds = opResult.value();
      content = Json::PriceSeedMarshaller::marshall(seeds);
      code = Pistache::Http::Code::Ok;
      simulator::log::info("selected {} price seed records", seeds.size());
    } catch (std::exception const& _ex) {
      simulator::log::warn("failed to marshall price seeds list to JSON: {}",
                           _ex.what());
      code = Pistache::Http::Code::Internal_Server_Error;
      content = formatResultResponse("failed to marshall price seeds");
    }
  } else {
    auto const failure = opResult.error();
    content = formatErrorResponse(failure);
    code = Pistache::Http::Code::Internal_Server_Error;
  }

  return std::make_pair(code, std::move(content));
}

auto PriceSeedController::insertPriceSeed(std::string const& _body) const
    -> Result {
  Pistache::Http::Code code{};
  std::string content{};

  DataLayer::PriceSeed::Patch seedSnapshot{};
  if (!unmarshallRequestBody(_body, seedSnapshot, code, content)) {
    return std::make_pair(code, std::move(content));
  }

  auto opResult = mSeedAccessor.get().add(seedSnapshot);
  if (opResult) {
    simulator::log::info("successfully added a new price seed");
    code = Pistache::Http::Code::Created;
    content = formatResultResponse("Successfully inserted new price seed");
  } else {
    auto const failure = opResult.error();
    content = formatErrorResponse(failure);
    switch (failure) {
      case DataBridge::Failure::MalformedInput:
      case DataBridge::Failure::DataIntegrityViolationError:
        code = Pistache::Http::Code::Bad_Request;
        break;
      default:
        code = Pistache::Http::Code::Internal_Server_Error;
        break;
    }
  }

  return std::make_pair(code, std::move(content));
}

auto PriceSeedController::updatePriceSeed(std::uint64_t _seedId,
                                          std::string const& _body) const
    -> Result {
  Pistache::Http::Code code{};
  std::string content{};

  DataLayer::PriceSeed::Patch patch{};
  if (!unmarshallRequestBody(_body, patch, code, content)) {
    return std::make_pair(code, std::move(content));
  }

  auto opResult = mSeedAccessor.get().update(patch, _seedId);
  if (opResult) {
    code = Pistache::Http::Code::Ok;
    content = formatResultResponse(fmt::format(
        "Successfully updated the price seed with {} identifier", _seedId));

    simulator::log::info(
        "successfully updated the price seed with `{}' identifier", _seedId);
  } else {
    auto const failure = opResult.error();
    content = formatErrorResponse(failure);
    switch (failure) {
      case DataBridge::Failure::MalformedInput:
      case DataBridge::Failure::DataIntegrityViolationError:
        code = Pistache::Http::Code::Bad_Request;
        break;
      case DataBridge::Failure::ResponseCardinalityError:
        code = Pistache::Http::Code::Not_Found;
        break;
      default:
        code = Pistache::Http::Code::Internal_Server_Error;
        break;
    }
  }

  return std::make_pair(code, std::move(content));
}

auto PriceSeedController::deletePriceSeed(std::uint64_t _seedId) const
    -> Result {
  Pistache::Http::Code code{};
  std::string content{};

  auto opResult = mSeedAccessor.get().dropSingle(_seedId);
  if (opResult) {
    code = Pistache::Http::Code::No_Content;
    content.clear();
  } else {
    auto const failure = opResult.error();
    content = formatErrorResponse(failure);
    switch (failure) {
      case DataBridge::Failure::ResponseCardinalityError:
        code = Pistache::Http::Code::Bad_Request;
        break;
      default:
        code = Pistache::Http::Code::Internal_Server_Error;
        break;
    }
  }

  return std::make_pair(code, std::move(content));
}

auto PriceSeedController::syncPriceSeeds() const -> Result {
  Pistache::Http::Code code{};
  std::string content{};

  auto settingRes = mSettingAccessor.get().selectSingle(msConnectionSetting);
  if (!settingRes) {
    code = Pistache::Http::Code::Conflict;
    content =
        formatResultResponse("Price seed database connection in not defined");
    return std::make_pair(code, std::move(content));
  }

  const auto& connection = settingRes->getValue();
  if (!connection.has_value()) {
    code = Pistache::Http::Code::Conflict;
    content =
        formatResultResponse("Price seed database connection has no value");
    return std::make_pair(code, std::move(content));
  }

  simulator::log::info("synchronizing price seeds from `{}'", *connection);
  auto opResult = mSeedAccessor.get().sync(*connection);
  if (!opResult) {
    code = Pistache::Http::Code::Internal_Server_Error;
    content = formatResultResponse(
        "Failed to synchronize price seeds because of unknown reason");
    return std::make_pair(code, std::move(content));
  }

  simulator::log::info(
      "updating the last price seed sync time general setting");
  auto const patch = makeSeedUpdateTimePatch();
  auto updateRes = mSettingAccessor.get().update(patch, msSyncTimeSetting);
  if (!updateRes) {
    simulator::log::warn(
        "Failed to update last price seeds sync setting in the general "
        "settings table");
  }

  code = Pistache::Http::Code::Ok;
  content = formatResultResponse("Price seeds successfully synchronized");

  return std::make_pair(code, std::move(content));
}

auto PriceSeedController::formatErrorResponse(DataBridge::Failure _failure)
    -> std::string {
  std::string message{};
  switch (_failure) {
    case DataBridge::Failure::MalformedInput:
      message = "Required fields missing for PriceSeed model in request";
      break;
    case DataBridge::Failure::DatabaseConnectionError:
      message = "Database connection failed";
      break;
    case DataBridge::Failure::ResponseDecodingError:
      message = "Failed to decode a database response";
      break;
    case DataBridge::Failure::ResponseCardinalityError:
      message = "Can not resolve a single PriceSeed by a given key";
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

std::string const PriceSeedController::msConnectionSetting{
    "SeedPriceDatabaseConnection"};

std::string const PriceSeedController::msSyncTimeSetting{
    "SeedPricesLastUpdated"};

}  // namespace Simulator::Http