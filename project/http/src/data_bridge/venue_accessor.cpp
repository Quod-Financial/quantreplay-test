#include "ih/data_bridge/venue_accessor.hpp"

#include "data_layer/api/data_access_layer.hpp"
#include "log/logging.hpp"

namespace Simulator::Http::DataBridge {

DataLayerVenueAccessor::DataLayerVenueAccessor(DbContext _context) noexcept
    : context_(std::move(_context)) {}

auto DataLayerVenueAccessor::selectSingle(std::string const& _id) const noexcept
    -> tl::expected<DataLayer::Venue, Failure> {
  using Column = DataLayer::Venue::Attribute;
  using DataLayer::VenueCmp;

  try {
    auto pred = VenueCmp::eq(Column::VenueId, _id);
    return DataLayer::selectOneVenue(context_, std::move(pred));
  } catch (DataLayer::ConnectionFailure const&) {
    return tl::unexpected{Failure::DatabaseConnectionError};
  } catch (DataLayer::CardinalityViolationError const&) {
    return tl::unexpected{Failure::ResponseCardinalityError};
  } catch (DataLayer::DataDecodingError const&) {
    return tl::unexpected{Failure::ResponseDecodingError};
  } catch (std::exception const& _ex) {
    simulator::log::warn(
        "data access layer raised unexpected exception while selecting "
        "single venue by `{}' identifier: {}",
        _id,
        _ex.what());
  } catch (...) {
    simulator::log::err(
        "unknown error is raised by data access layer while selecting "
        "single venue by `{}' identifier",
        _id);
  }

  return tl::unexpected{Failure::UnknownError};
}

auto DataLayerVenueAccessor::selectAll() const noexcept
    -> tl::expected<std::vector<DataLayer::Venue>, Failure> {
  try {
    return DataLayer::selectAllVenues(context_);
  } catch (DataLayer::ConnectionFailure const&) {
    return tl::unexpected{Failure::DatabaseConnectionError};
  } catch (DataLayer::DataDecodingError const&) {
    return tl::unexpected{Failure::ResponseDecodingError};
  } catch (std::exception const& _ex) {
    simulator::log::warn(
        "data access layer raised unexpected exception while selecting all "
        "venues: {}",
        _ex.what());
  } catch (...) {
    simulator::log::err(
        "unknown error is raised by data access layer while selecting all "
        "venues");
  }

  return tl::unexpected{Failure::UnknownError};
}
auto DataLayerVenueAccessor::add(DataLayer::Venue::Patch _init) const noexcept
    -> tl::expected<void, Failure> {
  try {
    DataLayer::insertVenue(context_, std::move(_init));
    return {};
  } catch (DataLayer::ConnectionFailure const&) {
    return tl::unexpected{Failure::DatabaseConnectionError};
  } catch (DataLayer::CardinalityViolationError const&) {
    return tl::unexpected{Failure::ResponseCardinalityError};
  } catch (DataLayer::MalformedPatch const&) {
    return tl::unexpected{Failure::MalformedInput};
  } catch (DataLayer::DataIntegrityError const&) {
    return tl::unexpected{Failure::DataIntegrityViolationError};
  } catch (DataLayer::DataDecodingError const&) {
    return tl::unexpected{Failure::ResponseDecodingError};
  } catch (std::exception const& _ex) {
    simulator::log::warn(
        "data access layer raised unexpected exception while adding a new "
        "venue: {}",
        _ex.what());
  } catch (...) {
    simulator::log::err(
        "Unknown error is raised by data access layer while adding a new "
        "venue");
  }

  return tl::unexpected{Failure::UnknownError};
}
auto DataLayerVenueAccessor::update(DataLayer::Venue::Patch _update,
                                    std::string const& _venueId) const noexcept
    -> tl::expected<void, Failure> {
  using Column = DataLayer::Venue::Attribute;
  using DataLayer::VenueCmp;

  try {
    auto pred = VenueCmp::eq(Column ::VenueId, _venueId);
    auto venue = DataLayer::updateOneVenue(
        context_, std::move(_update), std::move(pred));

    return {};
  } catch (DataLayer::ConnectionFailure const&) {
    return tl::unexpected{Failure::DatabaseConnectionError};
  } catch (DataLayer::CardinalityViolationError const&) {
    return tl::unexpected{Failure::ResponseCardinalityError};
  } catch (DataLayer::MalformedPatch const&) {
    return tl::unexpected{Failure::MalformedInput};
  } catch (DataLayer::DataIntegrityError const&) {
    return tl::unexpected{Failure::DataIntegrityViolationError};
  } catch (DataLayer::DataDecodingError const&) {
    return tl::unexpected{Failure::ResponseDecodingError};
  } catch (std::exception const& _ex) {
    simulator::log::warn(
        "data access layer raised unexpected exception while updating "
        "the venue with `{}' identifier: {}",
        _venueId,
        _ex.what());
  } catch (...) {
    simulator::log::err(
        "Unknown error is raised by data access layer while updating "
        "the venue with `{}' identifier",
        _venueId);
  }

  return tl::unexpected{Failure::UnknownError};
}

}  // namespace Simulator::Http::DataBridge
