#include "ih/pqxx/database/connector.hpp"

#include <fmt/format.h>

#include <pqxx/connection>
#include <pqxx/except>

#include "api/exceptions/exceptions.hpp"
#include "ih/formatters/pqxx/context.hpp"
#include "ih/pqxx/context.hpp"
#include "log/logging.hpp"

namespace Simulator::DataLayer::Pqxx {

auto Connector::connect(Pqxx::Context const& _context) -> pqxx::connection
{
    static auto const connectionStrategy = [](std::string const& _connUrl) {
        return pqxx::connection{_connUrl};
    };

    return Connector{connectionStrategy}.connectUnder(_context);
}

void Connector::ping(Pqxx::Context const& _context)
{
    Connector::connect(_context);
}

Connector::Connector(Connector::ConnectingStrategy _connectingStrategy) :
    mConnectingStrategy{std::move(_connectingStrategy)}
{
    if (!mConnectingStrategy) {
        throw DataLayer::ConnectionFailure{
            "bad connection strategy has been provided to Pqxx::Connector"};
    }
}

auto Connector::connectUnder(Pqxx::Context const& _context) const
    -> pqxx::connection
{
    if (_context.getConnectionString().empty()) {
        throw DataLayer::ConnectionFailure{
            "no connection string in pqxx context, has it been configured?"};
    }

    pqxx::connection connection =
        connectByConnString(_context.getConnectionString());
    simulator::log::debug(
      "connected to database using pqxx driver with {}", _context);

    return connection;
}

auto Connector::connectByConnString(std::string const& _connString) const
    -> pqxx::connection
{
    assert(mConnectingStrategy); // Must be validated on construction stage
    assert(!_connString.empty());

    try {
        return mConnectingStrategy(_connString);
    } catch (pqxx::broken_connection const& _brokenConnection) {
        throw DataLayer::ConnectionFailure{fmt::format(
            "pqxx driver reported about broken connection - \"{}\"",
            _brokenConnection.what()
        )};
    } catch (pqxx::sql_error const& _sqlError) {
        throw DataLayer::ConnectionFailure{fmt::format(
            "pqxx driver reported about an sql error - \"{}\"",
            _sqlError.what()
        )};
    } catch (pqxx::usage_error const& _usageError) {
        throw DataLayer::ConnectionFailure{fmt::format(
            "pqxx driver reported about a usage error - \"{}\"",
            _usageError.what()
        )};
    } catch (std::exception const& _ex) {
        throw DataLayer::ConnectionFailure{_ex.what()};
    } catch (...) {
        throw DataLayer::ConnectionFailure{"unknown connection error occurred"};
    }
}

} // namespace Simulator::DataLayer::Pqxx
