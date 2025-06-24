#include <stdexcept>

#include <gtest/gtest.h>

#include "data_layer/api/models/datasource.hpp"
#include "data_layer/api/models/market_phase.hpp"
#include "data_layer/api/models/venue.hpp"

#include "ih/marshalling/json/detail/enumeration_resolver.hpp"

using namespace Simulator;
using namespace Simulator::Http;
using Resolver = Json::EnumerationResolver;


TEST(Http_Json_EnumerationResolver_DatasourceFormat, Resolve_Enum_Undefined)
{
    using Format = DataLayer::Datasource::Format;
    EXPECT_THROW((void)Resolver::resolve(static_cast<Format>(-1)), std::logic_error);
}

TEST(Http_Json_EnumerationResolver_DatasourceFormat, Resolve_Enum_CSV)
{
    using Format = DataLayer::Datasource::Format;
    EXPECT_EQ(Resolver::resolve(Format::Csv), "CSV");
}

TEST(Http_Json_EnumerationResolver_DatasourceFormat, Resolve_Enum_Postgres)
{
    using Format = DataLayer::Datasource::Format;
    EXPECT_EQ(Resolver::resolve(Format::Postgres), "PSQL");
}

TEST(Http_Json_EnumerationResolver_DatasourceFormat, Resolve_String_Undefined)
{
    using Format = DataLayer::Datasource::Format;
    Format format{};
    EXPECT_THROW(Resolver::resolve("bad", format), std::runtime_error);
}

TEST(Http_Json_EnumerationResolver_DatasourceFormat, Resolve_String_CSV)
{
    using Format = DataLayer::Datasource::Format;
    Format format{};
    ASSERT_NO_THROW(Resolver::resolve("CSV", format));
    EXPECT_EQ(format, Format::Csv);
}

TEST(Http_Json_EnumerationResolver_DatasourceFormat, Resolve_String_Postgres)
{
    using Format = DataLayer::Datasource::Format;
    Format format{};
    ASSERT_NO_THROW(Resolver::resolve("PSQL", format));
    EXPECT_EQ(format, Format::Postgres);
}


TEST(Http_Json_EnumerationResolver_DatasourceType, Resolve_Enum_Undefined)
{
    using Type = DataLayer::Datasource::Type;
    EXPECT_THROW((void)Resolver::resolve(static_cast<Type>(-1)), std::logic_error);
}

TEST(Http_Json_EnumerationResolver_DatasourceType, Resolve_Enum_OrderBook)
{
    using Type = DataLayer::Datasource::Type;
    EXPECT_EQ(Resolver::resolve(Type::OrderBook), "OrderBook");
}

TEST(Http_Json_EnumerationResolver_DatasourceType, Resolve_String_Undefined)
{
    using Type = DataLayer::Datasource::Type;
    Type type{};
    EXPECT_THROW(Resolver::resolve("bad", type), std::runtime_error);
}

TEST(Http_Json_EnumerationResolver_DatasourceType, Resolve_String_OrderBook)
{
    using Type = DataLayer::Datasource::Type;
    Type type{};
    ASSERT_NO_THROW(Resolver::resolve("OrderBook", type));
    EXPECT_EQ(type, Type::OrderBook);
}


TEST(Http_Json_EnumerationResolver_MarketPhaseType, Resolve_Enum_Undefined)
{
    using Phase = DataLayer::MarketPhase::Phase;
    EXPECT_THROW(
        (void)Resolver::resolve(static_cast<Phase>(-1)),
        std::logic_error
    );
}

TEST(Http_Json_EnumerationResolver_MarketPhaseType, Resolve_Enum_Open)
{
    using Phase = DataLayer::MarketPhase::Phase;
    EXPECT_EQ(Resolver::resolve(Phase::Open), "Open");
}

TEST(Http_Json_EnumerationResolver_MarketPhaseType, Resolve_Enum_Closed)
{
    using Phase = DataLayer::MarketPhase::Phase;
    EXPECT_EQ(Resolver::resolve(Phase::Closed), "Closed");
}

TEST(Http_Json_EnumerationResolver_MarketPhaseType, Resolve_Enum_OpeningAuction)
{
    using Phase = DataLayer::MarketPhase::Phase;
    EXPECT_EQ(Resolver::resolve(Phase::OpeningAuction), "PreOpen");
}

TEST(Http_Json_EnumerationResolver_MarketPhaseType, Resolve_Enum_ClosingAuction)
{
    using Phase = DataLayer::MarketPhase::Phase;
    EXPECT_EQ(Resolver::resolve(Phase::ClosingAuction), "PreClose");
}

TEST(Http_Json_EnumerationResolver_MarketPhaseType, Resolve_Enum_IntradayAuction)
{
    using Phase = DataLayer::MarketPhase::Phase;
    EXPECT_EQ(Resolver::resolve(Phase::IntradayAuction), "Auction");
}

TEST(Http_Json_EnumerationResolver_MarketPhaseType, Resolve_Enum_Halted)
{
    using Phase = DataLayer::MarketPhase::Phase;
    EXPECT_EQ(Resolver::resolve(Phase::Halted), "Halted");
}

TEST(Http_Json_EnumerationResolver_MarketPhaseType, Resolve_String_Undefined)
{
    using Phase = DataLayer::MarketPhase::Phase;
    Phase phase{};
    EXPECT_THROW(Resolver::resolve("bad", phase), std::runtime_error);
}

TEST(Http_Json_EnumerationResolver_MarketPhaseType, Resolve_String_Open)
{
    using Phase = DataLayer::MarketPhase::Phase;
    Phase phase{};
    ASSERT_NO_THROW(Resolver::resolve("Open", phase));
    EXPECT_EQ(phase, Phase::Open);
}

TEST(Http_Json_EnumerationResolver_MarketPhaseType, Resolve_String_Closed)
{
    using Phase = DataLayer::MarketPhase::Phase;
    Phase phase{};
    ASSERT_NO_THROW(Resolver::resolve("Closed", phase));
    EXPECT_EQ(phase, Phase::Closed);
}

TEST(Http_Json_EnumerationResolver_MarketPhaseType, Resolve_String_OpeningAuction)
{
    using Phase = DataLayer::MarketPhase::Phase;
    Phase phase{};
    ASSERT_NO_THROW(Resolver::resolve("PreOpen", phase));
    EXPECT_EQ(phase, Phase::OpeningAuction);
}

TEST(Http_Json_EnumerationResolver_MarketPhaseType, Resolve_String_ClosingAuction)
{
    using Phase = DataLayer::MarketPhase::Phase;
    Phase phase{};
    ASSERT_NO_THROW(Resolver::resolve("PreClose", phase));
    EXPECT_EQ(phase, Phase::ClosingAuction);
}

TEST(Http_Json_EnumerationResolver_MarketPhaseType, Resolve_String_IntradayAuction)
{
    using Phase = DataLayer::MarketPhase::Phase;
    Phase phase{};
    ASSERT_NO_THROW(Resolver::resolve("Auction", phase));
    EXPECT_EQ(phase, Phase::IntradayAuction);
}

TEST(Http_Json_EnumerationResolver_MarketPhaseType, Resolve_String_Halted)
{
    using Phase = DataLayer::MarketPhase::Phase;
    Phase phase{};
    ASSERT_NO_THROW(Resolver::resolve("Halted", phase));
    EXPECT_EQ(phase, Phase::Halted);
}


TEST(Http_Json_EnumerationResolver_VenueEngineType, Resolve_Enum_Undefined)
{
    using EngineType = DataLayer::Venue::EngineType;
    EXPECT_THROW(
        (void)Resolver::resolve(static_cast<EngineType>(-1)),
        std::logic_error
    );
}

TEST(Http_Json_EnumerationResolver_VenueEngineType, Resolve_Enum_Matching)
{
    using EngineType = DataLayer::Venue::EngineType;
    EXPECT_EQ(Resolver::resolve(EngineType::Matching), "Matching");
}

TEST(Http_Json_EnumerationResolver_VenueEngineType, Resolve_Enum_Quoting)
{
    using EngineType = DataLayer::Venue::EngineType;
    EXPECT_EQ(Resolver::resolve(EngineType::Quoting), "Quoting");
}

TEST(Http_Json_EnumerationResolver_VenueEngineType, Resolve_String_Undefined)
{
    using EngineType = DataLayer::Venue::EngineType;
    EngineType type{};
    EXPECT_THROW(Resolver::resolve("bad", type), std::runtime_error);
}

TEST(Http_Json_EnumerationResolver_VenueEngineType, Resolve_String_Matching)
{
    using EngineType = DataLayer::Venue::EngineType;
    EngineType type{};
    ASSERT_NO_THROW(Resolver::resolve("Matching", type));
    EXPECT_EQ(type, EngineType::Matching);
}

TEST(Http_Json_EnumerationResolver_VenueEngineType, Resolve_String_Quoting)
{
    using EngineType = DataLayer::Venue::EngineType;
    EngineType type{};
    ASSERT_NO_THROW(Resolver::resolve("Quoting", type));
    EXPECT_EQ(type, EngineType::Quoting);
}