#include <gtest/gtest.h>

#include "api/models/datasource.hpp"
#include "api/models/market_phase.hpp"
#include "api/models/venue.hpp"
#include "ih/common/exceptions.hpp"
#include "ih/pqxx/common/enumeration_resolver.hpp"

using namespace Simulator::DataLayer;
using Resolver = Pqxx::EnumerationResolver;


TEST(DataLayer_Pqxx_EnumerationResolver_DatasourceFromat, String_Undefined)
{
    Resolver const resolver{};
    EXPECT_THROW(
        resolver(static_cast<Datasource::Format>(-1)),
        EnumEncodingError
    );
}

TEST(DataLayer_Pqxx_EnumerationResolver_DatasourceFromat, String_CSV)
{
    Resolver const resolver{};
    EXPECT_EQ(resolver(Datasource::Format::Csv), "CSV");
}

TEST(DataLayer_Pqxx_EnumerationResolver_DatasourceFromat, String_Postgres)
{
    Resolver const resolver{};
    EXPECT_EQ(resolver(Datasource::Format::Postgres), "PSQL");
}

TEST(DataLayer_Pqxx_EnumerationResolver_DatasourceFromat, Value_Undefined)
{
    Resolver const resolver{};
    auto format = static_cast<Datasource::Format>(-1);

    EXPECT_THROW(resolver("bad_string", format), EnumDecodingError);
}

TEST(DataLayer_Pqxx_EnumerationResolver_DatasourceFromat, Value_CSV)
{
    Resolver const resolver{};
    auto format = static_cast<Datasource::Format>(-1);

    EXPECT_NO_THROW(resolver("CSV", format));
    EXPECT_EQ(format, Datasource::Format::Csv);
}

TEST(DataLayer_Pqxx_EnumerationResolver_DatasourceFromat, Value_Postgres)
{
    Resolver const resolver{};
    auto format = static_cast<Datasource::Format>(-1);

    EXPECT_NO_THROW(resolver("PSQL", format));
    EXPECT_EQ(format, Datasource::Format::Postgres);
}


TEST(DataLayer_Pqxx_EnumerationResolver_DatasourceType, String_Undefined)
{
    Resolver const resolver{};
    EXPECT_THROW(
        resolver(static_cast<Datasource::Type>(-1)),
        EnumEncodingError
    );
}

TEST(DataLayer_Pqxx_EnumerationResolver_DatasourceType, String_OrderBook)
{
    Resolver const resolver{};
    EXPECT_EQ(resolver(Datasource::Type::OrderBook), "OrderBook");
}

TEST(DataLayer_Pqxx_EnumerationResolver_DatasourceType, Value_Undefined)
{
    Resolver const resolver{};
    auto type = static_cast<Datasource::Type>(-1);

    EXPECT_THROW(resolver("bad_string", type), EnumDecodingError);
}

TEST(DataLayer_Pqxx_EnumerationResolver_DatasourceType, Value_OrderBook)
{
    Resolver const resolver{};
    auto type = static_cast<Datasource::Type>(-1);

    EXPECT_NO_THROW(resolver("OrderBook", type));
    EXPECT_EQ(type, Datasource::Type::OrderBook);
}


TEST(DataLayer_Pqxx_EnumerationResolver_MarketPhase, String_Undefined)
{
    Resolver const resolver{};
    EXPECT_THROW(
        resolver(static_cast<MarketPhase::Phase>(-1)),
        EnumEncodingError
    );
}

TEST(DataLayer_Pqxx_EnumerationResolver_MarketPhase, String_Open)
{
    Resolver const resolver{};
    EXPECT_EQ(resolver(MarketPhase::Phase::Open), "Open");
}

TEST(DataLayer_Pqxx_EnumerationResolver_MarketPhase, String_Closed)
{
    Resolver const resolver{};
    EXPECT_EQ(resolver(MarketPhase::Phase::Closed), "Closed");
}

TEST(DataLayer_Pqxx_EnumerationResolver_MarketPhase, String_OpeningAuction)
{
    Resolver const resolver{};
    EXPECT_EQ(resolver(MarketPhase::Phase::OpeningAuction), "PreOpen");
}

TEST(DataLayer_Pqxx_EnumerationResolver_MarketPhase, String_ClosingAuction)
{
    Resolver const resolver{};
    EXPECT_EQ(resolver(MarketPhase::Phase::ClosingAuction), "PreClose");
}

TEST(DataLayer_Pqxx_EnumerationResolver_MarketPhase, String_IntradayAuction)
{
    Resolver const resolver{};
    EXPECT_EQ(resolver(MarketPhase::Phase::IntradayAuction), "Auction");
}

TEST(DataLayer_Pqxx_EnumerationResolver_MarketPhase, String_Halted)
{
    constexpr Resolver resolver;
    EXPECT_EQ(resolver(MarketPhase::Phase::Halted), "Halted");
}

TEST(DataLayer_Pqxx_EnumerationResolver_MarketPhase, Value_Undefined)
{
    Resolver const resolver{};
    auto type = static_cast<MarketPhase::Phase>(-1);

    EXPECT_THROW(resolver("bad_string", type), EnumDecodingError);
}

TEST(DataLayer_Pqxx_EnumerationResolver_MarketPhase, Value_Open)
{
    Resolver const resolver{};
    auto type = static_cast<MarketPhase::Phase>(-1);

    EXPECT_NO_THROW(resolver("Open", type));
    EXPECT_EQ(type, MarketPhase::Phase::Open);
}

TEST(DataLayer_Pqxx_EnumerationResolver_MarketPhase, Value_Closed)
{
    Resolver const resolver{};
    auto type = static_cast<MarketPhase::Phase>(-1);

    EXPECT_NO_THROW(resolver("Closed", type));
    EXPECT_EQ(type, MarketPhase::Phase::Closed);
}

TEST(DataLayer_Pqxx_EnumerationResolver_MarketPhase, Value_OpeningAuction)
{
    Resolver const resolver{};
    auto type = static_cast<MarketPhase::Phase>(-1);

    EXPECT_NO_THROW(resolver("PreOpen", type));
    EXPECT_EQ(type, MarketPhase::Phase::OpeningAuction);
}

TEST(DataLayer_Pqxx_EnumerationResolver_MarketPhase, Value_ClosingAuction)
{
    Resolver const resolver{};
    auto type = static_cast<MarketPhase::Phase>(-1);

    EXPECT_NO_THROW(resolver("PreClose", type));
    EXPECT_EQ(type, MarketPhase::Phase::ClosingAuction);
}

TEST(DataLayer_Pqxx_EnumerationResolver_MarketPhase, Value_IntradayAuction)
{
    Resolver const resolver{};
    auto type = static_cast<MarketPhase::Phase>(-1);

    EXPECT_NO_THROW(resolver("Auction", type));
    EXPECT_EQ(type, MarketPhase::Phase::IntradayAuction);
}

TEST(DataLayer_Pqxx_EnumerationResolver_MarketPhase, Value_Halted)
{
    constexpr Resolver resolver;
    auto type = static_cast<MarketPhase::Phase>(-1);

    EXPECT_NO_THROW(resolver("Halted", type));
    EXPECT_EQ(type, MarketPhase::Phase::Halted);
}

TEST(DataLayer_Pqxx_EnumerationResolver_VenueEngineType, String_Undefined)
{
    Resolver const resolver{};
    EXPECT_THROW(
        resolver(static_cast<Venue::EngineType>(-1)),
        EnumEncodingError
    );
}

TEST(DataLayer_Pqxx_EnumerationResolver_VenueEngineType, String_Matching)
{
    Resolver const resolver{};
    EXPECT_EQ(resolver(Venue::EngineType::Matching), "Matching");
}

TEST(DataLayer_Pqxx_EnumerationResolver_VenueEngineType, String_Quoting)
{
    Resolver const resolver{};
    EXPECT_EQ(resolver(Venue::EngineType::Quoting), "Quoting");
}

TEST(DataLayer_Pqxx_EnumerationResolver_VenueEngineType, Value_Undefined)
{
    Resolver const resolver{};
    auto engineType = static_cast<Venue::EngineType>(-1);

    EXPECT_THROW(resolver("bad_string", engineType), EnumDecodingError);
}

TEST(DataLayer_Pqxx_EnumerationResolver_VenueEngineType, Value_Matching)
{
    Resolver const resolver{};
    auto engineType = static_cast<Venue::EngineType>(-1);

    EXPECT_NO_THROW(resolver("Matching", engineType));
    EXPECT_EQ(engineType, Venue::EngineType::Matching);
}

TEST(DataLayer_Pqxx_EnumerationResolver_VenueEngineType, Value_Quoting)
{
    Resolver const resolver{};
    auto engineType = static_cast<Venue::EngineType>(-1);

    EXPECT_NO_THROW(resolver("Quoting", engineType));
    EXPECT_EQ(engineType, Venue::EngineType::Quoting);
}