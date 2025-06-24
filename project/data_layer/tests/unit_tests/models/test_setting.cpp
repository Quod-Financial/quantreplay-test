#include <optional>
#include <string>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "api/models/setting.hpp"
#include "ih/common/exceptions.hpp"

using ::testing::Optional;
using ::testing::Eq;

using namespace Simulator::DataLayer;

TEST(DataLayer_Model_Setting, Patch_Set_Key)
{
    Setting::Patch patch{};
    ASSERT_FALSE(patch.getKey().has_value());

    patch.withKey("Name");
    EXPECT_THAT(patch.getKey(), Optional(Eq("Name")));
}

TEST(DataLayer_Model_Setting, Patch_Set_Value)
{
    Setting::Patch patch{};
    ASSERT_FALSE(patch.getValue().has_value());

    patch.withValue("SiteName");
    EXPECT_THAT(patch.getValue(), Optional(Eq("SiteName")));
}

TEST(DataLayer_Model_Setting, Create_WithoutKey)
{
    Setting::Patch patch{};
    ASSERT_FALSE(patch.getKey().has_value());

    EXPECT_THROW((void)Setting::create(patch), RequiredAttributeMissing);
}

TEST(DataLayer_Model_Setting, Get_Key)
{
    Setting::Patch patch{};
    patch.withKey("MyKey");

    Setting setting = Setting::create(patch);
    EXPECT_EQ(setting.getKey(), "MyKey");
}

TEST(DataLayer_Model_Setting, Get_Value_Missing)
{
    Setting::Patch patch{};
    patch.withKey("MyKey");

    Setting setting = Setting::create(patch);
    EXPECT_EQ(setting.getValue(), std::nullopt);
}

TEST(DataLayer_Model_Setting, Get_Value_Specified)
{
    Setting::Patch patch{};
    patch.withKey("MyKey");
    patch.withValue("MyValue");

    Setting setting = Setting::create(patch);
    EXPECT_THAT(setting.getValue(), Optional(Eq("MyValue")));
}