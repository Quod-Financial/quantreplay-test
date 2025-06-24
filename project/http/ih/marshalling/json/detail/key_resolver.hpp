#ifndef SIMULATOR_HTTP_IH_MARSHALLING_JSON_DETAIL_KEY_RESOLVER_HPP_
#define SIMULATOR_HTTP_IH_MARSHALLING_JSON_DETAIL_KEY_RESOLVER_HPP_

#include "data_layer/api/models/column_mapping.hpp"
#include "data_layer/api/models/datasource.hpp"
#include "data_layer/api/models/listing.hpp"
#include "data_layer/api/models/market_phase.hpp"
#include "data_layer/api/models/price_seed.hpp"
#include "data_layer/api/models/setting.hpp"
#include "data_layer/api/models/venue.hpp"

namespace Simulator::Http::Json {

class KeyResolver {
 public:
  [[nodiscard]]
  static auto resolve_key(DataLayer::ColumnMapping::Attribute attribute)
      -> std::string_view;

  [[nodiscard]]
  static auto resolve_key(DataLayer::Datasource::Attribute attribute)
      -> std::string_view;

  [[nodiscard]]
  static auto resolve_key(DataLayer::Listing::Attribute attribute)
      -> std::string_view;

  [[nodiscard]]
  static auto resolve_key(DataLayer::MarketPhase::Attribute attribute)
      -> std::string_view;

  [[nodiscard]]
  static auto resolve_key(DataLayer::PriceSeed::Attribute attribute)
      -> std::string_view;

  [[nodiscard]]
  static auto resolve_key(DataLayer::Setting::Attribute attribute)
      -> std::string_view;

  [[nodiscard]]
  static auto resolve_key(DataLayer::Venue::Attribute attribute)
      -> std::string_view;
};

}  // namespace Simulator::Http::Json

#endif  // SIMULATOR_HTTP_IH_MARSHALLING_JSON_DETAIL_KEY_RESOLVER_HPP_
