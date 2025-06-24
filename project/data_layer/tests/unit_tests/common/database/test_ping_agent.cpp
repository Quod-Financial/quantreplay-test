#include <functional>

#include <gtest/gtest.h>

#include "api/exceptions/exceptions.hpp"
#include "ih/common/database/driver.hpp"
#include "ih/common/database/ping_agent.hpp"
#include "ih/pqxx/context.hpp"

using namespace Simulator::DataLayer;

class DataLayer_Database_PingAgent : public ::testing::Test {
  public:
    template<typename Callable>
    static auto makePqxxAgent(Callable _callable)
        -> Database::PingAgent::PqxxAgentStrategy
    {
        return [_callable]([[maybe_unused]] Pqxx::Context const& _ctx) {
            _callable();
        };
    }

    static auto makeNoopPqxxAgent() -> Database::PingAgent::PqxxAgentStrategy
    {
        return makePqxxAgent([]{});
    }
};

TEST_F(DataLayer_Database_PingAgent, Ping_Pqxx_ConnectionErrorOccurs)
{
    auto const context = Database::Driver::makeDatabaseContext(Pqxx::Context{});
    auto pqxxAgent =
        makePqxxAgent([] { throw ConnectionFailure{"test failure"}; });

    Database::PingAgent agent{pqxxAgent};
    EXPECT_FALSE(agent.pingWithContext(context));
}

TEST_F(DataLayer_Database_PingAgent, Ping_Pqxx_GenericExceptionThrown)
{
    auto const context = Database::Driver::makeDatabaseContext(Pqxx::Context{});
    auto pqxxAgent =
        makePqxxAgent([] { throw std::logic_error{"test failure"}; });
    Database::PingAgent agent{pqxxAgent};

    EXPECT_FALSE(agent.pingWithContext(context));
}

TEST_F(DataLayer_Database_PingAgent, Ping_Pqxx_NonExecptionValueThrown)
{
    auto const context = Database::Driver::makeDatabaseContext(Pqxx::Context{});
    auto pqxxAgent = makePqxxAgent([] { throw 42; });
    Database::PingAgent agent{pqxxAgent};

    EXPECT_FALSE(agent.pingWithContext(context));
}

TEST_F(DataLayer_Database_PingAgent, Ping_Pqxx_NoExceptionThrown)
{
    auto const context = Database::Driver::makeDatabaseContext(Pqxx::Context{});
    auto pqxxAgent = makeNoopPqxxAgent();
    Database::PingAgent agent{std::move(pqxxAgent)};

    EXPECT_TRUE(agent.pingWithContext(context));
}