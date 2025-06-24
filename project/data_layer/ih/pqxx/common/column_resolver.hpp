#ifndef SIMULATOR_DATA_LAYER_IH_PQXX_QUERIES_COLUMN_RESOLVER_HPP_
#define SIMULATOR_DATA_LAYER_IH_PQXX_QUERIES_COLUMN_RESOLVER_HPP_

#include "api/models/column_mapping.hpp"
#include "api/models/datasource.hpp"
#include "api/models/listing.hpp"
#include "api/models/market_phase.hpp"
#include "api/models/price_seed.hpp"
#include "api/models/setting.hpp"
#include "api/models/venue.hpp"

namespace Simulator::DataLayer::Pqxx {

class ColumnResolver {
  public:
    auto operator()(ColumnMapping::Attribute _attribute) const -> std::string;

    auto operator()(Datasource::Attribute _attribute) const -> std::string;

    auto operator()(Listing::Attribute _attribute) const -> std::string;

    auto operator()(MarketPhase::Attribute _attribute) const -> std::string;

    auto operator()(PriceSeed::Attribute _attribute) const -> std::string;

    auto operator()(Setting::Attribute _attribute) const -> std::string;

    auto operator()(Venue::Attribute _attribute) const -> std::string;

  private:
    static auto toColumnName(ColumnMapping::Attribute _attribute)
        -> std::string;

    static auto toColumnName(Datasource::Attribute _attribute) -> std::string;

    static auto toColumnName(Listing::Attribute _attribute) -> std::string;

    static auto toColumnName(MarketPhase::Attribute _attribute) -> std::string;

    static auto toColumnName(PriceSeed::Attribute _attribute) -> std::string;

    static auto toColumnName(Setting::Attribute _attribute) -> std::string;

    static auto toColumnName(Venue::Attribute _attribute) -> std::string;
};

} // namespace Simulator::DataLayer::Pqxx

#endif // SIMULATOR_DATA_LAYER_IH_PQXX_QUERIES_COLUMN_RESOLVER_HPP_
