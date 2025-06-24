#include "ih/controllers/venue_controller.hpp"

#include <fmt/format.h>

#include <string>
#include <utility>

#include "data_layer/api/models/venue.hpp"
#include "ih/marshalling/json/venue.hpp"
#include "ih/utils/response_formatters.hpp"
#include "log/logging.hpp"

namespace Simulator::Http {

namespace {

[[nodiscard]]
auto unmarshallRequestBody(std::string_view _body,
                           DataLayer::Venue::Patch& _dest,
                           Pistache::Http::Code& _code,
                           std::string& _content) noexcept -> bool {
  try {
    Json::VenueUnmarshaller::unmarshall(_body, _dest);
    return true;
  } catch (std::runtime_error const& _ex) {
    _code = Pistache::Http::Code::Bad_Request;
    _content = formatResultResponse(fmt::format("Bad request: {}", _ex.what()));
  } catch (...) {
    simulator::log::err(
        "failed to unmarshall Venue model, unexpected error raised");
    _code = Pistache::Http::Code::Internal_Server_Error;
    _content = formatResultResponse("Failed to unmarshall request body");
  }

  return false;
}

}  // namespace

VenueController::VenueController(DataBridge::VenueAccessor& _bridge) noexcept
    : mDataAccessor(_bridge) {}

auto VenueController::selectVenue(std::string const& _venueId) const -> Result {
  Pistache::Http::Code code{};
  std::string content{};

  auto opResult = mDataAccessor.get().selectSingle(_venueId);
  if (opResult) {
    const DataLayer::Venue& selected = opResult.value();
    try {
      content = Json::VenueMarshaller::marshall(selected);
      code = Pistache::Http::Code::Ok;
    } catch (std::exception const& _ex) {
      simulator::log::warn("failed to marshall venue model to JSON: {}",
                           _ex.what());
      code = Pistache::Http::Code::Internal_Server_Error;
      content = formatResultResponse("failed to marshall Venue model");
    }
  } else {
    const auto failure = opResult.error();
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

auto VenueController::selectAllVenues() const -> Result {
  Pistache::Http::Code code{};
  std::string content{};

  auto opResult = mDataAccessor.get().selectAll();
  if (opResult) {
    try {
      std::vector<DataLayer::Venue> const& venues = opResult.value();
      content = Json::VenueMarshaller::marshall(venues);
      code = Pistache::Http::Code::Ok;
      simulator::log::info("selected {} venue records", venues.size());
    } catch (std::exception const& _ex) {
      simulator::log::warn("failed to marshall venues list to JSON: {}",
                           _ex.what());
      code = Pistache::Http::Code::Internal_Server_Error;
      content = formatResultResponse("failed to marshall venues list");
    }
  } else {
    auto const failure = opResult.error();
    content = formatErrorResponse(failure);
    code = Pistache::Http::Code::Internal_Server_Error;
  }

  return std::make_pair(code, std::move(content));
}

auto VenueController::insertVenue(std::string const& _venueJson) const
    -> Result {
  Pistache::Http::Code code{};
  std::string content{};

  DataLayer::Venue::Patch venueSnapshot{};
  if (!unmarshallRequestBody(_venueJson, venueSnapshot, code, content)) {
    return std::make_pair(code, std::move(content));
  }

  auto opResult = mDataAccessor.get().add(venueSnapshot);
  if (opResult) {
    simulator::log::info("successfully added a new venue");
    code = Pistache::Http::Code::Created;
    content = formatResultResponse("Successfully inserted a new Venue");
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

auto VenueController::updateVenue(std::string const& _venueId,
                                  std::string const& _body) const -> Result {
  Pistache::Http::Code code{};
  std::string content{};

  DataLayer::Venue::Patch patch{};
  if (!unmarshallRequestBody(_body, patch, code, content)) {
    return std::make_pair(code, std::move(content));
  }

  auto opResult = mDataAccessor.get().update(patch, _venueId);
  if (opResult) {
    code = Pistache::Http::Code::Ok;
    content = formatResultResponse(
        fmt::format("Successfully updated the {} venue", _venueId));

    simulator::log::info("successfully updated the venue with `{}' identifier",
                         _venueId);
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

auto VenueController::formatErrorResponse(DataBridge::Failure _failure)
    -> std::string {
  std::string message{};
  switch (_failure) {
    case DataBridge::Failure::MalformedInput:
      message = "Required fields missing for Venue model in request";
      break;
    case DataBridge::Failure::DatabaseConnectionError:
      message = "Database connection failed";
      break;
    case DataBridge::Failure::ResponseDecodingError:
      message = "Failed to decode a database response";
      break;
    case DataBridge::Failure::ResponseCardinalityError:
      message = "Can not resolve a single Venue by a given key";
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
