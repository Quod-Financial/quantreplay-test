#ifndef SIMULATOR_DATA_LAYER_IH_PQXX_DATABASE_CONNECTION_HPP_
#define SIMULATOR_DATA_LAYER_IH_PQXX_DATABASE_CONNECTION_HPP_

#include <functional>
#include <string>

#include <pqxx/connection>

#include "ih/pqxx/context.hpp"

namespace Simulator::DataLayer::Pqxx {

class Connector {
  public:
    using ConnectingStrategy =
        std::function<pqxx::connection(std::string const&)>;


    static auto connect(Pqxx::Context const& _context) -> pqxx::connection;

    static void ping(Pqxx::Context const& _context);


    Connector() = delete;

    explicit Connector(ConnectingStrategy _connectingStrategy);

    [[nodiscard]]
    auto connectUnder(Pqxx::Context const& _context) const -> pqxx::connection;

  private:
    [[nodiscard]]
    auto connectByConnString(std::string const& _connString) const
        -> pqxx::connection;

    ConnectingStrategy mConnectingStrategy;
};

} // namespace Simulator::DataLayer::Pqxx

#endif // SIMULATOR_DATA_LAYER_IH_PQXX_DATABASE_CONNECTION_HPP_
