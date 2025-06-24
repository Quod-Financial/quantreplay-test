#include "ih/controllers/datasource_controller.hpp"

#include <fmt/format.h>

#include <string>
#include <utility>

#include "data_layer/api/models/datasource.hpp"
#include "ih/marshalling/json/datasource.hpp"
#include "ih/utils/response_formatters.hpp"
#include "log/logging.hpp"

namespace Simulator::Http {

namespace {

[[nodiscard]]
auto unmarshallRequestBody(std::string_view _body,
                           DataLayer::Datasource::Patch& _dest,
                           Pistache::Http::Code& _code,
                           std::string& _content) noexcept -> bool {
  try {
    Json::DatasourceUnmarshaller::unmarshall(_body, _dest);
    return true;
  } catch (std::runtime_error const& _ex) {
    _code = Pistache::Http::Code::Bad_Request;
    _content = formatResultResponse(fmt::format("Bad request: {}", _ex.what()));
  } catch (...) {
    simulator::log::err(
        "ailed to unmarshall datasource, unexpected error raised");
    _code = Pistache::Http::Code::Internal_Server_Error;
    _content = formatResultResponse("Failed to unmarshall request body");
  }

  return false;
}

}  // namespace

DatasourceController::DatasourceController(
    DataBridge::DatasourceAccessor& _dataAccessor) noexcept
    : mDataAccessor(_dataAccessor) {}

auto DatasourceController::selectDatasource(std::uint64_t _datasourceId) const
    -> Result {
  Pistache::Http::Code code{};
  std::string content{};

  auto opResult = mDataAccessor.get().selectSingle(_datasourceId);
  if (opResult) {
    const DataLayer::Datasource& selected = opResult.value();
    try {
      content = Json::DatasourceMarshaller::marshall(selected);
      code = Pistache::Http::Code::Ok;
    } catch (std::exception const& _ex) {
      simulator::log::warn("failed to marshall datasource to JSON: {}",
                           _ex.what());
      code = Pistache::Http::Code::Internal_Server_Error;
      content = formatResultResponse("failed to marshall datasource");
    }
  } else {
    const auto failure_info = opResult.error();
    content = formatErrorResponse(failure_info);
    switch (failure_info.failure) {
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

auto DatasourceController::selectAllDatasources() const -> Result {
  Pistache::Http::Code code{};
  std::string content{};

  auto opResult = mDataAccessor.get().selectAll();
  if (!opResult) {
    content = formatErrorResponse(opResult.error());
    code = Pistache::Http::Code::Internal_Server_Error;
    return std::make_pair(code, std::move(content));
  }

  const std::vector<DataLayer::Datasource>& selected = opResult.value();
  try {
    content = Json::DatasourceMarshaller::marshall(selected);
    code = Pistache::Http::Code::Ok;
    simulator::log::info("selected {} datasource records", selected.size());
  } catch (std::exception const& _ex) {
    simulator::log::warn("failed to marshall datasources to JSON: {}",
                         _ex.what());
    code = Pistache::Http::Code::Internal_Server_Error;
    content = formatResultResponse("failed to marshall datasources");
  }

  return std::make_pair(code, std::move(content));
}

auto DatasourceController::insertDatasource(std::string const& _body) const
    -> Result {
  Pistache::Http::Code code{};
  std::string content{};

  DataLayer::Datasource::Patch datasource{};
  if (!unmarshallRequestBody(_body, datasource, code, content)) {
    return std::make_pair(code, std::move(content));
  }

  auto opResult = mDataAccessor.get().add(datasource);
  if (opResult) {
    simulator::log::info("successfully added a new venue");
    code = Pistache::Http::Code::Created;
    content = formatResultResponse("Successfully inserted a new datasource");
    return std::make_pair(code, std::move(content));
  }

  const auto failure_info = opResult.error();
  content = formatErrorResponse(failure_info);
  switch (failure_info.failure) {
    case DataBridge::Failure::MalformedInput:
    case DataBridge::Failure::DataIntegrityViolationError:
      code = Pistache::Http::Code::Bad_Request;
      break;
    default:
      code = Pistache::Http::Code::Internal_Server_Error;
      break;
  }
  return std::make_pair(code, std::move(content));
}

auto DatasourceController::updateDatasource(std::uint64_t _datasourceId,
                                            std::string const& _body) const
    -> Result {
  Pistache::Http::Code code{};
  std::string content{};

  DataLayer::Datasource::Patch patch{};
  if (!unmarshallRequestBody(_body, patch, code, content)) {
    return std::make_pair(code, std::move(content));
  }

  auto opResult = mDataAccessor.get().update(patch, _datasourceId);
  if (opResult) {
    code = Pistache::Http::Code::Ok;
    content = formatResultResponse(
        fmt::format("Successfully updated the datasource with `{}' identifier",
                    _datasourceId));

    simulator::log::info(
        "successfully updated the datasource with `{}' identifier",
        _datasourceId);
  } else {
    const auto failure_info = opResult.error();
    content = formatErrorResponse(failure_info);
    switch (failure_info.failure) {
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

auto DatasourceController::formatErrorResponse(
    DataBridge::FailureInfo failure_info) -> std::string {
  std::string message{};
  switch (failure_info.failure) {
    case DataBridge::Failure::MalformedInput:
      message = failure_info.message.empty()
                    ? "Required fields missing for datasource model in request"
                    : failure_info.message;
      break;
    case DataBridge::Failure::DatabaseConnectionError:
      message = "Database connection failed";
      break;
    case DataBridge::Failure::ResponseDecodingError:
      message = "Failed to decode a database response";
      break;
    case DataBridge::Failure::ResponseCardinalityError:
      message = "Can not resolve a single datasource by a given key";
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

}  // namespace Simulator::Http
