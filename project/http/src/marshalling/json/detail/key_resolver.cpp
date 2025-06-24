#include "ih/marshalling/json/detail/key_resolver.hpp"

#include <fmt/format.h>

#include <stdexcept>
#include <string_view>
#include <type_traits>

#include "ih/marshalling/json/detail/keys.hpp"

namespace Simulator::Http::Json {

namespace {

template <typename Attribute>
[[noreturn]]
auto raise_bad_attribute_error(std::string_view modelName, Attribute attribute)
    -> void {
  static_assert(std::is_enum_v<Attribute>);
  using Underlying = std::underlying_type_t<Attribute>;

  throw std::logic_error{fmt::format(
      "unable to resolve a JSON key by an undefined attribute enumeration "
      "value `{:#x}' for a {} model ",
      static_cast<Underlying>(attribute),
      modelName)};
}

}  // namespace

auto KeyResolver::resolve_key(DataLayer::ColumnMapping::Attribute attribute)
    -> std::string_view {
  std::string_view key{};
  switch (attribute) {
    case DataLayer::ColumnMapping::Attribute::ColumnFrom:
      key = ColumnMappingKey::ColumnFrom;
      break;
    case DataLayer::ColumnMapping::Attribute::ColumnTo:
      key = ColumnMappingKey::ColumnTo;
      break;
    case DataLayer::ColumnMapping::Attribute::DatasourceId:
      key = ColumnMappingKey::DatasourceId;
      break;
  }

  if (!key.empty()) {
    return key;
  }
  raise_bad_attribute_error("ColumnMapping", attribute);
}

auto KeyResolver::resolve_key(DataLayer::Datasource::Attribute attribute)
    -> std::string_view {
  std::string_view key{};
  switch (attribute) {
    case DataLayer::Datasource::Attribute::DatasourceId:
      key = DatasourceKey::DatasourceId;
      break;
    case DataLayer::Datasource::Attribute::Enabled:
      key = DatasourceKey::Enabled;
      break;
    case DataLayer::Datasource::Attribute::Name:
      key = DatasourceKey::Name;
      break;
    case DataLayer::Datasource::Attribute::VenueId:
      key = DatasourceKey::VenueId;
      break;
    case DataLayer::Datasource::Attribute::Connection:
      key = DatasourceKey::Connection;
      break;
    case DataLayer::Datasource::Attribute::Format:
      key = DatasourceKey::Format;
      break;
    case DataLayer::Datasource::Attribute::Type:
      key = DatasourceKey::Type;
      break;
    case DataLayer::Datasource::Attribute::Repeat:
      key = DatasourceKey::Repeat;
      break;
    case DataLayer::Datasource::Attribute::TextDelimiter:
      key = DatasourceKey::TextDelimiter;
      break;
    case DataLayer::Datasource::Attribute::TextHeaderRow:
      key = DatasourceKey::TextHeaderRow;
      break;
    case DataLayer::Datasource::Attribute::TextDataRow:
      key = DatasourceKey::TextDataRow;
      break;
    case DataLayer::Datasource::Attribute::TableName:
      key = DatasourceKey::TableName;
      break;
    case DataLayer::Datasource::Attribute::MaxDepthLevels:
      key = DatasourceKey::MaxDepthLevels;
      break;
  }

  if (!key.empty()) {
    return key;
  }
  raise_bad_attribute_error("Datasource", attribute);
}

auto KeyResolver::resolve_key(DataLayer::Listing::Attribute attribute)
    -> std::string_view {
  std::string_view key{};
  switch (attribute) {
    case DataLayer::Listing::Attribute::ListingId:
      key = ListingKey::ListingId;
      break;
    case DataLayer::Listing::Attribute::Symbol:
      key = ListingKey::Symbol;
      break;
    case DataLayer::Listing::Attribute::VenueId:
      key = ListingKey::VenueId;
      break;
    case DataLayer::Listing::Attribute::SecurityType:
      key = ListingKey::SecurityType;
      break;
    case DataLayer::Listing::Attribute::PriceCurrency:
      key = ListingKey::PriceCurrency;
      break;
    case DataLayer::Listing::Attribute::FxBaseCurrency:
      key = ListingKey::FxBaseCurrency;
      break;
    case DataLayer::Listing::Attribute::InstrSymbol:
      key = ListingKey::InstrSymbol;
      break;
    case DataLayer::Listing::Attribute::QtyMinimum:
      key = ListingKey::QtyMinimum;
      break;
    case DataLayer::Listing::Attribute::QtyMaximum:
      key = ListingKey::QtyMaximum;
      break;
    case DataLayer::Listing::Attribute::QtyMultiple:
      key = ListingKey::QtyMultiple;
      break;
    case DataLayer::Listing::Attribute::PriceTickSize:
      key = ListingKey::PriceTickSize;
      break;
    case DataLayer::Listing::Attribute::Enabled:
      key = ListingKey::Enabled;
      break;
    case DataLayer::Listing::Attribute::RandomQtyMaximum:
      key = ListingKey::RandomQtyMaximum;
      break;
    case DataLayer::Listing::Attribute::RandomDepthLevels:
      key = ListingKey::RandomDepthLevels;
      break;
    case DataLayer::Listing::Attribute::RandomOrdersSpread:
      key = ListingKey::RandomOrdersSpread;
      break;
    case DataLayer::Listing::Attribute::RandomOrdersRate:
      key = ListingKey::RandomOrdersRate;
      break;
    case DataLayer::Listing::Attribute::RandomTickRange:
      key = ListingKey::RandomTickRange;
      break;
    case DataLayer::Listing::Attribute::SecurityExchange:
      key = ListingKey::SecurityExchange;
      break;
    case DataLayer::Listing::Attribute::PartyId:
      key = ListingKey::PartyId;
      break;
    case DataLayer::Listing::Attribute::PartyRole:
      key = ListingKey::PartyRole;
      break;
    case DataLayer::Listing::Attribute::CusipId:
      key = ListingKey::CusipId;
      break;
    case DataLayer::Listing::Attribute::SedolId:
      key = ListingKey::SedolId;
      break;
    case DataLayer::Listing::Attribute::IsinId:
      key = ListingKey::IsinId;
      break;
    case DataLayer::Listing::Attribute::RicId:
      key = ListingKey::RicId;
      break;
    case DataLayer::Listing::Attribute::ExchangeSymbolId:
      key = ListingKey::ExchangeSymbolId;
      break;
    case DataLayer::Listing::Attribute::BloombergSymbolId:
      key = ListingKey::BloombergSymbolId;
      break;
    case DataLayer::Listing::Attribute::RandomQtyMinimum:
      key = ListingKey::RandomQtyMinimum;
      break;
    case DataLayer::Listing::Attribute::RandomAmtMinimum:
      key = ListingKey::RandomAmtMinimum;
      break;
    case DataLayer::Listing::Attribute::RandomAmtMaximum:
      key = ListingKey::RandomAmtMaximum;
      break;
    case DataLayer::Listing::Attribute::RandomOrdersEnabled:
      key = ListingKey::RandomOrdersEnabled;
      break;
    case DataLayer::Listing::Attribute::RandomAggressiveQtyMinimum:
      key = ListingKey::RandomAggressiveQtyMinimum;
      break;
    case DataLayer::Listing::Attribute::RandomAggressiveQtyMaximum:
      key = ListingKey::RandomAggressiveQtyMaximum;
      break;
    case DataLayer::Listing::Attribute::RandomAggressiveAmtMinimum:
      key = ListingKey::RandomAggressiveAmtMinimum;
      break;
    case DataLayer::Listing::Attribute::RandomAggressiveAmtMaximum:
      key = ListingKey::RandomAggressiveAmtMaximum;
      break;
  }

  if (!key.empty()) {
    return key;
  }
  raise_bad_attribute_error("Listing", attribute);
}

auto KeyResolver::resolve_key(DataLayer::MarketPhase::Attribute attribute)
    -> std::string_view {
  std::string_view key{};
  switch (attribute) {
    case DataLayer::MarketPhase::Attribute::Phase:
      key = MarketPhaseKey::Phase;
      break;
    case DataLayer::MarketPhase::Attribute::StartTime:
      key = MarketPhaseKey::StartTime;
      break;
    case DataLayer::MarketPhase::Attribute::EndTime:
      key = MarketPhaseKey::EndTime;
      break;
    case DataLayer::MarketPhase::Attribute::VenueId:
      key = MarketPhaseKey::VenueId;
      break;
    case DataLayer::MarketPhase::Attribute::EndTimeRange:
      key = MarketPhaseKey::EndTimeRange;
      break;
    case DataLayer::MarketPhase::Attribute::AllowCancels:
      key = MarketPhaseKey::AllowCancels;
      break;
  }

  if (!key.empty()) {
    return key;
  }
  raise_bad_attribute_error("MarketPhase", attribute);
}

auto KeyResolver::resolve_key(DataLayer::PriceSeed::Attribute attribute)
    -> std::string_view {
  std::string_view key{};
  switch (attribute) {
    case DataLayer::PriceSeed::Attribute::PriceSeedId:
      key = PriceSeedKey::PriceSeedId;
      break;
    case DataLayer::PriceSeed::Attribute::Symbol:
      key = PriceSeedKey::Symbol;
      break;
    case DataLayer::PriceSeed::Attribute::SecurityType:
      key = PriceSeedKey::SecurityType;
      break;
    case DataLayer::PriceSeed::Attribute::PriceCurrency:
      key = PriceSeedKey::PriceCurrency;
      break;
    case DataLayer::PriceSeed::Attribute::SecurityId:
      key = PriceSeedKey::SecurityId;
      break;
    case DataLayer::PriceSeed::Attribute::SecurityIdSource:
      key = PriceSeedKey::SecurityIdSource;
      break;
    case DataLayer::PriceSeed::Attribute::InstrumentSymbol:
      key = PriceSeedKey::InstrumentSymbol;
      break;
    case DataLayer::PriceSeed::Attribute::MidPrice:
      key = PriceSeedKey::MidPrice;
      break;
    case DataLayer::PriceSeed::Attribute::BidPrice:
      key = PriceSeedKey::BidPrice;
      break;
    case DataLayer::PriceSeed::Attribute::OfferPrice:
      key = PriceSeedKey::OfferPrice;
      break;
    case DataLayer::PriceSeed::Attribute::LastUpdate:
      key = PriceSeedKey::LastUpdate;
      break;
  }

  if (!key.empty()) {
    return key;
  }
  raise_bad_attribute_error("PriceSeed", attribute);
}

auto KeyResolver::resolve_key(DataLayer::Setting::Attribute attribute)
    -> std::string_view {
  std::string_view key{};
  switch (attribute) {
    case DataLayer::Setting::Attribute::Key:
      key = SettingKey::Key;
      break;
    case DataLayer::Setting::Attribute::Value:
      key = SettingKey::Value;
      break;
  }

  if (!key.empty()) {
    return key;
  }
  raise_bad_attribute_error("Setting", attribute);
}

auto KeyResolver::resolve_key(DataLayer::Venue::Attribute attribute)
    -> std::string_view {
  std::string_view key{};
  switch (attribute) {
    case DataLayer::Venue::Attribute::VenueId:
      key = VenueKey::VenueId;
      break;
    case DataLayer::Venue::Attribute::Name:
      key = VenueKey::Name;
      break;
    case DataLayer::Venue::Attribute::EngineType:
      key = VenueKey::EngineType;
      break;
    case DataLayer::Venue::Attribute::SupportTifIoc:
      key = VenueKey::SupportTifIoc;
      break;
    case DataLayer::Venue::Attribute::SupportTifFok:
      key = VenueKey::SupportTifFok;
      break;
    case DataLayer::Venue::Attribute::SupportTifDay:
      key = VenueKey::SupportTifDay;
      break;
    case DataLayer::Venue::Attribute::IncludeOwnOrders:
      key = VenueKey::IncludeOwnOrders;
      break;
    case DataLayer::Venue::Attribute::RestPort:
      key = VenueKey::RestPort;
      break;
    case DataLayer::Venue::Attribute::OrderOnStartup:
      key = VenueKey::OrderOnStartup;
      break;
    case DataLayer::Venue::Attribute::RandomPartiesCount:
      key = VenueKey::RandomPartiesCount;
      break;
    case DataLayer::Venue::Attribute::TnsEnabled:
      key = VenueKey::TnsEnabled;
      break;
    case DataLayer::Venue::Attribute::TnsQtyEnabled:
      key = VenueKey::TnsQtyEnabled;
      break;
    case DataLayer::Venue::Attribute::TnsSideEnabled:
      key = VenueKey::TnsSideEnabled;
      break;
    case DataLayer::Venue::Attribute::TnsPartiesEnabled:
      key = VenueKey::TnsPartiesEnabled;
      break;
    case DataLayer::Venue::Attribute::Timezone:
      key = VenueKey::Timezone;
      break;
    case DataLayer::Venue::Attribute::CancelOnDisconnect:
      key = VenueKey::CancelOnDisconnect;
      break;
    case DataLayer::Venue::Attribute::PersistenceEnabled:
      key = VenueKey::PersistenceEnabled;
      break;
    case DataLayer::Venue::Attribute::PersistenceFilePath:
      key = VenueKey::PersistenceFilePath;
      break;
  }

  if (!key.empty()) {
    return key;
  }
  raise_bad_attribute_error("Venue", attribute);
}

}  // namespace Simulator::Http::Json