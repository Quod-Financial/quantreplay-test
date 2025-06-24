#include <initializer_list>
#include <string>
#include <string_view>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "data_layer/api/models/setting.hpp"

#include "ih/marshalling/json/setting.hpp"

using namespace Simulator;
using namespace Simulator::Http;

using ::testing::Eq;
using ::testing::Optional;

class Http_Json_SettingMarshaller : public ::testing::Test {
  public:
    using Setting = DataLayer::Setting;
    using Patch = Setting::Patch;

    static auto makeDefaultPatch() -> Patch { return Patch{}.withKey("dummy"); }

    static auto marshall(std::initializer_list<Setting> _settings)
        -> std::string
    {
        return Json::SettingMarshaller::marshall(_settings);
    }
};


TEST_F(Http_Json_SettingMarshaller, Marshall_Key)
{
    auto const patch = makeDefaultPatch().withKey("MyKey");
    auto const setting = Setting::create(patch);

    // clang-format off
    constexpr std::string_view expectedJson = "{"
        R"("settings":[)"
            "{"
                R"("key":"MyKey")"
            "}"
        "]"
    "}";
    // clang-format on

    EXPECT_EQ(marshall({setting}), expectedJson);
}

TEST_F(Http_Json_SettingMarshaller, Marshall_Value)
{
    auto const patch = makeDefaultPatch().withValue("MyValue");
    auto const setting = Setting::create(patch);

    // clang-format off
    constexpr std::string_view expectedJson = "{"
        R"("settings":[)"
            "{"
                R"("key":"dummy",)"
                R"("value":"MyValue")"
            "}"
        "]"
    "}";
    // clang-format on

    EXPECT_EQ(marshall({setting}), expectedJson);
}

TEST_F(Http_Json_SettingMarshaller, Marshall_MultipleSettings)
{
    auto const setting1 = Setting::create(makeDefaultPatch().withKey("Key1"));
    auto const setting2 = Setting::create(makeDefaultPatch().withKey("Key2"));

    // clang-format off
    constexpr std::string_view expectedJson = "{"
        R"("settings":[)"
            "{"
                R"("key":"Key1")"
            "},"
            "{"
                R"("key":"Key2")"
            "}"
        "]"
    "}";
    // clang-format on

    EXPECT_EQ(marshall({setting1, setting2}), expectedJson);
}

TEST(Http_Json_SettingUnmarshaller, Unmarshall_InvalidJson)
{
    constexpr std::string_view json = R"({"key": "KEY",})";

    std::vector<DataLayer::Setting::Patch> result{};
    EXPECT_THROW(
        Json::SettingUnmarshaller::unmarshall(json, result),
        std::runtime_error
    );
}


TEST(Http_Json_SettingUnmarshaller, Unmarshall_EmptyJson)
{
    constexpr std::string_view json = "{}";

    std::vector<DataLayer::Setting::Patch> result{};
    EXPECT_THROW(
        Json::SettingUnmarshaller::unmarshall(json, result),
        std::runtime_error
    );
}

TEST(Http_Json_SettingUnmarshaller, Unmarshall_UnexpectedJson)
{
    constexpr std::string_view json = R"({"settings":"Value"})";

    std::vector<DataLayer::Setting::Patch> result{};
    EXPECT_THROW(
        Json::SettingUnmarshaller::unmarshall(json, result),
        std::runtime_error
    );
}

TEST(Http_Json_SettingUnmarshaller, Unmarshall_ValidJson)
{
    // clang-format off
    constexpr std::string_view json = "{"
        R"("settings":[)"
            "{"
                R"("key":"Key1",)"
                R"("value":"Value1")"
            "},"
            "{"
                R"("key":"Key2",)"
                R"("value":"Value2")"
            "}"
        "]"
    "}";
    // clang-format on

    std::vector<DataLayer::Setting::Patch> result{};
    ASSERT_NO_THROW(Json::SettingUnmarshaller::unmarshall(json, result));

    ASSERT_EQ(result.size(), 2);
    EXPECT_THAT(result.at(0).getKey(), Optional(Eq("Key1")));
    EXPECT_THAT(result.at(0).getValue(), Optional(Eq("Value1")));
    EXPECT_THAT(result.at(1).getKey(), Optional(Eq("Key2")));
    EXPECT_THAT(result.at(1).getValue(), Optional(Eq("Value2")));
}
