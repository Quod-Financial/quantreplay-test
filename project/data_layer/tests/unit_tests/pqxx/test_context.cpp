#include <string>

#include <gtest/gtest.h>

#include "cfg/api/cfg.hpp"

#include "ih/common/exceptions.hpp"
#include "ih/pqxx/context.hpp"

using namespace Simulator;
using namespace Simulator::DataLayer;

TEST(DataLayer_Pqxx_Context_Configurator, HostAbsent)
{
    std::string const port = "5432";
    std::string const database = "database";
    std::string const user = "user";
    std::string const password = "password";

    Pqxx::Context::Configurator configurator{};
    configurator.withPort(port);
    configurator.withUser(user);
    configurator.withDatabase(database);
    configurator.withPassword(password);

    EXPECT_THROW(
        auto cstr = configurator.formatConnectionString(),
        ConnectionPropertyMissing
    );
}

TEST(DataLayer_Pqxx_Context_Configurator, PortAbsent)
{
    std::string const host = "host";
    std::string const database = "database";
    std::string const user = "user";
    std::string const password = "password";

    Pqxx::Context::Configurator configurator{};
    configurator.withHost(host);
    configurator.withUser(user);
    configurator.withDatabase(database);
    configurator.withPassword(password);

    EXPECT_THROW(
        auto cstr = configurator.formatConnectionString(),
        ConnectionPropertyMissing
    );
}

TEST(DataLayer_Pqxx_Context_Configurator, UserAbsent)
{
    std::string const host = "host";
    std::string const port = "5432";
    std::string const database = "database";
    std::string const password = "password";

    Pqxx::Context::Configurator configurator{};
    configurator.withHost(host);
    configurator.withPort(port);
    configurator.withDatabase(database);
    configurator.withPassword(password);

    EXPECT_THROW(
        auto cstr = configurator.formatConnectionString(),
        ConnectionPropertyMissing
    );
}

TEST(DataLayer_Pqxx_Context_Configurator, DatabaseAbsent)
{
    std::string const host = "host";
    std::string const port = "5432";
    std::string const user = "user";
    std::string const password = "password";

    Pqxx::Context::Configurator configurator{};
    configurator.withHost(host);
    configurator.withPort(port);
    configurator.withUser(user);
    configurator.withPassword(password);

    EXPECT_THROW(
        auto cstr = configurator.formatConnectionString(),
        ConnectionPropertyMissing
    );
}

TEST(DataLayer_Pqxx_Context_Configurator, PasswordAbsent)
{
    std::string const host = "host";
    std::string const port = "5432";
    std::string const database = "database";
    std::string const user = "user";

    Pqxx::Context::Configurator configurator{};
    configurator.withHost(host);
    configurator.withPort(port);
    configurator.withUser(user);
    configurator.withDatabase(database);

    EXPECT_THROW(
        auto cstr = configurator.formatConnectionString(),
        ConnectionPropertyMissing
    );
}

TEST(DataLayer_Pqxx_Context_Configurator, FormatConnectionString)
{
    std::string const host = "host";
    std::string const port = "5432";
    std::string const database = "database";
    std::string const user = "user";
    std::string const password = "password";

    Pqxx::Context::Configurator configurator{};
    configurator.withHost(host);
    configurator.withPort(port);
    configurator.withUser(user);
    configurator.withDatabase(database);
    configurator.withPassword(password);

    std::string const expected{"postgresql://user:password@host:5432/database"};

    EXPECT_EQ(configurator.formatConnectionString(), expected);
}

TEST(DataLayer_Pqxx_Context, GetConnectionString_NonConfigured)
{
    Pqxx::Context const context{};
    EXPECT_TRUE(context.getConnectionString().empty());
}

TEST(DataLayer_Pqxx_Context, GetConnectionString_CreatedWithConnectionString)
{
    Pqxx::Context const context{"my_connection_string"};
    EXPECT_EQ(context.getConnectionString(), "my_connection_string");
}

TEST(DataLayer_Pqxx_Context, GetConnectionString_ImproperlyConfigured)
{
    Cfg::DbConfiguration const config{};
    Pqxx::Context context{};
    ASSERT_ANY_THROW(context.configure(config));

    EXPECT_TRUE(context.getConnectionString().empty());
}

TEST(DataLayer_Pqxx_Context, GetConnectionString_ProperlyConfigured)
{
    Cfg::DbConfiguration config{};
    config.host = "host";
    config.port = "5432";
    config.user = "user";
    config.name = "database";
    config.password = "password";
    std::string const expected{"postgresql://user:password@host:5432/database"};

    Pqxx::Context context{};
    ASSERT_NO_THROW(context.configure(config));

    EXPECT_EQ(context.getConnectionString(), expected);
}
