#include "ih/marshalling/json/detail/enumeration_resolver.hpp"

#include <fmt/format.h>

#include <stdexcept>
#include <string_view>
#include <type_traits>

namespace Simulator::Http::Json {
namespace DatasourceFormat {

constexpr std::string_view Csv{"CSV"};
constexpr std::string_view Postgres{"PSQL"};

}  // namespace DatasourceFormat

namespace DatasourceType {

constexpr std::string_view OrderBook{"OrderBook"};

}  // namespace DatasourceType

namespace MarketPhaseType {

constexpr std::string_view Open{"Open"};
constexpr std::string_view Closed{"Closed"};
constexpr std::string_view OpeningAuction{"PreOpen"};
constexpr std::string_view ClosingAuction{"PreClose"};
constexpr std::string_view IntradayAuction{"Auction"};
constexpr std::string_view Halted{"Halted"};

}  // namespace MarketPhaseType

namespace VenueEngineType {

constexpr std::string_view Matching{"Matching"};
constexpr std::string_view Quoting{"Quoting"};

}  // namespace VenueEngineType

namespace {

template <typename Enumeration>
[[noreturn]]
auto raiseBadEnumerationError(std::string_view _enumName, Enumeration _badValue)
    -> void {
  static_assert(std::is_enum_v<Enumeration>);
  using Underlying = std::underlying_type_t<Enumeration>;

  throw std::logic_error{fmt::format(
      "unable to resolve a JSON value for {} enumeration by an undefined "
      "value `{:#x}'",
      _enumName,
      static_cast<Underlying>(_badValue))};
}

[[noreturn]]
auto raiseBadValueError(std::string_view _enumName, std::string_view _badValue)
    -> void {
  throw std::runtime_error{
      fmt::format("unable to decode a `{}' JSON string as a {} enumeration",
                  _badValue,
                  _enumName)};
}

}  // namespace

auto EnumerationResolver::resolve(DataLayer::Datasource::Format value)
    -> std::string_view {
  std::string_view resolved{};
  switch (value) {
    case DataLayer::Datasource::Format::Csv:
      resolved = DatasourceFormat::Csv;
      break;
    case DataLayer::Datasource::Format::Postgres:
      resolved = DatasourceFormat::Postgres;
      break;
  }

  if (!resolved.empty()) {
    return resolved;
  }
  raiseBadEnumerationError("Datasource::Format", value);
}

auto EnumerationResolver::resolve(const std::string& value,
                                  DataLayer::Datasource::Format& result)
    -> void {
  if (value == DatasourceFormat::Csv) {
    result = DataLayer::Datasource::Format::Csv;
  } else if (value == DatasourceFormat::Postgres) {
    result = DataLayer::Datasource::Format::Postgres;
  } else {
    raiseBadValueError("Datasource::Format", value);
  }
}

auto EnumerationResolver::resolve(DataLayer::Datasource::Type value)
    -> std::string_view {
  std::string_view resolved{};
  switch (value) {
    case DataLayer::Datasource::Type::OrderBook:
      resolved = DatasourceType::OrderBook;
      break;
  }

  if (!resolved.empty()) {
    return resolved;
  }
  raiseBadEnumerationError("Datasource::Type", value);
}

auto EnumerationResolver::resolve(const std::string& _value,
                                  DataLayer::Datasource::Type& _result)
    -> void {
  if (_value == DatasourceType::OrderBook) {
    _result = DataLayer::Datasource::Type::OrderBook;
  } else {
    raiseBadValueError("Datasource::Type", _value);
  }
}

auto EnumerationResolver::resolve(DataLayer::MarketPhase::Phase value)
    -> std::string_view {
  std::string_view resolved{};
  switch (value) {
    case DataLayer::MarketPhase::Phase::Open:
      resolved = MarketPhaseType::Open;
      break;
    case DataLayer::MarketPhase::Phase::Closed:
      resolved = MarketPhaseType::Closed;
      break;
    case DataLayer::MarketPhase::Phase::OpeningAuction:
      resolved = MarketPhaseType::OpeningAuction;
      break;
    case DataLayer::MarketPhase::Phase::ClosingAuction:
      resolved = MarketPhaseType::ClosingAuction;
      break;
    case DataLayer::MarketPhase::Phase::IntradayAuction:
      resolved = MarketPhaseType::IntradayAuction;
      break;
    case DataLayer::MarketPhase::Phase::Halted:
      resolved = MarketPhaseType::Halted;
      break;
  }

  if (!resolved.empty()) {
    return resolved;
  }
  raiseBadEnumerationError("MarketPhase::Phase", value);
}

auto EnumerationResolver::resolve(const std::string& value,
                                  DataLayer::MarketPhase::Phase& result)
    -> void {
  if (value == MarketPhaseType::Open) {
    result = DataLayer::MarketPhase::Phase::Open;
  } else if (value == MarketPhaseType::Closed) {
    result = DataLayer::MarketPhase::Phase::Closed;
  } else if (value == MarketPhaseType::OpeningAuction) {
    result = DataLayer::MarketPhase::Phase::OpeningAuction;
  } else if (value == MarketPhaseType::ClosingAuction) {
    result = DataLayer::MarketPhase::Phase::ClosingAuction;
  } else if (value == MarketPhaseType::IntradayAuction) {
    result = DataLayer::MarketPhase::Phase::IntradayAuction;
  } else if (value == MarketPhaseType::Halted) {
    result = DataLayer::MarketPhase::Phase::Halted;
  } else {
    raiseBadValueError("MarketPhase::Phase", value);
  }
}

auto EnumerationResolver::resolve(DataLayer::Venue::EngineType value)
    -> std::string_view {
  std::string_view resolved{};
  switch (value) {
    case DataLayer::Venue::EngineType::Matching:
      resolved = VenueEngineType::Matching;
      break;
    case DataLayer::Venue::EngineType::Quoting:
      resolved = VenueEngineType::Quoting;
      break;
  }

  if (!resolved.empty()) {
    return resolved;
  }
  raiseBadEnumerationError("Venue::EngineType", value);
}

auto EnumerationResolver::resolve(const std::string& value,
                                  DataLayer::Venue::EngineType& result)
    -> void {
  if (value == VenueEngineType::Matching) {
    result = DataLayer::Venue::EngineType::Matching;
  } else if (value == VenueEngineType::Quoting) {
    result = DataLayer::Venue::EngineType::Quoting;
  } else {
    raiseBadValueError("Venue::EngineType", value);
  }
}

}  // namespace Simulator::Http::Json