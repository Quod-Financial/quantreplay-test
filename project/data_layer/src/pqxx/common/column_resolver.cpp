#include "ih/pqxx/common/column_resolver.hpp"

#include <string>
#include <string_view>

#include "ih/common/exceptions.hpp"
#include "ih/pqxx/common/names/database_entries.hpp"

namespace Simulator::DataLayer::Pqxx {

auto ColumnResolver::operator()(ColumnMapping::Attribute _attribute) const
    -> std::string {
  return ColumnResolver::toColumnName(_attribute);
}

auto ColumnResolver::operator()(Datasource::Attribute _attribute) const
    -> std::string {
  return ColumnResolver::toColumnName(_attribute);
}

auto ColumnResolver::operator()(Listing::Attribute _attribute) const
    -> std::string {
  return ColumnResolver::toColumnName(_attribute);
}

auto ColumnResolver::operator()(MarketPhase::Attribute _attribute) const
    -> std::string {
  return ColumnResolver::toColumnName(_attribute);
}

auto ColumnResolver::operator()(PriceSeed::Attribute _attribute) const
    -> std::string {
  return ColumnResolver::toColumnName(_attribute);
}

auto ColumnResolver::operator()(Setting::Attribute _attribute) const
    -> std::string {
  return ColumnResolver::toColumnName(_attribute);
}

auto ColumnResolver::operator()(Venue::Attribute _attribute) const
    -> std::string {
  return ColumnResolver::toColumnName(_attribute);
}

auto ColumnResolver::toColumnName(ColumnMapping::Attribute _attribute)
    -> std::string {
  std::string_view columnName{};
  switch (_attribute) {
    case ColumnMapping::Attribute::ColumnFrom:
      columnName = ColumnMappingColumn::ColumnFrom;
      break;
    case ColumnMapping::Attribute::ColumnTo:
      columnName = ColumnMappingColumn::ColumnTo;
      break;
    case ColumnMapping::Attribute::DatasourceId:
      columnName = ColumnMappingColumn::DatasourceId;
      break;
  }

  if (!columnName.empty()) {
    return std::string{columnName};
  }
  throw ColumnNameEncodingError(Table::ColumnMapping, _attribute);
}

auto ColumnResolver::toColumnName(Datasource::Attribute _attribute)
    -> std::string {
  std::string_view columnName{};

  switch (_attribute) {
    case Datasource::Attribute::DatasourceId:
      columnName = DatasourceColumn::DatasourceId;
      break;
    case Datasource::Attribute::Enabled:
      columnName = DatasourceColumn::Enabled;
      break;
    case Datasource::Attribute::Name:
      columnName = DatasourceColumn::Name;
      break;
    case Datasource::Attribute::VenueId:
      columnName = DatasourceColumn::VenueId;
      break;
    case Datasource::Attribute::Connection:
      columnName = DatasourceColumn::Connection;
      break;
    case Datasource::Attribute::Format:
      columnName = DatasourceColumn::Format;
      break;
    case Datasource::Attribute::Type:
      columnName = DatasourceColumn::Type;
      break;
    case Datasource::Attribute::Repeat:
      columnName = DatasourceColumn::Repeat;
      break;
    case Datasource::Attribute::TextDelimiter:
      columnName = DatasourceColumn::TextDelimiter;
      break;
    case Datasource::Attribute::TextHeaderRow:
      columnName = DatasourceColumn::TextHeaderRow;
      break;
    case Datasource::Attribute::TextDataRow:
      columnName = DatasourceColumn::TextDataRow;
      break;
    case Datasource::Attribute::TableName:
      columnName = DatasourceColumn::TableName;
      break;
    case Datasource::Attribute::MaxDepthLevels:
      columnName = DatasourceColumn::MaxDepthLevels;
      break;
  }

  if (!columnName.empty()) {
    return std::string{columnName};
  }
  throw ColumnNameEncodingError(Table::Datasource, _attribute);
}

auto ColumnResolver::toColumnName(Listing::Attribute _attribute)
    -> std::string {
  std::string_view columnName{};
  switch (_attribute) {
    case Listing::Attribute::ListingId:
      columnName = ListingColumn::ListingId;
      break;
    case Listing::Attribute::Symbol:
      columnName = ListingColumn::Symbol;
      break;
    case Listing::Attribute::VenueId:
      columnName = ListingColumn::VenueId;
      break;
    case Listing::Attribute::SecurityType:
      columnName = ListingColumn::SecurityType;
      break;
    case Listing::Attribute::PriceCurrency:
      columnName = ListingColumn::PriceCurrency;
      break;
    case Listing::Attribute::FxBaseCurrency:
      columnName = ListingColumn::FxBaseCurrency;
      break;
    case Listing::Attribute::InstrSymbol:
      columnName = ListingColumn::InstrSymbol;
      break;
    case Listing::Attribute::QtyMinimum:
      columnName = ListingColumn::QtyMinimum;
      break;
    case Listing::Attribute::QtyMaximum:
      columnName = ListingColumn::QtyMaximum;
      break;
    case Listing::Attribute::QtyMultiple:
      columnName = ListingColumn::QtyMultiple;
      break;
    case Listing::Attribute::PriceTickSize:
      columnName = ListingColumn::PriceTickSize;
      break;
    case Listing::Attribute::Enabled:
      columnName = ListingColumn::Enabled;
      break;
    case Listing::Attribute::RandomQtyMaximum:
      columnName = ListingColumn::RandomQtyMaximum;
      break;
    case Listing::Attribute::RandomDepthLevels:
      columnName = ListingColumn::RandomDepthLevels;
      break;
    case Listing::Attribute::RandomOrdersSpread:
      columnName = ListingColumn::RandomOrdersSpread;
      break;
    case Listing::Attribute::RandomOrdersRate:
      columnName = ListingColumn::RandomOrdersRate;
      break;
    case Listing::Attribute::RandomTickRange:
      columnName = ListingColumn::RandomTickRange;
      break;
    case Listing::Attribute::SecurityExchange:
      columnName = ListingColumn::SecurityExchange;
      break;
    case Listing::Attribute::PartyId:
      columnName = ListingColumn::PartyId;
      break;
    case Listing::Attribute::PartyRole:
      columnName = ListingColumn::PartyRole;
      break;
    case Listing::Attribute::CusipId:
      columnName = ListingColumn::CusipId;
      break;
    case Listing::Attribute::SedolId:
      columnName = ListingColumn::SedolId;
      break;
    case Listing::Attribute::IsinId:
      columnName = ListingColumn::IsinId;
      break;
    case Listing::Attribute::RicId:
      columnName = ListingColumn::RicId;
      break;
    case Listing::Attribute::ExchangeSymbolId:
      columnName = ListingColumn::ExchangeSymbolId;
      break;
    case Listing::Attribute::BloombergSymbolId:
      columnName = ListingColumn::BloombergSymbolId;
      break;
    case Listing::Attribute::RandomQtyMinimum:
      columnName = ListingColumn::RandomQtyMinimum;
      break;
    case Listing::Attribute::RandomAmtMinimum:
      columnName = ListingColumn::RandomAmtMinimum;
      break;
    case Listing::Attribute::RandomAmtMaximum:
      columnName = ListingColumn::RandomAmtMaximum;
      break;
    case Listing::Attribute::RandomOrdersEnabled:
      columnName = ListingColumn::RandomOrdersEnabled;
      break;
    case Listing::Attribute::RandomAggressiveQtyMinimum:
      columnName = ListingColumn::RandomAggressiveQtyMinimum;
      break;
    case Listing::Attribute::RandomAggressiveQtyMaximum:
      columnName = ListingColumn::RandomAggressiveQtyMaximum;
      break;
    case Listing::Attribute::RandomAggressiveAmtMinimum:
      columnName = ListingColumn::RandomAggressiveAmtMinimum;
      break;
    case Listing::Attribute::RandomAggressiveAmtMaximum:
      columnName = ListingColumn::RandomAggressiveAmtMaximum;
      break;
  }

  if (!columnName.empty()) {
    return std::string{columnName};
  }
  throw ColumnNameEncodingError(Table::Listing, _attribute);
}

auto ColumnResolver::toColumnName(MarketPhase::Attribute _attribute)
    -> std::string {
  std::string_view columnName{};
  switch (_attribute) {
    case MarketPhase::Attribute::Phase:
      columnName = MarketPhaseColumn::Phase;
      break;
    case MarketPhase::Attribute::StartTime:
      columnName = MarketPhaseColumn::StartTime;
      break;
    case MarketPhase::Attribute::EndTime:
      columnName = MarketPhaseColumn::EndTime;
      break;
    case MarketPhase::Attribute::VenueId:
      columnName = MarketPhaseColumn::VenueId;
      break;
    case MarketPhase::Attribute::EndTimeRange:
      columnName = MarketPhaseColumn::EndTimeRange;
      break;
    case MarketPhase::Attribute::AllowCancels:
      columnName = MarketPhaseColumn::AllowCancels;
  }

  if (!columnName.empty()) {
    return std::string{columnName};
  }
  throw ColumnNameEncodingError(Table::MarketPhase, _attribute);
}

auto ColumnResolver::toColumnName(PriceSeed::Attribute _attribute)
    -> std::string {
  std::string_view columnName{};
  switch (_attribute) {
    case PriceSeed::Attribute::PriceSeedId:
      columnName = PriceSeedColumn::PriceSeedId;
      break;
    case PriceSeed::Attribute::Symbol:
      columnName = PriceSeedColumn::Symbol;
      break;
    case PriceSeed::Attribute::SecurityType:
      columnName = PriceSeedColumn::SecurityType;
      break;
    case PriceSeed::Attribute::PriceCurrency:
      columnName = PriceSeedColumn::PriceCurrency;
      break;
    case PriceSeed::Attribute::SecurityId:
      columnName = PriceSeedColumn::SecurityId;
      break;
    case PriceSeed::Attribute::SecurityIdSource:
      columnName = PriceSeedColumn::SecurityIdSource;
      break;
    case PriceSeed::Attribute::InstrumentSymbol:
      columnName = PriceSeedColumn::InstrumentSymbol;
      break;
    case PriceSeed::Attribute::MidPrice:
      columnName = PriceSeedColumn::MidPrice;
      break;
    case PriceSeed::Attribute::BidPrice:
      columnName = PriceSeedColumn::BidPrice;
      break;
    case PriceSeed::Attribute::OfferPrice:
      columnName = PriceSeedColumn::OfferPrice;
      break;
    case PriceSeed::Attribute::LastUpdate:
      columnName = PriceSeedColumn::LastUpdate;
      break;
  }

  if (!columnName.empty()) {
    return std::string{columnName};
  }
  throw ColumnNameEncodingError(Table::PriceSeed, _attribute);
}

auto ColumnResolver::toColumnName(Setting::Attribute _attribute)
    -> std::string {
  std::string_view columnName{};
  switch (_attribute) {
    case Setting::Attribute::Key:
      columnName = SettingColumn::Key;
      break;
    case Setting::Attribute::Value:
      columnName = SettingColumn::Value;
      break;
  }

  if (!columnName.empty()) {
    return std::string{columnName};
  }
  throw ColumnNameEncodingError(Table::Setting, _attribute);
}

auto ColumnResolver::toColumnName(Venue::Attribute _attribute) -> std::string {
  std::string_view columnName{};
  switch (_attribute) {
    case Venue::Attribute::VenueId:
      columnName = VenueColumn::VenueId;
      break;
    case Venue::Attribute::Name:
      columnName = VenueColumn::Name;
      break;
    case Venue::Attribute::EngineType:
      columnName = VenueColumn::EngineType;
      break;
    case Venue::Attribute::SupportTifIoc:
      columnName = VenueColumn::SupportTifIoc;
      break;
    case Venue::Attribute::SupportTifFok:
      columnName = VenueColumn::SupportTifFok;
      break;
    case Venue::Attribute::SupportTifDay:
      columnName = VenueColumn::SupportTifDay;
      break;
    case Venue::Attribute::IncludeOwnOrders:
      columnName = VenueColumn::IncludeOwnOrders;
      break;
    case Venue::Attribute::RestPort:
      columnName = VenueColumn::RestPort;
      break;
    case Venue::Attribute::OrderOnStartup:
      columnName = VenueColumn::OrderOnStartup;
      break;
    case Venue::Attribute::RandomPartiesCount:
      columnName = VenueColumn::RandomPartyCount;
      break;
    case Venue::Attribute::TnsEnabled:
      columnName = VenueColumn::TnsEnabled;
      break;
    case Venue::Attribute::TnsQtyEnabled:
      columnName = VenueColumn::TnsQtyEnabled;
      break;
    case Venue::Attribute::TnsSideEnabled:
      columnName = VenueColumn::TnsSideEnabled;
      break;
    case Venue::Attribute::TnsPartiesEnabled:
      columnName = VenueColumn::TnsPartiesEnabled;
      break;
    case Venue::Attribute::Timezone:
      columnName = VenueColumn::Timezone;
      break;
    case Venue::Attribute::CancelOnDisconnect:
      columnName = VenueColumn::CancelOnDisconnect;
      break;
    case Venue::Attribute::PersistenceEnabled:
      columnName = VenueColumn::PersistenceEnabled;
      break;
    case Venue::Attribute::PersistenceFilePath:
      columnName = VenueColumn::PersistenceFilePath;
      break;
  }

  if (!columnName.empty()) {
    return std::string{columnName};
  }
  throw ColumnNameEncodingError(Table::Venue, _attribute);
}

}  // namespace Simulator::DataLayer::Pqxx
