#include <optional>
#include <string>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "api/models/venue.hpp"
#include "ih/common/exceptions.hpp"

using ::testing::Optional;
using ::testing::Eq;

using namespace Simulator::DataLayer;
using EngineType = Venue::EngineType;


TEST(DataLayer_Model_Venue, Patch_Set_VenueID)
{
    Venue::Patch patch{};
    ASSERT_FALSE(patch.getVenueId().has_value());

    patch.withVenueId("FASTMATCH");
    EXPECT_THAT(patch.getVenueId(), Optional(Eq("FASTMATCH")));
}

TEST(DataLayer_Model_Venue, Patch_Set_Name)
{
    Venue::Patch patch{};
    ASSERT_FALSE(patch.getName().has_value());

    patch.withName("London Stock Exchange");
    EXPECT_THAT(patch.getName(), Optional(Eq("London Stock Exchange")));
}

TEST(DataLayer_Model_Venue, Patch_Set_EngineType)
{
    Venue::Patch patch{};
    ASSERT_FALSE(patch.getEngineType().has_value());

    patch.withEngineType(EngineType::Quoting);
    EXPECT_THAT(patch.getEngineType(), Optional(Eq(EngineType::Quoting)));
}

TEST(DataLayer_Model_Venue, Patch_Set_SupportTifIocFlag)
{
    Venue::Patch patch{};
    ASSERT_FALSE(patch.getSupportTifIocFlag().has_value());

    patch.withSupportTifIocFlag(true);
    EXPECT_THAT(patch.getSupportTifIocFlag(), Optional(Eq(true)));
}

TEST(DataLayer_Model_Venue, Patch_Set_SupportTifFokFlag)
{
    Venue::Patch patch{};
    ASSERT_FALSE(patch.getSupportTifFokFlag().has_value());

    patch.withSupportTifFokFlag(false);
    EXPECT_THAT(patch.getSupportTifFokFlag(), Optional(Eq(false)));
}

TEST(DataLayer_Model_Venue, Patch_Set_SupportTifDayFlag)
{
    Venue::Patch patch{};
    ASSERT_FALSE(patch.getSupportTifDayFlag().has_value());

    patch.withSupportTifDayFlag(true);
    EXPECT_THAT(patch.getSupportTifDayFlag(), Optional(Eq(true)));
}

TEST(DataLayer_Model_Venue, Patch_Set_IncludeOwnOrdersFlag)
{
    Venue::Patch patch{};
    ASSERT_FALSE(patch.getIncludeOwnOrdersFlag().has_value());

    patch.withIncludeOwnOrdersFlag(false);
    EXPECT_THAT(patch.getIncludeOwnOrdersFlag(), Optional(Eq(false)));
}

TEST(DataLayer_Model_Venue, Patch_Set_RestPort)
{
    Venue::Patch patch{};
    ASSERT_FALSE(patch.getRestPort().has_value());

    patch.withRestPort(9001); // NOLINT: Test value
    EXPECT_THAT(patch.getRestPort(), Optional(Eq(9001)));
}

TEST(DataLayer_Model_Venue, Patch_Set_OrdersOnStartupFlag)
{
    Venue::Patch patch{};
    ASSERT_FALSE(patch.getOrdersOnStartupFlag().has_value());

    patch.withOrdersOnStartupFlag(true);
    EXPECT_THAT(patch.getOrdersOnStartupFlag(), Optional(Eq(true)));
}

TEST(DataLayer_Model_Venue, Patch_Set_RandomPartiesCount)
{
    Venue::Patch patch{};
    ASSERT_FALSE(patch.getRandomPartiesCount().has_value());

    patch.withRandomPartiesCount(50); // NOLINT: Test value
    EXPECT_THAT(patch.getRandomPartiesCount(), Optional(Eq(50)));
}

TEST(DataLayer_Model_Venue, Patch_Set_TnsEnabledFlag)
{
    Venue::Patch patch{};
    ASSERT_FALSE(patch.getTnsEnabledFlag().has_value());

    patch.withTnsEnabledFlag(true);
    EXPECT_THAT(patch.getTnsEnabledFlag(), Optional(Eq(true)));
}

TEST(DataLayer_Model_Venue, Patch_Set_TnsQtyEnabledFlag)
{
    Venue::Patch patch{};
    ASSERT_FALSE(patch.getTnsQtyEnabledFlag().has_value());

    patch.withTnsQtyEnabledFlag(true);
    EXPECT_THAT(patch.getTnsQtyEnabledFlag(), Optional(Eq(true)));
}

TEST(DataLayer_Model_Venue, Patch_Set_TnsSideEnabledFlag)
{
    Venue::Patch patch{};
    ASSERT_FALSE(patch.getTnsSideEnabledFlag().has_value());

    patch.withTnsSideEnabledFlag(false);
    EXPECT_THAT(patch.getTnsSideEnabledFlag(), Optional(Eq(false)));
}

TEST(DataLayer_Model_Venue, Patch_Set_TnsPartiesEnabledFlag)
{
    Venue::Patch patch{};
    ASSERT_FALSE(patch.getTnsPartiesEnabledFlag().has_value());

    patch.withTnsPartiesEnabledFlag(false);
    EXPECT_THAT(patch.getTnsPartiesEnabledFlag(), Optional(Eq(false)));
}

TEST(DataLayer_Model_Venue, Patch_Set_Timezone)
{
    Venue::Patch patch{};
    ASSERT_FALSE(patch.getTimezone().has_value());

    patch.withTimezone("GMT+3");
    EXPECT_THAT(patch.getTimezone(), Optional(Eq("GMT+3")));
}

TEST(DataLayer_Model_Venue, Patch_Set_CancelOnDiconnectFlag)
{
    Venue::Patch patch{};
    ASSERT_FALSE(patch.getCancelOnDisconnectFlag().has_value());

    patch.withCancelOnDisconnectFlag(true);
    EXPECT_THAT(patch.getCancelOnDisconnectFlag(), Optional(Eq(true)));
}

TEST(DataLayer_Model_Venue, Patch_Set_PersistenceEnabledFlag)
{
    Venue::Patch patch{};
    ASSERT_FALSE(patch.getPersistenceEnabledFlag().has_value());

    patch.withPersistenceEnabledFlag(true);
    EXPECT_THAT(patch.getPersistenceEnabledFlag(), Optional(Eq(true)));
}

TEST(DataLayer_Model_Venue, Patch_Set_PersistenceFilePath)
{
    Venue::Patch patch{};
    ASSERT_FALSE(patch.getPersistenceFilePath().has_value());

    patch.withPersistenceFilePath("/rw/sim-storage.json");
    EXPECT_THAT(
        patch.getPersistenceFilePath(),
        Optional(Eq("/rw/sim-storage.json"))
    );
}

TEST(DataLayer_Model_Venue, Patch_Set_MarketPhases_Add)
{
    MarketPhase::Patch const marketPhase{};
    Venue::Patch patch{};
    ASSERT_FALSE(patch.getPersistenceFilePath().has_value());

    patch.withMarketPhase(marketPhase);
    patch.withMarketPhase(marketPhase);

    ASSERT_TRUE(patch.getMarketPhases().has_value());
    EXPECT_EQ(patch.getMarketPhases()->size(), 2); // NOLINT: check is upper
}

TEST(DataLayer_Model_Venue, Patch_Set_MarketPhases_Clean)
{
    Venue::Patch patch{};
    ASSERT_FALSE(patch.getPersistenceFilePath().has_value());

    patch.withoutMarketPhases();
    ASSERT_TRUE(patch.getMarketPhases().has_value());
    EXPECT_TRUE(patch.getMarketPhases()->empty()); // NOLINT: check is upper
}

TEST(DataLayer_Model_Venue, Patch_Set_MarketPhases_Clean_AssignedPreviously)
{
    MarketPhase::Patch const marketPhase{};
    Venue::Patch patch{};
    patch.withMarketPhase(marketPhase);
    patch.withMarketPhase(marketPhase);
    ASSERT_TRUE(patch.getMarketPhases().has_value());
    ASSERT_EQ(patch.getMarketPhases()->size(), 2); // NOLINT: check is upper

    patch.withoutMarketPhases();
    ASSERT_TRUE(patch.getMarketPhases().has_value());
    EXPECT_TRUE(patch.getMarketPhases()->empty()); // NOLINT: check is upper
}

TEST(DataLayer_Model_Venue, Get_VenueID_Missing)
{
    Venue::Patch const patch{};
    ASSERT_FALSE(patch.getVenueId().has_value());

    EXPECT_THROW((void)Venue::create(patch), RequiredAttributeMissing);
}

TEST(DataLayer_Model_Venue, Get_VenueID_Specified)
{
    Venue::Patch patch{};
    patch.withVenueId("XETRA");
    ASSERT_TRUE(patch.getVenueId().has_value());

    Venue const venue = Venue::create(patch);
    EXPECT_EQ(venue.getVenueId(), "XETRA");
}

TEST(DataLayer_Model_Venue, Get_Name_Missing)
{
    Venue::Patch patch{};
    patch.withVenueId("XETRA");

    Venue const venue = Venue::create(patch);
    EXPECT_EQ(venue.getName(), std::nullopt);
}

TEST(DataLayer_Model_Venue, Get_Name_Specified)
{
    Venue::Patch patch{};
    patch.withVenueId("LSE");
    patch.withName("London Stock Exchange");

    Venue const venue = Venue::create(patch);
    EXPECT_THAT(venue.getName(), Optional(Eq("London Stock Exchange")));
}

TEST(DataLayer_Model_Venue, Get_EngineType_Missing)
{
    Venue::Patch patch{};
    patch.withVenueId("LSE");

    Venue const venue = Venue::create(patch);
    EXPECT_EQ(venue.getEngineType(), std::nullopt);
}

TEST(DataLayer_Model_Venue, Get_EngineType_Specified)
{
    Venue::Patch patch{};
    patch.withVenueId("LSE");
    patch.withEngineType(EngineType::Matching);

    Venue const venue = Venue::create(patch);
    EXPECT_THAT(venue.getEngineType(), Optional(Eq(EngineType::Matching)));
}

TEST(DataLayer_Model_Venue, Get_SupportTifIocFlag_Missing)
{
    Venue::Patch patch{};
    patch.withVenueId("LSE");

    Venue const venue = Venue::create(patch);
    EXPECT_EQ(venue.getSupportTifIocFlag(), std::nullopt);
}

TEST(DataLayer_Model_Venue, Get_SupportTifIocFlag_Specified)
{
    Venue::Patch patch{};
    patch.withVenueId("LSE");
    patch.withSupportTifIocFlag(false);

    Venue const venue = Venue::create(patch);
    EXPECT_THAT(venue.getSupportTifIocFlag(), Optional(Eq(false)));
}

TEST(DataLayer_Model_Venue, Get_SupportTifFokFlag_Missing)
{
    Venue::Patch patch{};
    patch.withVenueId("LSE");

    Venue const venue = Venue::create(patch);
    EXPECT_EQ(venue.getSupportTifFokFlag(), std::nullopt);
}

TEST(DataLayer_Model_Venue, Get_SupportTifFokFlag_Specified)
{
    Venue::Patch patch{};
    patch.withVenueId("LSE");
    patch.withSupportTifFokFlag(false);

    Venue const venue = Venue::create(patch);
    EXPECT_THAT(venue.getSupportTifFokFlag(), Optional(Eq(false)));
}

TEST(DataLayer_Model_Venue, Get_SupportTifDayFlag_Missing)
{
    Venue::Patch patch{};
    patch.withVenueId("LSE");

    Venue const venue = Venue::create(patch);
    EXPECT_EQ(venue.getSupportTifDayFlag(), std::nullopt);
}

TEST(DataLayer_Model_Venue, Get_SupportTifDayFlag_Specified)
{
    Venue::Patch patch{};
    patch.withVenueId("LSE");
    patch.withSupportTifDayFlag(false);

    Venue const venue = Venue::create(patch);
    EXPECT_THAT(venue.getSupportTifDayFlag(), Optional(Eq(false)));
}

TEST(DataLayer_Model_Venue, Get_IncludeOwnOrdersFlag_Missing)
{
    Venue::Patch patch{};
    patch.withVenueId("LSE");

    Venue const venue = Venue::create(patch);
    EXPECT_EQ(venue.getIncludeOwnOrdersFlag(), std::nullopt);
}

TEST(DataLayer_Model_Venue, Get_IncludeOwnOrdersFlag_Specified)
{
    Venue::Patch patch{};
    patch.withVenueId("LSE");
    patch.withIncludeOwnOrdersFlag(false);

    Venue const venue = Venue::create(patch);
    EXPECT_THAT(venue.getIncludeOwnOrdersFlag(), Optional(Eq(false)));
}

TEST(DataLayer_Model_Venue, Get_RestPort_Missing)
{
    Venue::Patch patch{};
    patch.withVenueId("LSE");

    Venue const venue = Venue::create(patch);
    EXPECT_EQ(venue.getRestPort(), std::nullopt);
}

TEST(DataLayer_Model_Venue, Get_RestPort_Specified)
{
    Venue::Patch patch{};
    patch.withVenueId("LSE");
    patch.withRestPort(9001); // NOLINT: Test value

    Venue const venue = Venue::create(patch);
    EXPECT_THAT(venue.getRestPort(), Optional(Eq(9001)));
}

TEST(DataLayer_Model_Venue, Get_OrdersOnStartupFlag_Missing)
{
    Venue::Patch patch{};
    patch.withVenueId("LSE");

    Venue const venue = Venue::create(patch);
    EXPECT_EQ(venue.getOrderOnStartupFlag(), std::nullopt);
}

TEST(DataLayer_Model_Venue, Get_OrdersOnStartupFlag_Specified)
{
    Venue::Patch patch{};
    patch.withVenueId("LSE");
    patch.withOrdersOnStartupFlag(false);

    Venue const venue = Venue::create(patch);
    EXPECT_THAT(venue.getOrderOnStartupFlag(), Optional(Eq(false)));
}

TEST(DataLayer_Model_Venue, Get_RandomPartiesCount_Missing)
{
    Venue::Patch patch{};
    patch.withVenueId("LSE");

    Venue const venue = Venue::create(patch);
    EXPECT_EQ(venue.getRandomPartiesCount(), std::nullopt);
}

TEST(DataLayer_Model_Venue, Get_RandomPartiesCount_Specified)
{
    Venue::Patch patch{};
    patch.withVenueId("LSE");
    patch.withRandomPartiesCount(42);

    Venue const venue = Venue::create(patch);
    EXPECT_THAT(venue.getRandomPartiesCount(), Optional(Eq(42)));
}

TEST(DataLayer_Model_Venue, Get_TnsEnabledFlag_Missing)
{
    Venue::Patch patch{};
    patch.withVenueId("LSE");

    Venue const venue = Venue::create(patch);
    EXPECT_EQ(venue.getTnsEnabledFlag(), std::nullopt);
}

TEST(DataLayer_Model_Venue, Get_TnsEnabledFlag_Specified)
{
    Venue::Patch patch{};
    patch.withVenueId("LSE");
    patch.withTnsEnabledFlag(false);

    Venue const venue = Venue::create(patch);
    EXPECT_THAT(venue.getTnsEnabledFlag(), Optional(Eq(false)));
}

TEST(DataLayer_Model_Venue, Get_TnsQtyEnabledFlag_Missing)
{
    Venue::Patch patch{};
    patch.withVenueId("LSE");

    Venue const venue = Venue::create(patch);
    EXPECT_EQ(venue.getTnsQtyEnabledFlag(), std::nullopt);
}

TEST(DataLayer_Model_Venue, Get_TnsQtyEnabledFlag_Specified)
{
    Venue::Patch patch{};
    patch.withVenueId("LSE");
    patch.withTnsQtyEnabledFlag(false);

    Venue const venue = Venue::create(patch);
    EXPECT_THAT(venue.getTnsQtyEnabledFlag(), Optional(Eq(false)));
}

TEST(DataLayer_Model_Venue, Get_TnsSideEnabledFlag_Missing)
{
    Venue::Patch patch{};
    patch.withVenueId("LSE");

    Venue const venue = Venue::create(patch);
    EXPECT_EQ(venue.getTnsSideEnabledFlag(), std::nullopt);
}

TEST(DataLayer_Model_Venue, Get_TnsSideEnabledFlag_Specified)
{
    Venue::Patch patch{};
    patch.withVenueId("LSE");
    patch.withTnsSideEnabledFlag(false);

    Venue const venue = Venue::create(patch);
    EXPECT_THAT(venue.getTnsSideEnabledFlag(), Optional(Eq(false)));
}

TEST(DataLayer_Model_Venue, Get_TnsPartiesEnabledFlag_Missing)
{
    Venue::Patch patch{};
    patch.withVenueId("LSE");

    Venue const venue = Venue::create(patch);
    EXPECT_EQ(venue.getTnsPartiesEnabledFlag(), std::nullopt);
}

TEST(DataLayer_Model_Venue, Get_TnsPartiesEnabledFlag_Specified)
{
    Venue::Patch patch{};
    patch.withVenueId("LSE");
    patch.withTnsPartiesEnabledFlag(false);

    Venue const venue = Venue::create(patch);
    EXPECT_THAT(venue.getTnsPartiesEnabledFlag(), Optional(Eq(false)));
}

TEST(DataLayer_Model_Venue, Get_Timezone_Missing)
{
    Venue::Patch patch{};
    patch.withVenueId("LSE");

    Venue const venue = Venue::create(patch);
    EXPECT_EQ(venue.getTimezone(), std::nullopt);
}

TEST(DataLayer_Model_Venue, Get_Timezone_Specified)
{
    Venue::Patch patch{};
    patch.withVenueId("LSE");
    patch.withTimezone("GMT+3");

    Venue const venue = Venue::create(patch);
    EXPECT_THAT(venue.getTimezone(), Optional(Eq("GMT+3")));
}

TEST(DataLayer_Model_Venue, Get_CancelOnDisconnectFlag_Missing)
{
    Venue::Patch patch{};
    patch.withVenueId("LSE");
    ASSERT_FALSE(patch.getCancelOnDisconnectFlag().has_value());

    Venue const venue = Venue::create(patch);
    EXPECT_EQ(venue.getCancelOnDisconnectFlag(), std::nullopt);
}

TEST(DataLayer_Model_Venue, Get_CancelOnDisconnectFlag_Specified)
{
    Venue::Patch patch{};
    patch.withVenueId("LSE");
    patch.withCancelOnDisconnectFlag(false);

    Venue const venue = Venue::create(patch);
    EXPECT_THAT(venue.getCancelOnDisconnectFlag(), Optional(Eq(false)));
}

TEST(DataLayer_Model_Venue, Get_PersistenceEnabledFlag_Missing)
{
    Venue::Patch patch{};
    patch.withVenueId("LSE");

    Venue const venue = Venue::create(patch);
    EXPECT_EQ(venue.getPersistenceEnabledFlag(), std::nullopt);
}

TEST(DataLayer_Model_Venue, Get_PersistenceEnabledFlag_Specified)
{
    Venue::Patch patch{};
    patch.withVenueId("LSE");
    patch.withPersistenceEnabledFlag(true);

    Venue const venue = Venue::create(patch);
    EXPECT_THAT(venue.getPersistenceEnabledFlag(), Optional(Eq(true)));
}

TEST(DataLayer_Model_Venue, Get_PersistenceFilePath_Missing)
{
    Venue::Patch patch{};
    patch.withVenueId("LSE");

    Venue const venue = Venue::create(patch);
    EXPECT_EQ(venue.getPersistenceFilePath(), std::nullopt);
}

TEST(DataLayer_Model_Venue, Get_PersistenceFilePath_Specified)
{
    Venue::Patch patch{};
    patch.withVenueId("LSE");
    patch.withPersistenceFilePath("/rw/state.json");

    Venue const venue = Venue::create(patch);
    EXPECT_THAT(venue.getPersistenceFilePath(), Optional(Eq("/rw/state.json")));
}

TEST(DataLayer_Model_Venue, Get_MarketPhases_Missing)
{
    Venue::Patch patch{};
    patch.withVenueId("LSE");
    ASSERT_FALSE(patch.getMarketPhases().has_value());

    Venue const venue = Venue::create(patch);
    EXPECT_TRUE(venue.getMarketPhases().empty());
}

TEST(DataLayer_Model_Venue, Get_MarketPhases_Empty)
{
    Venue::Patch patch{};
    patch.withVenueId("LSE");
    patch.withoutMarketPhases();
    ASSERT_TRUE(patch.getMarketPhases().has_value());
    ASSERT_TRUE(patch.getMarketPhases()->empty()); // NOLINT: check is upper

    Venue const venue = Venue::create(patch);
    EXPECT_TRUE(venue.getMarketPhases().empty());
}

TEST(DataLayer_Model_Venue, Get_MarketPhases_Specified)
{
    MarketPhase::Patch phasePatch{};
    phasePatch.with_phase(MarketPhase::Phase::Closed)
        .with_start_time("20:00:00")
        .with_end_time("24:00:00");

    Venue::Patch patch{};
    patch.withVenueId("FASTMATCH").withMarketPhase(phasePatch);
    ASSERT_TRUE(patch.getMarketPhases().has_value());
    ASSERT_EQ(patch.getMarketPhases()->size(), 1); // NOLINT: check is upper

    Venue const venue = Venue::create(patch);
    ASSERT_EQ(venue.getMarketPhases().size(), 1);
    EXPECT_EQ(venue.getMarketPhases().front().venue_id(), "FASTMATCH");
}
