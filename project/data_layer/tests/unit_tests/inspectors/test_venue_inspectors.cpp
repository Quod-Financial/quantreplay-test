#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "api/inspectors/venue.hpp"
#include "api/models/venue.hpp"
#include "common/marshaller.hpp"

using namespace Simulator::DataLayer;

using ::testing::AnyNumber;
using ::testing::DoAll;
using ::testing::Eq;
using ::testing::Optional;
using ::testing::Return;
using ::testing::SetArgReferee;
using ::testing::_; // NOLINT

class DataLayer_Inspectors_VenueReader : public ::testing::Test {
  public:
    using MarshallerType = Marshaller<Venue>;
    using ReaderType = VenueReader<MarshallerType>;

    using Attribute = Venue::Attribute;
    using EngineType = Venue::EngineType;

    auto marshaller() -> MarshallerType& { return mMarshaller; }

    auto makeReader() -> ReaderType { return ReaderType{mMarshaller}; }

    static auto makeDefaultPatch() -> Venue::Patch
    {
        Venue::Patch patch{};
        patch.withVenueId("dummy");
        return patch;
    }

  protected:
    auto SetUp() -> void override
    {
        EXPECT_CALL(marshaller(), string(Eq(Attribute::VenueId), _))
            .Times(AnyNumber());
    }

  private:
    MarshallerType mMarshaller;
};

class DataLayer_Inspectors_VenuePatchReader : public ::testing::Test {
  public:
    using MarshallerType = Marshaller<Venue>;
    using ReaderType = VenuePatchReader<MarshallerType>;

    using Attribute = Venue::Attribute;
    using EngineType = Venue::EngineType;

    auto marshaller() -> MarshallerType& { return mMarshaller; }

    auto makeReader() -> ReaderType { return ReaderType{mMarshaller}; }

  private:
    MarshallerType mMarshaller;
};

class DataLayer_Inspectors_VenuePatchWriter : public ::testing::Test {
  public:
    using UnmarshallerType = Unmarshaller<Venue>;
    using WriterType = VenuePatchWriter<UnmarshallerType>;

    using Attribute = Venue::Attribute;
    using EngineType = Venue::EngineType;

    auto unmarshaller() -> UnmarshallerType& { return mUnmarshaller; }

    auto makeWriter() -> WriterType { return WriterType{mUnmarshaller}; }

  protected:
    auto SetUp() -> void override
    {
        EXPECT_CALL(unmarshaller(), boolean)
            .Times(AnyNumber())
            .WillRepeatedly(Return(false));

        EXPECT_CALL(unmarshaller(), uint16)
            .Times(AnyNumber())
            .WillRepeatedly(Return(false));

        EXPECT_CALL(unmarshaller(), uint32)
            .Times(AnyNumber())
            .WillRepeatedly(Return(false));

        EXPECT_CALL(unmarshaller(), string)
            .Times(AnyNumber())
            .WillRepeatedly(Return(false));

        EXPECT_CALL(unmarshaller(), engineType)
            .Times(AnyNumber())
            .WillRepeatedly(Return(false));
    }

  private:
    UnmarshallerType mUnmarshaller;
};


TEST_F(DataLayer_Inspectors_VenueReader, Read_VenueID)
{
    Venue::Patch patch{};
    patch.withVenueId("XETRA");
    ASSERT_THAT(patch.getVenueId(), Optional(Eq("XETRA")));

    auto const venue = Venue::create(patch);

    EXPECT_CALL(marshaller(), string(Eq(Attribute::VenueId), Eq("XETRA")))
        .Times(1);

    makeReader().read(venue);
}

TEST_F(DataLayer_Inspectors_VenueReader, Read_Name)
{
    auto const patch = makeDefaultPatch().withName("London Stock Exchange");
    ASSERT_THAT(patch.getName(), Optional(Eq("London Stock Exchange")));
    auto const venue = Venue::create(patch);

    auto& expect = EXPECT_CALL(
        marshaller(),
        string(Eq(Attribute::Name), Eq("London Stock Exchange"))
    );
    expect.Times(1);

    makeReader().read(venue);
}

TEST_F(DataLayer_Inspectors_VenueReader, Read_EngineType)
{
    auto const patch = makeDefaultPatch().withEngineType(EngineType::Quoting);
    ASSERT_THAT(patch.getEngineType(), Optional(Eq(EngineType::Quoting)));
    auto const venue = Venue::create(patch);

    auto& expect = EXPECT_CALL(
        marshaller(),
        engineType(Eq(Attribute::EngineType), Eq(EngineType::Quoting))
    );
    expect.Times(1);

    makeReader().read(venue);
}

TEST_F(DataLayer_Inspectors_VenueReader, Read_SupportTifIocFlag)
{
    auto const patch = makeDefaultPatch().withSupportTifIocFlag(false);
    ASSERT_THAT(patch.getSupportTifIocFlag(), Optional(Eq(false)));
    auto const venue = Venue::create(patch);

    EXPECT_CALL(marshaller(), boolean(Eq(Attribute::SupportTifIoc), Eq(false)))
        .Times(1);

    makeReader().read(venue);
}

TEST_F(DataLayer_Inspectors_VenueReader, Read_SupportTifFokFlag)
{
    auto const patch = makeDefaultPatch().withSupportTifFokFlag(false);
    ASSERT_THAT(patch.getSupportTifFokFlag(), Optional(Eq(false)));
    auto const venue = Venue::create(patch);

    EXPECT_CALL(marshaller(), boolean(Eq(Attribute::SupportTifFok), Eq(false)))
        .Times(1);

    makeReader().read(venue);
}

TEST_F(DataLayer_Inspectors_VenueReader, Read_SupportTifDayFlag)
{
    auto const patch = makeDefaultPatch().withSupportTifDayFlag(false);
    ASSERT_THAT(patch.getSupportTifDayFlag(), Optional(Eq(false)));
    auto const venue = Venue::create(patch);

    EXPECT_CALL(marshaller(), boolean(Eq(Attribute::SupportTifDay), Eq(false)))
        .Times(1);

    makeReader().read(venue);
}

TEST_F(DataLayer_Inspectors_VenueReader, Read_IncludeOwnOrdersFlag)
{
    auto const patch = makeDefaultPatch().withIncludeOwnOrdersFlag(true);
    ASSERT_THAT(patch.getIncludeOwnOrdersFlag(), Optional(Eq(true)));
    auto const venue = Venue::create(patch);

    auto& expect = EXPECT_CALL(
        marshaller(),
        boolean(Eq(Attribute::IncludeOwnOrders), Eq(true))
    );
    expect.Times(1);

    makeReader().read(venue);
}

TEST_F(DataLayer_Inspectors_VenueReader, Read_RestPort)
{
    auto const patch = makeDefaultPatch().withRestPort(9001); // NOLINT
    ASSERT_THAT(patch.getRestPort(), Optional(Eq(9001)));
    auto const venue = Venue::create(patch);

    EXPECT_CALL(marshaller(), uint16(Eq(Attribute::RestPort), Eq(9001)))
        .Times(1);

    makeReader().read(venue);
}

TEST_F(DataLayer_Inspectors_VenueReader, Read_OrdersOnStartupFlag)
{
    auto const patch = makeDefaultPatch().withOrdersOnStartupFlag(false);
    ASSERT_THAT(patch.getOrdersOnStartupFlag(), Optional(Eq(false)));
    auto const venue = Venue::create(patch);

    auto& expect = EXPECT_CALL(
        marshaller(),
        boolean(Eq(Attribute::OrderOnStartup), Eq(false))
    );
    expect.Times(1);

    makeReader().read(venue);
}

TEST_F(DataLayer_Inspectors_VenueReader, Read_RandomPartiesCount)
{
    auto const patch = makeDefaultPatch().withRandomPartiesCount(23); // NOLINT
    ASSERT_THAT(patch.getRandomPartiesCount(), Optional(Eq(23)));
    auto const venue = Venue::create(patch);

    EXPECT_CALL(marshaller(), uint32(Eq(Attribute::RandomPartiesCount), Eq(23)))
        .Times(1);

    makeReader().read(venue);
}

TEST_F(DataLayer_Inspectors_VenueReader, Read_TnsEnabledFlag)
{
    auto const patch = makeDefaultPatch().withTnsEnabledFlag(false);
    ASSERT_THAT(patch.getTnsEnabledFlag(), Optional(Eq(false)));
    auto const venue = Venue::create(patch);

    EXPECT_CALL(marshaller(), boolean(Eq(Attribute::TnsEnabled), Eq(false)))
        .Times(1);

    makeReader().read(venue);
}

TEST_F(DataLayer_Inspectors_VenueReader, Read_TnsQtyEnabledFlag)
{
    auto const patch = makeDefaultPatch().withTnsQtyEnabledFlag(false);
    ASSERT_THAT(patch.getTnsQtyEnabledFlag(), Optional(Eq(false)));
    auto const venue = Venue::create(patch);

    EXPECT_CALL(marshaller(), boolean(Eq(Attribute::TnsQtyEnabled), Eq(false)))
        .Times(1);

    makeReader().read(venue);
}

TEST_F(DataLayer_Inspectors_VenueReader, Read_TnsSideEnabledFlag)
{
    auto const patch = makeDefaultPatch().withTnsSideEnabledFlag(true);
    ASSERT_THAT(patch.getTnsSideEnabledFlag(), Optional(Eq(true)));
    auto const venue = Venue::create(patch);

    EXPECT_CALL(marshaller(), boolean(Eq(Attribute::TnsSideEnabled), Eq(true)))
        .Times(1);

    makeReader().read(venue);
}

TEST_F(DataLayer_Inspectors_VenueReader, Read_TnsPartiesEnabledFlag)
{
    auto const patch = makeDefaultPatch().withTnsPartiesEnabledFlag(false);
    ASSERT_THAT(patch.getTnsPartiesEnabledFlag(), Optional(Eq(false)));
    auto const venue = Venue::create(patch);

    auto& expect = EXPECT_CALL(
        marshaller(),
        boolean(Eq(Attribute::TnsPartiesEnabled), Eq(false))
    );
    expect.Times(1);

    makeReader().read(venue);
}

TEST_F(DataLayer_Inspectors_VenueReader, Read_Timezone)
{
    auto const patch = makeDefaultPatch().withTimezone("GMT+3");
    ASSERT_THAT(patch.getTimezone(), Optional(Eq("GMT+3")));
    auto const venue = Venue::create(patch);

    EXPECT_CALL(marshaller(), string(Eq(Attribute::Timezone), Eq("GMT+3")))
        .Times(1);

    makeReader().read(venue);
}

TEST_F(DataLayer_Inspectors_VenueReader, Read_CancelOnDisconnectFlag)
{
    auto const patch = makeDefaultPatch().withCancelOnDisconnectFlag(true);
    ASSERT_THAT(patch.getCancelOnDisconnectFlag(), Optional(Eq(true)));
    auto const venue = Venue::create(patch);

    auto& expect = EXPECT_CALL(
        marshaller(),
        boolean(Eq(Attribute::CancelOnDisconnect), Eq(true))
    );
    expect.Times(1);

    makeReader().read(venue);
}

TEST_F(DataLayer_Inspectors_VenueReader, Read_PersistenceEnabledFlag)
{
    auto const patch = makeDefaultPatch().withPersistenceEnabledFlag(false);
    ASSERT_THAT(patch.getPersistenceEnabledFlag(), Optional(Eq(false)));
    auto const venue = Venue::create(patch);

    auto& expect = EXPECT_CALL(
        marshaller(),
        boolean(Eq(Attribute::PersistenceEnabled), Eq(false))
    );
    expect.Times(1);

    makeReader().read(venue);
}

TEST_F(DataLayer_Inspectors_VenueReader, Read_PersistenceFilePath)
{
    auto const patch = makeDefaultPatch().withPersistenceFilePath("/rw/data.json");
    ASSERT_THAT(patch.getPersistenceFilePath(), Optional(Eq("/rw/data.json")));
    auto const venue = Venue::create(patch);

    auto& expect = EXPECT_CALL(
        marshaller(),
        string(Eq(Attribute::PersistenceFilePath), Eq("/rw/data.json"))
    );
    expect.Times(1);

    makeReader().read(venue);
}


TEST_F(DataLayer_Inspectors_VenuePatchReader, Read_VenueID)
{
    Venue::Patch patch{};
    patch.withVenueId("XETRA");
    ASSERT_THAT(patch.getVenueId(), Optional(Eq("XETRA")));

    EXPECT_CALL(marshaller(), string(Eq(Attribute::VenueId), Eq("XETRA")))
        .Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_VenuePatchReader, Read_Name)
{
    Venue::Patch patch{};
    patch.withName("London Stock Exchange");
    ASSERT_THAT(patch.getName(), Optional(Eq("London Stock Exchange")));

    auto& expect = EXPECT_CALL(
        marshaller(),
        string(Eq(Attribute::Name), Eq("London Stock Exchange"))
    );
    expect.Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_VenuePatchReader, Read_EngineType)
{
    Venue::Patch patch{};
    patch.withEngineType(EngineType::Quoting);
    ASSERT_THAT(patch.getEngineType(), Optional(Eq(EngineType::Quoting)));

    auto& expect = EXPECT_CALL(
        marshaller(),
        engineType(Eq(Attribute::EngineType), Eq(EngineType::Quoting))
    );
    expect.Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_VenuePatchReader, Read_SupportTifIocFlag)
{
    Venue::Patch patch{};
    patch.withSupportTifIocFlag(false);
    ASSERT_THAT(patch.getSupportTifIocFlag(), Optional(Eq(false)));

    EXPECT_CALL(marshaller(), boolean(Eq(Attribute::SupportTifIoc), Eq(false)))
        .Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_VenuePatchReader, Read_SupportTifFokFlag)
{
    Venue::Patch patch{};
    patch.withSupportTifFokFlag(false);
    ASSERT_THAT(patch.getSupportTifFokFlag(), Optional(Eq(false)));

    EXPECT_CALL(marshaller(), boolean(Eq(Attribute::SupportTifFok), Eq(false)))
        .Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_VenuePatchReader, Read_SupportTifDayFlag)
{
    Venue::Patch patch{};
    patch.withSupportTifDayFlag(false);
    ASSERT_THAT(patch.getSupportTifDayFlag(), Optional(Eq(false)));

    EXPECT_CALL(marshaller(), boolean(Eq(Attribute::SupportTifDay), Eq(false)))
        .Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_VenuePatchReader, Read_IncludeOwnOrdersFlag)
{
    Venue::Patch patch{};
    patch.withIncludeOwnOrdersFlag(true);
    ASSERT_THAT(patch.getIncludeOwnOrdersFlag(), Optional(Eq(true)));

    auto& expect = EXPECT_CALL(
        marshaller(),
        boolean(Eq(Attribute::IncludeOwnOrders), Eq(true))
    );
    expect.Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_VenuePatchReader, Read_RestPort)
{
    Venue::Patch patch{};
    patch.withRestPort(9001); // NOLINT: Test value
    ASSERT_THAT(patch.getRestPort(), Optional(Eq(9001)));

    EXPECT_CALL(marshaller(), uint16(Eq(Attribute::RestPort), Eq(9001)))
        .Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_VenuePatchReader, Read_OrdersOnStartupFlag)
{
    Venue::Patch patch{};
    patch.withOrdersOnStartupFlag(false);
    ASSERT_THAT(patch.getOrdersOnStartupFlag(), Optional(Eq(false)));

    auto& expect = EXPECT_CALL(
        marshaller(),
        boolean(Eq(Attribute::OrderOnStartup), Eq(false))
    );
    expect.Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_VenuePatchReader, Read_RandomPartiesCount)
{
    Venue::Patch patch{};
    patch.withRandomPartiesCount(23); // NOLINT: Test value
    ASSERT_THAT(patch.getRandomPartiesCount(), Optional(Eq(23)));

    EXPECT_CALL(marshaller(), uint32(Eq(Attribute::RandomPartiesCount), Eq(23)))
        .Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_VenuePatchReader, Read_TnsEnabledFlag)
{
    Venue::Patch patch{};
    patch.withTnsEnabledFlag(false);
    ASSERT_THAT(patch.getTnsEnabledFlag(), Optional(Eq(false)));

    EXPECT_CALL(marshaller(), boolean(Eq(Attribute::TnsEnabled), Eq(false)))
        .Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_VenuePatchReader, Read_TnsQtyEnabledFlag)
{
    Venue::Patch patch{};
    patch.withTnsQtyEnabledFlag(false);
    ASSERT_THAT(patch.getTnsQtyEnabledFlag(), Optional(Eq(false)));

    EXPECT_CALL(marshaller(), boolean(Eq(Attribute::TnsQtyEnabled), Eq(false)))
        .Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_VenuePatchReader, Read_TnsSideEnabledFlag)
{
    Venue::Patch patch{};
    patch.withTnsSideEnabledFlag(true);
    ASSERT_THAT(patch.getTnsSideEnabledFlag(), Optional(Eq(true)));

    EXPECT_CALL(marshaller(), boolean(Eq(Attribute::TnsSideEnabled), Eq(true)))
        .Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_VenuePatchReader, Read_TnsPartiesEnabledFlag)
{
    Venue::Patch patch{};
    patch.withTnsPartiesEnabledFlag(false);
    ASSERT_THAT(patch.getTnsPartiesEnabledFlag(), Optional(Eq(false)));

    auto& expect = EXPECT_CALL(
        marshaller(),
        boolean(Eq(Attribute::TnsPartiesEnabled), Eq(false))
    );
    expect.Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_VenuePatchReader, Read_Timezone)
{
    Venue::Patch patch{};
    patch.withTimezone("GMT+3");
    ASSERT_THAT(patch.getTimezone(), Optional(Eq("GMT+3")));

    EXPECT_CALL(marshaller(), string(Eq(Attribute::Timezone), Eq("GMT+3")))
        .Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_VenuePatchReader, Read_CancelOnDisconnectFlag)
{
    Venue::Patch patch{};
    patch.withCancelOnDisconnectFlag(true);
    ASSERT_THAT(patch.getCancelOnDisconnectFlag(), Optional(Eq(true)));

    auto& expect = EXPECT_CALL(
        marshaller(),
        boolean(Eq(Attribute::CancelOnDisconnect), Eq(true))
    );
    expect.Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_VenuePatchReader, Read_PersistenceEnabledFlag)
{
    Venue::Patch patch{};
    patch.withPersistenceEnabledFlag(false);
    ASSERT_THAT(patch.getPersistenceEnabledFlag(), Optional(Eq(false)));

    auto& expect = EXPECT_CALL(
        marshaller(),
        boolean(Eq(Attribute::PersistenceEnabled), Eq(false))
    );
    expect.Times(1);

    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_VenuePatchReader, Read_PersistenceFilePath)
{
    Venue::Patch patch{};
    patch.withPersistenceFilePath("/rw/data.json");
    ASSERT_THAT(patch.getPersistenceFilePath(), Optional(Eq("/rw/data.json")));

    auto& expect = EXPECT_CALL(
        marshaller(),
        string(Eq(Attribute::PersistenceFilePath), Eq("/rw/data.json"))
    );
    expect.Times(1);

    makeReader().read(patch);
}


TEST_F(DataLayer_Inspectors_VenuePatchWriter, Write_VenueID)
{
    EXPECT_CALL(unmarshaller(), string(Eq(Attribute::VenueId), _))
        .WillOnce(DoAll(SetArgReferee<1>("XETRA"), Return(true)));

    Venue::Patch patch{};
    makeWriter().write(patch);

    EXPECT_THAT(patch.getVenueId(), Optional(Eq("XETRA")));
}

TEST_F(DataLayer_Inspectors_VenuePatchWriter, Write_Name)
{
    EXPECT_CALL(unmarshaller(), string(Eq(Attribute::Name), _))
        .WillOnce(
            DoAll(SetArgReferee<1>("National Stock Exchange"), Return(true))
        );

    Venue::Patch patch{};
    makeWriter().write(patch);

    EXPECT_THAT(patch.getName(), Optional(Eq("National Stock Exchange")));
}

TEST_F(DataLayer_Inspectors_VenuePatchWriter, Write_EngineType)
{
    EXPECT_CALL(unmarshaller(), engineType(Eq(Attribute::EngineType), _))
        .WillOnce(DoAll(SetArgReferee<1>(EngineType::Matching), Return(true)));

    Venue::Patch patch{};
    makeWriter().write(patch);

    EXPECT_THAT(patch.getEngineType(), Optional(Eq(EngineType::Matching)));
}

TEST_F(DataLayer_Inspectors_VenuePatchWriter, Write_SupportTifIocFlag)
{
    EXPECT_CALL(unmarshaller(), boolean(Eq(Attribute::SupportTifIoc), _))
        .WillOnce(DoAll(SetArgReferee<1>(false), Return(true)));

    Venue::Patch patch{};
    makeWriter().write(patch);

    EXPECT_THAT(patch.getSupportTifIocFlag(), Optional(Eq(false)));
}

TEST_F(DataLayer_Inspectors_VenuePatchWriter, Write_SupportTifFokFlag)
{
    EXPECT_CALL(unmarshaller(), boolean(Eq(Attribute::SupportTifFok), _))
        .WillOnce(DoAll(SetArgReferee<1>(true), Return(true)));

    Venue::Patch patch{};
    makeWriter().write(patch);

    EXPECT_THAT(patch.getSupportTifFokFlag(), Optional(Eq(true)));
}

TEST_F(DataLayer_Inspectors_VenuePatchWriter, Write_InclideOwnOrdersFlag)
{
    EXPECT_CALL(unmarshaller(), boolean(Eq(Attribute::IncludeOwnOrders), _))
        .WillOnce(DoAll(SetArgReferee<1>(true), Return(true)));

    Venue::Patch patch{};
    makeWriter().write(patch);

    EXPECT_THAT(patch.getIncludeOwnOrdersFlag(), Optional(Eq(true)));
}

TEST_F(DataLayer_Inspectors_VenuePatchWriter, Write_RestPort)
{
    EXPECT_CALL(unmarshaller(), uint16(Eq(Attribute::RestPort), _))
        .WillOnce(DoAll(SetArgReferee<1>(9001), Return(true))); // NOLINT

    Venue::Patch patch{};
    makeWriter().write(patch);

    EXPECT_THAT(patch.getRestPort(), Optional(Eq(9001)));
}

TEST_F(DataLayer_Inspectors_VenuePatchWriter, Write_OrdersOnStartupFlag)
{
    EXPECT_CALL(unmarshaller(), boolean(Eq(Attribute::OrderOnStartup), _))
        .WillOnce(DoAll(SetArgReferee<1>(false), Return(true)));

    Venue::Patch patch{};
    makeWriter().write(patch);

    EXPECT_THAT(patch.getOrdersOnStartupFlag(), Optional(Eq(false)));
}

TEST_F(DataLayer_Inspectors_VenuePatchWriter, Write_RandomPartiesCount)
{
    EXPECT_CALL(unmarshaller(), uint32(Eq(Attribute::RandomPartiesCount), _))
        .WillOnce(DoAll(SetArgReferee<1>(12), Return(true))); // NOLINT

    Venue::Patch patch{};
    makeWriter().write(patch);

    EXPECT_THAT(patch.getRandomPartiesCount(), Optional(Eq(12)));
}

TEST_F(DataLayer_Inspectors_VenuePatchWriter, Write_TnsEnabledFlag)
{
    EXPECT_CALL(unmarshaller(), boolean(Eq(Attribute::TnsEnabled), _))
        .WillOnce(DoAll(SetArgReferee<1>(true), Return(true)));

    Venue::Patch patch{};
    makeWriter().write(patch);

    EXPECT_THAT(patch.getTnsEnabledFlag(), Optional(Eq(true)));
}

TEST_F(DataLayer_Inspectors_VenuePatchWriter, Write_TnsQtyEnabledFlag)
{
    EXPECT_CALL(unmarshaller(), boolean(Eq(Attribute::TnsQtyEnabled), _))
        .WillOnce(DoAll(SetArgReferee<1>(true), Return(true)));

    Venue::Patch patch{};
    makeWriter().write(patch);

    EXPECT_THAT(patch.getTnsQtyEnabledFlag(), Optional(Eq(true)));
}

TEST_F(DataLayer_Inspectors_VenuePatchWriter, Write_TnsSideEnabledFlag)
{
    EXPECT_CALL(unmarshaller(), boolean(Eq(Attribute::TnsSideEnabled), _))
        .WillOnce(DoAll(SetArgReferee<1>(false), Return(true)));

    Venue::Patch patch{};
    makeWriter().write(patch);

    EXPECT_THAT(patch.getTnsSideEnabledFlag(), Optional(Eq(false)));
}

TEST_F(DataLayer_Inspectors_VenuePatchWriter, Write_TnsPartiesEnabledFlag)
{
    EXPECT_CALL(unmarshaller(), boolean(Eq(Attribute::TnsPartiesEnabled), _))
        .WillOnce(DoAll(SetArgReferee<1>(false), Return(true)));

    Venue::Patch patch{};
    makeWriter().write(patch);

    EXPECT_THAT(patch.getTnsPartiesEnabledFlag(), Optional(Eq(false)));
}

TEST_F(DataLayer_Inspectors_VenuePatchWriter, Write_Timezone)
{
    EXPECT_CALL(unmarshaller(), string(Eq(Attribute::Timezone), _))
        .WillOnce(DoAll(SetArgReferee<1>("GMT+3"), Return(true)));

    Venue::Patch patch{};
    makeWriter().write(patch);

    EXPECT_THAT(patch.getTimezone(), Optional(Eq("GMT+3")));
}

TEST_F(DataLayer_Inspectors_VenuePatchWriter, Write_CancelOnDisconnectFlag)
{
    EXPECT_CALL(unmarshaller(), boolean(Eq(Attribute::CancelOnDisconnect), _))
        .WillOnce(DoAll(SetArgReferee<1>(true), Return(true)));

    Venue::Patch patch{};
    makeWriter().write(patch);

    EXPECT_THAT(patch.getCancelOnDisconnectFlag(), Optional(Eq(true)));
}

TEST_F(DataLayer_Inspectors_VenuePatchWriter, Write_PersistenceEnabledFlag)
{
    EXPECT_CALL(unmarshaller(), boolean(Eq(Attribute::PersistenceEnabled), _))
        .WillOnce(DoAll(SetArgReferee<1>(true), Return(true)));

    Venue::Patch patch{};
    makeWriter().write(patch);

    EXPECT_THAT(patch.getPersistenceEnabledFlag(), Optional(Eq(true)));
}

TEST_F(DataLayer_Inspectors_VenuePatchWriter, Write_PersistenceFilePath)
{
    EXPECT_CALL(unmarshaller(), string(Eq(Attribute::PersistenceFilePath), _))
        .WillOnce(DoAll(SetArgReferee<1>("/rw/file.json"), Return(true)));

    Venue::Patch patch{};
    makeWriter().write(patch);

    EXPECT_THAT(patch.getPersistenceFilePath(), Optional(Eq("/rw/file.json")));
}