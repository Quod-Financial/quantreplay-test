#include "instruments/sources.hpp"

#include <utility>

#include "cfg/api/cfg.hpp"
#include "data_layer/api/data_access_layer.hpp"
#include "data_layer/api/models/listing.hpp"
#include "data_layer/api/predicate/predicate.hpp"
#include "log/logging.hpp"

namespace simulator::trading_system::instrument {

namespace detail {
namespace {

template <typename Original, typename Converted>
auto convert(const std::optional<Original>& original,
             std::optional<Converted>& converted) -> void {
  if (original.has_value()) {
    converted = Converted{*original};
  }
}

template <>
auto convert(const std::optional<std::string>& original,
             std::optional<SecurityType>& converted) -> void {
  if (!original.has_value()) {
    return;
  }

  const std::string_view security_type_string = *original;
  converted = convert_security_type_from_string(security_type_string);
  if (!converted.has_value()) {
    log::warn("ignoring an unsupported security type value `{}'",
              security_type_string);
  }
}

template <>
auto convert(const std::optional<std::string>& original,
             std::optional<PartyRole>& converted) -> void {
  if (!original.has_value()) {
    return;
  }

  const std::string_view party_role_string = *original;
  converted = convert_party_role_from_string(party_role_string);
  if (!converted.has_value()) {
    log::warn("ignoring an unsupported party role value `{}'",
              party_role_string);
  }
}

}  // namespace

auto create_instrument(const Simulator::DataLayer::Listing& listing) noexcept
    -> Instrument {
  Instrument instrument{};
  instrument.database_id = DatabaseId{listing.getListingId()};

  convert(listing.getSymbol(), instrument.symbol);
  convert(listing.getSecurityType(), instrument.security_type);
  convert(listing.getPriceCurrency(), instrument.price_currency);
  convert(listing.getFxBaseCurrency(), instrument.base_currency);
  convert(listing.getSecurityExchange(), instrument.security_exchange);
  convert(listing.getPartyId(), instrument.party_id);
  convert(listing.getPartyRole(), instrument.party_role);
  convert(listing.getCusipId(), instrument.cusip);
  convert(listing.getSedolId(), instrument.sedol);
  convert(listing.getIsinId(), instrument.isin);
  convert(listing.getRicId(), instrument.ric);
  convert(listing.getExchangeSymbolId(), instrument.exchange_id);
  convert(listing.getBloombergSymbolId(), instrument.bloomberg_id);
  convert(listing.getPriceTickSize(), instrument.price_tick);
  convert(listing.getQtyMultiple(), instrument.quantity_tick);
  convert(listing.getQtyMinimum(), instrument.min_quantity);
  convert(listing.getQtyMaximum(), instrument.max_quantity);

  return instrument;
}

}  // namespace detail

DatabaseSource::DatabaseSource(
    Simulator::DataLayer::Database::Context db) noexcept
    : db_(std::move(db)) {}

auto DatabaseSource::load_instruments(LoadingProcedure loading_proc) const
    -> void {
  using Simulator::DataLayer::Listing;
  using Simulator::DataLayer::selectAllListings;

  log::info("loading listings from the database");

  const std::vector<Listing> listings = selectAllListings(db_, predicate());
  log::debug("{} matching listings selected", listings.size());

  for (const auto& listing : listings) {
    loading_proc(detail::create_instrument(listing));
  }
}

auto DatabaseSource::predicate() -> Simulator::DataLayer::Listing::Predicate {
  using Simulator::DataLayer::Listing;
  using Simulator::DataLayer::ListingCmp;
  namespace Cfg = Simulator::Cfg;

  return ListingCmp::eq(Listing::Attribute::VenueId, Cfg::venue().name) &&
         ListingCmp::eq(Listing::Attribute::Enabled, true);
}

MemorySource::MemorySource(std::vector<Instrument> instruments) noexcept
    : instruments_(std::move(instruments)) {}

auto MemorySource::load_instruments(LoadingProcedure loading_proc) const
    -> void {
  for (const auto& instrument : instruments_) {
    loading_proc(instrument);
  }
}

auto MemorySource::add_instrument(Instrument instrument) -> MemorySource& {
  instruments_.emplace_back(std::move(instrument));
  return *this;
}

}  // namespace simulator::trading_system::instrument