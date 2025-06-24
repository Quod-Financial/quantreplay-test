#include "api/data_access_layer.hpp"

#include <string>
#include <utility>

#include "cfg/api/cfg.hpp"

#include "api/database/context.hpp"
#include "ih/common/command/commands.hpp"
#include "ih/common/command/handlers.hpp"
#include "ih/common/database/driver.hpp"
#include "ih/common/database/ping_agent.hpp"
#include "ih/pqxx/context.hpp"

namespace Simulator::DataLayer::Database {

auto setup(Cfg::DbConfiguration const& _cfg) -> Database::Context
{
    return Driver::configure(_cfg);
}

auto createPqxxConnection(std::string _connectionString) -> Database::Context
{
    Pqxx::Context pqxxContext{std::move(_connectionString)};
    return Driver::makeDatabaseContext(std::move(pqxxContext));
}

auto ping(Database::Context const& _context) noexcept -> bool
{
    return PingAgent::ping(_context);
}

} // namespace Simulator::DataLayer::Database


namespace Simulator::DataLayer {

auto insertDatasource(
    Database::Context const& _context,
    Datasource::Patch _initialPatch
) -> Datasource
{
    using Command = Command::Insert<Datasource>;
    auto command = Command::create(std::move(_initialPatch));
    DatasourceCommandHandler<Command>::handle(command, _context);

    return Datasource{std::move(command.takeResult())};
}

auto selectOneDatasource(
    Database::Context const& _context,
    Datasource::Predicate _pred
) -> Datasource
{
    using Command = Command::SelectOne<Datasource>;
    auto command = Command::create(std::move(_pred));
    DatasourceCommandHandler<Command>::handle(command, _context);

    return Datasource{std::move(command.takeResult())};
}

auto selectAllDatasources(
    Database::Context const& _context,
    std::optional<Datasource::Predicate> _pred
) -> std::vector<Datasource>
{
    using Command = Command::SelectAll<Datasource>;
    auto command = Command::create(std::move(_pred));
    DatasourceCommandHandler<Command>::handle(command, _context);

    return std::vector<Datasource>{std::move(command.takeResult())};
}

auto updateOneDatasource(
    Database::Context const& _context,
    Datasource::Patch _update,
    Datasource::Predicate _pred
) -> Datasource
{
    using Command = Command::UpdateOne<Datasource>;
    auto command = Command::create(std::move(_update), std::move(_pred));
    DatasourceCommandHandler<Command>::handle(command, _context);

    return Datasource{std::move(command.takeResult())};
}

auto insertListing(
    Database::Context const& _context,
    Listing::Patch _initialPatch
) -> Listing
{
    using Command = Command::Insert<Listing>;
    auto command = Command::create(std::move(_initialPatch));
    ListingCommandHandler<Command>::handle(command, _context);

    return Listing{std::move(command.takeResult())};
}

auto selectOneListing(
    Database::Context const& _context,
    Listing::Predicate _pred
) -> Listing
{
    using Command = Command::SelectOne<Listing>;
    auto command = Command::create(std::move(_pred));
    ListingCommandHandler<Command>::handle(command, _context);

    return Listing{std::move(command.takeResult())};
}

auto selectAllListings(
    Database::Context const& _context,
    std::optional<Listing::Predicate> _pred
) -> std::vector<Listing>
{
    using Command = Command::SelectAll<Listing>;
    auto command = Command::create(std::move(_pred));
    ListingCommandHandler<Command>::handle(command, _context);

    return std::vector<Listing>{std::move(command.takeResult())};
}

auto updateOneListing(
    Database::Context const& _context,
    Listing::Patch _update,
    Listing::Predicate _pred
) -> Listing
{
    using Command = Command::UpdateOne<Listing>;
    auto command = Command::create(std::move(_update), std::move(_pred));
    ListingCommandHandler<Command>::handle(command, _context);

    return Listing{std::move(command.takeResult())};
}

auto insertPriceSeed(
    Database::Context const& _context,
    PriceSeed::Patch _initialPatch
) -> PriceSeed
{
    using Command = Command::Insert<PriceSeed>;
    auto command = Command::create(std::move(_initialPatch));
    PriceSeedCommandHandler<Command>::handle(command, _context);

    return PriceSeed{std::move(command.takeResult())};
}

auto selectOnePriceSeed(
    Database::Context const& _context,
    PriceSeed::Predicate _pred
) -> PriceSeed
{
    using Command = Command::SelectOne<PriceSeed>;
    auto command = Command::create(std::move(_pred));
    PriceSeedCommandHandler<Command>::handle(command, _context);

    return PriceSeed{std::move(command.takeResult())};
}

auto selectAllPriceSeeds(
    Database::Context const& _context,
    std::optional<PriceSeed::Predicate> _pred
) -> std::vector<PriceSeed>
{
    using Command = Command::SelectAll<PriceSeed>;
    auto command = Command::create(std::move(_pred));
    PriceSeedCommandHandler<Command>::handle(command, _context);

    return std::vector<PriceSeed>{std::move(command.takeResult())};
}

auto updateOnePriceSeed(
    Database::Context const& _context,
    PriceSeed::Patch _update,
    PriceSeed::Predicate _pred
) -> PriceSeed
{
    using Command = Command::UpdateOne<PriceSeed>;
    auto command = Command::create(std::move(_update), std::move(_pred));
    PriceSeedCommandHandler<Command>::handle(command, _context);

    return PriceSeed{std::move(command.takeResult())};
}

auto updateAllPriceSeeds(
    Database::Context const& _context,
    PriceSeed::Patch _update,
    std::optional<PriceSeed::Predicate> _pred
) -> std::vector<PriceSeed>
{
    using Command = Command::UpdateAll<PriceSeed>;
    auto command = Command::create(std::move(_update), std::move(_pred));
    PriceSeedCommandHandler<Command>::handle(command, _context);

    return std::vector<PriceSeed>{std::move(command.takeResult())};
}

auto deleteOnePriceSeed(
    Database::Context const& _context,
    PriceSeed::Predicate _pred
) -> void
{
    using Command = Command::DeleteOne<PriceSeed>;
    auto command = Command::create(std::move(_pred));
    PriceSeedCommandHandler<Command>::handle(command, _context);
}

auto deleteAllPriceSeeds(
    Database::Context const& _context,
    std::optional<PriceSeed::Predicate> _pred
) -> void
{
    using Command = Command::DeleteAll<PriceSeed>;
    auto command = Command::create(std::move(_pred));
    PriceSeedCommandHandler<Command>::handle(command, _context);
}

auto insertSetting(
    Database::Context const& _context,
    Setting::Patch _initialPatch
) -> DataLayer::Setting
{
    using Command = Command::Insert<Setting>;
    auto command = Command::create(std::move(_initialPatch));
    SettingCommandHandler<Command>::handle(command, _context);

    return Setting{std::move(command.takeResult())};
}

auto selectOneSetting(
    Database::Context const& _context,
    Setting::Predicate _pred
) -> DataLayer::Setting
{
    using Command = Command::SelectOne<Setting>;
    auto command = Command::create(std::move(_pred));
    SettingCommandHandler<Command>::handle(command, _context);

    return Setting{std::move(command.takeResult())};
}

auto selectAllSettings(
    Database::Context const& _context,
    std::optional<Setting::Predicate> _pred
) -> std::vector<DataLayer::Setting>
{
    using Command = Command::SelectAll<Setting>;
    auto command = Command::create(std::move(_pred));
    SettingCommandHandler<Command>::handle(command, _context);

    return std::vector<Setting>{std::move(command.takeResult())};
}

auto updateOneSetting(
    Database::Context const& _context,
    Setting::Patch _update,
    Setting::Predicate _pred
) -> DataLayer::Setting
{
    using Command = Command::UpdateOne<Setting>;
    auto command = Command::create(std::move(_update), std::move(_pred));
    SettingCommandHandler<Command>::handle(command, _context);

    return Setting{std::move(command.takeResult())};
}

auto insertVenue(Database::Context const& _context, Venue::Patch _initialPatch)
    -> Venue
{
    using Command = Command::Insert<Venue>;
    auto command = Command::create(std::move(_initialPatch));
    VenueCommandHandler<Command>::handle(command, _context);

    return Venue{std::move(command.takeResult())};
}

auto selectOneVenue(Database::Context const& _context, Venue::Predicate _pred)
    -> Venue
{
    using Command = Command::SelectOne<Venue>;
    auto command = Command::create(std::move(_pred));
    VenueCommandHandler<Command>::handle(command, _context);

    return Venue{std::move(command.takeResult())};
}

auto selectAllVenues(
    Database::Context const& _context,
    std::optional<Venue::Predicate> _pred
) -> std::vector<Venue>
{
    using Command = Command::SelectAll<Venue>;
    auto command = Command::create(std::move(_pred));
    VenueCommandHandler<Command>::handle(command, _context);

    return std::vector<Venue>{std::move(command.takeResult())};
}

auto updateOneVenue(
    Database::Context const& _context,
    Venue::Patch _update,
    Venue::Predicate _pred
) -> Venue
{
    using Command = Command::UpdateOne<Venue>;
    auto command = Command::create(std::move(_update), std::move(_pred));
    VenueCommandHandler<Command>::handle(command, _context);

    return Venue{std::move(command.takeResult())};
}

auto select_simulated_venue(const Database::Context& database)
    -> DataLayer::Venue {
  return selectOneVenue(
      database, VenueCmp::eq(Venue::Attribute::VenueId, Cfg::venue().name));
}


} // namespace Simulator::DataLayer