#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "api/inspectors/setting.hpp"
#include "api/models/setting.hpp"
#include "common/marshaller.hpp"

using namespace Simulator::DataLayer;

using ::testing::AnyNumber;
using ::testing::DoAll;
using ::testing::Eq;
using ::testing::Optional;
using ::testing::Return;
using ::testing::SetArgReferee;
using ::testing::_; // NOLINT

class DataLayer_Inspectors_SettingReader : public ::testing::Test {
  public:
    using MarshallerType = Marshaller<Setting>;
    using ReaderType = SettingReader<MarshallerType>;

    using Attribute = Setting::Attribute;

    auto marshaller() -> MarshallerType& { return mMarshaller; }

    auto makeReader() -> ReaderType { return ReaderType{mMarshaller}; }

    static auto makeDefaultPatch() -> Setting::Patch
    {
        return Setting::Patch{}.withKey("dummy");
    }

  protected:
    void SetUp() override
    {
        EXPECT_CALL(marshaller(), string(Eq(Attribute::Key), _))
            .Times(AnyNumber());
    }

  private:

    MarshallerType mMarshaller;
};

class DataLayer_Inspectors_SettingPatchReader : public ::testing::Test {
  public:
    using MarshallerType = Marshaller<Setting>;
    using ReaderType = SettingPatchReader<MarshallerType>;

    using Attribute = Setting::Attribute;

    auto marshaller() -> MarshallerType& { return mMarshaller; }

    auto makeReader() -> ReaderType { return ReaderType{mMarshaller}; }

  private:
    MarshallerType mMarshaller;
};

class DataLayer_Inspectors_SettingPatchWriter : public ::testing::Test {
  public:
    using UnmarshallerType = Unmarshaller<Setting>;
    using WriterType = SettingPatchWriter<UnmarshallerType>;

    using Attribute = Setting::Attribute;

    auto unmarshaller() -> UnmarshallerType& { return mUnmarshaller; }

    auto makeWriter() -> WriterType { return WriterType{mUnmarshaller}; }

  protected:
    auto SetUp() -> void override
    {
        EXPECT_CALL(unmarshaller(), string)
            .Times(AnyNumber())
            .WillRepeatedly(Return(false));
    }

  private:
    UnmarshallerType mUnmarshaller;
};


TEST_F(DataLayer_Inspectors_SettingReader, Read_Key)
{
    auto const patch = makeDefaultPatch().withKey("MyKey");
    auto const setting = Setting::create(patch);
    ASSERT_EQ(setting.getKey(), "MyKey");

    EXPECT_CALL(marshaller(), string(Eq(Attribute::Key), Eq("MyKey"))).Times(1);
    makeReader().read(setting);
}

TEST_F(DataLayer_Inspectors_SettingReader, Read_Value)
{
    auto const patch = makeDefaultPatch().withValue("MyValue");
    auto const setting = Setting::create(patch);
    ASSERT_THAT(setting.getValue(), Optional(Eq("MyValue")));

    EXPECT_CALL(marshaller(), string(Eq(Attribute::Value), Eq("MyValue")))
        .Times(1);
    makeReader().read(setting);
}


TEST_F(DataLayer_Inspectors_SettingPatchReader, Read_Key)
{
    Setting::Patch patch{};
    patch.withKey("Name");
    ASSERT_THAT(patch.getKey(), Optional(Eq("Name")));

    EXPECT_CALL(marshaller(), string(Eq(Attribute::Key), Eq("Name"))).Times(1);
    makeReader().read(patch);
}

TEST_F(DataLayer_Inspectors_SettingPatchReader, Read_Value)
{
    Setting::Patch patch{};
    patch.withValue("Val");
    ASSERT_THAT(patch.getValue(), Optional(Eq("Val")));

    EXPECT_CALL(marshaller(), string(Eq(Attribute::Value), Eq("Val"))).Times(1);
    makeReader().read(patch);
}


TEST_F(DataLayer_Inspectors_SettingPatchWriter, Write_Key)
{
    EXPECT_CALL(unmarshaller(), string(Eq(Attribute::Key), _))
        .WillOnce(DoAll(SetArgReferee<1>("Name"), Return(true)));

    Setting::Patch patch{};
    makeWriter().write(patch);
    EXPECT_THAT(patch.getKey(), Optional(Eq("Name")));
}

TEST_F(DataLayer_Inspectors_SettingPatchWriter, Write_Value)
{
    EXPECT_CALL(unmarshaller(), string(Eq(Attribute::Value), _))
        .WillOnce(DoAll(SetArgReferee<1>("Val"), Return(true)));

    Setting::Patch patch{};
    makeWriter().write(patch);
    EXPECT_THAT(patch.getValue(), Optional(Eq("Val")));
}