#include <string>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "api/inspectors/listing.hpp"
#include "api/models/listing.hpp"
#include "common/marshaller.hpp"

namespace Simulator::DataLayer {
namespace {

using namespace testing;

// NOLINTBEGIN(*magic-numbers*)

class DataLayer_Inspectors_ListingReader : public ::testing::Test {
  public:
    using MarshallerType = Marshaller<Listing>;
    using ReaderType = ListingReader<MarshallerType>;

    using Attribute = Listing::Attribute;

    auto marshaller() -> MarshallerType& { return mMarshaller; }

    auto makeReader() -> ReaderType { return ReaderType{mMarshaller}; }

    static auto makeDefaultPatch() -> Listing::Patch
    {
        return Listing::Patch{}.withVenueId("dummy");
    }

  protected:
    void SetUp() override
    {
        EXPECT_CALL(marshaller(), uint64(Eq(Attribute::ListingId), _))
            .Times(AnyNumber());

        EXPECT_CALL(marshaller(), string(Eq(Attribute::VenueId), _))
            .Times(AnyNumber());
    }

  private:
    MarshallerType mMarshaller;
};

class DataLayer_Inspectors_ListingPatchReader : public ::testing::Test {
  public:
    using MarshallerType = Marshaller<Listing>;
    using ReaderType = ListingPatchReader<MarshallerType>;

    using Attribute = Listing::Attribute;

    auto marshaller() -> MarshallerType& { return mMarshaller; }

    auto makeReader() -> ReaderType { return ReaderType{mMarshaller}; }

  private:
    MarshallerType mMarshaller;
};

class DataLayer_Inspectors_ListingPatchWriter : public ::testing::Test {
  public:
    using UnmarshallerType = Unmarshaller<Listing>;
    using WriterType = ListingPatchWriter<UnmarshallerType>;

    using Attribute = Listing::Attribute;

    auto unmarshaller() -> UnmarshallerType& { return mUnmarshaller; }

    auto makeWriter() -> WriterType { return WriterType{mUnmarshaller}; }

  protected:
    auto SetUp() -> void override
    {
        EXPECT_CALL(unmarshaller(), boolean)
            .Times(AnyNumber())
            .WillRepeatedly(Return(false));

        EXPECT_CALL(unmarshaller(), uint32)
            .Times(AnyNumber())
            .WillRepeatedly(Return(false));

        EXPECT_CALL(unmarshaller(), real)
            .Times(AnyNumber())
            .WillRepeatedly(Return(false));

        EXPECT_CALL(unmarshaller(), string)
            .Times(AnyNumber())
            .WillRepeatedly(Return(false));
    }

  private:
    UnmarshallerType mUnmarshaller;
};


TEST_F(DataLayer_Inspectors_ListingReader, Read_ListingID)
{
    auto const patch = makeDefaultPatch();
    auto const listing = Listing::create(patch, 42);
    ASSERT_EQ(listing.getListingId(), 42);

    EXPECT_CALL(marshaller(), uint64(Eq(Attribute::ListingId), Eq(42)))
        .Times(1);

    makeReader().read(listing);
}

TEST_F(DataLayer_Inspectors_ListingReader, Read_Symbol)
{
    auto const patch = makeDefaultPatch().withSymbol("AAPL");
    auto const listing = Listing::create(patch, 42);
    ASSERT_THAT(listing.getSymbol(), Optional(Eq("AAPL")));

    EXPECT_CALL(marshaller(), string(Eq(Attribute::Symbol), Eq("AAPL")))
        .Times(1);

    makeReader().read(listing);
}

TEST_F(DataLayer_Inspectors_ListingReader, Read_VenueID)
{
    auto const patch = makeDefaultPatch().withVenueId("NASDAQ");
    auto const listing = Listing::create(patch, 42);
    ASSERT_EQ(listing.getVenueId(), "NASDAQ");

    EXPECT_CALL(marshaller(), string(Eq(Attribute::VenueId), Eq("NASDAQ")))
        .Times(1);

    makeReader().read(listing);
}

TEST_F(DataLayer_Inspectors_ListingReader, Read_SecurityType)
{
    auto const patch = makeDefaultPatch().withSecurityType("Equity");
    auto const listing = Listing::create(patch, 42);
    ASSERT_THAT(listing.getSecurityType(), Optional(Eq("Equity")));

    EXPECT_CALL(marshaller(), string(Eq(Attribute::SecurityType), Eq("Equity")))
        .Times(1);

    makeReader().read(listing);
}

TEST_F(DataLayer_Inspectors_ListingReader, Read_PriceCurrency)
{
    auto const patch = makeDefaultPatch().withPriceCurrency("USD");
    auto const listing = Listing::create(patch, 42);
    ASSERT_THAT(listing.getPriceCurrency(), Optional(Eq("USD")));

    EXPECT_CALL(marshaller(), string(Eq(Attribute::PriceCurrency), Eq("USD")))
        .Times(1);

    makeReader().read(listing);
}

TEST_F(DataLayer_Inspectors_ListingReader, Read_FxBaseCurrency)
{
    auto const patch = makeDefaultPatch().withFxBaseCurrency("USD");
    auto const listing = Listing::create(patch, 42);
    ASSERT_THAT(listing.getFxBaseCurrency(), Optional(Eq("USD")));

    EXPECT_CALL(marshaller(), string(Eq(Attribute::FxBaseCurrency), Eq("USD")))
        .Times(1);

    makeReader().read(listing);
}

TEST_F(DataLayer_Inspectors_ListingReader, Read_InstrSymbol)
{
    auto const patch = makeDefaultPatch().withInstrSymbol("AAPL");
    auto const listing = Listing::create(patch, 42);
    ASSERT_THAT(listing.getInstrSymbol(), Optional(Eq("AAPL")));

    EXPECT_CALL(marshaller(), string(Eq(Attribute::InstrSymbol), Eq("AAPL")))
        .Times(1);

    makeReader().read(listing);
}

TEST_F(DataLayer_Inspectors_ListingReader, Read_QtyMinimum)
{
    auto const patch = makeDefaultPatch().withQtyMinimum(42.42);
    auto const listing = Listing::create(patch, 42);
    ASSERT_THAT(listing.getQtyMinimum(), Optional(DoubleEq(42.42)));

    EXPECT_CALL(marshaller(), real(Eq(Attribute::QtyMinimum), DoubleEq(42.42)))
        .Times(1);

    makeReader().read(listing);
}

TEST_F(DataLayer_Inspectors_ListingReader, Read_QtyMaximum)
{
    auto const patch = makeDefaultPatch().withQtyMaximum(42.42);
    auto const listing = Listing::create(patch, 42);
    ASSERT_THAT(listing.getQtyMaximum(), Optional(DoubleEq(42.42)));

    EXPECT_CALL(marshaller(), real(Eq(Attribute::QtyMaximum), DoubleEq(42.42)))
        .Times(1);

    makeReader().read(listing);
}

TEST_F(DataLayer_Inspectors_ListingReader, Read_QtyMultiple)
{
    auto const patch = makeDefaultPatch().withQtyMultiple(42.42);
    auto const listing = Listing::create(patch, 42);
    ASSERT_THAT(listing.getQtyMultiple(), Optional(DoubleEq(42.42)));

    EXPECT_CALL(marshaller(), real(Eq(Attribute::QtyMultiple), DoubleEq(42.42)))
        .Times(1);

    makeReader().read(listing);
}

TEST_F(DataLayer_Inspectors_ListingReader, Read_PriceTickSize)
{
    auto const patch = makeDefaultPatch().withPriceTickSize(42.42);
    auto const listing = Listing::create(patch, 42);
    ASSERT_THAT(listing.getPriceTickSize(), Optional(DoubleEq(42.42)));

    auto& expect = EXPECT_CALL(
        marshaller(),
        real(Eq(Attribute::PriceTickSize), DoubleEq(42.42))
    );
    expect.Times(1);

    makeReader().read(listing);
}

TEST_F(DataLayer_Inspectors_ListingReader, Read_Enabled)
{
    auto const patch = makeDefaultPatch().withEnabledFlag(true);
    auto const listing = Listing::create(patch, 42);
    ASSERT_THAT(listing.getEnabledFlag(), Optional(Eq(true)));

    EXPECT_CALL(marshaller(), boolean(Eq(Attribute::Enabled), Eq(true)))
        .Times(1);

    makeReader().read(listing);
}

TEST_F(DataLayer_Inspectors_ListingReader, Read_RandomQtyMaximum)
{
    auto const patch = makeDefaultPatch().withRandomQtyMaximum(42.42);
    auto const listing = Listing::create(patch, 42);
    ASSERT_THAT(listing.getRandomQtyMaximum(), Optional(DoubleEq(42.42)));

    auto& expect = EXPECT_CALL(
        marshaller(),
        real(Eq(Attribute::RandomQtyMaximum), DoubleEq(42.42))
    );
    expect.Times(1);

    makeReader().read(listing);
}

TEST_F(DataLayer_Inspectors_ListingReader, Read_RandomDepthLevels)
{
    auto const patch = makeDefaultPatch().withRandomDepthLevels(42);
    auto const listing = Listing::create(patch, 42);
    ASSERT_THAT(listing.getRandomDepthLevels(), Optional(Eq(42)));

    auto& expect = EXPECT_CALL(
        marshaller(),
        uint32(Eq(Attribute::RandomDepthLevels), Eq(42))
    );
    expect.Times(1);

    makeReader().read(listing);
}

TEST_F(DataLayer_Inspectors_ListingReader, Read_RandomOrdersSpread)
{
    auto const patch = makeDefaultPatch().withRandomOrdersSpread(42.42);
    auto const listing = Listing::create(patch, 42);
    ASSERT_THAT(listing.getRandomOrdersSpread(), Optional(DoubleEq(42.42)));

    auto& expect = EXPECT_CALL(
        marshaller(),
        real(Eq(Attribute::RandomOrdersSpread), DoubleEq(42.42))
    );
    expect.Times(1);

    makeReader().read(listing);
}

TEST_F(DataLayer_Inspectors_ListingReader, Read_RandomOrdersRate)
{
    auto const patch = makeDefaultPatch().withRandomOrdersRate(42);
    auto const listing = Listing::create(patch, 42);
    ASSERT_THAT(listing.getRandomOrdersRate(), Optional(Eq(42)));

    auto& expect = EXPECT_CALL(
        marshaller(),
        uint32(Eq(Attribute::RandomOrdersRate), Eq(42))
    );
    expect.Times(1);

    makeReader().read(listing);
}

TEST_F(DataLayer_Inspectors_ListingReader, Read_RandomTickRange)
{
    auto const patch = makeDefaultPatch().withRandomTickRange(42);
    auto const listing = Listing::create(patch, 42);
    ASSERT_THAT(listing.getRandomTickRange(), Optional(Eq(42)));

    auto& expect = EXPECT_CALL(
        marshaller(),
        uint32(Eq(Attribute::RandomTickRange), Eq(42))
    );
    expect.Times(1);

    makeReader().read(listing);
}

TEST_F(DataLayer_Inspectors_ListingReader, Read_SecurityExchange)
{
    auto const patch = makeDefaultPatch().withSecurityExchange("NASDAQ");
    auto const listing = Listing::create(patch, 42);
    ASSERT_THAT(listing.getSecurityExchange(), Optional(Eq("NASDAQ")));

    auto& expect = EXPECT_CALL(
        marshaller(),
        string(Eq(Attribute::SecurityExchange), Eq("NASDAQ"))
    );
    expect.Times(1);

    makeReader().read(listing);
}

TEST_F(DataLayer_Inspectors_ListingReader, Read_PartyID)
{
    auto const patch = makeDefaultPatch().withPartyId("GGV");
    auto const listing = Listing::create(patch, 42);
    ASSERT_THAT(listing.getPartyId(), Optional(Eq("GGV")));

    EXPECT_CALL(marshaller(), string(Eq(Attribute::PartyId), Eq("GGV")))
        .Times(1);

    makeReader().read(listing);
}

TEST_F(DataLayer_Inspectors_ListingReader, Read_PartyRole)
{
    auto const patch = makeDefaultPatch().withPartyRole("Role");
    auto const listing = Listing::create(patch, 42);
    ASSERT_THAT(listing.getPartyRole(), Optional(Eq("Role")));

    EXPECT_CALL(marshaller(), string(Eq(Attribute::PartyRole), Eq("Role")))
        .Times(1);

    makeReader().read(listing);
}

TEST_F(DataLayer_Inspectors_ListingReader, Read_CusipID)
{
    auto const patch = makeDefaultPatch().withCusipId("Cusip");
    auto const listing = Listing::create(patch, 42);
    ASSERT_THAT(listing.getCusipId(), Optional(Eq("Cusip")));

    EXPECT_CALL(marshaller(), string(Eq(Attribute::CusipId), Eq("Cusip")))
        .Times(1);

    makeReader().read(listing);
}

TEST_F(DataLayer_Inspectors_ListingReader, Read_SedolID)
{
    auto const patch = makeDefaultPatch().withSedolId("Sedol");
    auto const listing = Listing::create(patch, 42);
    ASSERT_THAT(listing.getSedolId(), Optional(Eq("Sedol")));

    EXPECT_CALL(marshaller(), string(Eq(Attribute::SedolId), Eq("Sedol")))
        .Times(1);

    makeReader().read(listing);
}

TEST_F(DataLayer_Inspectors_ListingReader, Read_IsinID)
{
    auto const patch = makeDefaultPatch().withIsinId("Isin");
    auto const listing = Listing::create(patch, 42);
    ASSERT_THAT(listing.getIsinId(), Optional(Eq("Isin")));

    EXPECT_CALL(marshaller(), string(Eq(Attribute::IsinId), Eq("Isin")))
        .Times(1);

    makeReader().read(listing);
}

TEST_F(DataLayer_Inspectors_ListingReader, Read_RicID)
{
    auto const patch = makeDefaultPatch().withRicId("Ric");
    auto const listing = Listing::create(patch, 42);
    ASSERT_THAT(listing.getRicId(), Optional(Eq("Ric")));

    EXPECT_CALL(marshaller(), string(Eq(Attribute::RicId), Eq("Ric"))).Times(1);

    makeReader().read(listing);
}

TEST_F(DataLayer_Inspectors_ListingReader, Read_ExchangeSymbolID)
{
    auto const patch = makeDefaultPatch().withExchangeSymbolId("EX");
    auto const listing = Listing::create(patch, 42);
    ASSERT_THAT(listing.getExchangeSymbolId(), Optional(Eq("EX")));

    EXPECT_CALL(marshaller(), string(Eq(Attribute::ExchangeSymbolId), Eq("EX")))
        .Times(1);

    makeReader().read(listing);
}

TEST_F(DataLayer_Inspectors_ListingReader, Read_BloombergSymbolID)
{
    auto const patch = makeDefaultPatch().withBloombergSymbolId("B");
    auto const listing = Listing::create(patch, 42);
    ASSERT_THAT(listing.getBloombergSymbolId(), Optional(Eq("B")));

    EXPECT_CALL(marshaller(), string(Eq(Attribute::BloombergSymbolId), Eq("B")))
        .Times(1);

    makeReader().read(listing);
}

TEST_F(DataLayer_Inspectors_ListingReader, Read_RandomQtyMinimum)
{
    auto const patch = makeDefaultPatch().withRandomQtyMinimum(42.42);
    auto const listing = Listing::create(patch, 42);
    ASSERT_THAT(listing.getRandomQtyMinimum(), Optional(DoubleEq(42.42)));


    auto& expect = EXPECT_CALL(
        marshaller(),
        real(Eq(Attribute::RandomQtyMinimum), DoubleEq(42.42))
    );
    expect.Times(1);

    makeReader().read(listing);
}

TEST_F(DataLayer_Inspectors_ListingReader, Read_RandomAmtMinimum)
{
    auto const patch = makeDefaultPatch().withRandomAmtMinimum(42.42);
    auto const listing = Listing::create(patch, 42);
    ASSERT_THAT(listing.getRandomAmtMinimum(), Optional(DoubleEq(42.42)));

    auto& expect = EXPECT_CALL(
        marshaller(),
        real(Eq(Attribute::RandomAmtMinimum), DoubleEq(42.42))
    );
    expect.Times(1);

    makeReader().read(listing);
}

TEST_F(DataLayer_Inspectors_ListingReader, Read_RandomAmtMaximum)
{
    auto const patch = makeDefaultPatch().withRandomAmtMaximum(42.42);
    auto const listing = Listing::create(patch, 42);
    ASSERT_THAT(listing.getRandomAmtMaximum(), Optional(DoubleEq(42.42)));

    auto& expect = EXPECT_CALL(
        marshaller(),
        real(Eq(Attribute::RandomAmtMaximum), DoubleEq(42.42))
    );
    expect.Times(1);

    makeReader().read(listing);
}

TEST_F(DataLayer_Inspectors_ListingReader, Read_RandomOrdersEnabled)
{
    auto const patch = makeDefaultPatch().withRandomOrdersEnabledFlag(true);
    auto const listing = Listing::create(patch, 42);
    ASSERT_THAT(listing.getRandomOrdersEnabledFlag(), Optional(Eq(true)));

    auto& expect = EXPECT_CALL(
        marshaller(),
        boolean(Eq(Attribute::RandomOrdersEnabled), Eq(true))
    );
    expect.Times(1);

    makeReader().read(listing);
}

TEST_F(DataLayer_Inspectors_ListingReader, Read_RandomAggressiveQtyMinimum)
{
    auto const patch = makeDefaultPatch().withRandomAggressiveQtyMinimum(42.42);
    auto const listing = Listing::create(patch, 42);
    ASSERT_THAT(listing.getRandomAggressiveQtyMinimum(), Optional(DoubleEq(42.42)));

    auto& expect = EXPECT_CALL(
        marshaller(),
        real(Eq(Attribute::RandomAggressiveQtyMinimum), DoubleEq(42.42))
    );
    expect.Times(1);

    makeReader().read(listing);
}

TEST_F(DataLayer_Inspectors_ListingReader, Read_RandomAggressiveQtyMaximum)
{
    auto const patch = makeDefaultPatch().withRandomAggressiveQtyMaximum(42.42);
    auto const listing = Listing::create(patch, 42);
    ASSERT_THAT(listing.getRandomAggressiveQtyMaximum(), Optional(DoubleEq(42.42)));

    auto& expect = EXPECT_CALL(
        marshaller(),
        real(Eq(Attribute::RandomAggressiveQtyMaximum), DoubleEq(42.42))
    );
    expect.Times(1);

    makeReader().read(listing);
}

TEST_F(DataLayer_Inspectors_ListingReader, Read_RandomAggressiveAmtMinimum)
{
    auto const patch = makeDefaultPatch().withRandomAggressiveAmtMinimum(42.42);
    auto const listing = Listing::create(patch, 42);
    ASSERT_THAT(listing.getRandomAggressiveAmtMinimum(), Optional(DoubleEq(42.42)));

    auto& expect = EXPECT_CALL(
        marshaller(),
        real(Eq(Attribute::RandomAggressiveAmtMinimum), DoubleEq(42.42))
    );
    expect.Times(1);

    makeReader().read(listing);
}

TEST_F(DataLayer_Inspectors_ListingReader, Read_RandomAggressiveAmtMaximum)
{
    auto const patch = makeDefaultPatch().withRandomAggressiveAmtMaximum(42.42);
    auto const listing = Listing::create(patch, 42);
    ASSERT_THAT(listing.getRandomAggressiveAmtMaximum(), Optional(DoubleEq(42.42)));

    auto& expect = EXPECT_CALL(
        marshaller(),
        real(Eq(Attribute::RandomAggressiveAmtMaximum), DoubleEq(42.42))
    );
    expect.Times(1);

    makeReader().read(listing);
}


TEST_F(DataLayer_Inspectors_ListingPatchReader, Read_Symbol)
{
    Listing::Patch patch{};
    patch.withSymbol("AAPL");
    ASSERT_THAT(patch.getSymbol(), Optional(Eq("AAPL")));

    EXPECT_CALL(marshaller(), string(Eq(Attribute::Symbol), Eq("AAPL")))
        .Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_ListingPatchReader, Read_VenueID)
{
    Listing::Patch patch{};
    patch.withVenueId("NASDAQ");
    ASSERT_THAT(patch.getVenueId(), Optional(Eq("NASDAQ")));

    EXPECT_CALL(marshaller(), string(Eq(Attribute::VenueId), Eq("NASDAQ")))
        .Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_ListingPatchReader, Read_SecurityType)
{
    Listing::Patch patch{};
    patch.withSecurityType("CS");
    ASSERT_THAT(patch.getSecurityType(), Optional(Eq("CS")));

    EXPECT_CALL(marshaller(), string(Eq(Attribute::SecurityType), Eq("CS")))
        .Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_ListingPatchReader, Read_PriceCurrency)
{
    Listing::Patch patch{};
    patch.withPriceCurrency("USD");
    ASSERT_THAT(patch.getPriceCurrency(), Optional(Eq("USD")));

    EXPECT_CALL(marshaller(), string(Eq(Attribute::PriceCurrency), Eq("USD")))
        .Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_ListingPatchReader, Read_FxBaseCurrency)
{
    Listing::Patch patch{};
    patch.withFxBaseCurrency("USD");
    ASSERT_THAT(patch.getFxBaseCurrency(), Optional(Eq("USD")));

    EXPECT_CALL(marshaller(), string(Eq(Attribute::FxBaseCurrency), Eq("USD")))
        .Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_ListingPatchReader, Read_InstrSymbol)
{
    Listing::Patch patch{};
    patch.withInstrSymbol("AAPL");
    ASSERT_THAT(patch.getInstrSymbol(), Optional(Eq("AAPL")));

    EXPECT_CALL(marshaller(), string(Eq(Attribute::InstrSymbol), Eq("AAPL")))
        .Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_ListingPatchReader, Read_QtyMinimum)
{
    Listing::Patch patch{};
    patch.withQtyMinimum(42.42);
    ASSERT_THAT(patch.getQtyMinimum(), Optional(DoubleEq(42.42)));

    EXPECT_CALL(marshaller(), real(Eq(Attribute::QtyMinimum), DoubleEq(42.42)))
        .Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_ListingPatchReader, Read_QtyMaximum)
{
    Listing::Patch patch{};
    patch.withQtyMaximum(42.42);
    ASSERT_THAT(patch.getQtyMaximum(), Optional(DoubleEq(42.42)));

    EXPECT_CALL(marshaller(), real(Eq(Attribute::QtyMaximum), DoubleEq(42.42)))
        .Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_ListingPatchReader, Read_QtyMultiple)
{
    Listing::Patch patch{};
    patch.withQtyMultiple(42.42);
    ASSERT_THAT(patch.getQtyMultiple(), Optional(DoubleEq(42.42)));

    EXPECT_CALL(marshaller(), real(Eq(Attribute::QtyMultiple), DoubleEq(42.42)))
        .Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_ListingPatchReader, Read_PriceTickSize)
{
    Listing::Patch patch{};
    patch.withPriceTickSize(42.42);
    ASSERT_THAT(patch.getPriceTickSize(), Optional(DoubleEq(42.42)));

    auto& expect = EXPECT_CALL(
        marshaller(),
        real(Eq(Attribute::PriceTickSize), DoubleEq(42.42))
    );
    expect.Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_ListingPatchReader, Read_Enabled)
{
    Listing::Patch patch{};
    patch.withEnabledFlag(true);
    ASSERT_THAT(patch.getEnabledFlag(), Optional(Eq(true)));

    EXPECT_CALL(marshaller(), boolean(Eq(Attribute::Enabled), Eq(true)))
        .Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_ListingPatchReader, Read_RandomQtyMaximum)
{
    Listing::Patch patch{};
    patch.withRandomQtyMaximum(42.42);
    ASSERT_THAT(patch.getRandomQtyMaximum(), Optional(DoubleEq(42.42)));

    auto& expect = EXPECT_CALL(
        marshaller(),
        real(Eq(Attribute::RandomQtyMaximum), DoubleEq(42.42))
    );
    expect.Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_ListingPatchReader, Read_RandomDepthLevels)
{
    Listing::Patch patch{};
    patch.withRandomDepthLevels(42);
    ASSERT_THAT(patch.getRandomDepthLevels(), Optional(Eq(42)));

    auto& expect = EXPECT_CALL(
        marshaller(),
        uint32(Eq(Attribute::RandomDepthLevels), Eq(42))
    );
    expect.Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_ListingPatchReader, Read_RandomOrdersSpread)
{
    Listing::Patch patch{};
    patch.withRandomOrdersSpread(42.42);
    ASSERT_THAT(patch.getRandomOrdersSpread(), Optional(DoubleEq(42.42)));

    auto& expect = EXPECT_CALL(
        marshaller(),
        real(Eq(Attribute::RandomOrdersSpread), DoubleEq(42.42))
    );
    expect.Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_ListingPatchReader, Read_RandomOrdersRate)
{
    Listing::Patch patch{};
    patch.withRandomOrdersRate(42);
    ASSERT_THAT(patch.getRandomOrdersRate(), Optional(Eq(42)));

    auto& expect = EXPECT_CALL(
        marshaller(),
        uint32(Eq(Attribute::RandomOrdersRate), Eq(42))
    );
    expect.Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_ListingPatchReader, Read_RandomTickRange)
{
    Listing::Patch patch{};
    patch.withRandomTickRange(42);
    ASSERT_THAT(patch.getRandomTickRange(), Optional(Eq(42)));

    auto& expect = EXPECT_CALL(
        marshaller(),
        uint32(Eq(Attribute::RandomTickRange), Eq(42))
    );
    expect.Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_ListingPatchReader, Read_SecurityExchange)
{
    Listing::Patch patch{};
    patch.withSecurityExchange("NASDAQ");
    ASSERT_THAT(patch.getSecurityExchange(), Optional(Eq("NASDAQ")));

    auto& expect = EXPECT_CALL(
        marshaller(),
        string(Eq(Attribute::SecurityExchange), Eq("NASDAQ"))
    );
    expect.Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_ListingPatchReader, Read_PartyID)
{
    Listing::Patch patch{};
    patch.withPartyId("GGV");
    ASSERT_THAT(patch.getPartyId(), Optional(Eq("GGV")));

    EXPECT_CALL(marshaller(), string(Eq(Attribute::PartyId), Eq("GGV")))
        .Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_ListingPatchReader, Read_PartyRole)
{
    Listing::Patch patch{};
    patch.withPartyRole("Role");
    ASSERT_THAT(patch.getPartyRole(), Optional(Eq("Role")));

    EXPECT_CALL(marshaller(), string(Eq(Attribute::PartyRole), Eq("Role")))
        .Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_ListingPatchReader, Read_CusipID)
{
    Listing::Patch patch{};
    patch.withCusipId("Cusip");
    ASSERT_THAT(patch.getCusipId(), Optional(Eq("Cusip")));

    EXPECT_CALL(marshaller(), string(Eq(Attribute::CusipId), Eq("Cusip")))
        .Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_ListingPatchReader, Read_SedolID)
{
    Listing::Patch patch{};
    patch.withSedolId("Sedol");
    ASSERT_THAT(patch.getSedolId(), Optional(Eq("Sedol")));

    EXPECT_CALL(marshaller(), string(Eq(Attribute::SedolId), Eq("Sedol")))
        .Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_ListingPatchReader, Read_IsinID)
{
    Listing::Patch patch{};
    patch.withIsinId("Isin");
    ASSERT_THAT(patch.getIsinId(), Optional(Eq("Isin")));

    EXPECT_CALL(marshaller(), string(Eq(Attribute::IsinId), Eq("Isin")))
        .Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_ListingPatchReader, Read_RicID)
{
    Listing::Patch patch{};
    patch.withRicId("Ric");
    ASSERT_THAT(patch.getRicId(), Optional(Eq("Ric")));

    EXPECT_CALL(marshaller(), string(Eq(Attribute::RicId), Eq("Ric"))).Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_ListingPatchReader, Read_ExchangeSymbolID)
{
    Listing::Patch patch{};
    patch.withExchangeSymbolId("EX");
    ASSERT_THAT(patch.getExchangeSymbolId(), Optional(Eq("EX")));

    EXPECT_CALL(marshaller(), string(Eq(Attribute::ExchangeSymbolId), Eq("EX")))
        .Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_ListingPatchReader, Read_BloombergSymbolID)
{
    Listing::Patch patch{};
    patch.withBloombergSymbolId("B");
    ASSERT_THAT(patch.getBloombergSymbolId(), Optional(Eq("B")));

    EXPECT_CALL(marshaller(), string(Eq(Attribute::BloombergSymbolId), Eq("B")))
        .Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_ListingPatchReader, Read_RandomQtyMinimum)
{
    Listing::Patch patch{};
    patch.withRandomQtyMinimum(42.42);
    ASSERT_THAT(patch.getRandomQtyMinimum(), Optional(DoubleEq(42.42)));

    auto& expect = EXPECT_CALL(
        marshaller(),
        real(Eq(Attribute::RandomQtyMinimum), DoubleEq(42.42))
    );
    expect.Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_ListingPatchReader, Read_RandomAmtMinimum)
{
    Listing::Patch patch{};
    patch.withRandomAmtMinimum(42.42);
    ASSERT_THAT(patch.getRandomAmtMinimum(), Optional(DoubleEq(42.42)));

    auto& expect = EXPECT_CALL(
        marshaller(),
        real(Eq(Attribute::RandomAmtMinimum), DoubleEq(42.42))
    );
    expect.Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_ListingPatchReader, Read_RandomAmtMaximum)
{
    Listing::Patch patch{};
    patch.withRandomAmtMaximum(42.42);
    ASSERT_THAT(patch.getRandomAmtMaximum(), Optional(DoubleEq(42.42)));

    auto& expect = EXPECT_CALL(
        marshaller(),
        real(Eq(Attribute::RandomAmtMaximum), DoubleEq(42.42))
    );
    expect.Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_ListingPatchReader, Read_RandomOrdersEnabled)
{
    Listing::Patch patch{};
    patch.withRandomOrdersEnabledFlag(false);
    ASSERT_THAT(patch.getRandomOrdersEnabledFlag(), Optional(Eq(false)));

    auto& expect = EXPECT_CALL(
        marshaller(),
        boolean(Eq(Attribute::RandomOrdersEnabled), Eq(false))
    );
    expect.Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_ListingPatchReader, Read_RandomAggressiveQtyMinimum)
{
    Listing::Patch patch;
    patch.withRandomAggressiveQtyMinimum(42.42);
    ASSERT_THAT(patch.getRandomAggressiveQtyMinimum(), Optional(DoubleEq(42.42)));

    auto& expect = EXPECT_CALL(
        marshaller(),
        real(Eq(Attribute::RandomAggressiveQtyMinimum), DoubleEq(42.42))
    );
    expect.Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_ListingPatchReader, Read_RandomAggressiveQtyMaximum)
{
    Listing::Patch patch;
    patch.withRandomAggressiveQtyMaximum(42.42);
    ASSERT_THAT(patch.getRandomAggressiveQtyMaximum(), Optional(DoubleEq(42.42)));

    auto& expect = EXPECT_CALL(
        marshaller(),
        real(Eq(Attribute::RandomAggressiveQtyMaximum), DoubleEq(42.42))
    );
    expect.Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_ListingPatchReader, Read_RandomAggressiveAmtMinimum)
{
    Listing::Patch patch;
    patch.withRandomAggressiveAmtMinimum(42.42);
    ASSERT_THAT(patch.getRandomAggressiveAmtMinimum(), Optional(DoubleEq(42.42)));

    auto& expect = EXPECT_CALL(
        marshaller(),
        real(Eq(Attribute::RandomAggressiveAmtMinimum), DoubleEq(42.42))
    );
    expect.Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_ListingPatchReader, Read_RandomAggressiveAmtMaximum)
{
    Listing::Patch patch;
    patch.withRandomAggressiveAmtMaximum(42.42);
    ASSERT_THAT(patch.getRandomAggressiveAmtMaximum(), Optional(DoubleEq(42.42)));

    auto& expect = EXPECT_CALL(
        marshaller(),
        real(Eq(Attribute::RandomAggressiveAmtMaximum), DoubleEq(42.42))
    );
    expect.Times(1);

    makeReader().read(patch);
}


TEST_F(DataLayer_Inspectors_ListingPatchWriter, Write_Symbol)
{
    Listing::Patch patch{};
    EXPECT_CALL(unmarshaller(), string(Eq(Attribute::Symbol), _))
        .WillOnce(DoAll(SetArgReferee<1>("AAPL"), Return(true)));

    makeWriter().write(patch);
    EXPECT_THAT(patch.getSymbol(), Optional(Eq("AAPL")));
}

TEST_F(DataLayer_Inspectors_ListingPatchWriter, Write_VenueID)
{
    Listing::Patch patch{};
    EXPECT_CALL(unmarshaller(), string(Eq(Attribute::VenueId), _))
        .WillOnce(DoAll(SetArgReferee<1>("NASDAQ"), Return(true)));

    makeWriter().write(patch);
    EXPECT_THAT(patch.getVenueId(), Optional(Eq("NASDAQ")));
}

TEST_F(DataLayer_Inspectors_ListingPatchWriter, Write_SecurityType)
{
    Listing::Patch patch{};
    EXPECT_CALL(unmarshaller(), string(Eq(Attribute::SecurityType), _))
        .WillOnce(DoAll(SetArgReferee<1>("CS"), Return(true)));

    makeWriter().write(patch);
    EXPECT_THAT(patch.getSecurityType(), Optional(Eq("CS")));
}

TEST_F(DataLayer_Inspectors_ListingPatchWriter, Write_PriceCurrency)
{
    Listing::Patch patch{};
    EXPECT_CALL(unmarshaller(), string(Eq(Attribute::PriceCurrency), _))
        .WillOnce(DoAll(SetArgReferee<1>("USD"), Return(true)));

    makeWriter().write(patch);
    EXPECT_THAT(patch.getPriceCurrency(), Optional(Eq("USD")));
}

TEST_F(DataLayer_Inspectors_ListingPatchWriter, Write_FxBaseCurrency)
{
    Listing::Patch patch{};
    EXPECT_CALL(unmarshaller(), string(Eq(Attribute::FxBaseCurrency), _))
        .WillOnce(DoAll(SetArgReferee<1>("USD"), Return(true)));

    makeWriter().write(patch);
    EXPECT_THAT(patch.getFxBaseCurrency(), Optional(Eq("USD")));
}

TEST_F(DataLayer_Inspectors_ListingPatchWriter, Write_InstrSymbol)
{
    Listing::Patch patch{};
    EXPECT_CALL(unmarshaller(), string(Eq(Attribute::InstrSymbol), _))
        .WillOnce(DoAll(SetArgReferee<1>("AAPL"), Return(true)));

    makeWriter().write(patch);
    EXPECT_THAT(patch.getInstrSymbol(), Optional(Eq("AAPL")));
}

TEST_F(DataLayer_Inspectors_ListingPatchWriter, Write_QtyMinimum)
{
    Listing::Patch patch{};
    EXPECT_CALL(unmarshaller(), real(Eq(Attribute::QtyMinimum), _))
        .WillOnce(DoAll(SetArgReferee<1>(42.42), Return(true)));

    makeWriter().write(patch);
    EXPECT_THAT(patch.getQtyMinimum(), Optional(DoubleEq(42.42)));
}

TEST_F(DataLayer_Inspectors_ListingPatchWriter, Write_QtyMaximum)
{
    Listing::Patch patch{};
    EXPECT_CALL(unmarshaller(), real(Eq(Attribute::QtyMaximum), _))
        .WillOnce(DoAll(SetArgReferee<1>(42.42), Return(true)));

    makeWriter().write(patch);
    EXPECT_THAT(patch.getQtyMaximum(), Optional(DoubleEq(42.42)));
}

TEST_F(DataLayer_Inspectors_ListingPatchWriter, Write_PriceTickSize)
{
    Listing::Patch patch{};
    EXPECT_CALL(unmarshaller(), real(Eq(Attribute::PriceTickSize), _))
        .WillOnce(DoAll(SetArgReferee<1>(42.42), Return(true)));

    makeWriter().write(patch);
    EXPECT_THAT(patch.getPriceTickSize(), Optional(DoubleEq(42.42)));
}

TEST_F(DataLayer_Inspectors_ListingPatchWriter, Write_Enabled)
{
    Listing::Patch patch{};
    EXPECT_CALL(unmarshaller(), boolean(Eq(Attribute::Enabled), _))
        .WillOnce(DoAll(SetArgReferee<1>(true), Return(true)));

    makeWriter().write(patch);
    EXPECT_THAT(patch.getEnabledFlag(), Optional(Eq(true)));
}

TEST_F(DataLayer_Inspectors_ListingPatchWriter, Write_RandomQtyMaximum)
{
    Listing::Patch patch{};
    EXPECT_CALL(unmarshaller(), real(Eq(Attribute::RandomQtyMaximum), _))
        .WillOnce(DoAll(SetArgReferee<1>(42.42), Return(true)));

    makeWriter().write(patch);
    EXPECT_THAT(patch.getRandomQtyMaximum(), Optional(DoubleEq(42.42)));
}

TEST_F(DataLayer_Inspectors_ListingPatchWriter, Write_RandomDepthLevels)
{
    Listing::Patch patch{};
    EXPECT_CALL(unmarshaller(), uint32(Eq(Attribute::RandomDepthLevels), _))
        .WillOnce(DoAll(SetArgReferee<1>(42), Return(true)));

    makeWriter().write(patch);
    EXPECT_THAT(patch.getRandomDepthLevels(), Optional(Eq(42)));
}

TEST_F(DataLayer_Inspectors_ListingPatchWriter, Write_RandomOrdersSpread)
{
    Listing::Patch patch{};
    EXPECT_CALL(unmarshaller(), real(Eq(Attribute::RandomOrdersSpread), _))
        .WillOnce(DoAll(SetArgReferee<1>(42.42), Return(true)));

    makeWriter().write(patch);
    EXPECT_THAT(patch.getRandomOrdersSpread(), Optional(DoubleEq(42.42)));
}

TEST_F(DataLayer_Inspectors_ListingPatchWriter, Write_RandomOrdersRate)
{
    Listing::Patch patch{};
    EXPECT_CALL(unmarshaller(), uint32(Eq(Attribute::RandomOrdersRate), _))
        .WillOnce(DoAll(SetArgReferee<1>(42), Return(true)));

    makeWriter().write(patch);
    EXPECT_THAT(patch.getRandomOrdersRate(), Optional(Eq(42)));
}

TEST_F(DataLayer_Inspectors_ListingPatchWriter, Write_RandomTickRange)
{
    Listing::Patch patch{};
    EXPECT_CALL(unmarshaller(), uint32(Eq(Attribute::RandomTickRange), _))
        .WillOnce(DoAll(SetArgReferee<1>(42), Return(true)));

    makeWriter().write(patch);
    EXPECT_THAT(patch.getRandomTickRange(), Optional(Eq(42)));
}

TEST_F(DataLayer_Inspectors_ListingPatchWriter, Write_SecurityExchange)
{
    Listing::Patch patch{};
    EXPECT_CALL(unmarshaller(), string(Eq(Attribute::SecurityExchange), _))
        .WillOnce(DoAll(SetArgReferee<1>("NASDAQ"), Return(true)));

    makeWriter().write(patch);
    EXPECT_THAT(patch.getSecurityExchange(), Optional(Eq("NASDAQ")));
}

TEST_F(DataLayer_Inspectors_ListingPatchWriter, Write_PartyID)
{
    Listing::Patch patch{};
    EXPECT_CALL(unmarshaller(), string(Eq(Attribute::PartyId), _))
        .WillOnce(DoAll(SetArgReferee<1>("P1"), Return(true)));

    makeWriter().write(patch);
    EXPECT_THAT(patch.getPartyId(), Optional(Eq("P1")));
}

TEST_F(DataLayer_Inspectors_ListingPatchWriter, Write_PartyRole)
{
    Listing::Patch patch{};
    EXPECT_CALL(unmarshaller(), string(Eq(Attribute::PartyRole), _))
        .WillOnce(DoAll(SetArgReferee<1>("Role"), Return(true)));

    makeWriter().write(patch);
    EXPECT_THAT(patch.getPartyRole(), Optional(Eq("Role")));
}

TEST_F(DataLayer_Inspectors_ListingPatchWriter, Write_CusipID)
{
    Listing::Patch patch{};
    EXPECT_CALL(unmarshaller(), string(Eq(Attribute::CusipId), _))
        .WillOnce(DoAll(SetArgReferee<1>("Cusip"), Return(true)));

    makeWriter().write(patch);
    EXPECT_THAT(patch.getCusipId(), Optional(Eq("Cusip")));
}

TEST_F(DataLayer_Inspectors_ListingPatchWriter, Write_SedolID)
{
    Listing::Patch patch{};
    EXPECT_CALL(unmarshaller(), string(Eq(Attribute::SedolId), _))
        .WillOnce(DoAll(SetArgReferee<1>("Sedol"), Return(true)));

    makeWriter().write(patch);
    EXPECT_THAT(patch.getSedolId(), Optional(Eq("Sedol")));
}

TEST_F(DataLayer_Inspectors_ListingPatchWriter, Write_IsinID)
{
    Listing::Patch patch{};
    EXPECT_CALL(unmarshaller(), string(Eq(Attribute::IsinId), _))
        .WillOnce(DoAll(SetArgReferee<1>("Isin"), Return(true)));

    makeWriter().write(patch);
    EXPECT_THAT(patch.getIsinId(), Optional(Eq("Isin")));
}

TEST_F(DataLayer_Inspectors_ListingPatchWriter, Write_RicID)
{
    Listing::Patch patch{};
    EXPECT_CALL(unmarshaller(), string(Eq(Attribute::RicId), _))
        .WillOnce(DoAll(SetArgReferee<1>("Ric"), Return(true)));

    makeWriter().write(patch);
    EXPECT_THAT(patch.getRicId(), Optional(Eq("Ric")));
}

TEST_F(DataLayer_Inspectors_ListingPatchWriter, Write_ExchangeSymbolID)
{
    Listing::Patch patch{};
    EXPECT_CALL(unmarshaller(), string(Eq(Attribute::ExchangeSymbolId), _))
        .WillOnce(DoAll(SetArgReferee<1>("EXC"), Return(true)));

    makeWriter().write(patch);
    EXPECT_THAT(patch.getExchangeSymbolId(), Optional(Eq("EXC")));
}

TEST_F(DataLayer_Inspectors_ListingPatchWriter, Write_BloombergSymbolID)
{
    Listing::Patch patch{};
    EXPECT_CALL(unmarshaller(), string(Eq(Attribute::BloombergSymbolId), _))
        .WillOnce(DoAll(SetArgReferee<1>("BBG"), Return(true)));

    makeWriter().write(patch);
    EXPECT_THAT(patch.getBloombergSymbolId(), Optional(Eq("BBG")));
}


TEST_F(DataLayer_Inspectors_ListingPatchWriter, Write_RandomQtyMinimum)
{
    Listing::Patch patch{};
    EXPECT_CALL(unmarshaller(), real(Eq(Attribute::RandomQtyMinimum), _))
        .WillOnce(DoAll(SetArgReferee<1>(42.42), Return(true)));

    makeWriter().write(patch);
    EXPECT_THAT(patch.getRandomQtyMinimum(), Optional(DoubleEq(42.42)));
}

TEST_F(DataLayer_Inspectors_ListingPatchWriter, Write_RandomAmtMinimum)
{
    Listing::Patch patch{};
    EXPECT_CALL(unmarshaller(), real(Eq(Attribute::RandomAmtMinimum), _))
        .WillOnce(DoAll(SetArgReferee<1>(42.42), Return(true)));

    makeWriter().write(patch);
    EXPECT_THAT(patch.getRandomAmtMinimum(), Optional(DoubleEq(42.42)));
}

TEST_F(DataLayer_Inspectors_ListingPatchWriter, Write_RandomAmtMaximum)
{
    Listing::Patch patch{};
    EXPECT_CALL(unmarshaller(), real(Eq(Attribute::RandomAmtMaximum), _))
        .WillOnce(DoAll(SetArgReferee<1>(42.42), Return(true)));

    makeWriter().write(patch);
    EXPECT_THAT(patch.getRandomAmtMaximum(), Optional(DoubleEq(42.42)));
}

TEST_F(DataLayer_Inspectors_ListingPatchWriter, Write_RandomOrdersEnabled)
{
    Listing::Patch patch{};
    EXPECT_CALL(unmarshaller(), boolean(Eq(Attribute::RandomOrdersEnabled), _))
        .WillOnce(DoAll(SetArgReferee<1>(false), Return(true)));

    makeWriter().write(patch);
    EXPECT_THAT(patch.getRandomOrdersEnabledFlag(), Optional(Eq(false)));
}

TEST_F(DataLayer_Inspectors_ListingPatchWriter, Write_RandomAggressiveQtyMinimum)
{
    Listing::Patch patch;
    EXPECT_CALL(unmarshaller(), real(Eq(Attribute::RandomAggressiveQtyMinimum), _))
        .WillOnce(DoAll(SetArgReferee<1>(42.42), Return(true)));

    makeWriter().write(patch);
    EXPECT_THAT(patch.getRandomAggressiveQtyMinimum(), Optional(DoubleEq(42.42)));
}

TEST_F(DataLayer_Inspectors_ListingPatchWriter, Write_RandomAggressiveQtyMaximum)
{
    Listing::Patch patch;
    EXPECT_CALL(unmarshaller(), real(Eq(Attribute::RandomAggressiveQtyMaximum), _))
        .WillOnce(DoAll(SetArgReferee<1>(42.42), Return(true)));

    makeWriter().write(patch);
    EXPECT_THAT(patch.getRandomAggressiveQtyMaximum(), Optional(DoubleEq(42.42)));
}

TEST_F(DataLayer_Inspectors_ListingPatchWriter, Write_RandomAggressiveAmtMinimum)
{
    Listing::Patch patch;
    EXPECT_CALL(unmarshaller(), real(Eq(Attribute::RandomAggressiveAmtMinimum), _))
        .WillOnce(DoAll(SetArgReferee<1>(42.42), Return(true)));

    makeWriter().write(patch);
    EXPECT_THAT(patch.getRandomAggressiveAmtMinimum(), Optional(DoubleEq(42.42)));
}

TEST_F(DataLayer_Inspectors_ListingPatchWriter, Write_RandomAggressiveAmtMaximum)
{
    Listing::Patch patch;
    EXPECT_CALL(unmarshaller(), real(Eq(Attribute::RandomAggressiveAmtMaximum), _))
        .WillOnce(DoAll(SetArgReferee<1>(42.42), Return(true)));

    makeWriter().write(patch);
    EXPECT_THAT(patch.getRandomAggressiveAmtMaximum(), Optional(DoubleEq(42.42)));
}

// NOLINTEND(*magic-numbers*)

}  // namespace
}  // namespace Simulator::DataLayer