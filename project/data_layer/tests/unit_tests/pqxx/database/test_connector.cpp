#include <functional>
#include <stdexcept>
#include <string>

#include <gtest/gtest.h>
#include <pqxx/connection>
#include <pqxx/except>

#include "cfg/api/cfg.hpp"

#include "api/exceptions/exceptions.hpp"
#include "ih/pqxx/context.hpp"
#include "ih/pqxx/database/connector.hpp"

using namespace Simulator;
using namespace Simulator::DataLayer;

class DataLayer_Pqxx_Connector : public ::testing::Test {
  public:
    static auto makeConnector(Pqxx::Connector::ConnectingStrategy _connStrategy)
        -> Pqxx::Connector
    {
        return Pqxx::Connector{std::move(_connStrategy)};
    }

    static auto makeDefaultConfiguration() -> Cfg::DbConfiguration
    {
        Cfg::DbConfiguration config{};
        config.host = "host";
        config.port = "5432";
        config.user = "user";
        config.name = "database";
        config.password = "password";
        return config;
    }

    static auto getExpectedConnectionString() -> std::string
    {
        return "postgresql://user:password@host:5432/database";
    }
};

TEST_F(DataLayer_Pqxx_Connector, BadStrategyProvided)
{
    EXPECT_THROW(
        Pqxx::Connector{Pqxx::Connector::ConnectingStrategy{}},
        ConnectionFailure
    );
}

TEST_F(DataLayer_Pqxx_Connector, ValidStrategyProvided)
{
    EXPECT_NO_THROW(
        Pqxx::Connector{[]([[maybe_unused]] auto const& _connUrl) {
            return pqxx::connection{};
        }}
    );
}

TEST_F(DataLayer_Pqxx_Connector, Connect_WithNonConfiguredContext)
{
    Pqxx::Context const context{};
    ASSERT_TRUE(context.getConnectionString().empty());

    Pqxx::Connector const connector = makeConnector(
        []([[maybe_unused]] auto const& _connUrl) -> pqxx::connection {
            return pqxx::connection{};
        }
    );

    EXPECT_THROW(auto cnn = connector.connectUnder(context), ConnectionFailure);
}

TEST_F(DataLayer_Pqxx_Connector, Connect_WithConfiguredContext)
{
    Pqxx::Context context{};
    ASSERT_NO_THROW(context.configure(makeDefaultConfiguration()));
    ASSERT_FALSE(context.getConnectionString().empty());

    Pqxx::Connector const connector =
        makeConnector([](std::string const& _connUrl) -> pqxx::connection {
            EXPECT_EQ(_connUrl, getExpectedConnectionString());
            throw std::logic_error("test error");
        });

    EXPECT_THROW(auto cnn = connector.connectUnder(context), ConnectionFailure);
}

TEST_F(DataLayer_Pqxx_Connector, Connect_Failure_PqxxBrokenConnection)
{
    Pqxx::Context context{};
    ASSERT_NO_THROW(context.configure(makeDefaultConfiguration()));
    ASSERT_FALSE(context.getConnectionString().empty());

    Pqxx::Connector const connector = makeConnector(
        []([[maybe_unused]] auto const& _connUrl) -> pqxx::connection {
            throw pqxx::broken_connection("test error message");
        }
    );

    EXPECT_THROW(auto cnn = connector.connectUnder(context), ConnectionFailure);
}

TEST_F(DataLayer_Pqxx_Connector, Connect_Failure_PqxxSqlError)
{
    Pqxx::Context context{};
    ASSERT_NO_THROW(context.configure(makeDefaultConfiguration()));
    ASSERT_FALSE(context.getConnectionString().empty());

    Pqxx::Connector const connector = makeConnector(
        []([[maybe_unused]] auto const& _connUrl) -> pqxx::connection {
            throw pqxx::sql_error("test error message");
        }
    );

    EXPECT_THROW(auto cnn = connector.connectUnder(context), ConnectionFailure);
}

TEST_F(DataLayer_Pqxx_Connector, Connect_Failure_PqxxUsageError)
{
    Pqxx::Context context{};
    ASSERT_NO_THROW(context.configure(makeDefaultConfiguration()));
    ASSERT_FALSE(context.getConnectionString().empty());

    Pqxx::Connector const connector = makeConnector(
        []([[maybe_unused]] auto const& _connUrl) -> pqxx::connection {
            throw pqxx::usage_error("test error message");
        }
    );

    EXPECT_THROW(auto cnn = connector.connectUnder(context), ConnectionFailure);
}

TEST_F(DataLayer_Pqxx_Connector, Connect_Failure_GenericException)
{
    Pqxx::Context context{};
    ASSERT_NO_THROW(context.configure(makeDefaultConfiguration()));
    ASSERT_FALSE(context.getConnectionString().empty());

    Pqxx::Connector const connector = makeConnector(
        []([[maybe_unused]] auto const& _connUrl) -> pqxx::connection {
            throw std::logic_error("test error message");
        }
    );

    EXPECT_THROW(auto cnn = connector.connectUnder(context), ConnectionFailure);
}

TEST_F(DataLayer_Pqxx_Connector, Connect_Failure_NonExceptionValueThrown)
{
    Pqxx::Context context{};
    ASSERT_NO_THROW(context.configure(makeDefaultConfiguration()));
    ASSERT_FALSE(context.getConnectionString().empty());

    Pqxx::Connector const connector = makeConnector(
        []([[maybe_unused]] auto const& _connUrl) -> pqxx::connection {
            throw 42;
        }
    );

    EXPECT_THROW(auto cnn = connector.connectUnder(context), ConnectionFailure);
}