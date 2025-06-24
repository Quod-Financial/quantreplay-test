#include "ih/common/database/ping_agent.hpp"

#include "api/exceptions/exceptions.hpp"
#include "ih/pqxx/context.hpp"
#include "ih/pqxx/database/connector.hpp"
#include "log/logging.hpp"

namespace Simulator::DataLayer::Database {

auto PingAgent::ping(Database::Context const& _context) noexcept -> bool
{
    static auto const pqxxAgent = [](Pqxx::Context const& _pqxxContext) {
        Pqxx::Connector::ping(_pqxxContext);
    };

    return PingAgent{pqxxAgent}.pingWithContext(_context);
}

PingAgent::PingAgent(PqxxAgentStrategy _pqxxAgent) noexcept :
    mPqxxAgent(std::move(_pqxxAgent))
{}

auto PingAgent::pingWithContext(Database::Context const& _context) noexcept
    -> bool
{
    try {
        // Resolve the context to call one of `executeWith` methods
        // with concrete DBMS context to enrich the correct strategy
        ContextResolver::resolve(_context);
        simulator::log::debug("ping reached the database");
        return true;
    } catch (DataLayer::ConnectionFailure const& _ex) {
        simulator::log::warn("failed to ping the database - {}", _ex.what());
    } catch (std::exception const& _ex) {
        simulator::log::err(
            "failed to ping the database, error occurred: {}", _ex.what());
    } catch (...) {
        simulator::log::err(
            "failed to ping the database, unknown error occurred");
    }

    return false;
}

void PingAgent::executeWith(Pqxx::Context const& _pqxxContext)
{
    simulator::log::debug("pinging database with the pqxx (postgres) driver");
    mPqxxAgent(_pqxxContext);
}

} // namespace Simulator::DataLayer::Database
