#include "ih/data_bridge/listing_accessor.hpp"

#include "cfg/api/cfg.hpp"
#include "data_layer/api/data_access_layer.hpp"
#include "log/logging.hpp"

namespace Simulator::Http::DataBridge {

DataLayerListingAccessor::DataLayerListingAccessor(DbContext _ctx) noexcept
    : context_(std::move(_ctx)) {}

auto DataLayerListingAccessor::selectSingle(std::uint64_t _listingId)
    const noexcept -> tl::expected<DataLayer::Listing, Failure> {
  using Column = DataLayer::Listing::Attribute;
  using DataLayer::ListingCmp;

  try {
    auto pred = ListingCmp::eq(Column::ListingId, _listingId);
    return DataLayer::selectOneListing(context_, std::move(pred));
  } catch (DataLayer::ConnectionFailure const&) {
    return tl::unexpected{Failure::DatabaseConnectionError};
  } catch (DataLayer::CardinalityViolationError const&) {
    return tl::unexpected{Failure::ResponseCardinalityError};
  } catch (DataLayer::DataDecodingError const&) {
    return tl::unexpected{Failure::ResponseDecodingError};
  } catch (std::exception const& _ex) {
    simulator::log::warn(
        "data access layer raised unexpected exception while selecting "
        "single listing by `{}' identifier: {}",
        _listingId,
        _ex.what());
  } catch (...) {
    simulator::log::err(
        "unknown error is raised by data access layer while selecting "
        "single listing by `{}' identifier",
        _listingId);
  }

  return tl::unexpected{Failure::UnknownError};
}

auto DataLayerListingAccessor::selectSingle(std::string const& _symbol)
    const noexcept -> tl::expected<DataLayer::Listing, Failure> {
  using Column = DataLayer::Listing::Attribute;
  using DataLayer::ListingCmp;

  try {
    auto pred = ListingCmp::eq(Column::Symbol, _symbol) &&
                ListingCmp::eq(Column ::VenueId, Cfg::venue().name);
    return DataLayer::selectOneListing(context_, std::move(pred));
  } catch (DataLayer::ConnectionFailure const&) {
    return tl::unexpected{Failure::DatabaseConnectionError};
  } catch (DataLayer::CardinalityViolationError const&) {
    return tl::unexpected{Failure::ResponseCardinalityError};
  } catch (DataLayer::DataDecodingError const&) {
    return tl::unexpected{Failure::ResponseDecodingError};
  } catch (std::exception const& _ex) {
    simulator::log::warn(
        "data access layer raised unexpected exception while selecting "
        "single listing by `{}' symbol: {}",
        _symbol,
        _ex.what());
  } catch (...) {
    simulator::log::err(
        "Unknown error is raised by data access layer while selecting "
        "single listing by `{}' symbol",
        _symbol);
  }

  return tl::unexpected{Failure::UnknownError};
}

auto DataLayerListingAccessor::selectAll() const noexcept
    -> tl::expected<std::vector<DataLayer::Listing>, Failure> {
  try {
    return DataLayer::selectAllListings(context_);
  } catch (DataLayer::ConnectionFailure const&) {
    return tl::unexpected{Failure::DatabaseConnectionError};
  } catch (DataLayer::DataDecodingError const&) {
    return tl::unexpected{Failure::ResponseDecodingError};
  } catch (std::exception const& _ex) {
    simulator::log::warn(
        "data access layer raised unexpected exception while selecting all "
        "listings: {}",
        _ex.what());
  } catch (...) {
    simulator::log::err(
        "unknown error is raised by data access layer while selecting all "
        "listings");
  }

  return tl::unexpected{Failure::UnknownError};
}

auto DataLayerListingAccessor::add(DataLayer::Listing::Patch _snapshot)
    const noexcept -> tl::expected<void, Failure> {
  try {
    DataLayer::insertListing(context_, std::move(_snapshot));
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
        "listing: {}",
        _ex.what());
  } catch (...) {
    simulator::log::err(
        "unknown error is raised by data access layer while adding a new "
        "listing");
  }

  return tl::unexpected{Failure::UnknownError};
}

auto DataLayerListingAccessor::update(DataLayer::Listing::Patch _update,
                                      std::uint64_t _listingId) const noexcept
    -> tl::expected<void, Failure> {
  using Column = DataLayer::Listing::Attribute;
  using DataLayer::ListingCmp;

  try {
    auto pred = ListingCmp::eq(Column::ListingId, _listingId);
    DataLayer::updateOneListing(context_, std::move(_update), std::move(pred));
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
        "the listing with `{}' identifier: {}",
        _listingId,
        _ex.what());
  } catch (...) {
    simulator::log::err(
        "unknown error is raised by data access layer while updating "
        "the listing with `{}' identifier",
        _listingId);
  }

  return tl::unexpected{Failure::UnknownError};
}

auto DataLayerListingAccessor::update(DataLayer::Listing::Patch _update,
                                      std::string const& _symbol) const noexcept
    -> tl::expected<void, Failure> {
  using Column = DataLayer::Listing::Attribute;
  using DataLayer::ListingCmp;

  try {
    auto pred = ListingCmp::eq(Column::Symbol, _symbol) &&
                ListingCmp::eq(Column::VenueId, Cfg::venue().name);
    DataLayer::updateOneListing(context_, std::move(_update), std::move(pred));
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
        "the listing with `{}' symbol: {}",
        _symbol,
        _ex.what());
  } catch (...) {
    simulator::log::err(
        "unknown error is raised by data access layer while updating "
        "the listing with `{}' _symbol",
        _symbol);
  }

  return tl::unexpected{Failure::UnknownError};
}

}  // namespace Simulator::Http::DataBridge
