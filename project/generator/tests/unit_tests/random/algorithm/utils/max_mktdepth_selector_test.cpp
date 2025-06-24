#include "ih/random/algorithm/utils/max_mktdepth_selector.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <optional>

#include "data_layer/api/models/listing.hpp"

namespace Simulator::Generator::Random {
namespace {

using namespace ::testing;  // NOLINT

struct GeneratorRandomMaxMktDepthSelector : public testing::Test {
  DataLayer::Listing::Patch patch;
  static constexpr std::uint64_t listing_id = 42;

  GeneratorRandomMaxMktDepthSelector() {
    patch.withVenueId("Venue");
  }
};

TEST_F(GeneratorRandomMaxMktDepthSelector,
       DoesNotSelectAbsentRandomDepthLevels) {
  constexpr std::uint32_t parties_count = 20;
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  const auto max_mkt_depth =
      MaxMktDepthSelector::select(listing, parties_count);
  EXPECT_FALSE(max_mkt_depth.has_value());
}

TEST_F(GeneratorRandomMaxMktDepthSelector,
       SelectsRandomDepthLevelsIfItIsLessThenPartiesCount) {
  constexpr std::uint32_t parties_count = 20;
  constexpr std::uint32_t random_depth_levels = 19;

  patch.withRandomDepthLevels(random_depth_levels);
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  const auto max_mkt_depth =
      MaxMktDepthSelector::select(listing, parties_count);
  EXPECT_THAT(max_mkt_depth, Optional(Eq(random_depth_levels)));
}

TEST_F(GeneratorRandomMaxMktDepthSelector,
       SelectsPartiesCountIfItIsLessThenRandomDepthLevels) {
  constexpr std::uint32_t parties_count = 18;
  constexpr std::uint32_t random_depth_levels = 19;

  patch.withRandomDepthLevels(random_depth_levels);
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  const auto max_mkt_depth =
      MaxMktDepthSelector::select(listing, parties_count);
  EXPECT_THAT(max_mkt_depth, Optional(Eq(parties_count)));
}

TEST_F(GeneratorRandomMaxMktDepthSelector,
       SelectsPartiesCountEqualRandomDepthLevels) {
  constexpr std::uint32_t partiesCount = 19;
  constexpr std::uint32_t randomDepthLevels = 19;

  patch.withRandomDepthLevels(randomDepthLevels);
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  const auto max_mkt_depth = MaxMktDepthSelector::select(listing, partiesCount);
  EXPECT_THAT(max_mkt_depth, Optional(Eq(randomDepthLevels)));
}

}  // namespace
}  // namespace Simulator::Generator::Random