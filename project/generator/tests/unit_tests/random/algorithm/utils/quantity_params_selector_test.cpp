#include "ih/random/algorithm/utils/quantity_params_selector.hpp"

#include <gtest/gtest.h>

#include "data_layer/api/models/listing.hpp"
#include "ih/constants.hpp"
#include "ih/random/values/event.hpp"
#include "ih/random/values/quantity_generation_params.hpp"

namespace Simulator::Generator::Random {
namespace {

struct GeneratorRandomQuantityParamsSelector : public ::testing::Test {
  DataLayer::Listing::Patch patch;
  static constexpr std::uint64_t listing_id = 42;
  static constexpr double price = 3.0;
  static inline const Event resting_event{
      static_cast<Event::RandomInteger>(Event::Type::RestingBuy)};
  static inline const Event aggressive_event{
      static_cast<Event::RandomInteger>(Event::Type::AggressiveBuy)};

  GeneratorRandomQuantityParamsSelector() { patch.withVenueId("Venue"); }
};

TEST_F(GeneratorRandomQuantityParamsSelector,
       SelectsDefaultMultiplierIfItIsMissing) {
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  QuantityParamsSelector selector{listing};
  const auto params = selector.select(price, resting_event);

  EXPECT_DOUBLE_EQ(params.getMultiplier(), Constant::DefaultListingQtyMultiple);
}

TEST_F(GeneratorRandomQuantityParamsSelector, SelectsMultiplier) {
  constexpr double multiplier = 0.15;
  patch.withQtyMultiple(multiplier);

  const auto listing = DataLayer::Listing::create(patch, listing_id);

  QuantityParamsSelector selector{listing};
  const auto params = selector.select(price, resting_event);

  EXPECT_DOUBLE_EQ(params.getMultiplier(), multiplier);
}

TEST_F(GeneratorRandomQuantityParamsSelector,
       SelectsDefaultListingMinQtyAsMinimalQtyIfItIsMissing) {
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  QuantityParamsSelector selector{listing};
  const auto params = selector.select(price, resting_event);

  EXPECT_DOUBLE_EQ(params.getMinimalQuantity(),
                   Constant::DefaultListingQtyMinimum);
}

TEST_F(GeneratorRandomQuantityParamsSelector,
       SelectsListingMinQtyAsMinimalQty) {
  constexpr double listing_min_qty = 1000.998;
  constexpr double listing_max_qty = 1000.999;
  patch.withQtyMinimum(listing_min_qty).withQtyMaximum(listing_max_qty);
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  QuantityParamsSelector selector{listing};
  const auto params = selector.select(price, resting_event);

  EXPECT_DOUBLE_EQ(params.getMinimalQuantity(), listing_min_qty);
}

TEST_F(GeneratorRandomQuantityParamsSelector,
       SelectsRandomMinQtyAsMinimalQtyIfItIsGreaterThanListingMinQty) {
  constexpr double listing_min_qty = 1000.444;
  constexpr double random_min_qty = 1000.445;
  constexpr double listing_max_qty = 1000.999;
  patch.withQtyMinimum(listing_min_qty)
      .withRandomQtyMinimum(random_min_qty)
      .withQtyMaximum(listing_max_qty);
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  QuantityParamsSelector selector{listing};
  const auto params = selector.select(price, resting_event);

  EXPECT_DOUBLE_EQ(params.getMinimalQuantity(), random_min_qty);
}

TEST_F(GeneratorRandomQuantityParamsSelector,
       SelectsListingMinQtyAsMinimalQtyIfItIsGreaterThanRandomMinQty) {
  constexpr double listing_min_qty = 1000.444;
  constexpr double random_min_qty = 1000.0;
  constexpr double listing_max_qty = 1000.999;
  patch.withQtyMinimum(listing_min_qty)
      .withRandomQtyMinimum(random_min_qty)
      .withQtyMaximum(listing_max_qty);
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  QuantityParamsSelector selector{listing};
  const auto params = selector.select(price, resting_event);

  EXPECT_DOUBLE_EQ(params.getMinimalQuantity(), listing_min_qty);
}

TEST_F(
    GeneratorRandomQuantityParamsSelector,
    SelectsValueFromRandomAmtMinimumAsMinimalQtyIfPriceIsNotZeroAndItIsGreaterThanListingMinQty) {
  constexpr double non_zero_price = 3.0;
  constexpr double random_min_amount_qty = 12000.0;
  constexpr double listing_min_qty = 1000.998;
  constexpr double listing_max_qty = 5000.0;
  constexpr double random_min_qty = 1000.999;

  patch.withQtyMinimum(listing_min_qty)
      .withQtyMaximum(listing_max_qty)
      .withRandomQtyMinimum(random_min_qty)
      .withRandomAmtMinimum(random_min_amount_qty);
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  QuantityParamsSelector selector{listing};
  const auto params = selector.select(non_zero_price, resting_event);

  constexpr double expected_min_qty = random_min_amount_qty / non_zero_price;
  EXPECT_DOUBLE_EQ(params.getMinimalQuantity(), expected_min_qty);
}

TEST_F(
    GeneratorRandomQuantityParamsSelector,
    SelectsListingMinQtyAsMinimalQtyIfItIsGreaterThanValueFromRandomAmtMinimum) {
  constexpr double non_zero_price = 3.0;
  constexpr double random_min_amount_qty = 3000.0;
  constexpr double listing_min_qty = 1000.998;
  constexpr double listing_max_qty = 5000.0;

  patch.withQtyMinimum(listing_min_qty)
      .withQtyMaximum(listing_max_qty)
      .withRandomAmtMinimum(random_min_amount_qty);
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  QuantityParamsSelector selector{listing};
  const auto params = selector.select(non_zero_price, resting_event);

  EXPECT_DOUBLE_EQ(params.getMinimalQuantity(), listing_min_qty);
}

TEST_F(GeneratorRandomQuantityParamsSelector,
       SelectsListingMinQtyAsMinimalQtyIfThePriceIsZero) {
  constexpr double zero_price = 0.0;
  constexpr double random_min_amount_qty = 12000.0;
  constexpr double listing_min_qty = 1000.998;
  constexpr double listing_max_qty = 5000.0;

  patch.withQtyMinimum(listing_min_qty)
      .withQtyMaximum(listing_max_qty)
      .withRandomAmtMinimum(random_min_amount_qty);
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  QuantityParamsSelector selector{listing};
  const auto params = selector.select(zero_price, resting_event);

  EXPECT_DOUBLE_EQ(params.getMinimalQuantity(), listing_min_qty);
}

TEST_F(GeneratorRandomQuantityParamsSelector,
       SelectsDefaultListingMinQtyAsMinimalQtyForAggressiveOrderIfItIsMissing) {
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  QuantityParamsSelector selector{listing};
  const auto params = selector.select(price, aggressive_event);

  EXPECT_DOUBLE_EQ(params.getMinimalQuantity(),
                   Constant::DefaultListingQtyMinimum);
}

TEST_F(GeneratorRandomQuantityParamsSelector,
       SelectsListingMinQtyAsMinimalQtyIfItIsMissing) {
  constexpr double listing_min_qty = 1000.998;
  constexpr double listing_max_qty = 1000.999;
  patch.withQtyMinimum(listing_min_qty).withQtyMaximum(listing_max_qty);
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  QuantityParamsSelector selector{listing};
  const auto params = selector.select(price, aggressive_event);

  EXPECT_DOUBLE_EQ(params.getMinimalQuantity(), listing_min_qty);
}

TEST_F(GeneratorRandomQuantityParamsSelector,
       SelectsRandomAggressiveQtyMinAsMinimalQty) {
  constexpr double random_aggressive_qty_min = 1000.997;
  constexpr double listing_max_qty = 1000.999;
  patch.withRandomAggressiveQtyMinimum(random_aggressive_qty_min)
      .withQtyMaximum(listing_max_qty);
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  QuantityParamsSelector selector{listing};
  const auto params = selector.select(price, aggressive_event);

  EXPECT_DOUBLE_EQ(params.getMinimalQuantity(), random_aggressive_qty_min);
}

TEST_F(
    GeneratorRandomQuantityParamsSelector,
    SelectsListingMinQtyAsMinimalQtyIfItIsGreaterThanRandomAggressiveMinQty) {
  constexpr double listing_min_qty = 1000.998;
  constexpr double random_aggressive_min_qty = 1000.0;
  constexpr double listing_max_qty = 1000.999;
  patch.withQtyMinimum(listing_min_qty)
      .withRandomAggressiveQtyMinimum(random_aggressive_min_qty)
      .withQtyMaximum(listing_max_qty);
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  QuantityParamsSelector selector{listing};
  const auto params = selector.select(price, aggressive_event);

  EXPECT_DOUBLE_EQ(params.getMinimalQuantity(), listing_min_qty);
}

TEST_F(
    GeneratorRandomQuantityParamsSelector,
    SelectsValueFromRandomAggressiveAmtMinimumAsMinimalQtyIfPriceIsNotZeroAndItIsGreaterThanListingMinQty) {
  constexpr double non_zero_price = 3.0;
  constexpr double listing_min_qty = 1000;
  constexpr double random_aggressive_min_amount_qty = 12000.0;
  constexpr double listing_max_qty = 5000.0;

  patch.withQtyMinimum(listing_min_qty)
      .withQtyMaximum(listing_max_qty)
      .withRandomAggressiveAmtMinimum(random_aggressive_min_amount_qty);
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  QuantityParamsSelector selector{listing};
  const auto params = selector.select(non_zero_price, aggressive_event);

  constexpr double expected_min_qty =
      random_aggressive_min_amount_qty / non_zero_price;
  EXPECT_DOUBLE_EQ(params.getMinimalQuantity(), expected_min_qty);
}

TEST_F(
    GeneratorRandomQuantityParamsSelector,
    SelectsValueFromRandomAggressiveAmtMinimumAsMinimalQtyIfPriceIsNotZeroAndItIsGreaterThanRandomAggressiveMinQty) {
  constexpr double non_zero_price = 3.0;
  constexpr double random_aggressive_min_qty = 1000.444;
  constexpr double random_aggressive_min_amount_qty = 12000.0;
  constexpr double listing_max_qty = 5000.0;

  patch.withQtyMaximum(listing_max_qty)
      .withRandomAggressiveQtyMinimum(random_aggressive_min_qty)
      .withRandomAggressiveAmtMinimum(random_aggressive_min_amount_qty);
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  QuantityParamsSelector selector{listing};
  const auto params = selector.select(non_zero_price, aggressive_event);

  constexpr double expected_min_qty =
      random_aggressive_min_amount_qty / non_zero_price;
  EXPECT_DOUBLE_EQ(params.getMinimalQuantity(), expected_min_qty);
}

TEST_F(
    GeneratorRandomQuantityParamsSelector,
    SelectsRandomAggressiveQtyMinimumAsMinimalQtyIfItIsGreaterThanValueFromRandomAggressiveMinAmt) {
  constexpr double non_zero_price = 3.0;
  constexpr double random_aggressive_min_qty = 4500.0;
  constexpr double random_aggressive_min_amount_qty = 12000.0;
  constexpr double listing_max_qty = 5000.0;

  patch.withQtyMaximum(listing_max_qty)
      .withRandomAggressiveQtyMinimum(random_aggressive_min_qty)
      .withRandomAggressiveAmtMinimum(random_aggressive_min_amount_qty);
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  QuantityParamsSelector selector{listing};
  const auto params = selector.select(non_zero_price, aggressive_event);

  EXPECT_DOUBLE_EQ(params.getMinimalQuantity(), random_aggressive_min_qty);
}

TEST_F(GeneratorRandomQuantityParamsSelector,
       SelectsListingMinQtyAsMinimalQtyForAggressiveOrdersIfThePriceIsZero) {
  constexpr double zero_price = 0.0;
  constexpr double random_aggressive_min_qty = 4500.0;
  constexpr double random_aggressive_min_amount_qty = 12000.0;
  constexpr double listing_max_qty = 5000.0;

  patch.withQtyMaximum(listing_max_qty)
      .withRandomAggressiveQtyMinimum(random_aggressive_min_qty)
      .withRandomAggressiveAmtMinimum(random_aggressive_min_amount_qty);
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  QuantityParamsSelector selector{listing};
  const auto params = selector.select(zero_price, aggressive_event);

  EXPECT_DOUBLE_EQ(params.getMinimalQuantity(), random_aggressive_min_qty);
}

TEST_F(
    GeneratorRandomQuantityParamsSelector,
    SelectsValueFromRandomAmtMinimumAsMinimalQtyIfAggressiveParametersAreMissing) {
  constexpr double non_zero_price = 3.0;
  constexpr double random_min_amount_qty = 12000.0;
  constexpr double listing_min_qty = 1000.998;
  constexpr double listing_max_qty = 5000.0;
  constexpr double random_min_qty = 1000.999;

  patch.withQtyMinimum(listing_min_qty)
      .withQtyMaximum(listing_max_qty)
      .withRandomQtyMinimum(random_min_qty)
      .withRandomAmtMinimum(random_min_amount_qty);
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  QuantityParamsSelector selector{listing};
  const auto params = selector.select(non_zero_price, aggressive_event);

  constexpr double expected_min_qty = random_min_amount_qty / non_zero_price;
  EXPECT_DOUBLE_EQ(params.getMinimalQuantity(), expected_min_qty);
}

TEST_F(GeneratorRandomQuantityParamsSelector,
       SelectsDefaultListingMaxQtyAsMaximalQtyIfItIsMissing) {
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  QuantityParamsSelector selector{listing};
  const auto params = selector.select(price, resting_event);

  EXPECT_DOUBLE_EQ(params.getMaximalQuantity(),
                   Constant::DefaultListingQtyMaximum);
}

TEST_F(GeneratorRandomQuantityParamsSelector,
       SelectsListingMaxQtyAsMaximalRestingQty) {
  constexpr double listing_max_qty = 1200.234;
  patch.withQtyMaximum(listing_max_qty);
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  QuantityParamsSelector selector{listing};
  const auto params = selector.select(price, resting_event);

  EXPECT_DOUBLE_EQ(params.getMaximalQuantity(), listing_max_qty);
}

TEST_F(GeneratorRandomQuantityParamsSelector,
       SelectsListingMaxQtyAsMaximalQtyIfItIsLessThanRandomMaxQty) {
  constexpr double listing_max_qty = 1200.234;
  constexpr double random_max_qty = 1200.235;
  patch.withQtyMaximum(listing_max_qty).withRandomQtyMaximum(random_max_qty);
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  QuantityParamsSelector selector{listing};
  const auto params = selector.select(price, resting_event);

  EXPECT_DOUBLE_EQ(params.getMaximalQuantity(), listing_max_qty);
}

TEST_F(GeneratorRandomQuantityParamsSelector,
       SelectsRandomMaxQtyAsMaximalQtyIfItIsLessThanListingMinQty) {
  constexpr double listing_max_qty = 1200.234;
  constexpr double random_max_qty = 1200.233;
  patch.withQtyMaximum(listing_max_qty).withRandomQtyMaximum(random_max_qty);

  const auto listing = DataLayer::Listing::create(patch, listing_id);

  QuantityParamsSelector selector{listing};
  const auto params = selector.select(price, resting_event);

  EXPECT_DOUBLE_EQ(params.getMaximalQuantity(), random_max_qty);
}

TEST_F(
    GeneratorRandomQuantityParamsSelector,
    SelectsValueFromRandomAmtMaximumAsMaximalQtyIfPriceIsNotZeroAndItIsLessThanListingMaxQty) {
  constexpr double non_zero_price = 3.0;
  constexpr double random_max_amount_qty = 12000.0;
  constexpr double listing_max_qty = 10000.0;
  constexpr double random_max_qty = 9000.0;

  patch.withQtyMaximum(listing_max_qty)
      .withRandomQtyMaximum(random_max_qty)
      .withRandomAmtMaximum(random_max_amount_qty);
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  QuantityParamsSelector selector{listing};
  const auto params = selector.select(non_zero_price, resting_event);

  constexpr double expected_max_qty = random_max_amount_qty / non_zero_price;
  EXPECT_DOUBLE_EQ(params.getMaximalQuantity(), expected_max_qty);
}

TEST_F(GeneratorRandomQuantityParamsSelector,
       SelectsRandomMaxQtyAsMaximalQtyIfItIsLessThanValueFromRandomAmtMaximum) {
  constexpr double non_zero_price = 3.0;
  constexpr double random_max_amount_qty = 12000.0;
  constexpr double listing_max_qty = 10000.0;
  constexpr double random_max_qty = 2000.0;

  patch.withQtyMaximum(listing_max_qty)
      .withRandomQtyMaximum(random_max_qty)
      .withRandomAmtMaximum(random_max_amount_qty);
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  QuantityParamsSelector selector{listing};
  const auto params = selector.select(non_zero_price, resting_event);

  EXPECT_DOUBLE_EQ(params.getMaximalQuantity(), random_max_qty);
}

TEST_F(GeneratorRandomQuantityParamsSelector,
       SelectsRandomMaxQtyAsMaximalQtyIfPriceIsZero) {
  constexpr double zero_price = 0.0;
  constexpr double random_max_amount_qty = 12000.0;
  constexpr double listing_max_qty = 10000.0;
  constexpr double random_max_qty = 9000.0;

  patch.withQtyMaximum(listing_max_qty)
      .withRandomQtyMaximum(random_max_qty)
      .withRandomAmtMaximum(random_max_amount_qty);
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  QuantityParamsSelector selector{listing};
  const auto params = selector.select(zero_price, resting_event);

  EXPECT_DOUBLE_EQ(params.getMaximalQuantity(), random_max_qty);
}

TEST_F(
    GeneratorRandomQuantityParamsSelector,
    SelectsDefaultListingMaxQtyAsMaximalQtyForAggressiveOrdersIfItIsMissing) {
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  QuantityParamsSelector selector{listing};
  const auto params = selector.select(price, aggressive_event);

  EXPECT_DOUBLE_EQ(params.getMaximalQuantity(),
                   Constant::DefaultListingQtyMaximum);
}

TEST_F(GeneratorRandomQuantityParamsSelector,
       SelectsListingMaxQtyAsMaximalQtyIfItIsMissing) {
  constexpr double listing_min_qty = 1000.998;
  constexpr double listing_max_qty = 1000.999;
  patch.withQtyMinimum(listing_min_qty).withQtyMaximum(listing_max_qty);
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  QuantityParamsSelector selector{listing};
  const auto params = selector.select(price, aggressive_event);

  EXPECT_DOUBLE_EQ(params.getMaximalQuantity(), listing_max_qty);
}

TEST_F(GeneratorRandomQuantityParamsSelector,
       SelectsListingMaxQtyAsMaximalQtyIfItIsLessThanRandomAggressiveQtyMax) {
  constexpr double listing_min_qty = 1000.997;
  constexpr double random_aggressive_qty_max = 2500.0;
  constexpr double listing_max_qty = 2000.0;

  patch.withRandomAggressiveQtyMaximum(random_aggressive_qty_max)
      .withQtyMinimum(listing_min_qty)
      .withQtyMaximum(listing_max_qty);
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  QuantityParamsSelector selector{listing};
  const auto params = selector.select(price, aggressive_event);

  EXPECT_DOUBLE_EQ(params.getMaximalQuantity(), listing_max_qty);
}

TEST_F(GeneratorRandomQuantityParamsSelector,
       SelectsRandomAggressiveQtyMaxAsMaximalQtyIfItIsLessThanListingMaxQty) {
  constexpr double listing_min_qty = 1000.997;
  constexpr double random_aggressive_qty_max = 1500.0;
  constexpr double listing_max_qty = 2000.0;

  patch.withRandomAggressiveQtyMaximum(random_aggressive_qty_max)
      .withQtyMinimum(listing_min_qty)
      .withQtyMaximum(listing_max_qty);
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  QuantityParamsSelector selector{listing};
  const auto params = selector.select(price, aggressive_event);

  EXPECT_DOUBLE_EQ(params.getMaximalQuantity(), random_aggressive_qty_max);
}

TEST_F(
    GeneratorRandomQuantityParamsSelector,
    SelectsValueFromRandomAggressiveAmtMaximumAsMaximalQtyIfPriceIsNotZeroItIsLessThanListingMaxQty) {
  constexpr double non_zero_price = 3.0;
  constexpr double random_aggressive_max_amount_qty = 12000.0;
  constexpr double listing_min_qty = 3000.0;
  constexpr double listing_max_qty = 5000.0;

  patch.withQtyMinimum(listing_min_qty)
      .withQtyMaximum(listing_max_qty)
      .withRandomAggressiveAmtMaximum(random_aggressive_max_amount_qty);
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  QuantityParamsSelector selector{listing};
  const auto params = selector.select(non_zero_price, aggressive_event);

  constexpr double expected_max_qty =
      random_aggressive_max_amount_qty / non_zero_price;
  EXPECT_DOUBLE_EQ(params.getMaximalQuantity(), expected_max_qty);
}

TEST_F(
    GeneratorRandomQuantityParamsSelector,
    SelectsValueFromRandomAggressiveAmtMaximumAsMaximumQtyIfPriceIsNotZeroAndItIsLessThanRandomAggressiveMaxQty) {
  constexpr double non_zero_price = 3.0;
  constexpr double random_aggressive_max_qty = 5000.0;
  constexpr double random_aggressive_max_amount_qty = 12000.0;
  constexpr double listing_min_qty = 1000.0;
  constexpr double listing_max_qty = 6000.0;

  patch.withQtyMinimum(listing_min_qty)
      .withQtyMaximum(listing_max_qty)
      .withRandomAggressiveQtyMaximum(random_aggressive_max_qty)
      .withRandomAggressiveAmtMaximum(random_aggressive_max_amount_qty);
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  QuantityParamsSelector selector{listing};
  const auto params = selector.select(non_zero_price, aggressive_event);

  constexpr double expected_max_qty =
      random_aggressive_max_amount_qty / non_zero_price;
  EXPECT_DOUBLE_EQ(params.getMaximalQuantity(), expected_max_qty);
}

TEST_F(
    GeneratorRandomQuantityParamsSelector,
    SelectsRandomAggressiveQtyMaximumAsMaximumQtyIfItIsLessThanValueFromRandomAggressiveMaxAmt) {
  constexpr double non_zero_price = 3.0;
  constexpr double random_aggressive_max_qty = 3000.444;
  constexpr double random_aggressive_max_amount_qty = 12000.0;
  constexpr double listing_min_qty = 1000.0;
  constexpr double listing_max_qty = 5000.0;

  patch.withQtyMinimum(listing_min_qty)
      .withQtyMaximum(listing_max_qty)
      .withRandomAggressiveQtyMaximum(random_aggressive_max_qty)
      .withRandomAggressiveAmtMaximum(random_aggressive_max_amount_qty);
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  QuantityParamsSelector selector{listing};
  const auto params = selector.select(non_zero_price, aggressive_event);

  EXPECT_DOUBLE_EQ(params.getMaximalQuantity(), random_aggressive_max_qty);
}

TEST_F(GeneratorRandomQuantityParamsSelector,
       SelectsRandomAggressiveQtyMaximumAsMaximumQtyIfItIsPriceIsZero) {
  constexpr double zero_price = 0.0;
  constexpr double random_aggressive_max_qty = 3000.444;
  constexpr double random_aggressive_max_amount_qty = 12000.0;
  constexpr double listing_min_qty = 1000.0;
  constexpr double listing_max_qty = 5000.0;

  patch.withQtyMinimum(listing_min_qty)
      .withQtyMaximum(listing_max_qty)
      .withRandomAggressiveQtyMaximum(random_aggressive_max_qty)
      .withRandomAggressiveAmtMaximum(random_aggressive_max_amount_qty);
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  QuantityParamsSelector selector{listing};
  const auto params = selector.select(zero_price, aggressive_event);

  EXPECT_DOUBLE_EQ(params.getMaximalQuantity(), random_aggressive_max_qty);
}

TEST_F(
    GeneratorRandomQuantityParamsSelector,
    SelectsValueFromRandomAmtMaximumAsMaximalQtyIfPriceIsNotZeroAndAggressiveParametersAreMissing) {
  constexpr double non_zero_price = 3.0;
  constexpr double random_max_amount_qty = 12000.0;
  constexpr double listing_max_qty = 10000.0;
  constexpr double random_max_qty = 9000.0;

  patch.withQtyMaximum(listing_max_qty)
      .withRandomQtyMaximum(random_max_qty)
      .withRandomAmtMaximum(random_max_amount_qty);
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  QuantityParamsSelector selector{listing};
  const auto params = selector.select(non_zero_price, aggressive_event);

  constexpr double expected_max_qty = random_max_amount_qty / non_zero_price;
  EXPECT_DOUBLE_EQ(params.getMaximalQuantity(), expected_max_qty);
}

}  // namespace
}  // namespace Simulator::Generator::Random