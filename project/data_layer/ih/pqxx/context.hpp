#ifndef SIMULATOR_PROJECT_DATA_LAYER_IH_PQXX_CONTEXT_HPP_
#define SIMULATOR_PROJECT_DATA_LAYER_IH_PQXX_CONTEXT_HPP_

#include <string>

#include "cfg/api/cfg.hpp"

namespace Simulator::DataLayer::Pqxx {

class Context {
  public:
    class Configurator {
      public:
        void withHost(std::string _host) noexcept;

        void withPort(std::string _port) noexcept;

        void withUser(std::string _user) noexcept;

        void withDatabase(std::string _databaseName) noexcept;

        void withPassword(std::string _password) noexcept;

        [[nodiscard]]
        auto formatConnectionString() const -> std::string;

      private:
        std::string mHost;
        std::string mPort;
        std::string mUser;
        std::string mDatabase;
        std::string mPassword;

        static std::string_view const msConnectionStringFormat;
    };

    Context() = default;

    explicit Context(std::string _connectionString) noexcept;

    [[nodiscard]]
    auto getConnectionString() const noexcept -> std::string const&;

    // May throw ConnectionPropertyMissing exception in case passed _cfg
    // does not contain a required connection property
    void configure(Cfg::DbConfiguration const& _cfg);


  private:
    std::string mConnection;
};

} // namespace Simulator::DataLayer::Pqxx

#endif // SIMULATOR_PROJECT_DATA_LAYER_IH_PQXX_CONTEXT_HPP_
