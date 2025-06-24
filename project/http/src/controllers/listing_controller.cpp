#include "ih/controllers/listing_controller.hpp"

#include <fmt/format.h>

#include <string>
#include <utility>

#include "core/tools/numeric.hpp"
#include "data_layer/api/models/listing.hpp"
#include "ih/marshalling/json/listing.hpp"
#include "ih/utils/response_formatters.hpp"
#include "log/logging.hpp"

namespace Simulator::Http {

namespace {

[[nodiscard]]
auto unmarshallRequestBody(std::string_view _body,
                           DataLayer::Listing::Patch& _dest,
                           Pistache::Http::Code& _code,
                           std::string& _content) noexcept -> bool {
  try {
    Json::ListingUnmarshaller::unmarshall(_body, _dest);
    return true;
  } catch (std::runtime_error const& _ex) {
    _code = Pistache::Http::Code::Bad_Request;
    _content = formatResultResponse(fmt::format("Bad request: {}", _ex.what()));
  } catch (...) {
    simulator::log::err(
        "failed to unmarshall Listing model, unexpected error raised");

    _code = Pistache::Http::Code::Internal_Server_Error;
    _content = formatResultResponse("Failed to unmarshall request body");
  }

  return false;
}

}  // namespace

ListingController::ListingController(
    DataBridge::ListingAccessor& _dataAccessor) noexcept
    : mDataAccessor(_dataAccessor) {}

auto ListingController::selectListing(std::string const& _key) const -> Result {
  Pistache::Http::Code code{};
  std::string content{};

  auto opResult = [&] {
    if (simulator::core::is_number(_key)) {
      std::uint64_t const listingId = std::stoull(_key);
      return mDataAccessor.get().selectSingle(listingId);
    }
    return mDataAccessor.get().selectSingle(_key);
  }();

  if (opResult) {
    const DataLayer::Listing& selected = opResult.value();
    try {
      content = Json::ListingMarshaller::marshall(selected);
      code = Pistache::Http::Code::Ok;
    } catch (std::exception const& _ex) {
      simulator::log::warn("failed to marshall listing model to JSON: {}",
                           _ex.what());
      code = Pistache::Http::Code::Internal_Server_Error;
      content = formatResultResponse("failed to marshall Listing model");
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

auto ListingController::selectAllListings() const -> Result {
  Pistache::Http::Code code{};
  std::string content{};

  auto opResult = mDataAccessor.get().selectAll();
  if (opResult) {
    try {
      std::vector<DataLayer::Listing> const& selected = opResult.value();
      content = Json::ListingMarshaller::marshall(selected);
      code = Pistache::Http::Code::Ok;
      simulator::log::info("selected {} listing records", selected.size());
    } catch (std::exception const& _ex) {
      simulator::log::warn("failed to marshall listings list to JSON: {}",
                           _ex.what());
      code = Pistache::Http::Code::Internal_Server_Error;
      content = formatResultResponse("failed to marshall listings list");
    }
  } else {
    auto const failure = opResult.error();
    content = formatErrorResponse(failure);
    code = Pistache::Http::Code::Internal_Server_Error;
  }

  return std::make_pair(code, std::move(content));
}

auto ListingController::insertListing(std::string const& _body) const
    -> Result {
  Pistache::Http::Code code{};
  std::string content{};

  DataLayer::Listing::Patch listingSnapshot{};
  if (!unmarshallRequestBody(_body, listingSnapshot, code, content)) {
    return std::make_pair(code, std::move(content));
  }

  auto opResult = mDataAccessor.get().add(listingSnapshot);
  if (opResult) {
    simulator::log::info("successfully added a new listing");
    code = Pistache::Http::Code::Created;
    content = formatResultResponse("Successfully inserted a new Listing");
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

auto ListingController::updateListing(std::string const& _key,
                                      std::string const& _body) const
    -> Result {
  Pistache::Http::Code code{};
  std::string content{};

  DataLayer::Listing::Patch patch{};
  if (!unmarshallRequestBody(_body, patch, code, content)) {
    return std::make_pair(code, std::move(content));
  }

  auto opResult = [&] {
    if (simulator::core::is_number(_key)) {
      std::uint64_t const listingId = std::stoull(_key);
      return mDataAccessor.get().update(std::move(patch), listingId);
    }
    return mDataAccessor.get().update(std::move(patch), _key);
  }();

  if (opResult) {
    code = Pistache::Http::Code::Ok;
    content = formatResultResponse(
        fmt::format("Successfully updated the {} listing", _key));

    simulator::log::info("successfully updated the listing by `{}' key", _key);
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

auto ListingController::formatErrorResponse(DataBridge::Failure _failure)
    -> std::string {
  std::string message{};
  switch (_failure) {
    case DataBridge::Failure::MalformedInput:
      message = "Required fields missing for Listing model in request";
      break;
    case DataBridge::Failure::DatabaseConnectionError:
      message = "Database connection failed";
      break;
    case DataBridge::Failure::ResponseDecodingError:
      message = "Failed to decode a database response";
      break;
    case DataBridge::Failure::ResponseCardinalityError:
      message = "Can not resolve a single Listing by a given key";
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
