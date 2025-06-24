#ifndef SIMULATOR_DATA_LAYER_IH_PQXX_COMMON_ENUMERATION_RESOLVER_HPP_
#define SIMULATOR_DATA_LAYER_IH_PQXX_COMMON_ENUMERATION_RESOLVER_HPP_

#include <string>
#include <string_view>

#include "api/models/datasource.hpp"
#include "api/models/market_phase.hpp"
#include "api/models/venue.hpp"

namespace Simulator::DataLayer::Pqxx {

class EnumerationResolver {
 public:
  [[nodiscard]]
  auto operator()(Datasource::Format format) const -> std::string;
  auto operator()(std::string_view format, Datasource::Format& value) const
      -> void;

  [[nodiscard]]
  auto operator()(Datasource::Type type) const -> std::string;
  auto operator()(std::string_view type, Datasource::Type& _value) const
      -> void;

  [[nodiscard]]
  auto operator()(MarketPhase::Phase phase) const -> std::string;
  auto operator()(std::string_view phase, MarketPhase::Phase& value) const
      -> void;

  [[nodiscard]]
  auto operator()(Venue::EngineType engineType) const -> std::string;
  auto operator()(std::string_view engineType, Venue::EngineType& value) const
      -> void;

 private:
  [[nodiscard]]
  static auto toString(Datasource::Format format) -> std::string;
  static auto fromString(std::string_view format, Datasource::Format& value)
      -> void;

  [[nodiscard]]
  static auto toString(Datasource::Type type) -> std::string;
  static auto fromString(std::string_view type, Datasource::Type& value)
      -> void;

  [[nodiscard]]
  static auto toString(MarketPhase::Phase value) -> std::string;
  static auto fromString(std::string_view _phase, MarketPhase::Phase& value)
      -> void;

  [[nodiscard]]
  static auto toString(Venue::EngineType value) -> std::string;
  static auto fromString(std::string_view _engineType, Venue::EngineType& value)
      -> void;
};

}  // namespace Simulator::DataLayer::Pqxx

#endif  // SIMULATOR_DATA_LAYER_IH_PQXX_COMMON_ENUMERATION_RESOLVER_HPP_
