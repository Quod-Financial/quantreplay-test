#ifndef SIMULATOR_DATA_LAYER_IH_COMMON_COMMAND_HANDLERS_HPP_
#define SIMULATOR_DATA_LAYER_IH_COMMON_COMMAND_HANDLERS_HPP_

#include <functional>
#include <type_traits>

#include "api/database/context.hpp"
#include "api/models/datasource.hpp"
#include "api/models/listing.hpp"
#include "api/models/price_seed.hpp"
#include "api/models/venue.hpp"
#include "ih/common/database/context_resolver.hpp"
#include "ih/pqxx/context.hpp"
#include "ih/pqxx/dao/datasource_dao.hpp"
#include "ih/pqxx/dao/listing_dao.hpp"
#include "ih/pqxx/dao/price_seed_dao.hpp"
#include "ih/pqxx/dao/setting_dao.hpp"
#include "ih/pqxx/dao/venue_dao.hpp"

namespace Simulator::DataLayer {

template<typename Command>
class DatasourceCommandHandler final : public Database::ContextResolver {
  public:
    using CommandType = Command;
    using ModelType = typename CommandType::ModelType;

    static_assert(
        std::is_same_v<ModelType, DataLayer::Datasource>,
        "Datasource command handler is instantiated with a command, "
        "which is not designed to run with Datasource model"
    );

    static auto handle(CommandType& _cmd, Database::Context const& _context)
        -> void
    {
        // Redirects control to `executeWith` method with a proper context type
        DatasourceCommandHandler{_cmd}.resolve(_context);
    }

  private:
    explicit DatasourceCommandHandler(CommandType& _command) noexcept :
        mCommand(_command)
    {}

    auto executeWith(Pqxx::Context const& _pqxxContext) -> void override
    {
        Pqxx::DatasourceDao::setupWith(_pqxxContext).execute(mCommand);
    }

    std::reference_wrapper<CommandType> mCommand;
};

template<typename Command>
class ListingCommandHandler final : public Database::ContextResolver {
  public:
    using CommandType = Command;
    using ModelType = typename CommandType::ModelType;

    static_assert(
        std::is_same_v<ModelType, DataLayer::Listing>,
        "Listing command handler is instantiated with a command, "
        "which is not designed to run with Listing model"
    );

    static auto handle(CommandType& _cmd, Database::Context const& _context)
        -> void
    {
        // Redirects control to `executeWith` method with a proper context type
        ListingCommandHandler{_cmd}.resolve(_context);
    }

  private:
    explicit ListingCommandHandler(CommandType& _command) noexcept :
        mCommand(_command)
    {}

    auto executeWith(Pqxx::Context const& _pqxxContext) -> void override
    {
        Pqxx::ListingDao::setupWith(_pqxxContext).execute(mCommand);
    }

    std::reference_wrapper<CommandType> mCommand;
};

template<typename Command>
class PriceSeedCommandHandler final : public Database::ContextResolver {
  public:
    using CommandType = Command;
    using ModelType = typename CommandType::ModelType;

    static_assert(
        std::is_same_v<ModelType, DataLayer::PriceSeed>,
        "Price seed command handler is instantiated with a command, "
        "which is not designed to run with PriceSeed model"
    );

    static auto handle(CommandType& _cmd, Database::Context const& _context)
        -> void
    {
        // Redirects control to `executeWith` method with a proper context type
        PriceSeedCommandHandler{_cmd}.resolve(_context);
    }

  private:
    explicit PriceSeedCommandHandler(CommandType& _command) noexcept :
        mCommand(_command)
    {}

    auto executeWith(Pqxx::Context const& _pqxxContext) -> void override
    {
        Pqxx::PriceSeedDao::setupWith(_pqxxContext).execute(mCommand);
    }

    std::reference_wrapper<CommandType> mCommand;
};

template<typename Command>
class SettingCommandHandler final : public Database::ContextResolver {
  public:
    using CommandType = Command;
    using ModelType = typename CommandType::ModelType;

    static_assert(
        std::is_same_v<ModelType, DataLayer::Setting>,
        "Setting command handler is instantiated with a command, "
        "which is not designed to run with Setting model"
    );

    static auto handle(CommandType& _cmd, Database::Context const& _context)
        -> void
    {
        // Redirects control to `executeWith` method with a proper context type
        SettingCommandHandler{_cmd}.resolve(_context);
    }

  private:
    explicit SettingCommandHandler(CommandType& _command) noexcept :
        mCommand(_command)
    {}

    auto executeWith(Pqxx::Context const& _pqxxContext) -> void override
    {
        Pqxx::SettingDao::setupWith(_pqxxContext).execute(mCommand);
    }

    std::reference_wrapper<CommandType> mCommand;
};

template<typename Command>
class VenueCommandHandler final : public Database::ContextResolver {
  public:
    using CommandType = Command;
    using ModelType = typename CommandType::ModelType;

    static_assert(
        std::is_same_v<ModelType, DataLayer::Venue>,
        "Venue command handler is instantiated with a command, "
        "which is not designed to run with Venue model"
    );

    static auto handle(CommandType& _cmd, Database::Context const& _context)
        -> void
    {
        // Redirects control to `executeWith` method with a proper context type
        VenueCommandHandler{_cmd}.resolve(_context);
    }

  private:
    explicit VenueCommandHandler(CommandType& _command) noexcept :
        mCommand(_command)
    {}

    auto executeWith(Pqxx::Context const& _pqxxContext) -> void override
    {
        Pqxx::VenueDao::setupWith(_pqxxContext).execute(mCommand);
    }

    std::reference_wrapper<CommandType> mCommand;
};

} // namespace Simulator::DataLayer

#endif // SIMULATOR_DATA_LAYER_IH_COMMON_COMMAND_HANDLERS_HPP_
