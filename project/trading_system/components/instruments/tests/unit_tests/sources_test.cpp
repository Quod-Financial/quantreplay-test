#include "instruments/sources.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "data_layer/api/models/listing.hpp"

namespace simulator::trading_system::instrument::test {
namespace {

using Simulator::DataLayer::Listing;
using testing::Eq;
using testing::Optional;

// NOLINTBEGIN(*-magic-numbers)

[[nodiscard]] auto make_listing_patch() -> Listing::Patch {
  return Listing::Patch{}.withVenueId("VENUE");
}

TEST(InstrumentsCreateInstrument, WithDefaultFields) {
  const Listing listing = Listing::create(make_listing_patch(), 42);

  const Instrument instrument = detail::create_instrument(listing);

  EXPECT_EQ(instrument.identifier, InstrumentId{0});
  EXPECT_THAT(instrument.database_id, Optional(Eq(DatabaseId{42})));
}

TEST(InstrumentsCreateInstrument, WithSymbol) {
  const Listing listing =
      Listing::create(make_listing_patch().withSymbol("AAPL"), 43);

  const Instrument instrument = detail::create_instrument(listing);

  EXPECT_THAT(instrument.symbol, Optional(Eq(Symbol{"AAPL"})));
}

TEST(InstrumentsCreateInstrument, WithoutSymbol) {
  const Listing listing = Listing::create(make_listing_patch(), 43);

  const Instrument instrument = detail::create_instrument(listing);

  EXPECT_EQ(instrument.symbol, std::nullopt);
}

TEST(InstrumentsCreateInstrument, WithSecurityType) {
  const Listing listing =
      Listing::create(make_listing_patch().withSecurityType("CS"), 43);

  const Instrument instrument = detail::create_instrument(listing);

  EXPECT_THAT(instrument.security_type,
              Optional(Eq(SecurityType::Option::CommonStock)));
}

TEST(InstrumentsCreateInstrument, WithUndefinedSecurityType) {
  const Listing listing =
      Listing::create(make_listing_patch().withSecurityType("bad-value"), 43);

  const Instrument instrument = detail::create_instrument(listing);

  EXPECT_EQ(instrument.security_type, std::nullopt);
}

TEST(InstrumentsCreateInstrument, WithoutSecurityType) {
  const Listing listing = Listing::create(make_listing_patch(), 43);

  const Instrument instrument = detail::create_instrument(listing);

  EXPECT_EQ(instrument.security_type, std::nullopt);
}

TEST(InstrumentsCreateInstrument, WithPriceCurrency) {
  const Listing listing =
      Listing::create(make_listing_patch().withPriceCurrency("USD"), 43);

  const Instrument instrument = detail::create_instrument(listing);

  EXPECT_THAT(instrument.price_currency, Optional(Eq(PriceCurrency{"USD"})));
}

TEST(InstrumentsCreateInstrument, WithoutPriceCurrency) {
  const Listing listing = Listing::create(make_listing_patch(), 43);

  const Instrument instrument = detail::create_instrument(listing);

  EXPECT_EQ(instrument.price_currency, std::nullopt);
}

TEST(InstrumentsCreateInstrument, WithBaseCurrency) {
  const Listing listing =
      Listing::create(make_listing_patch().withFxBaseCurrency("EUR"), 43);

  const Instrument instrument = detail::create_instrument(listing);

  EXPECT_THAT(instrument.base_currency, Optional(Eq(BaseCurrency{"EUR"})));
}

TEST(InstrumentsCreateInstrument, WithoutBaseCurrency) {
  const Listing listing = Listing::create(make_listing_patch(), 43);

  const Instrument instrument = detail::create_instrument(listing);

  EXPECT_EQ(instrument.base_currency, std::nullopt);
}

TEST(InstrumentsCreateInstrument, WithSecurityExchange) {
  const Listing listing =
      Listing::create(make_listing_patch().withSecurityExchange("XLON"), 43);

  const Instrument instrument = detail::create_instrument(listing);

  EXPECT_THAT(instrument.security_exchange,
              Optional(Eq(SecurityExchange{"XLON"})));
}

TEST(InstrumentsCreateInstrument, WithoutSecurityExchange) {
  const Listing listing = Listing::create(make_listing_patch(), 43);

  const Instrument instrument = detail::create_instrument(listing);

  EXPECT_EQ(instrument.security_exchange, std::nullopt);
}

TEST(InstrumentsCreateInstrument, WithPartyId) {
  const Listing listing =
      Listing::create(make_listing_patch().withPartyId("P123"), 43);

  const Instrument instrument = detail::create_instrument(listing);

  EXPECT_THAT(instrument.party_id, Optional(Eq(PartyId{"P123"})));
}

TEST(InstrumentsCreateInstrument, WithoutPartyId) {
  const Listing listing = Listing::create(make_listing_patch(), 43);

  const Instrument instrument = detail::create_instrument(listing);

  EXPECT_EQ(instrument.party_id, std::nullopt);
}

TEST(InstrumentsCreateInstrument, WithPartyRole) {
  const Listing listing =
      Listing::create(make_listing_patch().withPartyRole("ExecutingFirm"), 43);

  const Instrument instrument = detail::create_instrument(listing);

  EXPECT_THAT(instrument.party_role,
              Optional(Eq(PartyRole::Option::ExecutingFirm)));
}

TEST(InstrumentsCreateInstrument, WithUndefinedPartyRole) {
  const Listing listing =
      Listing::create(make_listing_patch().withPartyRole("bad-value"), 43);

  const Instrument instrument = detail::create_instrument(listing);

  EXPECT_EQ(instrument.party_role, std::nullopt);
}

TEST(InstrumentsCreateInstrument, WithoutPartyRole) {
  const Listing listing = Listing::create(make_listing_patch(), 43);

  const Instrument instrument = detail::create_instrument(listing);

  EXPECT_EQ(instrument.party_role, std::nullopt);
}

TEST(InstrumentsCreateInstrument, WithCusipId) {
  const Listing listing =
      Listing::create(make_listing_patch().withCusipId("CUSIP"), 43);

  const Instrument instrument = detail::create_instrument(listing);

  EXPECT_THAT(instrument.cusip, Optional(Eq(CusipId{"CUSIP"})));
}

TEST(InstrumentsCreateInstrument, WithoutCusipId) {
  const Listing listing = Listing::create(make_listing_patch(), 43);

  const Instrument instrument = detail::create_instrument(listing);

  EXPECT_EQ(instrument.cusip, std::nullopt);
}

TEST(InstrumentsCreateInstrument, WithSedolId) {
  const Listing listing =
      Listing::create(make_listing_patch().withSedolId("SEDOL"), 43);

  const Instrument instrument = detail::create_instrument(listing);

  EXPECT_THAT(instrument.sedol, Optional(Eq(SedolId{"SEDOL"})));
}

TEST(InstrumentsCreateInstrument, WithoutSedolId) {
  const Listing listing = Listing::create(make_listing_patch(), 43);

  const Instrument instrument = detail::create_instrument(listing);

  EXPECT_EQ(instrument.sedol, std::nullopt);
}

TEST(InstrumentsCreateInstrument, WithIsinId) {
  const Listing listing =
      Listing::create(make_listing_patch().withIsinId("ISIN"), 43);

  const Instrument instrument = detail::create_instrument(listing);

  EXPECT_THAT(instrument.isin, Optional(Eq(IsinId{"ISIN"})));
}

TEST(InstrumentsCreateInstrument, WithoutIsinId) {
  const Listing listing = Listing::create(make_listing_patch(), 43);

  const Instrument instrument = detail::create_instrument(listing);

  EXPECT_EQ(instrument.isin, std::nullopt);
}

TEST(InstrumentsCreateInstrument, WithRicId) {
  const Listing listing =
      Listing::create(make_listing_patch().withRicId("RIC"), 43);

  const Instrument instrument = detail::create_instrument(listing);

  EXPECT_THAT(instrument.ric, Optional(Eq(RicId{"RIC"})));
}

TEST(InstrumentsCreateInstrument, WithoutRicId) {
  const Listing listing = Listing::create(make_listing_patch(), 43);

  const Instrument instrument = detail::create_instrument(listing);

  EXPECT_EQ(instrument.ric, std::nullopt);
}

TEST(InstrumentsCreateInstrument, WithExchangeId) {
  const Listing listing =
      Listing::create(make_listing_patch().withExchangeSymbolId("EXC-ID"), 43);

  const Instrument instrument = detail::create_instrument(listing);

  EXPECT_THAT(instrument.exchange_id, Optional(Eq(ExchangeId{"EXC-ID"})));
}

TEST(InstrumentsCreateInstrument, WithoutExchangeId) {
  const Listing listing = Listing::create(make_listing_patch(), 43);

  const Instrument instrument = detail::create_instrument(listing);

  EXPECT_EQ(instrument.exchange_id, std::nullopt);
}

TEST(InstrumentsCreateInstrument, WithBloombergId) {
  const Listing listing =
      Listing::create(make_listing_patch().withBloombergSymbolId("BBG-ID"), 43);

  const Instrument instrument = detail::create_instrument(listing);

  EXPECT_THAT(instrument.bloomberg_id, Optional(Eq(BloombergId{"BBG-ID"})));
}

TEST(InstrumentsCreateInstrument, WithoutBloombergId) {
  const Listing listing = Listing::create(make_listing_patch(), 43);

  const Instrument instrument = detail::create_instrument(listing);

  EXPECT_EQ(instrument.bloomberg_id, std::nullopt);
}

TEST(InstrumentsCreateInstrument, WithPriceTick) {
  const Listing listing =
      Listing::create(make_listing_patch().withPriceTickSize(42.42), 43);

  const Instrument instrument = detail::create_instrument(listing);

  EXPECT_THAT(instrument.price_tick, Optional(Eq(PriceTick{42.42})));
}

TEST(InstrumentsCreateInstrument, WithoutPriceTick) {
  const Listing listing = Listing::create(make_listing_patch(), 43);

  const Instrument instrument = detail::create_instrument(listing);

  EXPECT_EQ(instrument.price_tick, std::nullopt);
}

TEST(InstrumentsCreateInstrument, WithQuantityTick) {
  const Listing listing =
      Listing::create(make_listing_patch().withQtyMultiple(42.42), 43);

  const Instrument instrument = detail::create_instrument(listing);

  EXPECT_THAT(instrument.quantity_tick, Optional(Eq(QuantityTick{42.42})));
}

TEST(InstrumentsCreateInstrument, WithoutQuantityTick) {
  const Listing listing = Listing::create(make_listing_patch(), 43);

  const Instrument instrument = detail::create_instrument(listing);

  EXPECT_EQ(instrument.quantity_tick, std::nullopt);
}

TEST(InstrumentsCreateInstrument, WithMinimalQuantity) {
  const Listing listing =
      Listing::create(make_listing_patch().withQtyMinimum(42.42), 43);

  const Instrument instrument = detail::create_instrument(listing);

  EXPECT_THAT(instrument.min_quantity, Optional(Eq(MinQuantity{42.42})));
}

TEST(InstrumentsCreateInstrument, WithoutMinimalQuantity) {
  const Listing listing = Listing::create(make_listing_patch(), 43);

  const Instrument instrument = detail::create_instrument(listing);

  EXPECT_EQ(instrument.min_quantity, std::nullopt);
}

TEST(InstrumentsCreateInstrument, WithMaximalQuantity) {
  const Listing listing =
      Listing::create(make_listing_patch().withQtyMaximum(42.42), 43);

  const Instrument instrument = detail::create_instrument(listing);

  EXPECT_THAT(instrument.max_quantity, Optional(Eq(MaxQuantity{42.42})));
}

TEST(InstrumentsCreateInstrument, WithoutMaximalQuantity) {
  const Listing listing = Listing::create(make_listing_patch(), 43);

  const Instrument instrument = detail::create_instrument(listing);

  EXPECT_EQ(instrument.max_quantity, std::nullopt);
}

// NOLINTEND(*-magic-numbers)

}  // namespace
}  // namespace simulator::trading_system::instrument::test