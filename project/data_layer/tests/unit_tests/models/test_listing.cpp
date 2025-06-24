#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "api/models/listing.hpp"
#include "ih/common/exceptions.hpp"

namespace Simulator::DataLayer {
namespace {

using namespace ::testing;  // NOLINT

// NOLINTBEGIN(*magic-numbers*)

struct DataLayerListingPatch : public Test {
  Listing::Patch patch;
};

TEST_F(DataLayerListingPatch, SetsSymbol) {
  ASSERT_FALSE(patch.getSymbol().has_value());

  patch.withSymbol("AAPL");
  EXPECT_THAT(patch.getSymbol(), Optional(Eq("AAPL")));
}

TEST_F(DataLayerListingPatch, SetsVenueID) {
  ASSERT_FALSE(patch.getVenueId().has_value());

  patch.withVenueId("NASDAQ");
  EXPECT_THAT(patch.getVenueId(), Optional(Eq("NASDAQ")));
}

TEST_F(DataLayerListingPatch, SetsSecurityType) {
  ASSERT_FALSE(patch.getSecurityType().has_value());

  patch.withSecurityType("CS");
  EXPECT_THAT(patch.getSecurityType(), Optional(Eq("CS")));
}

TEST_F(DataLayerListingPatch, SetsPriceCurrency) {
  ASSERT_FALSE(patch.getPriceCurrency().has_value());

  patch.withPriceCurrency("USD");
  EXPECT_THAT(patch.getPriceCurrency(), Optional(Eq("USD")));
}

TEST_F(DataLayerListingPatch, SetsFxBaseCurrency) {
  ASSERT_FALSE(patch.getFxBaseCurrency().has_value());

  patch.withFxBaseCurrency("USD");
  EXPECT_THAT(patch.getFxBaseCurrency(), Optional(Eq("USD")));
}

TEST_F(DataLayerListingPatch, SetsInstrSymbol) {
  ASSERT_FALSE(patch.getInstrSymbol().has_value());

  patch.withInstrSymbol("AAPL");
  EXPECT_THAT(patch.getInstrSymbol(), Optional(Eq("AAPL")));
}

TEST_F(DataLayerListingPatch, SetsQtyMinimum) {
  ASSERT_FALSE(patch.getQtyMinimum().has_value());

  patch.withQtyMinimum(42.42);
  EXPECT_THAT(patch.getQtyMinimum(), Optional(DoubleEq(42.42)));
}

TEST_F(DataLayerListingPatch, SetsQtyMaximum) {
  ASSERT_FALSE(patch.getQtyMaximum().has_value());

  patch.withQtyMaximum(42.42);
  EXPECT_THAT(patch.getQtyMaximum(), Optional(DoubleEq(42.42)));
}

TEST_F(DataLayerListingPatch, SetsQtyMultiple) {
  ASSERT_FALSE(patch.getQtyMultiple().has_value());

  patch.withQtyMultiple(42.42);
  EXPECT_THAT(patch.getQtyMultiple(), Optional(DoubleEq(42.42)));
}

TEST_F(DataLayerListingPatch, SetsPriceTickSize) {
  ASSERT_FALSE(patch.getPriceTickSize().has_value());

  patch.withPriceTickSize(42.42);
  EXPECT_THAT(patch.getPriceTickSize(), Optional(DoubleEq(42.42)));
}

TEST_F(DataLayerListingPatch, SetsEnabled) {
  ASSERT_FALSE(patch.getEnabledFlag().has_value());

  patch.withEnabledFlag(true);
  EXPECT_THAT(patch.getEnabledFlag(), Optional(Eq(true)));
}

TEST_F(DataLayerListingPatch, SetsRandomQtyMaximum) {
  ASSERT_FALSE(patch.getRandomQtyMaximum().has_value());

  patch.withRandomQtyMaximum(42.42);
  EXPECT_THAT(patch.getRandomQtyMaximum(), Optional(DoubleEq(42.42)));
}

TEST_F(DataLayerListingPatch, SetsRandomDepthLevels) {
  ASSERT_FALSE(patch.getRandomDepthLevels().has_value());

  patch.withRandomDepthLevels(42);
  EXPECT_THAT(patch.getRandomDepthLevels(), Optional(Eq(42)));
}

TEST_F(DataLayerListingPatch, SetsRandomOrderSpread) {
  ASSERT_FALSE(patch.getRandomOrdersSpread().has_value());

  patch.withRandomOrdersSpread(42.42);
  EXPECT_THAT(patch.getRandomOrdersSpread(), Optional(DoubleEq(42.42)));
}

TEST_F(DataLayerListingPatch, SetsRandomOrdersRate) {
  ASSERT_FALSE(patch.getRandomOrdersRate().has_value());

  patch.withRandomOrdersRate(42);
  EXPECT_THAT(patch.getRandomOrdersRate(), Optional(Eq(42)));
}

TEST_F(DataLayerListingPatch, SetsRandomTickRange) {
  ASSERT_FALSE(patch.getRandomTickRange().has_value());

  patch.withRandomTickRange(42);
  EXPECT_THAT(patch.getRandomTickRange(), Optional(Eq(42)));
}

TEST_F(DataLayerListingPatch, SetsSecurityExchage) {
  ASSERT_FALSE(patch.getSecurityExchange().has_value());

  patch.withSecurityExchange("NASDAQ");
  EXPECT_THAT(patch.getSecurityExchange(), Optional(Eq("NASDAQ")));
}

TEST_F(DataLayerListingPatch, SetsPartyID) {
  ASSERT_FALSE(patch.getPartyId().has_value());

  patch.withPartyId("GGV");
  EXPECT_THAT(patch.getPartyId(), Optional(Eq("GGV")));
}

TEST_F(DataLayerListingPatch, SetsPartyRole) {
  ASSERT_FALSE(patch.getPartyRole().has_value());

  patch.withPartyRole("ExecutingFirm");
  EXPECT_THAT(patch.getPartyRole(), Optional(Eq("ExecutingFirm")));
}

TEST_F(DataLayerListingPatch, SetsCusipID) {
  ASSERT_FALSE(patch.getCusipId().has_value());

  patch.withCusipId("037833100");
  EXPECT_THAT(patch.getCusipId(), Optional(Eq("037833100")));
}

TEST_F(DataLayerListingPatch, SetsSedolID) {
  ASSERT_FALSE(patch.getSedolId().has_value());

  patch.withSedolId("2046251");
  EXPECT_THAT(patch.getSedolId(), Optional(Eq("2046251")));
}

TEST_F(DataLayerListingPatch, SetsIsinID) {
  ASSERT_FALSE(patch.getIsinId().has_value());

  patch.withIsinId("US0378331005");
  EXPECT_THAT(patch.getIsinId(), Optional(Eq("US0378331005")));
}

TEST_F(DataLayerListingPatch, SetsRicID) {
  ASSERT_FALSE(patch.getRicId().has_value());

  patch.withRicId("AAPL.OQ");
  EXPECT_THAT(patch.getRicId(), Optional(Eq("AAPL.OQ")));
}

TEST_F(DataLayerListingPatch, SetsExchangeSymbolID) {
  ASSERT_FALSE(patch.getExchangeSymbolId().has_value());

  patch.withExchangeSymbolId("NASDAQ:AAPL");
  EXPECT_THAT(patch.getExchangeSymbolId(), Optional(Eq("NASDAQ:AAPL")));
}

TEST_F(DataLayerListingPatch, SetsBloombergSymbolID) {
  ASSERT_FALSE(patch.getBloombergSymbolId().has_value());

  patch.withBloombergSymbolId("AAPL:US");
  EXPECT_THAT(patch.getBloombergSymbolId(), Optional(Eq("AAPL:US")));
}

TEST_F(DataLayerListingPatch, SetsRandomQtyMinimum) {
  ASSERT_FALSE(patch.getRandomQtyMinimum().has_value());

  patch.withRandomQtyMinimum(42.42);
  EXPECT_THAT(patch.getRandomQtyMinimum(), Optional(DoubleEq(42.42)));
}

TEST_F(DataLayerListingPatch, SetsRandomAmtMinimum) {
  ASSERT_FALSE(patch.getRandomAmtMinimum().has_value());

  patch.withRandomAmtMinimum(42.42);
  EXPECT_THAT(patch.getRandomAmtMinimum(), Optional(DoubleEq(42.42)));
}

TEST_F(DataLayerListingPatch, SetsRandomAmtMaximum) {
  ASSERT_FALSE(patch.getRandomAmtMinimum().has_value());

  patch.withRandomAmtMaximum(42.42);
  EXPECT_THAT(patch.getRandomAmtMaximum(), Optional(DoubleEq(42.42)));
}

TEST_F(DataLayerListingPatch, SetsRandomOrdersEnabledFlag) {
  ASSERT_FALSE(patch.getRandomOrdersEnabledFlag().has_value());

  patch.withRandomOrdersEnabledFlag(false);
  EXPECT_THAT(patch.getRandomOrdersEnabledFlag(), Optional(Eq(false)));
}

TEST_F(DataLayerListingPatch, SetsRandomAggressiveQtyMinimum) {
  ASSERT_FALSE(patch.getRandomAggressiveQtyMinimum().has_value());

  patch.withRandomAggressiveQtyMinimum(42.42);
  EXPECT_THAT(patch.getRandomAggressiveQtyMinimum(), Optional(DoubleEq(42.42)));
}

TEST_F(DataLayerListingPatch, SetsRandomAggressiveQtyMaximum) {
  ASSERT_FALSE(patch.getRandomAggressiveQtyMaximum().has_value());

  patch.withRandomAggressiveQtyMaximum(42.42);
  EXPECT_THAT(patch.getRandomAggressiveQtyMaximum(), Optional(DoubleEq(42.42)));
}

TEST_F(DataLayerListingPatch, SetsRandomAggressiveAmtMinimum) {
  ASSERT_FALSE(patch.getRandomAggressiveAmtMinimum().has_value());

  patch.withRandomAggressiveAmtMinimum(42.42);
  EXPECT_THAT(patch.getRandomAggressiveAmtMinimum(), Optional(DoubleEq(42.42)));
}

TEST_F(DataLayerListingPatch, SetsRandomAggressiveAmtMaximum) {
  ASSERT_FALSE(patch.getRandomAggressiveAmtMaximum().has_value());

  patch.withRandomAggressiveAmtMaximum(42.42);
  EXPECT_THAT(patch.getRandomAggressiveAmtMaximum(), Optional(DoubleEq(42.42)));
}

struct DataLayerListing : public Test {
  Listing::Patch patch;
};

TEST_F(DataLayerListing, CreatedWithoutVenueID) {
  ASSERT_FALSE(patch.getVenueId().has_value());

  EXPECT_THROW((void)Listing::create(patch, 42), RequiredAttributeMissing);
}

TEST_F(DataLayerListing, GetsListingID) {
  patch.withVenueId("NASDAQ");

  Listing listing = Listing::create(patch, 42);
  EXPECT_EQ(listing.getListingId(), 42);
}

TEST_F(DataLayerListing, GetsVenueID) {
  patch.withVenueId("NASDAQ");

  Listing listing = Listing::create(patch, 42);
  EXPECT_EQ(listing.getVenueId(), "NASDAQ");
}

TEST_F(DataLayerListing, GetsSymbolMissing) {
  patch.withVenueId("NASDAQ");

  Listing listing = Listing::create(patch, 42);
  EXPECT_EQ(listing.getSymbol(), std::nullopt);
}

TEST_F(DataLayerListing, GetsSymbolSpecified) {
  patch.withVenueId("NASDAQ");
  patch.withSymbol("AAPL");

  Listing listing = Listing::create(patch, 42);
  EXPECT_THAT(listing.getSymbol(), Optional(Eq("AAPL")));
}

TEST_F(DataLayerListing, GetsSecurityTypeMissing) {
  patch.withVenueId("NASDAQ");

  Listing listing = Listing::create(patch, 42);
  EXPECT_EQ(listing.getSecurityType(), std::nullopt);
}

TEST_F(DataLayerListing, GetsSecurityTypeSpecified) {
  patch.withVenueId("NASDAQ");
  patch.withSecurityType("CS");

  Listing listing = Listing::create(patch, 42);
  EXPECT_THAT(listing.getSecurityType(), Optional(Eq("CS")));
}

TEST_F(DataLayerListing, GetsPriceCurrencyMissing) {
  patch.withVenueId("NASDAQ");

  Listing listing = Listing::create(patch, 42);
  EXPECT_EQ(listing.getPriceCurrency(), std::nullopt);
}

TEST_F(DataLayerListing, GetsPriceCurrencySpecified) {
  patch.withVenueId("NASDAQ");
  patch.withPriceCurrency("USD");

  Listing listing = Listing::create(patch, 42);
  EXPECT_THAT(listing.getPriceCurrency(), Optional(Eq("USD")));
}

TEST_F(DataLayerListing, GetsFxBaseCurrencyMissing) {
  patch.withVenueId("NASDAQ");

  Listing listing = Listing::create(patch, 42);
  EXPECT_EQ(listing.getFxBaseCurrency(), std::nullopt);
}

TEST_F(DataLayerListing, GetsFxBaseCurrencySpecified) {
  patch.withVenueId("NASDAQ");
  patch.withFxBaseCurrency("USD");

  Listing listing = Listing::create(patch, 42);
  EXPECT_THAT(listing.getFxBaseCurrency(), Optional(Eq("USD")));
}

TEST_F(DataLayerListing, GetsInstrSymbolMissing) {
  patch.withVenueId("NASDAQ");

  Listing listing = Listing::create(patch, 42);
  EXPECT_EQ(listing.getInstrSymbol(), std::nullopt);
}

TEST_F(DataLayerListing, GetsInstrSymbolSpecified) {
  patch.withVenueId("NASDAQ");
  patch.withInstrSymbol("AAPL");

  Listing listing = Listing::create(patch, 42);
  EXPECT_THAT(listing.getInstrSymbol(), Optional(Eq("AAPL")));
}

TEST_F(DataLayerListing, GetsQtyMinimumMissing) {
  patch.withVenueId("NASDAQ");

  Listing listing = Listing::create(patch, 42);
  EXPECT_EQ(listing.getQtyMinimum(), std::nullopt);
}

TEST_F(DataLayerListing, GetsQtyMinimumSpecified) {
  patch.withVenueId("NASDAQ");
  patch.withQtyMinimum(42.42);

  Listing listing = Listing::create(patch, 42);
  EXPECT_THAT(listing.getQtyMinimum(), Optional(DoubleEq(42.42)));
}

TEST_F(DataLayerListing, GetsQtyMaximumMissing) {
  patch.withVenueId("NASDAQ");

  Listing listing = Listing::create(patch, 42);
  EXPECT_EQ(listing.getQtyMaximum(), std::nullopt);
}

TEST_F(DataLayerListing, GetsQtyMaximumSpecified) {
  patch.withVenueId("NASDAQ");
  patch.withQtyMaximum(42.42);

  Listing listing = Listing::create(patch, 42);
  EXPECT_THAT(listing.getQtyMaximum(), Optional(DoubleEq(42.42)));
}

TEST_F(DataLayerListing, GetsQtyMultipleMissing) {
  patch.withVenueId("NASDAQ");

  Listing listing = Listing::create(patch, 42);
  EXPECT_EQ(listing.getQtyMultiple(), std::nullopt);
}

TEST_F(DataLayerListing, GetsQtyMultipleSpecified) {
  patch.withVenueId("NASDAQ");
  patch.withQtyMultiple(42.42);

  Listing listing = Listing::create(patch, 42);
  EXPECT_THAT(listing.getQtyMultiple(), Optional(DoubleEq(42.42)));
}

TEST_F(DataLayerListing, GetsQtyPriceTickSizeMissing) {
  patch.withVenueId("NASDAQ");

  Listing listing = Listing::create(patch, 42);
  EXPECT_EQ(listing.getPriceTickSize(), std::nullopt);
}

TEST_F(DataLayerListing, GetsPriceTickSizeSpecified) {
  patch.withVenueId("NASDAQ");
  patch.withPriceTickSize(42.42);

  Listing listing = Listing::create(patch, 42);
  EXPECT_THAT(listing.getPriceTickSize(), Optional(DoubleEq(42.42)));
}

TEST_F(DataLayerListing, GetsEnabledMissing) {
  patch.withVenueId("NASDAQ");

  Listing listing = Listing::create(patch, 42);
  EXPECT_EQ(listing.getEnabledFlag(), std::nullopt);
}

TEST_F(DataLayerListing, GetsEnabledSpecified) {
  patch.withVenueId("NASDAQ");
  patch.withEnabledFlag(true);

  Listing listing = Listing::create(patch, 42);
  EXPECT_THAT(listing.getEnabledFlag(), Optional(Eq(true)));
}

TEST_F(DataLayerListing, GetsRandomQtyMaximumMissing) {
  patch.withVenueId("NASDAQ");

  Listing listing = Listing::create(patch, 42);
  EXPECT_EQ(listing.getRandomQtyMaximum(), std::nullopt);
}

TEST_F(DataLayerListing, GetsRandomQtyMaximumSpecified) {
  patch.withVenueId("NASDAQ");
  patch.withRandomQtyMaximum(42.42);

  Listing listing = Listing::create(patch, 42);
  EXPECT_THAT(listing.getRandomQtyMaximum(), Optional(DoubleEq(42.42)));
}

TEST_F(DataLayerListing, GetsRandomDepthLevelsMissing) {
  patch.withVenueId("NASDAQ");

  Listing listing = Listing::create(patch, 42);
  EXPECT_EQ(listing.getRandomDepthLevels(), std::nullopt);
}

TEST_F(DataLayerListing, GetsRandomDepthLevelsSpecified) {
  patch.withVenueId("NASDAQ");
  patch.withRandomDepthLevels(42);

  Listing listing = Listing::create(patch, 42);
  EXPECT_THAT(listing.getRandomDepthLevels(), Optional(Eq(42)));
}

TEST_F(DataLayerListing, GetsRandomOrdersSpreadMissing) {
  patch.withVenueId("NASDAQ");

  Listing listing = Listing::create(patch, 42);
  EXPECT_EQ(listing.getRandomOrdersSpread(), std::nullopt);
}

TEST_F(DataLayerListing, GetsRandomOrdersSpreadSpecified) {
  patch.withVenueId("NASDAQ");
  patch.withRandomOrdersSpread(42.42);

  Listing listing = Listing::create(patch, 42);
  EXPECT_THAT(listing.getRandomOrdersSpread(), Optional(DoubleEq(42.42)));
}

TEST_F(DataLayerListing, GetsRandomOrdersRateMissing) {
  patch.withVenueId("NASDAQ");

  Listing listing = Listing::create(patch, 42);
  EXPECT_EQ(listing.getRandomOrdersRate(), std::nullopt);
}

TEST_F(DataLayerListing, GetsRandomOrdersRateSpecified) {
  patch.withVenueId("NASDAQ");
  patch.withRandomOrdersRate(42);

  Listing listing = Listing::create(patch, 42);
  EXPECT_THAT(listing.getRandomOrdersRate(), Optional(Eq(42)));
}

TEST_F(DataLayerListing, GetsRandomTickRangeMissing) {
  patch.withVenueId("NASDAQ");

  Listing listing = Listing::create(patch, 42);
  EXPECT_EQ(listing.getRandomTickRange(), std::nullopt);
}

TEST_F(DataLayerListing, GetsRandomTickRangeSpecified) {
  patch.withVenueId("NASDAQ");
  patch.withRandomTickRange(42);

  Listing listing = Listing::create(patch, 42);
  EXPECT_THAT(listing.getRandomTickRange(), Optional(Eq(42)));
}

TEST_F(DataLayerListing, GetsSecurityExchangeMissing) {
  patch.withVenueId("NASDAQ");

  Listing listing = Listing::create(patch, 42);
  EXPECT_EQ(listing.getSecurityExchange(), std::nullopt);
}

TEST_F(DataLayerListing, GetsSecurityExchangeSpecified) {
  patch.withVenueId("NASDAQ");
  patch.withSecurityExchange("NASDAQ");

  Listing listing = Listing::create(patch, 42);
  EXPECT_THAT(listing.getSecurityExchange(), Optional(Eq("NASDAQ")));
}

TEST_F(DataLayerListing, GetsPartyIDMissing) {
  patch.withVenueId("NASDAQ");

  Listing listing = Listing::create(patch, 42);
  EXPECT_EQ(listing.getPartyId(), std::nullopt);
}

TEST_F(DataLayerListing, GetsPartyIDSpecified) {
  patch.withVenueId("NASDAQ");
  patch.withPartyId("GGV");

  Listing listing = Listing::create(patch, 42);
  EXPECT_THAT(listing.getPartyId(), Optional(Eq("GGV")));
}

TEST_F(DataLayerListing, GetsPartyRoleMissing) {
  patch.withVenueId("NASDAQ");

  Listing listing = Listing::create(patch, 42);
  EXPECT_EQ(listing.getPartyRole(), std::nullopt);
}

TEST_F(DataLayerListing, GetsPartyRoleSpecified) {
  patch.withVenueId("NASDAQ");
  patch.withPartyRole("ExecutingFirm");

  Listing listing = Listing::create(patch, 42);
  EXPECT_THAT(listing.getPartyRole(), Optional(Eq("ExecutingFirm")));
}

TEST_F(DataLayerListing, GetsCusipIDMissing) {
  patch.withVenueId("NASDAQ");

  Listing listing = Listing::create(patch, 42);
  EXPECT_EQ(listing.getCusipId(), std::nullopt);
}

TEST_F(DataLayerListing, GetsCusipIDSpecified) {
  patch.withVenueId("NASDAQ");
  patch.withCusipId("037833100");

  Listing listing = Listing::create(patch, 42);
  EXPECT_THAT(listing.getCusipId(), Optional(Eq("037833100")));
}

TEST_F(DataLayerListing, GetsSedolIDMissing) {
  patch.withVenueId("NASDAQ");

  Listing listing = Listing::create(patch, 42);
  EXPECT_EQ(listing.getSedolId(), std::nullopt);
}

TEST_F(DataLayerListing, GetsSedolIDSpecified) {
  patch.withVenueId("NASDAQ");
  patch.withSedolId("2046251");

  Listing listing = Listing::create(patch, 42);
  EXPECT_THAT(listing.getSedolId(), Optional(Eq("2046251")));
}

TEST_F(DataLayerListing, GetsIsinIDMissing) {
  patch.withVenueId("NASDAQ");

  Listing listing = Listing::create(patch, 42);
  EXPECT_EQ(listing.getIsinId(), std::nullopt);
}

TEST_F(DataLayerListing, GetsIsinIDSpecified) {
  patch.withVenueId("NASDAQ");
  patch.withIsinId("US0378331005");

  Listing listing = Listing::create(patch, 42);
  EXPECT_THAT(listing.getIsinId(), Optional(Eq("US0378331005")));
}

TEST_F(DataLayerListing, GetsRicIDMissing) {
  patch.withVenueId("NASDAQ");

  Listing listing = Listing::create(patch, 42);
  EXPECT_EQ(listing.getRicId(), std::nullopt);
}

TEST_F(DataLayerListing, GetsRicIDSpecified) {
  patch.withVenueId("NASDAQ");
  patch.withRicId("AAPL.OQ");

  Listing listing = Listing::create(patch, 42);
  EXPECT_THAT(listing.getRicId(), Optional(Eq("AAPL.OQ")));
}

TEST_F(DataLayerListing, GetsExchangeSymbolIDMissing) {
  patch.withVenueId("NASDAQ");

  Listing listing = Listing::create(patch, 42);
  EXPECT_EQ(listing.getExchangeSymbolId(), std::nullopt);
}

TEST_F(DataLayerListing, GetsExchangeSymbolIDSpecified) {
  patch.withVenueId("NASDAQ");
  patch.withExchangeSymbolId("NASDAQ:AAPL");

  Listing listing = Listing::create(patch, 42);
  EXPECT_THAT(listing.getExchangeSymbolId(), Optional(Eq("NASDAQ:AAPL")));
}

TEST_F(DataLayerListing, GetsBloombergSymbolIDMissing) {
  patch.withVenueId("NASDAQ");

  Listing listing = Listing::create(patch, 42);
  EXPECT_EQ(listing.getBloombergSymbolId(), std::nullopt);
}

TEST_F(DataLayerListing, GetsBloombergSymbolIDSpecified) {
  patch.withVenueId("NASDAQ");
  patch.withBloombergSymbolId("AAPL:US");

  Listing listing = Listing::create(patch, 42);
  EXPECT_THAT(listing.getBloombergSymbolId(), Optional(Eq("AAPL:US")));
}

TEST_F(DataLayerListing, GetsRandomQtyMinimumMissing) {
  patch.withVenueId("NASDAQ");

  Listing listing = Listing::create(patch, 42);
  EXPECT_EQ(listing.getRandomQtyMinimum(), std::nullopt);
}

TEST_F(DataLayerListing, GetsRandomQtyMinimumSpecified) {
  patch.withVenueId("NASDAQ");
  patch.withRandomQtyMinimum(42.42);

  Listing listing = Listing::create(patch, 42);
  EXPECT_THAT(listing.getRandomQtyMinimum(), Optional(DoubleEq(42.42)));
}

TEST_F(DataLayerListing, GetsRandomAmtMinimumMissing) {
  patch.withVenueId("NASDAQ");

  Listing listing = Listing::create(patch, 42);
  EXPECT_EQ(listing.getRandomAmtMinimum(), std::nullopt);
}

TEST_F(DataLayerListing, GetsRandomAmtMinimumSpecified) {
  patch.withVenueId("NASDAQ");
  patch.withRandomAmtMinimum(42.42);

  Listing listing = Listing::create(patch, 42);
  EXPECT_THAT(listing.getRandomAmtMinimum(), Optional(DoubleEq(42.42)));
}

TEST_F(DataLayerListing, GetsRandomAmtMaximumMissing) {
  patch.withVenueId("NASDAQ");

  Listing listing = Listing::create(patch, 42);
  EXPECT_EQ(listing.getRandomAmtMaximum(), std::nullopt);
}

TEST_F(DataLayerListing, GetsRandomAmtMaximumSpecified) {
  patch.withVenueId("NASDAQ");
  patch.withRandomAmtMaximum(42.42);

  Listing listing = Listing::create(patch, 42);
  EXPECT_THAT(listing.getRandomAmtMaximum(), Optional(DoubleEq(42.42)));
}

TEST_F(DataLayerListing, GetsRandomOrdersEnabledMissing) {
  patch.withVenueId("NASDAQ");

  Listing listing = Listing::create(patch, 42);
  EXPECT_EQ(listing.getRandomOrdersEnabledFlag(), std::nullopt);
}

TEST_F(DataLayerListing, GetsRandomOrdersEnabledSpecified) {
  patch.withVenueId("NASDAQ");
  patch.withRandomOrdersEnabledFlag(false);

  Listing listing = Listing::create(patch, 42);
  EXPECT_THAT(listing.getRandomOrdersEnabledFlag(), Optional(Eq(false)));
}

TEST_F(DataLayerListing, GetsRandomAggressiveQtyMinimumMissing) {
  patch.withVenueId("NASDAQ");

  Listing listing = Listing::create(patch, 42);
  EXPECT_EQ(listing.getRandomAggressiveQtyMinimum(), std::nullopt);
}

TEST_F(DataLayerListing, GetsRandomAggressiveQtyMinimumSpecified) {
  patch.withVenueId("NASDAQ");
  patch.withRandomAggressiveQtyMinimum(42.42);

  Listing listing = Listing::create(patch, 42);
  EXPECT_THAT(listing.getRandomAggressiveQtyMinimum(),
              Optional(DoubleEq(42.42)));
}

TEST_F(DataLayerListing, GetsRandomAggressiveQtyMaximumMissing) {
  patch.withVenueId("NASDAQ");

  Listing listing = Listing::create(patch, 42);
  EXPECT_EQ(listing.getRandomAggressiveQtyMaximum(), std::nullopt);
}

TEST_F(DataLayerListing, GetsRandomAggressiveQtyMaximumSpecified) {
  patch.withVenueId("NASDAQ");
  patch.withRandomAggressiveQtyMaximum(42.42);

  Listing listing = Listing::create(patch, 42);
  EXPECT_THAT(listing.getRandomAggressiveQtyMaximum(),
              Optional(DoubleEq(42.42)));
}

TEST_F(DataLayerListing, GetsRandomAggressiveAmtMinimumMissing) {
  patch.withVenueId("NASDAQ");

  Listing listing = Listing::create(patch, 42);
  EXPECT_EQ(listing.getRandomAggressiveAmtMinimum(), std::nullopt);
}

TEST_F(DataLayerListing, GetsRandomAggressiveAmtMinimumSpecified) {
  patch.withVenueId("NASDAQ");
  patch.withRandomAggressiveAmtMinimum(42.42);

  Listing listing = Listing::create(patch, 42);
  EXPECT_THAT(listing.getRandomAggressiveAmtMinimum(),
              Optional(DoubleEq(42.42)));
}

TEST_F(DataLayerListing, GetsRandomAggressiveAmtMaximumMissing) {
  patch.withVenueId("NASDAQ");

  Listing listing = Listing::create(patch, 42);
  EXPECT_EQ(listing.getRandomAggressiveAmtMaximum(), std::nullopt);
}

TEST_F(DataLayerListing, GetsRandomAggressiveAmtMaximumSpecified) {
  patch.withVenueId("NASDAQ");
  patch.withRandomAggressiveAmtMaximum(42.42);

  Listing listing = Listing::create(patch, 42);
  EXPECT_THAT(listing.getRandomAggressiveAmtMaximum(),
              Optional(DoubleEq(42.42)));
}

// NOLINTEND(*magic-numbers*)

}  // namespace
}  // namespace Simulator::DataLayer