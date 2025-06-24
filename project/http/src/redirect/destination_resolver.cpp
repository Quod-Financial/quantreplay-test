#include "ih/redirect/destination_resolver.hpp"

#include "cfg/api/cfg.hpp"
#include "ih/formatters/redirect.hpp"
#include "log/logging.hpp"

namespace Simulator::Http::Redirect {

DestinationResolver::DestinationResolver(
    DataBridge::VenueAccessor const& _venueAccessor,
    bool _useVenueIdAsPeerHost) noexcept
    : mVenueAccessor{_venueAccessor},
      mUseVenueIdAsPeerHost{_useVenueIdAsPeerHost} {}

Resolver::ResolvingResult DestinationResolver::resolveByVenueID(
    std::string const& _venueID) const noexcept try {
  auto opResult = mVenueAccessor.get().selectSingle(_venueID);
  if (!opResult) {
    simulator::log::err(
        "failed to resolve destination with '{0}' VenueID - "
        "venue {0} was not found in the database",
        _venueID);
    return std::make_pair(std::nullopt, Status::NonexistentInstance);
  }

  const DataLayer::Venue& venue = opResult.value();
  auto const destination_port = venue.getRestPort();
  if (!destination_port.has_value()) {
    simulator::log::err(
        "failed to resolve destination with '{0}' VenueID - "
        "venue entry with an '{0}' identifier has no REST port specified",
        _venueID);
    return std::make_pair(std::nullopt, Status::ResolvingFailed);
  }

  std::string destination_host = mUseVenueIdAsPeerHost ? _venueID : "localhost";

  Destination destination{std::move(destination_host), *destination_port};
  simulator::log::debug(
      "successfully resolved {} for '{}'", destination, _venueID);
  return std::make_pair(std::move(destination), Status::Success);
} catch (std::exception const& _ex) {
  simulator::log::err(
      "failed to resolve destination with '{}' VenueID - "
      "an error occurred while resolving destination: {}",
      _venueID,
      _ex.what());
  return std::make_pair(std::nullopt, Status::UnknownError);
} catch (...) {
  simulator::log::err(
      "failed to resolve destination with '{}' VenueID - "
      "unknown error occurred while resolving destination",
      _venueID);
  return std::make_pair(std::nullopt, Status::UnknownError);
}

std::shared_ptr<DestinationResolver> DestinationResolver::create(
    DataBridge::VenueAccessor const& _venueAccessor) {
  const bool resolveHostnameAsVenueId =
      Cfg::http().peer_resolution ==
      Cfg::HttpConfiguration::PeerHostResolution::VenueId;

  return std::make_shared<DestinationResolver>(_venueAccessor,
                                               resolveHostnameAsVenueId);
}

}  // namespace Simulator::Http::Redirect