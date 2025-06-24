#include <gtest/gtest.h>

#include "data_layer/api/models/venue.hpp"
#include "ih/config/config.hpp"
#include "ih/config/venue_entry_reader.hpp"

namespace simulator::trading_system::test {
namespace {

using namespace testing;  // NOLINT

namespace data = Simulator::DataLayer;

struct TradingSystemVenueEntryReader : public Test {
  auto SetUp() -> void override { patch.withVenueId("LSE"); }

  Config config;
  VenueEntryReader reader{config};

  data::Venue::Patch patch;
};

TEST_F(TradingSystemVenueEntryReader, SetsSupportDayOrdersByDefaultTrue) {
  reader(data::Venue::create(patch));
  ASSERT_TRUE(config.support_day_orders_enabled());
}

TEST_F(TradingSystemVenueEntryReader, SetsSupportDayOrdersFromVenue) {
  patch.withSupportTifDayFlag(false);
  reader(data::Venue::create(patch));
  ASSERT_FALSE(config.support_day_orders_enabled());
}

TEST_F(TradingSystemVenueEntryReader, SetsSupportIocOrdersByDefaultTrue) {
  reader(data::Venue::create(patch));
  ASSERT_TRUE(config.support_ioc_orders_enabled());
}

TEST_F(TradingSystemVenueEntryReader, SetsSupportIocOrdersFromVenue) {
  patch.withSupportTifIocFlag(false);
  reader(data::Venue::create(patch));
  ASSERT_FALSE(config.support_ioc_orders_enabled());
}

TEST_F(TradingSystemVenueEntryReader, SetsSupportFokOrdersByDefaultTrue) {
  reader(data::Venue::create(patch));
  ASSERT_TRUE(config.support_fok_orders_enabled());
}

TEST_F(TradingSystemVenueEntryReader, SetsSupportFokOrdersFromVenue) {
  patch.withSupportTifFokFlag(false);
  reader(data::Venue::create(patch));
  ASSERT_FALSE(config.support_fok_orders_enabled());
}

TEST_F(TradingSystemVenueEntryReader, SetsCancelOnDisconnectByDefaultFalse) {
  reader(data::Venue::create(patch));
  ASSERT_FALSE(config.cod_enabled());
}

TEST_F(TradingSystemVenueEntryReader, SetsCancelOnDisconnectFromVenue) {
  patch.withCancelOnDisconnectFlag(true);
  reader(data::Venue::create(patch));
  ASSERT_TRUE(config.cod_enabled());
}

TEST_F(TradingSystemVenueEntryReader, SetsTradeStreamingByDefaultFalse) {
  reader(data::Venue::create(patch));
  ASSERT_FALSE(config.trade_streaming_enabled());
}

TEST_F(TradingSystemVenueEntryReader, SetsTradeStreamingFromVenue) {
  patch.withTnsEnabledFlag(true);
  reader(data::Venue::create(patch));
  ASSERT_TRUE(config.trade_streaming_enabled());
}

TEST_F(TradingSystemVenueEntryReader, SetsTradeVolumeStreamingByDefaultFalse) {
  reader(data::Venue::create(patch));
  ASSERT_FALSE(config.trade_volume_streaming_enabled());
}

TEST_F(TradingSystemVenueEntryReader, SetsTradeVolumeStreamingFromVenue) {
  patch.withTnsQtyEnabledFlag(true);
  reader(data::Venue::create(patch));
  ASSERT_TRUE(config.trade_volume_streaming_enabled());
}

TEST_F(TradingSystemVenueEntryReader, SetsTradePartiesStreamingByDefaultFalse) {
  reader(data::Venue::create(patch));
  ASSERT_FALSE(config.trade_parties_streaming_enabled());
}

TEST_F(TradingSystemVenueEntryReader, SetsTradePartiesStreamingFromVenue) {
  patch.withTnsPartiesEnabledFlag(true);
  reader(data::Venue::create(patch));
  ASSERT_TRUE(config.trade_parties_streaming_enabled());
}

TEST_F(TradingSystemVenueEntryReader,
       SetsTradeAggressorStreamingByDefaultFalse) {
  reader(data::Venue::create(patch));
  ASSERT_FALSE(config.trade_aggressor_streaming_enabled());
}

TEST_F(TradingSystemVenueEntryReader, SetsTradeAggressorStreamingFromVenue) {
  patch.withTnsSideEnabledFlag(true);
  reader(data::Venue::create(patch));
  ASSERT_TRUE(config.trade_aggressor_streaming_enabled());
}

TEST_F(TradingSystemVenueEntryReader, SetsDepthOrdersExclusionByDefaultFalse) {
  reader(data::Venue::create(patch));
  ASSERT_FALSE(config.depth_orders_exclusion_enabled());
}

TEST_F(TradingSystemVenueEntryReader,
       SetsDepthOrdersExclusionFromVenueAsInvertedIncludeOwnOrders) {
  patch.withIncludeOwnOrdersFlag(false);
  reader(data::Venue::create(patch));
  ASSERT_TRUE(config.depth_orders_exclusion_enabled());
}

TEST_F(TradingSystemVenueEntryReader, SetsPersistenceByDefaultFalse) {
  reader(data::Venue::create(patch));
  ASSERT_FALSE(config.persistence_enabled());
}

TEST_F(TradingSystemVenueEntryReader, SetsPersistenceFromVenue) {
  patch.withPersistenceEnabledFlag(true);
  reader(data::Venue::create(patch));
  ASSERT_TRUE(config.persistence_enabled());
}

TEST_F(TradingSystemVenueEntryReader, LeavesPersistenceFilePathByDefaultEmpty) {
  reader(data::Venue::create(patch));
  ASSERT_TRUE(config.persistence_file_path().empty());
}

TEST_F(TradingSystemVenueEntryReader, SetsPersistenceFilePathFromVenueAsEmpty) {
  patch.withPersistenceFilePath({});
  reader(data::Venue::create(patch));
  ASSERT_TRUE(config.persistence_file_path().empty());
}

TEST_F(TradingSystemVenueEntryReader, SetsPersistenceFilePathFromVenue) {
  patch.withPersistenceFilePath("persistence_path");
  reader(data::Venue::create(patch));
  ASSERT_EQ(config.persistence_file_path(), "persistence_path");
}

TEST_F(TradingSystemVenueEntryReader, LeavesTzClockDefaultConstructableByDefault) {
  reader(data::Venue::create(patch));
  ASSERT_EQ(config.timezone_clock(), core::TzClock{});
}

TEST_F(TradingSystemVenueEntryReader, SetsTzClockWithTimezoneFromVenue) {
  patch.withTimezone("Europe/Kyiv");
  reader(data::Venue::create(patch));
  ASSERT_EQ(config.timezone_clock(), core::TzClock{"Europe/Kyiv"});
}

TEST_F(TradingSystemVenueEntryReader, SetsTzClockWithDefaultTimezone) {
  patch.withTimezone("UTC");
  reader(data::Venue::create(patch));
  ASSERT_EQ(config.timezone_clock(), core::TzClock{"UTC"});
}

}  // namespace
}  // namespace simulator::trading_system::test
