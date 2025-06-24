#include "ih/utils/validator.hpp"

#include <gtest/gtest.h>

#include <string>

#include "data_layer/api/models/price_seed.hpp"

namespace Simulator::Generator {
namespace {

// NOLINTBEGIN(*magic-numbers*)

class GeneratorValidator : public testing::Test {
 public:
  static constexpr std::uint64_t listingID = 11;
  DataLayer::Listing::Patch listing_patch;
  DataLayer::PriceSeed::Patch price_seed_patch;

  GeneratorValidator() {
    listing_patch.withVenueId("Venue");
    price_seed_patch.withSymbol("AAPL");
  }
};

TEST_F(GeneratorValidator, Listing_IsAcceptable_SymbolAbsent) {
  const auto listing = DataLayer::Listing::create(listing_patch, listingID);
  ASSERT_FALSE(listing.getSymbol());

  EXPECT_FALSE(Validator::isAcceptable(listing));
}

TEST_F(GeneratorValidator, Listing_IsAcceptable_SymbolIsEmpty) {
  listing_patch.withSymbol({});
  const auto listing = DataLayer::Listing::create(listing_patch, listingID);
  ASSERT_TRUE(listing.getSymbol());

  EXPECT_FALSE(Validator::isAcceptable(listing));
}

TEST_F(GeneratorValidator, Listing_IsAcceptable_ListingDisabled) {
  listing_patch.withEnabledFlag(false);
  const auto listing = DataLayer::Listing::create(listing_patch, listingID);

  EXPECT_FALSE(Validator::isAcceptable(listing));
}

TEST_F(GeneratorValidator, Listing_IsAcceptable_AcceptableListing) {
  listing_patch.withEnabledFlag(true).withSymbol("AAPL");
  const auto listing = DataLayer::Listing::create(listing_patch, listingID);

  EXPECT_TRUE(Validator::isAcceptable(listing));
}

TEST_F(GeneratorValidator, Listing_IsAcceptableForGeneration_SymbolAbsent) {
  listing_patch.withEnabledFlag(true);
  const auto listing = DataLayer::Listing::create(listing_patch, listingID);
  ASSERT_FALSE(listing.getSymbol());

  EXPECT_FALSE(Validator::isAcceptableForRandomGeneration(listing));
}

TEST_F(GeneratorValidator, Listing_IsAcceptableForGeneration_SymbolIsEmpty) {
  listing_patch.withEnabledFlag(true).withSymbol({});
  const auto listing = DataLayer::Listing::create(listing_patch, listingID);

  EXPECT_FALSE(Validator::isAcceptableForRandomGeneration(listing));
}

TEST_F(GeneratorValidator, Listing_IsAcceptableForGeneration_ListingDisabled) {
  listing_patch.withEnabledFlag(false);
  const auto listing = DataLayer::Listing::create(listing_patch, listingID);

  EXPECT_FALSE(Validator::isAcceptableForRandomGeneration(listing));
}

TEST_F(GeneratorValidator,
       Listing_IsAcceptableForGeneration_RandomOrdersDisabled) {
  listing_patch.withEnabledFlag(true)
      .withSymbol("AAPL")
      .withRandomOrdersEnabledFlag(false);
  const auto listing = DataLayer::Listing::create(listing_patch, listingID);

  EXPECT_FALSE(Validator::isAcceptableForRandomGeneration(listing));
}

TEST_F(GeneratorValidator,
       Listing_IsAcceptableForGeneration_RandomOrdersRate_Zero) {
  listing_patch.withEnabledFlag(true)
      .withSymbol("AAPL")
      .withRandomOrdersEnabledFlag(true)
      .withRandomOrdersRate(0);
  const auto listing = DataLayer::Listing::create(listing_patch, listingID);

  EXPECT_FALSE(Validator::isAcceptableForRandomGeneration(listing));
}

TEST_F(GeneratorValidator,
       Listing_IsAcceptableForGeneration_AcceptableListing) {
  listing_patch.withEnabledFlag(true)
      .withSymbol("AAPL")
      .withRandomOrdersEnabledFlag(true)
      .withRandomOrdersRate(1);
  const auto listing = DataLayer::Listing::create(listing_patch, listingID);

  EXPECT_TRUE(Validator::isAcceptableForRandomGeneration(listing));
}

TEST_F(GeneratorValidator, PriceSeed_IsAcceptable_PricesAbsent) {
  const auto price_seed = DataLayer::PriceSeed::create(price_seed_patch, 1);
  ASSERT_FALSE(price_seed.getBidPrice().has_value());
  ASSERT_FALSE(price_seed.getOfferPrice().has_value());
  ASSERT_FALSE(price_seed.getMidPrice().has_value());

  EXPECT_FALSE(Validator::isAcceptable(price_seed));
}

TEST_F(GeneratorValidator, PriceSeed_IsAcceptable_BidPriceOnly) {
  price_seed_patch.withBidPrice(118.5);
  const auto price_seed = DataLayer::PriceSeed::create(price_seed_patch, 1);

  ASSERT_TRUE(price_seed.getBidPrice().has_value());
  ASSERT_FALSE(price_seed.getOfferPrice().has_value());
  ASSERT_FALSE(price_seed.getMidPrice().has_value());

  EXPECT_FALSE(Validator::isAcceptable(price_seed));
}

TEST_F(GeneratorValidator, PriceSeed_IsAcceptable_OfferPriceOnly) {
  price_seed_patch.withOfferPrice(122.5);
  const auto price_seed = DataLayer::PriceSeed::create(price_seed_patch, 1);

  ASSERT_FALSE(price_seed.getBidPrice().has_value());
  ASSERT_TRUE(price_seed.getOfferPrice().has_value());
  ASSERT_FALSE(price_seed.getMidPrice().has_value());

  EXPECT_FALSE(Validator::isAcceptable(price_seed));
}

TEST_F(GeneratorValidator, PriceSeed_IsAcceptable_MidPriceOnly) {
  price_seed_patch.withMidPrice(120.5);
  const auto price_seed = DataLayer::PriceSeed::create(price_seed_patch, 1);

  ASSERT_FALSE(price_seed.getBidPrice().has_value());
  ASSERT_FALSE(price_seed.getOfferPrice().has_value());
  ASSERT_TRUE(price_seed.getMidPrice().has_value());

  EXPECT_TRUE(Validator::isAcceptable(price_seed));
}

TEST_F(GeneratorValidator, PriceSeed_IsAcceptable_BidAndMidPrices) {
  price_seed_patch.withBidPrice(118.5).withMidPrice(120.5);
  const auto price_seed = DataLayer::PriceSeed::create(price_seed_patch, 1);

  ASSERT_TRUE(price_seed.getBidPrice().has_value());
  ASSERT_FALSE(price_seed.getOfferPrice().has_value());
  ASSERT_TRUE(price_seed.getMidPrice().has_value());

  EXPECT_TRUE(Validator::isAcceptable(price_seed));
}

TEST_F(GeneratorValidator, PriceSeed_IsAcceptable_OfferAndMidPrices) {
  price_seed_patch.withMidPrice(120.5).withOfferPrice(122.5);
  const auto price_seed = DataLayer::PriceSeed::create(price_seed_patch, 1);

  ASSERT_FALSE(price_seed.getBidPrice().has_value());
  ASSERT_TRUE(price_seed.getOfferPrice().has_value());
  ASSERT_TRUE(price_seed.getMidPrice().has_value());

  EXPECT_TRUE(Validator::isAcceptable(price_seed));
}

TEST_F(GeneratorValidator, PriceSeed_IsAcceptable_BidAndOfferPrices) {
  price_seed_patch.withBidPrice(118.5).withOfferPrice(122.5);
  const auto price_seed = DataLayer::PriceSeed::create(price_seed_patch, 1);

  ASSERT_TRUE(price_seed.getBidPrice().has_value());
  ASSERT_TRUE(price_seed.getOfferPrice().has_value());
  ASSERT_FALSE(price_seed.getMidPrice().has_value());

  EXPECT_TRUE(Validator::isAcceptable(price_seed));
}

TEST_F(GeneratorValidator, PriceSeed_IsAcceptable_AllPrices) {
  price_seed_patch.withBidPrice(118.5).withMidPrice(120.5).withOfferPrice(
      122.5);
  const auto price_seed = DataLayer::PriceSeed::create(price_seed_patch, 1);

  ASSERT_TRUE(price_seed.getBidPrice().has_value());
  ASSERT_TRUE(price_seed.getOfferPrice().has_value());
  ASSERT_TRUE(price_seed.getMidPrice().has_value());

  EXPECT_TRUE(Validator::isAcceptable(price_seed));
}

// NOLINTEND(*magic-numbers*)

}  // namespace
}  // namespace Simulator::Generator