#include "ih/pqxx/common/enumeration_resolver.hpp"

#include <string>
#include <string_view>

#include "api/models/venue.hpp"
#include "ih/common/exceptions.hpp"
#include "ih/pqxx/common/names/enumerations.hpp"

namespace Simulator::DataLayer::Pqxx {

auto EnumerationResolver::operator()(Datasource::Format format) const
    -> std::string {
  return EnumerationResolver::toString(format);
}

auto EnumerationResolver::operator()(std::string_view format,
                                     Datasource::Format& value) const -> void {
  EnumerationResolver::fromString(format, value);
}

auto EnumerationResolver::operator()(Datasource::Type type) const
    -> std::string {
  return EnumerationResolver::toString(type);
}

auto EnumerationResolver::operator()(std::string_view type,
                                     Datasource::Type& _value) const -> void {
  EnumerationResolver::fromString(type, _value);
}

auto EnumerationResolver::operator()(MarketPhase::Phase phase) const
    -> std::string {
  return toString(phase);
}

auto EnumerationResolver::operator()(std::string_view phase,
                                     MarketPhase::Phase& value) const -> void {
  EnumerationResolver::fromString(phase, value);
}

auto EnumerationResolver::operator()(Venue::EngineType engineType) const
    -> std::string {
  return EnumerationResolver::toString(engineType);
}

auto EnumerationResolver::operator()(std::string_view engineType,
                                     DataLayer::Venue::EngineType& value) const
    -> void {
  EnumerationResolver::fromString(engineType, value);
}

auto EnumerationResolver::toString(Datasource::Format format) -> std::string {
  std::string_view string{};
  switch (format) {
    case Datasource::Format::Csv:
      string = Pqxx::DatasourceFormat::Csv;
      break;
    case Datasource::Format::Postgres:
      string = Pqxx::DatasourceFormat::Postgres;
      break;
  }

  if (!string.empty()) {
    return std::string{string};
  }
  throw EnumEncodingError("Datasource::Format", format);
}

auto EnumerationResolver::fromString(std::string_view format,
                                     Datasource::Format& value) -> void {
  if (format == Pqxx::DatasourceFormat::Csv) {
    value = Datasource::Format::Csv;
  } else if (format == Pqxx::DatasourceFormat::Postgres) {
    value = Datasource::Format::Postgres;
  } else {
    throw EnumDecodingError("Datasource::Format", format);
  }
}

auto EnumerationResolver::toString(Datasource::Type type) -> std::string {
  std::string_view string{};
  switch (type) {
    case Datasource::Type::OrderBook:
      string = Pqxx::DatasourceType::OrderBook;
      break;
  }

  if (!string.empty()) {
    return std::string{string};
  }
  throw EnumEncodingError("Datasource::Type", type);
}

auto EnumerationResolver::fromString(std::string_view type,
                                     Datasource::Type& value) -> void {
  if (type == Pqxx::DatasourceType::OrderBook) {
    value = Datasource::Type::OrderBook;
  } else {
    throw EnumDecodingError("Datasource::Type", type);
  }
}

auto EnumerationResolver::toString(MarketPhase::Phase value) -> std::string {
  std::string_view string{};
  switch (value) {
    case MarketPhase::Phase::Open:
      string = Pqxx::MarketPhaseType::Open;
      break;
    case MarketPhase::Phase::Closed:
      string = Pqxx::MarketPhaseType::Closed;
      break;
    case MarketPhase::Phase::OpeningAuction:
      string = Pqxx::MarketPhaseType::OpeningAuction;
      break;
    case MarketPhase::Phase::ClosingAuction:
      string = Pqxx::MarketPhaseType::ClosingAuction;
      break;
    case MarketPhase::Phase::IntradayAuction:
      string = Pqxx::MarketPhaseType::IntradayAuction;
      break;
    case MarketPhase::Phase::Halted:
      string = Pqxx::MarketPhaseType::Halted;
      break;
  }

  if (!string.empty()) {
    return std::string{string};
  }
  throw EnumEncodingError("MarketPhase::Phase", value);
}

auto EnumerationResolver::fromString(std::string_view string,
                                     DataLayer::MarketPhase::Phase& value)
    -> void {
  if (string == Pqxx::MarketPhaseType::Open) {
    value = MarketPhase::Phase::Open;
  } else if (string == Pqxx::MarketPhaseType::Closed) {
    value = MarketPhase::Phase::Closed;
  } else if (string == MarketPhaseType::OpeningAuction) {
    value = DataLayer::MarketPhase::Phase::OpeningAuction;
  } else if (string == MarketPhaseType::ClosingAuction) {
    value = DataLayer::MarketPhase::Phase::ClosingAuction;
  } else if (string == MarketPhaseType::IntradayAuction) {
    value = DataLayer::MarketPhase::Phase::IntradayAuction;
  } else if (string == MarketPhaseType::Halted) {
    value = DataLayer::MarketPhase::Phase::Halted;
  } else {
    throw EnumDecodingError("MarketPhase::Phase", string);
  }
}

auto EnumerationResolver::toString(Venue::EngineType value) -> std::string {
  std::string_view string{};
  switch (value) {
    case Venue::EngineType::Matching:
      string = Pqxx::VenueEngineType::Matching;
      break;
    case Venue::EngineType::Quoting:
      string = Pqxx::VenueEngineType::Quoting;
      break;
  }

  if (!string.empty()) {
    return std::string{string};
  }
  throw EnumEncodingError("Venue::EngineType", value);
}

auto EnumerationResolver::fromString(std::string_view string,
                                     DataLayer::Venue::EngineType& value)
    -> void {
  if (string == Pqxx::VenueEngineType::Matching) {
    value = Venue::EngineType::Matching;
  } else if (string == Pqxx::VenueEngineType::Quoting) {
    value = Venue::EngineType::Quoting;
  } else {
    throw EnumDecodingError("Venue::EngineType", string);
  }
}

}  // namespace Simulator::DataLayer::Pqxx
