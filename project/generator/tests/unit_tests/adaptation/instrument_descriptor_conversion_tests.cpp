#include <gmock/gmock.h>

#include <optional>

#include "core/domain/attributes.hpp"
#include "core/domain/party.hpp"
#include "ih/adaptation/protocol_conversion.hpp"

namespace Simulator::Generator {
namespace {

using namespace testing;  // NOLINT

struct GeneratorInstrumentDescriptorConversion : public Test {
  DataLayer::Listing::Patch patch;
  static constexpr int listing_id = 42;

  GeneratorInstrumentDescriptorConversion() { patch.withVenueId("Venue"); }
};

TEST_F(GeneratorInstrumentDescriptorConversion, ConvertsSymbol) {
  patch.withSymbol("AAPL");
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  const auto descriptor = convert_to_instrument_descriptor(listing);

  ASSERT_THAT(descriptor.symbol, Optional(Eq(simulator::Symbol{"AAPL"})));
}

TEST_F(GeneratorInstrumentDescriptorConversion, IgnoresAbsentSymbol) {
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  const auto descriptor = convert_to_instrument_descriptor(listing);

  ASSERT_THAT(descriptor.symbol, Eq(std::nullopt));
}

TEST_F(GeneratorInstrumentDescriptorConversion, IgnoresEmptySymbol) {
  patch.withSymbol({});
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  const auto descriptor = convert_to_instrument_descriptor(listing);

  ASSERT_THAT(descriptor.symbol, Eq(std::nullopt));
}

TEST_F(GeneratorInstrumentDescriptorConversion, CovertsSupportedSecurityType) {
  patch.withSecurityType("CS");
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  const auto descriptor = convert_to_instrument_descriptor(listing);

  ASSERT_THAT(descriptor.security_type,
              Optional(Eq(simulator::SecurityType::Option::CommonStock)));
}

TEST_F(GeneratorInstrumentDescriptorConversion, IgnoresUnknownSecurityType) {
  patch.withSecurityType("UNKNOWN");
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  const auto descriptor = convert_to_instrument_descriptor(listing);

  ASSERT_THAT(descriptor.security_type, Eq(std::nullopt));
}

TEST_F(GeneratorInstrumentDescriptorConversion, IgnoresAbsentSecurityType) {
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  const auto descriptor = convert_to_instrument_descriptor(listing);

  ASSERT_THAT(descriptor.security_type, Eq(std::nullopt));
}

TEST_F(GeneratorInstrumentDescriptorConversion,
       ConvertsPriceCurrencyInEqListing) {
  patch.withSecurityType("CS").withPriceCurrency("USD");
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  const auto descriptor = convert_to_instrument_descriptor(listing);

  ASSERT_THAT(descriptor.currency, Optional(Eq(simulator::Currency{"USD"})));
}

TEST_F(GeneratorInstrumentDescriptorConversion,
       ConvertsFxBaseCurrencyInFxListing) {
  patch.withSecurityType("FXSPOT").withFxBaseCurrency("EUR");
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  const auto descriptor = convert_to_instrument_descriptor(listing);

  ASSERT_THAT(descriptor.currency, Optional(Eq(simulator::Currency{"EUR"})));
}

TEST_F(GeneratorInstrumentDescriptorConversion,
       IgnoresAbsentPriceCurrencyInEqListing) {
  patch.withSecurityType("CS");
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  const auto descriptor = convert_to_instrument_descriptor(listing);

  ASSERT_THAT(descriptor.currency, Eq(std::nullopt));
}

TEST_F(GeneratorInstrumentDescriptorConversion,
       IgnoresAbsentFxBaseCurrencyInFxListing) {
  patch.withSecurityType("FXFWD");
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  const auto descriptor = convert_to_instrument_descriptor(listing);

  ASSERT_THAT(descriptor.currency, Eq(std::nullopt));
}

TEST_F(GeneratorInstrumentDescriptorConversion,
       IgnoresCurrencyWithoutSecurityType) {
  patch.withPriceCurrency("USD").withFxBaseCurrency("EUR");
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  const auto descriptor = convert_to_instrument_descriptor(listing);

  ASSERT_THAT(descriptor.currency, Eq(std::nullopt));
}

TEST_F(GeneratorInstrumentDescriptorConversion, ConvertsSecurityExchange) {
  patch.withSecurityExchange("XLON");
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  const auto descriptor = convert_to_instrument_descriptor(listing);

  ASSERT_THAT(descriptor.security_exchange,
              Optional(Eq(simulator::SecurityExchange{"XLON"})));
}

TEST_F(GeneratorInstrumentDescriptorConversion, IgnoresEmptySecurityExchange) {
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  const auto descriptor = convert_to_instrument_descriptor(listing);

  ASSERT_THAT(descriptor.security_exchange, Eq(std::nullopt));
}

TEST_F(GeneratorInstrumentDescriptorConversion, ConvertsListingParty) {
  patch.withPartyId("PARTY").withPartyRole("ExecutingFirm");
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  const auto descriptor = convert_to_instrument_descriptor(listing);

  ASSERT_THAT(descriptor.parties,
              ElementsAre(AllOf(
                  Property(&simulator::Party::party_id,
                           Eq(simulator::PartyId{"PARTY"})),
                  Property(&simulator::Party::role,
                           Eq(simulator::PartyRole::Option::ExecutingFirm)))));
}

TEST_F(GeneratorInstrumentDescriptorConversion,
       SetsPartyIdSourceProprietaryInListingParty) {
  patch.withPartyId("PARTY").withPartyRole("ExecutingFirm");
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  const auto descriptor = convert_to_instrument_descriptor(listing);

  ASSERT_THAT(
      descriptor.parties,
      ElementsAre(Property(&simulator::Party::source,
                           Eq(simulator::PartyIdSource::Option::Proprietary))));
}

TEST_F(GeneratorInstrumentDescriptorConversion, IgnoresPartyWithUnknownRole) {
  patch.withPartyId("PARTY").withPartyRole("UNKNOWN_ROLE");
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  const auto descriptor = convert_to_instrument_descriptor(listing);

  ASSERT_THAT(descriptor.parties, IsEmpty());
}

TEST_F(GeneratorInstrumentDescriptorConversion, IgnoresPartyWithoutRole) {
  patch.withPartyId("PARTY");
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  const auto descriptor = convert_to_instrument_descriptor(listing);

  ASSERT_THAT(descriptor.parties, IsEmpty());
}

TEST_F(GeneratorInstrumentDescriptorConversion, IgnoresPartyWithoutPartyId) {
  patch.withPartyRole("ExecutingFirm");
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  const auto descriptor = convert_to_instrument_descriptor(listing);

  ASSERT_THAT(descriptor.parties, IsEmpty());
}

TEST_F(GeneratorInstrumentDescriptorConversion, ConvertsCusipIdentifier) {
  patch.withCusipId("CUSIP");
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  const auto descriptor = convert_to_instrument_descriptor(listing);

  EXPECT_THAT(descriptor.security_id,
              Optional(Eq(simulator::SecurityId{"CUSIP"})));
  EXPECT_THAT(descriptor.security_id_source,
              Optional(Eq(simulator::SecurityIdSource::Option::Cusip)));
}

TEST_F(GeneratorInstrumentDescriptorConversion, ConvertsSedolIdentifier) {
  patch.withSedolId("SEDOL");
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  const auto descriptor = convert_to_instrument_descriptor(listing);

  EXPECT_THAT(descriptor.security_id,
              Optional(Eq(simulator::SecurityId{"SEDOL"})));
  EXPECT_THAT(descriptor.security_id_source,
              Optional(Eq(simulator::SecurityIdSource::Option::Sedol)));
}

TEST_F(GeneratorInstrumentDescriptorConversion, ConvertsIsinIdentifier) {
  patch.withIsinId("ISIN");
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  const auto descriptor = convert_to_instrument_descriptor(listing);

  EXPECT_THAT(descriptor.security_id,
              Optional(Eq(simulator::SecurityId{"ISIN"})));
  EXPECT_THAT(descriptor.security_id_source,
              Optional(Eq(simulator::SecurityIdSource::Option::Isin)));
}

TEST_F(GeneratorInstrumentDescriptorConversion, ConvertsRicIdentifier) {
  patch.withRicId("RIC");
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  const auto descriptor = convert_to_instrument_descriptor(listing);

  EXPECT_THAT(descriptor.security_id,
              Optional(Eq(simulator::SecurityId{"RIC"})));
  EXPECT_THAT(descriptor.security_id_source,
              Optional(Eq(simulator::SecurityIdSource::Option::Ric)));
}

TEST_F(GeneratorInstrumentDescriptorConversion, ConvertsExchangeIdentifier) {
  patch.withExchangeSymbolId("EXC");
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  const auto descriptor = convert_to_instrument_descriptor(listing);

  EXPECT_THAT(descriptor.security_id,
              Optional(Eq(simulator::SecurityId{"EXC"})));
  EXPECT_THAT(
      descriptor.security_id_source,
      Optional(Eq(simulator::SecurityIdSource::Option::ExchangeSymbol)));
}

TEST_F(GeneratorInstrumentDescriptorConversion, ConvertsBloombergIdentifier) {
  patch.withBloombergSymbolId("BBG");
  const auto listing = DataLayer::Listing::create(patch, listing_id);

  const auto descriptor = convert_to_instrument_descriptor(listing);

  EXPECT_THAT(descriptor.security_id,
              Optional(Eq(simulator::SecurityId{"BBG"})));
  EXPECT_THAT(
      descriptor.security_id_source,
      Optional(Eq(simulator::SecurityIdSource::Option::BloombergSymbol)));
}

TEST_F(GeneratorInstrumentDescriptorConversion, IgnoresEmptySecurityId) {
  const auto listing = DataLayer::Listing::create(patch, listing_id);
  const auto descriptor = convert_to_instrument_descriptor(listing);

  EXPECT_THAT(descriptor.security_id, Eq(std::nullopt));
  EXPECT_THAT(descriptor.security_id_source, Eq(std::nullopt));
}

}  // namespace
}  // namespace Simulator::Generator