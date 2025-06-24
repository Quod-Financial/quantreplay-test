#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "api/inspectors/price_seed.hpp"
#include "api/models/price_seed.hpp"
#include "common/marshaller.hpp"

using namespace Simulator::DataLayer;

using ::testing::AnyNumber;
using ::testing::DoAll;
using ::testing::DoubleEq;
using ::testing::Eq;
using ::testing::Optional;
using ::testing::Return;
using ::testing::SetArgReferee;
using ::testing::_; // NOLINT

class DataLayer_Inspectors_PriceSeedReader : public ::testing::Test {
  public:
    using MarshallerType = Marshaller<PriceSeed>;
    using ReaderType = PriceSeedReader<MarshallerType>;

    using Attribute = PriceSeed::Attribute;

    auto marshaller() -> MarshallerType& { return mMarshaller; }

    auto makeReader() -> ReaderType { return ReaderType{mMarshaller}; }

    static auto makeDefaultPatch() -> PriceSeed::Patch { return {}; }

  protected:
    void SetUp() override
    {
        EXPECT_CALL(marshaller(), uint64(Eq(Attribute::PriceSeedId), _))
            .Times(AnyNumber());
    }

  private:
    MarshallerType mMarshaller;
};

class DataLayer_Inspectors_PriceSeedPatchReader : public ::testing::Test {
  public:
    using MarshallerType = Marshaller<PriceSeed>;
    using ReaderType = PriceSeedPatchReader<MarshallerType>;

    using Attribute = PriceSeed::Attribute;

    auto marshaller() -> MarshallerType& { return mMarshaller; }

    auto makeReader() -> ReaderType { return ReaderType{mMarshaller}; }

  private:
    MarshallerType mMarshaller;
};

class DataLayer_Inspectors_PriceSeedPatchWriter : public ::testing::Test {
  public:
    using UnmarshallerType = Unmarshaller<PriceSeed>;
    using WriterType = PriceSeedPatchWriter<UnmarshallerType>;

    using Attribute = PriceSeed::Attribute;

    auto unmarshaller() -> UnmarshallerType& { return mUnmarshaller; }

    auto makeWriter() -> WriterType { return WriterType{mUnmarshaller}; }

  protected:
    auto SetUp() -> void override
    {
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


TEST_F(DataLayer_Inspectors_PriceSeedReader, Read_PriceSeedID)
{
    auto const patch = makeDefaultPatch();
    auto const seed = PriceSeed::create(patch, 42); // NOLINT: test value
    ASSERT_EQ(seed.getPriceSeedId(), 42);

    EXPECT_CALL(marshaller(), uint64(Eq(Attribute::PriceSeedId), Eq(42)))
        .Times(1);
    makeReader().read(seed);
}

TEST_F(DataLayer_Inspectors_PriceSeedReader, Read_Symbol)
{
    auto const patch = makeDefaultPatch().withSymbol("AAPL");
    auto const seed = PriceSeed::create(patch, 42); // NOLINT: test value
    ASSERT_THAT(seed.getSymbol(), Optional(Eq("AAPL")));

    EXPECT_CALL(marshaller(), string(Eq(Attribute::Symbol), Eq("AAPL")))
        .Times(1);
    makeReader().read(seed);
}

TEST_F(DataLayer_Inspectors_PriceSeedReader, Read_SecurityType)
{
    auto const patch = makeDefaultPatch().withSecurityType("Equity");
    auto const seed = PriceSeed::create(patch, 42); // NOLINT: test value
    ASSERT_THAT(seed.getSecurityType(), Optional(Eq("Equity")));

    EXPECT_CALL(marshaller(), string(Eq(Attribute::SecurityType), Eq("Equity")))
        .Times(1);
    makeReader().read(seed);
}

TEST_F(DataLayer_Inspectors_PriceSeedReader, Read_PriceCurrency)
{
    auto const patch = makeDefaultPatch().withPriceCurrency("USD");
    auto const seed = PriceSeed::create(patch, 42); // NOLINT: test value
    ASSERT_THAT(seed.getPriceCurrency(), Optional(Eq("USD")));

    EXPECT_CALL(marshaller(), string(Eq(Attribute::PriceCurrency), Eq("USD")))
        .Times(1);
    makeReader().read(seed);
}

TEST_F(DataLayer_Inspectors_PriceSeedReader, Read_SecurityID)
{
    auto const patch = makeDefaultPatch().withSecurityId("US0378331005");
    auto const seed = PriceSeed::create(patch, 42); // NOLINT: test value
    ASSERT_THAT(seed.getSecurityId(), Optional(Eq("US0378331005")));

    auto& expect = EXPECT_CALL(
        marshaller(),
        string(Eq(Attribute::SecurityId), Eq("US0378331005"))
    );
    expect.Times(1);

    makeReader().read(seed);
}

TEST_F(DataLayer_Inspectors_PriceSeedReader, Read_SecurityIDSource)
{
    auto const patch = makeDefaultPatch().withSecurityIdSource("ISIN");
    auto const seed = PriceSeed::create(patch, 42); // NOLINT: test value
    ASSERT_THAT(seed.getSecurityIdSource(), Optional(Eq("ISIN")));

    auto& expect = EXPECT_CALL(
        marshaller(),
        string(Eq(Attribute::SecurityIdSource), Eq("ISIN"))
    );
    expect.Times(1);

    makeReader().read(seed);
}

TEST_F(DataLayer_Inspectors_PriceSeedReader, Read_InstrumentSymbol)
{
    auto const patch = makeDefaultPatch().withInstrumentSymbol("AAPL");
    auto const seed = PriceSeed::create(patch, 42); // NOLINT: test value
    ASSERT_THAT(seed.getInstrumentSymbol(), Optional(Eq("AAPL")));

    auto& expect = EXPECT_CALL(
        marshaller(),
        string(Eq(Attribute::InstrumentSymbol), Eq("AAPL"))
    );
    expect.Times(1);

    makeReader().read(seed);
}

TEST_F(DataLayer_Inspectors_PriceSeedReader, Read_MidPrice)
{
    auto const patch = makeDefaultPatch().withMidPrice(42.42); // NOLINT
    auto const seed = PriceSeed::create(patch, 42); // NOLINT: test value
    ASSERT_THAT(seed.getMidPrice(), Optional(DoubleEq(42.42)));

    EXPECT_CALL(marshaller(), real(Eq(Attribute::MidPrice), DoubleEq(42.42)))
        .Times(1);
    makeReader().read(seed);
}

TEST_F(DataLayer_Inspectors_PriceSeedReader, Read_BidPrice)
{
    auto const patch = makeDefaultPatch().withBidPrice(42.42); // NOLINT
    auto const seed = PriceSeed::create(patch, 42); // NOLINT: test value
    ASSERT_THAT(seed.getBidPrice(), Optional(DoubleEq(42.42)));

    EXPECT_CALL(marshaller(), real(Eq(Attribute::BidPrice), DoubleEq(42.42)))
        .Times(1);
    makeReader().read(seed);
}

TEST_F(DataLayer_Inspectors_PriceSeedReader, Read_OfferPrice)
{
    auto const patch = makeDefaultPatch().withOfferPrice(42.42); // NOLINT
    auto const seed = PriceSeed::create(patch, 42); // NOLINT: test value
    ASSERT_THAT(seed.getOfferPrice(), Optional(DoubleEq(42.42)));

    EXPECT_CALL(marshaller(), real(Eq(Attribute::OfferPrice), DoubleEq(42.42)))
        .Times(1);
    makeReader().read(seed);
}

TEST_F(DataLayer_Inspectors_PriceSeedReader, Read_LastUpdate)
{
    auto const patch = makeDefaultPatch().withLastUpdate("2023-08-31");
    auto const seed = PriceSeed::create(patch, 42); // NOLINT: test value
    ASSERT_THAT(seed.getLastUpdate(), Optional(Eq("2023-08-31")));

    auto& expect = EXPECT_CALL(
        marshaller(),
        string(Eq(Attribute::LastUpdate), Eq("2023-08-31"))
    );
    expect.Times(1);

    makeReader().read(seed);
}


TEST_F(DataLayer_Inspectors_PriceSeedPatchReader, Read_Symbol)
{
    PriceSeed::Patch patch{};
    patch.withSymbol("AAPL");
    ASSERT_THAT(patch.getSymbol(), Optional(Eq("AAPL")));

    EXPECT_CALL(marshaller(), string(Eq(Attribute::Symbol), Eq("AAPL")))
        .Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_PriceSeedPatchReader, Read_SecurityType)
{
    PriceSeed::Patch patch{};
    patch.withSecurityType("CS");
    ASSERT_THAT(patch.getSecurityType(), Optional(Eq("CS")));

    EXPECT_CALL(marshaller(), string(Eq(Attribute::SecurityType), Eq("CS")))
        .Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_PriceSeedPatchReader, Read_PriceCurrency)
{
    PriceSeed::Patch patch{};
    patch.withPriceCurrency("USD");
    ASSERT_THAT(patch.getPriceCurrency(), Optional(Eq("USD")));

    EXPECT_CALL(marshaller(), string(Eq(Attribute::PriceCurrency), Eq("USD")))
        .Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_PriceSeedPatchReader, Read_SecurityID)
{
    PriceSeed::Patch patch{};
    patch.withSecurityId("US0378331005");
    ASSERT_THAT(patch.getSecurityId(), Optional(Eq("US0378331005")));

    auto& expect = EXPECT_CALL(
        marshaller(),
        string(Eq(Attribute::SecurityId), Eq("US0378331005"))
    );
    expect.Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_PriceSeedPatchReader, Read_SecurityIDSource)
{
    PriceSeed::Patch patch{};
    patch.withSecurityIdSource("ISIN");
    ASSERT_THAT(patch.getSecurityIdSource(), Optional(Eq("ISIN")));

    auto& expect = EXPECT_CALL(
        marshaller(),
        string(Eq(Attribute::SecurityIdSource), Eq("ISIN"))
    );
    expect.Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_PriceSeedPatchReader, Read_InstrumentSymbol)
{
    PriceSeed::Patch patch{};
    patch.withInstrumentSymbol("AAPL");
    ASSERT_THAT(patch.getInstrumentSymbol(), Optional(Eq("AAPL")));

    auto& expect = EXPECT_CALL(
        marshaller(),
        string(Eq(Attribute::InstrumentSymbol), Eq("AAPL"))
    );
    expect.Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_PriceSeedPatchReader, Read_MidPrice)
{
    PriceSeed::Patch patch{};
    patch.withMidPrice(42.42); // NOLINT: test value
    ASSERT_THAT(patch.getMidPrice(), Optional(DoubleEq(42.42)));

    // NOLINTNEXTLINE: test value
    EXPECT_CALL(marshaller(), real(Eq(Attribute::MidPrice), DoubleEq(42.42)))
        .Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_PriceSeedPatchReader, Read_BidPrice)
{
    PriceSeed::Patch patch{};
    patch.withBidPrice(42.42); // NOLINT: test value
    ASSERT_THAT(patch.getBidPrice(), Optional(DoubleEq(42.42)));

    // NOLINTNEXTLINE: test value
    EXPECT_CALL(marshaller(), real(Eq(Attribute::BidPrice), DoubleEq(42.42)))
        .Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_PriceSeedPatchReader, Read_OfferPrice)
{
    PriceSeed::Patch patch{};
    patch.withOfferPrice(42.42); // NOLINT: test value
    ASSERT_THAT(patch.getOfferPrice(), Optional(DoubleEq(42.42)));

    // NOLINTNEXTLINE: test value
    EXPECT_CALL(marshaller(), real(Eq(Attribute::OfferPrice), DoubleEq(42.42)))
        .Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_PriceSeedPatchReader, Read_LastUpdate)
{
    PriceSeed::Patch patch{};
    patch.withLastUpdate("2023-08-25 12:03:45");
    ASSERT_THAT(patch.getLastUpdate(), Optional(Eq("2023-08-25 12:03:45")));

    auto& expect = EXPECT_CALL(
        marshaller(),
        string(Eq(Attribute::LastUpdate), Eq("2023-08-25 12:03:45"))
    );
    expect.Times(1);

    makeReader().read(patch);
}


TEST_F(DataLayer_Inspectors_PriceSeedPatchWriter, Write_Symbol)
{
    EXPECT_CALL(unmarshaller(), string(Eq(Attribute::Symbol), _))
        .WillOnce(DoAll(SetArgReferee<1>("AAPL"), Return(true)));

    PriceSeed::Patch patch{};
    makeWriter().write(patch);

    EXPECT_THAT(patch.getSymbol(), Optional(Eq("AAPL")));
}

TEST_F(DataLayer_Inspectors_PriceSeedPatchWriter, Write_SecurityType)
{
    EXPECT_CALL(unmarshaller(), string(Eq(Attribute::SecurityType), _))
        .WillOnce(DoAll(SetArgReferee<1>("CS"), Return(true)));

    PriceSeed::Patch patch{};
    makeWriter().write(patch);

    EXPECT_THAT(patch.getSecurityType(), Optional(Eq("CS")));
}

TEST_F(DataLayer_Inspectors_PriceSeedPatchWriter, Write_PriceCurrency)
{
    EXPECT_CALL(unmarshaller(), string(Eq(Attribute::PriceCurrency), _))
        .WillOnce(DoAll(SetArgReferee<1>("USD"), Return(true)));

    PriceSeed::Patch patch{};
    makeWriter().write(patch);

    EXPECT_THAT(patch.getPriceCurrency(), Optional(Eq("USD")));
}

TEST_F(DataLayer_Inspectors_PriceSeedPatchWriter, Write_SecurityID)
{
    EXPECT_CALL(unmarshaller(), string(Eq(Attribute::SecurityId), _))
        .WillOnce(DoAll(SetArgReferee<1>("US0378331005"), Return(true)));

    PriceSeed::Patch patch{};
    makeWriter().write(patch);

    EXPECT_THAT(patch.getSecurityId(), Optional(Eq("US0378331005")));
}

TEST_F(DataLayer_Inspectors_PriceSeedPatchWriter, Write_SecurityIDSource)
{
    EXPECT_CALL(unmarshaller(), string(Eq(Attribute::SecurityIdSource), _))
        .WillOnce(DoAll(SetArgReferee<1>("ISIN"), Return(true)));

    PriceSeed::Patch patch{};
    makeWriter().write(patch);

    EXPECT_THAT(patch.getSecurityIdSource(), Optional(Eq("ISIN")));
}

TEST_F(DataLayer_Inspectors_PriceSeedPatchWriter, Write_InstrumentSymbol)
{
    EXPECT_CALL(unmarshaller(), string(Eq(Attribute::InstrumentSymbol), _))
        .WillOnce(DoAll(SetArgReferee<1>("AAPL"), Return(true)));

    PriceSeed::Patch patch{};
    makeWriter().write(patch);

    EXPECT_THAT(patch.getInstrumentSymbol(), Optional(Eq("AAPL")));
}

TEST_F(DataLayer_Inspectors_PriceSeedPatchWriter, Write_MidPrice)
{
    EXPECT_CALL(unmarshaller(), real(Eq(Attribute::MidPrice), _))
        .WillOnce(DoAll(SetArgReferee<1>(42.42), Return(true))); // NOLINT

    PriceSeed::Patch patch{};
    makeWriter().write(patch);

    EXPECT_THAT(patch.getMidPrice(), Optional(DoubleEq(42.42)));
}

TEST_F(DataLayer_Inspectors_PriceSeedPatchWriter, Write_BidPrice)
{
    EXPECT_CALL(unmarshaller(), real(Eq(Attribute::BidPrice), _))
        .WillOnce(DoAll(SetArgReferee<1>(42.42), Return(true))); // NOLINT

    PriceSeed::Patch patch{};
    makeWriter().write(patch);

    EXPECT_THAT(patch.getBidPrice(), Optional(DoubleEq(42.42)));
}

TEST_F(DataLayer_Inspectors_PriceSeedPatchWriter, Write_OfferPrice)
{
    EXPECT_CALL(unmarshaller(), real(Eq(Attribute::OfferPrice), _))
        .WillOnce(DoAll(SetArgReferee<1>(42.42), Return(true))); // NOLINT

    PriceSeed::Patch patch{};
    makeWriter().write(patch);

    EXPECT_THAT(patch.getOfferPrice(), Optional(DoubleEq(42.42)));
}

TEST_F(DataLayer_Inspectors_PriceSeedPatchWriter, Write_LastUpdate)
{
    EXPECT_CALL(unmarshaller(), string(Eq(Attribute::LastUpdate), _))
        .WillOnce(DoAll(SetArgReferee<1>("2023-08-25 12:03:45"), Return(true)));

    PriceSeed::Patch patch{};
    makeWriter().write(patch);

    EXPECT_THAT(patch.getLastUpdate(), Optional(Eq("2023-08-25 12:03:45")));
}