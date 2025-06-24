#ifndef SIMULATOR_HTTP_IH_MARSHALLING_JSON_DETAIL_ENUMERATION_RESOLVER_HPP_
#define SIMULATOR_HTTP_IH_MARSHALLING_JSON_DETAIL_ENUMERATION_RESOLVER_HPP_

#include "data_layer/api/models/datasource.hpp"
#include "data_layer/api/models/venue.hpp"

namespace Simulator::Http::Json {

class EnumerationResolver {
 public:
  [[nodiscard]]
  static auto resolve(DataLayer::Datasource::Format value) -> std::string_view;

  static auto resolve(const std::string& value,
                      DataLayer::Datasource::Format& result) -> void;

  [[nodiscard]]
  static auto resolve(DataLayer::Datasource::Type value) -> std::string_view;

  static auto resolve(const std::string& _value,
                      DataLayer::Datasource::Type& _result) -> void;

  [[nodiscard]]
  static auto resolve(DataLayer::MarketPhase::Phase value) -> std::string_view;

  static auto resolve(const std::string& value,
                      DataLayer::MarketPhase::Phase& result) -> void;

  [[nodiscard]]
  static auto resolve(DataLayer::Venue::EngineType value) -> std::string_view;

  static auto resolve(const std::string& value,
                      DataLayer::Venue::EngineType& result) -> void;
};

}  // namespace Simulator::Http::Json

#endif  // SIMULATOR_HTTP_IH_MARSHALLING_JSON_DETAIL_ENUMERATION_RESOLVER_HPP_
