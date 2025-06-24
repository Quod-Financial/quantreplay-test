#include "ih/random/algorithm/utils/price_params_selector.hpp"

#include <gtest/gtest.h>

#include "data_layer/api/models/listing.hpp"
#include "ih/constants.hpp"
#include "ih/random/values/price_generation_params.hpp"

namespace Simulator::Generator::Random {
namespace {

struct GeneratorRandomPriceParamsSelector : public ::testing::Test {
  DataLayer::Listing::Patch patch;
  static constexpr std::uint64_t listing_id = 42;

  GeneratorRandomPriceParamsSelector() { patch.withVenueId("Venue"); }
};

TEST_F(GeneratorRandomPriceParamsSelector,
       SelectsDefaultPriceTickRangeIfItIsMissing) {
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  const auto params = PriceParamsSelector::select(listing);
  EXPECT_EQ(params.getPriceTickRange(), Constant::DefaultListingTickRange);
}

TEST_F(GeneratorRandomPriceParamsSelector, SelectsPriceTickRange) {
  constexpr std::uint32_t price_tick_range = 120;
  patch.withRandomTickRange(price_tick_range);
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  const auto params = PriceParamsSelector::select(listing);
  EXPECT_EQ(params.getPriceTickRange(), price_tick_range);
}

TEST_F(GeneratorRandomPriceParamsSelector,
       SelectsDefaultPriceTickSizeIfItIsMissing) {
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  const auto params = PriceParamsSelector::select(listing);
  EXPECT_DOUBLE_EQ(params.getPriceTickSize(),
                   Constant::DefaultListingPriceTickSize);
}

TEST_F(GeneratorRandomPriceParamsSelector, SelectsPriceTickSize) {
  constexpr double tick_size = 12.34;
  patch.withPriceTickSize(tick_size);
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  const auto params = PriceParamsSelector::select(listing);
  EXPECT_DOUBLE_EQ(params.getPriceTickSize(), tick_size);
}

TEST_F(GeneratorRandomPriceParamsSelector,
       SelectsDefaultPriceTickSizeIfPriceSpreadIsMissing) {
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  PriceGenerationParams const params = PriceParamsSelector::select(listing);
  EXPECT_DOUBLE_EQ(params.getPriceSpread(),
                   Constant::DefaultListingPriceTickSize);
}

TEST_F(GeneratorRandomPriceParamsSelector,
       SelectsPriceTickSizeIfPriceSpreadIsMissing) {
  constexpr double tick_size = 12.34;
  patch.withPriceTickSize(tick_size);
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  PriceGenerationParams const params = PriceParamsSelector::select(listing);
  EXPECT_DOUBLE_EQ(params.getPriceSpread(), tick_size);
}

TEST_F(GeneratorRandomPriceParamsSelector, SelectsPriceSpread) {
  constexpr double price_spread = 0.15;
  patch.withRandomOrdersSpread(price_spread);
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  PriceGenerationParams const params = PriceParamsSelector::select(listing);
  EXPECT_DOUBLE_EQ(params.getPriceSpread(), price_spread);
}

}  // namespace
}  // namespace Simulator::Generator::Random