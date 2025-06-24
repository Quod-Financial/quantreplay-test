#include <initializer_list>
#include <stdexcept>
#include <string>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "data_layer/api/models/market_phase.hpp"
#include "data_layer/api/models/venue.hpp"

#include "ih/marshalling/json/venue.hpp"

using namespace Simulator;
using namespace Simulator::Http;

using ::testing::Optional;
using ::testing::Eq;

// NOLINTBEGIN(*magic-numbers*)

class Http_Json_VenueMarshaller : public ::testing::Test {
  public:
    using Venue = DataLayer::Venue;
    using Patch = Venue::Patch;

    static auto makeDefaultPatch() -> Patch
    {
        Patch patch{};
        patch.withVenueId("dummy");
        return patch;
    }

    static auto makeVenue(Patch _patch)
    {
        return Venue::create(std::move(_patch));
    }

    static auto marshall(Venue const& _venue) -> std::string
    {
        return Json::VenueMarshaller::marshall(_venue);
    }

    static auto marshall(std::initializer_list<Venue> _venues) -> std::string
    {
        return Json::VenueMarshaller::marshall(std::vector<Venue>{_venues});
    }
};


TEST_F(Http_Json_VenueMarshaller, Marshall_VenueID)
{
    auto const patch = makeDefaultPatch().withVenueId("VenueID");
    auto const venue = makeVenue(patch);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":"VenueID",)"
        R"("phases":[])"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(venue), expectedJson);
}

TEST_F(Http_Json_VenueMarshaller, Marshall_Name)
{
    auto const patch = makeDefaultPatch().withName("VenueName");
    auto const venue = makeVenue(patch);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":"dummy",)"
        R"("name":"VenueName",)"
        R"("phases":[])"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(venue), expectedJson);
}

TEST_F(Http_Json_VenueMarshaller, Marshall_EngineType)
{
    using EngineType = DataLayer::Venue::EngineType;
    auto const patch = makeDefaultPatch().withEngineType(EngineType::Matching);
    auto const venue = makeVenue(patch);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":"dummy",)"
        R"("engineType":"Matching",)"
        R"("phases":[])"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(venue), expectedJson);
}

TEST_F(Http_Json_VenueMarshaller, Marshall_SupportTifIoc)
{
    auto const patch = makeDefaultPatch().withSupportTifIocFlag(true);
    auto const venue = makeVenue(patch);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":"dummy",)"
        R"("supportTifIoc":true,)"
        R"("phases":[])"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(venue), expectedJson);
}

TEST_F(Http_Json_VenueMarshaller, Marshall_SupportTifFok)
{
    auto const patch = makeDefaultPatch().withSupportTifFokFlag(true);
    auto const venue = makeVenue(patch);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":"dummy",)"
        R"("supportTifFok":true,)"
        R"("phases":[])"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(venue), expectedJson);
}

TEST_F(Http_Json_VenueMarshaller, Marshall_SupportTifDay)
{
    auto const patch = makeDefaultPatch().withSupportTifDayFlag(true);
    auto const venue = makeVenue(patch);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":"dummy",)"
        R"("supportTifDay":true,)"
        R"("phases":[])"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(venue), expectedJson);
}

TEST_F(Http_Json_VenueMarshaller, Marshall_IncludeOwnOrders)
{
    auto const patch = makeDefaultPatch().withIncludeOwnOrdersFlag(true);
    auto const venue = makeVenue(patch);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":"dummy",)"
        R"("includeOwnOrders":true,)"
        R"("phases":[])"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(venue), expectedJson);
}

TEST_F(Http_Json_VenueMarshaller, Marshall_RestPort)
{
    auto const patch = makeDefaultPatch().withRestPort(9001);
    auto const venue = makeVenue(patch);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":"dummy",)"
        R"("restPort":9001,)"
        R"("phases":[])"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(venue), expectedJson);
}

TEST_F(Http_Json_VenueMarshaller, Marshall_OrderOnStartup)
{
    auto const patch = makeDefaultPatch().withOrdersOnStartupFlag(false);
    auto const venue = makeVenue(patch);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":"dummy",)"
        R"("orderOnStartup":false,)"
        R"("phases":[])"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(venue), expectedJson);
}

TEST_F(Http_Json_VenueMarshaller, Marshall_RandomPartiesCount)
{
    auto const patch = makeDefaultPatch().withRandomPartiesCount(42);
    auto const venue = makeVenue(patch);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":"dummy",)"
        R"("randomPartyCount":42,)"
        R"("phases":[])"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(venue), expectedJson);
}

TEST_F(Http_Json_VenueMarshaller, Marshall_TnsEnabled)
{
    auto const patch = makeDefaultPatch().withTnsEnabledFlag(true);
    auto const venue = makeVenue(patch);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":"dummy",)"
        R"("timeAndSalesEnabled":true,)"
        R"("phases":[])"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(venue), expectedJson);
}

TEST_F(Http_Json_VenueMarshaller, Marshall_TnsQtyEnabled)
{
    auto const patch = makeDefaultPatch().withTnsQtyEnabledFlag(true);
    auto const venue = makeVenue(patch);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":"dummy",)"
        R"("timeAndSalesQuantityEnabled":true,)"
        R"("phases":[])"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(venue), expectedJson);
}

TEST_F(Http_Json_VenueMarshaller, Marshall_TnsSideEnabled)
{
    auto const patch = makeDefaultPatch().withTnsSideEnabledFlag(true);
    auto const venue = makeVenue(patch);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":"dummy",)"
        R"("timeAndSalesSideEnabled":true,)"
        R"("phases":[])"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(venue), expectedJson);
}

TEST_F(Http_Json_VenueMarshaller, Marshall_TnsPartiesEnabled)
{
    auto const patch = makeDefaultPatch().withTnsPartiesEnabledFlag(true);
    auto const venue = makeVenue(patch);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":"dummy",)"
        R"("timeAndSalesPartiesEnabled":true,)"
        R"("phases":[])"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(venue), expectedJson);
}

TEST_F(Http_Json_VenueMarshaller, Marshall_Timezone)
{
    auto const patch = makeDefaultPatch().withTimezone("GMT");
    auto const venue = makeVenue(patch);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":"dummy",)"
        R"("timezone":"GMT",)"
        R"("phases":[])"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(venue), expectedJson);
}

TEST_F(Http_Json_VenueMarshaller, Marshall_CancelOnDisconnect)
{
    auto const patch = makeDefaultPatch().withCancelOnDisconnectFlag(false);
    auto const venue = makeVenue(patch);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":"dummy",)"
        R"("cancelOnDisconnect":false,)"
        R"("phases":[])"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(venue), expectedJson);
}

TEST_F(Http_Json_VenueMarshaller, Marshall_PersistenceEnabled)
{
    auto const patch = makeDefaultPatch().withPersistenceEnabledFlag(true);
    auto const venue = makeVenue(patch);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":"dummy",)"
        R"("persistenceEnabled":true,)"
        R"("phases":[])"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(venue), expectedJson);
}

TEST_F(Http_Json_VenueMarshaller, Marshall_PersistenceFilePath)
{
    auto const patch = makeDefaultPatch().withPersistenceFilePath("/file/path");
    auto const venue = makeVenue(patch);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":"dummy",)"
        R"("persistenceFilePath":"/file/path",)"
        R"("phases":[])"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(venue), expectedJson);
}

TEST_F(Http_Json_VenueMarshaller, Marshall_MarketPhases)
{
    DataLayer::MarketPhase::Patch phase{};
    phase.with_phase(DataLayer::MarketPhase::Phase::Open)
        .with_start_time("12:00:00")
        .with_end_time("17:00:00")
        .with_end_time_range(5)
        .with_allow_cancels(true);

    auto patch = makeDefaultPatch().withMarketPhase(phase);
    auto const venue = makeVenue(patch);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":"dummy",)"
        R"("phases":[)"
            "{"
                R"("phase":"Open",)"
                R"("startTime":"12:00:00",)"
                R"("endTime":"17:00:00",)"
                R"("venueId":"dummy",)"
                R"("allowCancels":true,)"
                R"("endTimeRange":5)"
            "}"
        "]"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(venue), expectedJson);
}

TEST_F(Http_Json_VenueMarshaller, Marshall_VenuesList)
{
    auto patch = makeDefaultPatch();
    auto const venue1 = makeVenue(patch.withVenueId("Venue1"));
    auto const venue2 = makeVenue(patch.withVenueId("Venue2"));

    // clang-format off
    std::string const expectedJson{"{"
        R"("venues":[)"
            "{"
                R"("id":"Venue1",)"
                R"("phases":[])"
            "},"
            "{"
                R"("id":"Venue2",)"
                R"("phases":[])"
            "}"
        "]"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall({venue1, venue2}), expectedJson);
}


TEST(Http_Json_VenueUnmarshaller, Unmarshall_VenueID)
{
    constexpr std::string_view json{R"({"id":"VenueID"})"};

    DataLayer::Venue::Patch patch{};
    Json::VenueUnmarshaller::unmarshall(json, patch);
    EXPECT_THAT(patch.getVenueId(), Optional(Eq("VenueID")));
}

TEST(Http_Json_VenueUnmarshaller, Unmarshall_Name)
{
    constexpr std::string_view json{R"({"name":"VenueName"})"};

    DataLayer::Venue::Patch patch{};
    Json::VenueUnmarshaller::unmarshall(json, patch);
    EXPECT_THAT(patch.getName(), Optional(Eq("VenueName")));
}

TEST(Http_Json_VenueUnmarshaller, Unmarshall_EngineType)
{
    using EngineType = DataLayer::Venue::EngineType;
    constexpr std::string_view json{R"({"engineType":"Quoting"})"};

    DataLayer::Venue::Patch patch{};
    Json::VenueUnmarshaller::unmarshall(json, patch);
    EXPECT_THAT(patch.getEngineType(), Optional(Eq(EngineType::Quoting)));
}

TEST(Http_Json_VenueUnmarshaller, Unmarshall_SupportTifIoc)
{
    constexpr std::string_view json{R"({"supportTifIoc":true})"};

    DataLayer::Venue::Patch patch{};
    Json::VenueUnmarshaller::unmarshall(json, patch);
    EXPECT_THAT(patch.getSupportTifIocFlag(), Optional(Eq(true)));
}

TEST(Http_Json_VenueUnmarshaller, Unmarshall_SupportTifFok)
{
    constexpr std::string_view json{R"({"supportTifFok":false})"};

    DataLayer::Venue::Patch patch{};
    Json::VenueUnmarshaller::unmarshall(json, patch);
    EXPECT_THAT(patch.getSupportTifFokFlag(), Optional(Eq(false)));
}

TEST(Http_Json_VenueUnmarshaller, Unmarshall_SupportTifDay)
{
    constexpr std::string_view json{R"({"supportTifDay":true})"};

    DataLayer::Venue::Patch patch{};
    Json::VenueUnmarshaller::unmarshall(json, patch);
    EXPECT_THAT(patch.getSupportTifDayFlag(), Optional(Eq(true)));
}

TEST(Http_Json_VenueUnmarshaller, Unmarshall_IncludeOwnOrders)
{
    constexpr std::string_view json{R"({"includeOwnOrders":false})"};

    DataLayer::Venue::Patch patch{};
    Json::VenueUnmarshaller::unmarshall(json, patch);
    EXPECT_THAT(patch.getIncludeOwnOrdersFlag(), Optional(Eq(false)));
}

TEST(Http_Json_VenueUnmarshaller, Unmarshall_RestPort)
{
    constexpr std::string_view json{R"({"restPort":42})"};

    DataLayer::Venue::Patch patch{};
    Json::VenueUnmarshaller::unmarshall(json, patch);
    EXPECT_THAT(patch.getRestPort(), Optional(Eq(42)));
}

TEST(Http_Json_VenueUnmarshaller, Unmarshall_OrdersOnStartup)
{
    constexpr std::string_view json{R"({"orderOnStartup":true})"};

    DataLayer::Venue::Patch patch{};
    Json::VenueUnmarshaller::unmarshall(json, patch);
    EXPECT_THAT(patch.getOrdersOnStartupFlag(), Optional(Eq(true)));
}

TEST(Http_Json_VenueUnmarshaller, Unmarshall_RandomPartiesCount)
{
    constexpr std::string_view json{R"({"randomPartyCount":42})"};

    DataLayer::Venue::Patch patch{};
    Json::VenueUnmarshaller::unmarshall(json, patch);
    EXPECT_THAT(patch.getRandomPartiesCount(), Optional(Eq(42)));
}

TEST(Http_Json_VenueUnmarshaller, Unmarshall_TnsEnabled)
{
    constexpr std::string_view json{R"({"timeAndSalesEnabled":true})"};

    DataLayer::Venue::Patch patch{};
    Json::VenueUnmarshaller::unmarshall(json, patch);
    EXPECT_THAT(patch.getTnsEnabledFlag(), Optional(Eq(true)));
}

TEST(Http_Json_VenueUnmarshaller, Unmarshall_TnsQtyEnabled)
{
    constexpr std::string_view json{R"({"timeAndSalesQuantityEnabled":true})"};

    DataLayer::Venue::Patch patch{};
    Json::VenueUnmarshaller::unmarshall(json, patch);
    EXPECT_THAT(patch.getTnsQtyEnabledFlag(), Optional(Eq(true)));
}

TEST(Http_Json_VenueUnmarshaller, Unmarshall_TnsSideEnabled)
{
    constexpr std::string_view json{R"({"timeAndSalesSideEnabled":true})"};

    DataLayer::Venue::Patch patch{};
    Json::VenueUnmarshaller::unmarshall(json, patch);
    EXPECT_THAT(patch.getTnsSideEnabledFlag(), Optional(Eq(true)));
}

TEST(Http_Json_VenueUnmarshaller, Unmarshall_TnsPartiesEnabled)
{
    constexpr std::string_view json{R"({"timeAndSalesPartiesEnabled":false})"};

    DataLayer::Venue::Patch patch{};
    Json::VenueUnmarshaller::unmarshall(json, patch);
    EXPECT_THAT(patch.getTnsPartiesEnabledFlag(), Optional(Eq(false)));
}

TEST(Http_Json_VenueUnmarshaller, Unmarshall_Timezone)
{
    constexpr std::string_view json{R"({"timezone":"GMT"})"};

    DataLayer::Venue::Patch patch{};
    Json::VenueUnmarshaller::unmarshall(json, patch);
    EXPECT_THAT(patch.getTimezone(), Optional(Eq("GMT")));
}

TEST(Http_Json_VenueUnmarshaller, Unmarshall_CancelOnDisconnect)
{
    constexpr std::string_view json{R"({"cancelOnDisconnect":true})"};

    DataLayer::Venue::Patch patch{};
    Json::VenueUnmarshaller::unmarshall(json, patch);
    EXPECT_THAT(patch.getCancelOnDisconnectFlag(), Optional(Eq(true)));
}

TEST(Http_Json_VenueUnmarshaller, Unmarshall_PersistenceEnabled)
{
    constexpr std::string_view json{R"({"persistenceEnabled":true})"};

    DataLayer::Venue::Patch patch{};
    Json::VenueUnmarshaller::unmarshall(json, patch);
    EXPECT_THAT(patch.getPersistenceEnabledFlag(), Optional(Eq(true)));
}

TEST(Http_Json_VenueUnmarshaller, Unmarshall_PersistenceFilePath)
{
    constexpr std::string_view json{R"({"persistenceFilePath":"/file.csv"})"};

    DataLayer::Venue::Patch patch{};
    Json::VenueUnmarshaller::unmarshall(json, patch);
    EXPECT_THAT(patch.getPersistenceFilePath(), Optional(Eq("/file.csv")));
}

TEST(Http_Json_VenueUnmarshaller, Unmarshall_MarketPhases_KeyNotExist)
{
    constexpr std::string_view json{"{}"};

    DataLayer::Venue::Patch patch{};
    Json::VenueUnmarshaller::unmarshall(json, patch);
    EXPECT_FALSE(patch.getMarketPhases().has_value());
}

TEST(Http_Json_VenueUnmarshaller, Unmarshall_MarketPhases_NotAnArray)
{
    constexpr std::string_view json{R"({"phases":{}})"};

    DataLayer::Venue::Patch patch{};
    EXPECT_THROW(
        Json::VenueUnmarshaller::unmarshall(json, patch),
        std::runtime_error
    );
}

TEST(Http_Json_VenueUnmarshaller, Unmarshall_MarketPhases_InvalidElemType)
{
    constexpr std::string_view json{R"({"phases":[5, 1, 2]})"};

    DataLayer::Venue::Patch patch{};
    EXPECT_THROW(
        Json::VenueUnmarshaller::unmarshall(json, patch),
        std::runtime_error
    );
}

TEST(Http_Json_VenueUnmarshaller, UnmarshallsMarketPhasesAllowCancelsIsFalse)
{
  constexpr std::string_view json{R"({"phases":[{"allowCancels":false}]})"};

  DataLayer::Venue::Patch patch;
  Json::VenueUnmarshaller::unmarshall(json, patch);

  const auto& optPhases = patch.getMarketPhases();
  EXPECT_EQ(optPhases->size(), 1);
  EXPECT_EQ(optPhases->at(0).allow_cancels(), false);
}

TEST(Http_Json_VenueUnmarshaller, UnmarshallsMarketPhasesAllowCancelsIsTrue)
{
    constexpr std::string_view json{R"({"phases":[{"allowCancels":true}]})"};

    DataLayer::Venue::Patch patch;
    Json::VenueUnmarshaller::unmarshall(json, patch);

    const auto& optPhases = patch.getMarketPhases();
    EXPECT_EQ(optPhases->size(), 1);
    EXPECT_EQ(optPhases->at(0).allow_cancels(), true);
}

TEST(Http_Json_VenueUnmarshaller, Unmarshall_MarketPhases_ValidArray)
{
    // clang-format off
    constexpr std::string_view json{"{"
        R"("phases":[)"
            "{"
                R"("phase":"Open",)"
                R"("startTime":"12:00:00",)"
                R"("endTime":"17:00:00",)"
                R"("venueId":"dummy",)"
                R"("allowCancels":true,)"
                R"("endTimeRange":5)"
            "},"
            "{"
                R"("phase":"Closed",)"
                R"("startTime":"17:00:00",)"
                R"("endTime":"22:00:00",)"
                R"("venueId":"dummy",)"
                R"("allowCancels":false,)"
                R"("endTimeRange":5)"
            "}"
        "]"
    "}"};
    // clang-format on


    DataLayer::Venue::Patch patch{};
    Json::VenueUnmarshaller::unmarshall(json, patch);

    auto const& optPhases = patch.getMarketPhases();
    ASSERT_TRUE(optPhases.has_value());
    ASSERT_EQ(optPhases->size(), 2);
    EXPECT_EQ(optPhases->at(0).phase(), DataLayer::MarketPhase::Phase::Open);
    EXPECT_EQ(optPhases->at(1).phase(), DataLayer::MarketPhase::Phase::Closed);
}

// NOLINTEND(*magic-numbers*)