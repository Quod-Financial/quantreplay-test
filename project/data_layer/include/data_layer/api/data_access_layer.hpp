#ifndef SIMULATOR_DATA_LAYER_INCLUDE_DATA_LAYER_API_DATA_ACCESS_LAYER_HPP_
#define SIMULATOR_DATA_LAYER_INCLUDE_DATA_LAYER_API_DATA_ACCESS_LAYER_HPP_

#include <vector>

#include "cfg/api/cfg.hpp"

#include "data_layer/api/database/context.hpp"
#include "data_layer/api/exceptions/exceptions.hpp"
#include "data_layer/api/models/datasource.hpp"
#include "data_layer/api/models/listing.hpp"
#include "data_layer/api/models/price_seed.hpp"
#include "data_layer/api/models/setting.hpp"
#include "data_layer/api/models/venue.hpp"
#include "data_layer/api/predicate/predicate.hpp"

namespace Simulator::DataLayer::Database {

auto setup(Cfg::DbConfiguration const& _cfg) -> Database::Context;

auto createPqxxConnection(std::string _connectionString) -> Database::Context;

auto ping(Database::Context const& _context) noexcept -> bool;

} // namespace Simulator::DataLayer::Database

namespace Simulator::DataLayer {

// Data sources

auto insertDatasource(
    Database::Context const& _context,
    Datasource::Patch _initialPatch
) -> DataLayer::Datasource;

auto selectOneDatasource(
    Database::Context const& _context,
    Datasource::Predicate _pred
) -> DataLayer::Datasource;

auto selectAllDatasources(
    Database::Context const& _context,
    std::optional<Datasource::Predicate> _pred = std::nullopt
) -> std::vector<DataLayer::Datasource>;

auto updateOneDatasource(
    Database::Context const& _context,
    Datasource::Patch _update,
    Datasource::Predicate _pred
) -> DataLayer::Datasource;


// Listings

auto insertListing(
    Database::Context const& _context,
    Listing::Patch _initialPatch
) -> DataLayer::Listing;

auto selectOneListing(
    Database::Context const& _context,
    Listing::Predicate _pred
) -> DataLayer::Listing;

auto selectAllListings(
    Database::Context const& _context,
    std::optional<Listing::Predicate> _pred = std::nullopt
) -> std::vector<DataLayer::Listing>;

auto updateOneListing(
    Database::Context const& _context,
    Listing::Patch _update,
    Listing::Predicate _pred
) -> DataLayer::Listing;


// Price Seeds

auto insertPriceSeed(
    Database::Context const& _context,
    PriceSeed::Patch _initialPatch
) -> DataLayer::PriceSeed;

auto selectOnePriceSeed(
    Database::Context const& _context,
    PriceSeed::Predicate _pred
) -> DataLayer::PriceSeed;

auto selectAllPriceSeeds(
    Database::Context const& _context,
    std::optional<PriceSeed::Predicate> _pred = std::nullopt
) -> std::vector<DataLayer::PriceSeed>;

auto updateOnePriceSeed(
    Database::Context const& _context,
    PriceSeed::Patch _update,
    PriceSeed::Predicate _pred
) -> DataLayer::PriceSeed;

auto updateAllPriceSeeds(
    Database::Context const& _context,
    PriceSeed::Patch _update,
    std::optional<PriceSeed::Predicate> _pred = std::nullopt
) -> std::vector<DataLayer::PriceSeed>;

auto deleteOnePriceSeed(
    Database::Context const& _context,
    PriceSeed::Predicate _pred
) -> void;

auto deleteAllPriceSeeds(
    Database::Context const& _context,
    std::optional<PriceSeed::Predicate> _pred = std::nullopt
) -> void;


// Settings

auto insertSetting(
    Database::Context const& _context,
    Setting::Patch _initialPatch
) -> DataLayer::Setting;

auto selectOneSetting(
    Database::Context const& _context,
    Setting::Predicate _pred
) -> DataLayer::Setting;

auto selectAllSettings(
    Database::Context const& _context,
    std::optional<Setting::Predicate> _pred = std::nullopt
) -> std::vector<DataLayer::Setting>;

auto updateOneSetting(
    Database::Context const& _context,
    Setting::Patch _update,
    Setting::Predicate _pred
) -> DataLayer::Setting;


// Venues

auto insertVenue(Database::Context const& _context, Venue::Patch _initialPatch)
    -> DataLayer::Venue;

auto selectOneVenue(Database::Context const& _context, Venue::Predicate _pred)
    -> DataLayer::Venue;

auto selectAllVenues(
    Database::Context const& _context,
    std::optional<Venue::Predicate> _pred = std::nullopt
) -> std::vector<DataLayer::Venue>;

auto updateOneVenue(
    Database::Context const& _context,
    Venue::Patch _update,
    Venue::Predicate _pred
) -> DataLayer::Venue;

// Common utils

[[nodiscard]]
auto select_simulated_venue(const Database::Context& database)
    -> DataLayer::Venue;

} // namespace Simulator::DataLayer

#endif // SIMULATOR_DATA_LAYER_INCLUDE_DATA_LAYER_API_DATA_ACCESS_LAYER_HPP_
