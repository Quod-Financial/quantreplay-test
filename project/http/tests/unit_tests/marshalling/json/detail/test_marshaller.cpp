#include <stdexcept>
#include <string>

#include <gtest/gtest.h>

#include <rapidjson/document.h>

#include "data_layer/api/models/venue.hpp"
#include "ih/marshalling/json/detail/marshaller.hpp"

using namespace Simulator::Http;
using namespace Simulator::DataLayer;

TEST(Http_Json_Marshaller, Create_WithNonObjectDocument)
{
    rapidjson::Document document{};
    ASSERT_FALSE(document.IsObject());

    Json::Marshaller const marshaller{document};
    EXPECT_TRUE(document.IsObject());
}

TEST(Http_Json_Marshaller, Create_WithObjectDocument)
{
    rapidjson::Document document{};
    document.SetObject();
    ASSERT_TRUE(document.IsObject());

    Json::Marshaller const marshaller{document};
    EXPECT_TRUE(document.IsObject());
}

TEST(Http_Json_Marshaller, Marshall_BadAttributeValue)
{
    constexpr auto badAttribute = static_cast<Venue::Attribute>(-1);
    rapidjson::Document document{};
    Json::Marshaller marshaller{document};

    // The exception is being thrown by an Attribute key resolver
    EXPECT_THROW(marshaller(badAttribute, true), std::logic_error);
}

TEST(Http_Json_Marshaller, Marshall_Boolean)
{
    constexpr auto attribute = Venue::Attribute::VenueId;
    constexpr char const* key = "id";

    rapidjson::Document document{};
    Json::Marshaller marshaller{document};

    ASSERT_NO_THROW(marshaller(attribute, true));

    ASSERT_TRUE(document.HasMember(key));
    ASSERT_TRUE(document[key].IsBool());
    EXPECT_EQ(document[key].GetBool(), true);
}

TEST(Http_Json_Marshaller, Marshall_Character)
{
    constexpr auto attribute = Venue::Attribute::VenueId;
    constexpr char const* key = "id";

    rapidjson::Document document{};
    Json::Marshaller marshaller{document};

    constexpr char character = ';';
    ASSERT_NO_THROW(marshaller(attribute, character));

    ASSERT_TRUE(document.HasMember(key));
    ASSERT_TRUE(document[key].IsString());
    EXPECT_EQ(document[key].GetStringLength(), 1);
    EXPECT_STREQ(document[key].GetString(), ";");
}

TEST(Http_Json_Marshaller, Marshall_Double)
{
    constexpr auto attribute = Venue::Attribute::VenueId;
    constexpr char const* key = "id";

    rapidjson::Document document{};
    Json::Marshaller marshaller{document};

    ASSERT_NO_THROW(marshaller(attribute, 42.42)); // NOLINT

    ASSERT_TRUE(document.HasMember(key));
    ASSERT_TRUE(document[key].IsDouble());
    EXPECT_DOUBLE_EQ(document[key].GetDouble(), 42.42);
}

TEST(Http_Json_Marshaller, Marshall_Integer_Signed)
{
    constexpr auto attribute = Venue::Attribute::VenueId;
    constexpr char const* key = "id";

    rapidjson::Document document{};
    Json::Marshaller marshaller{document};

    ASSERT_NO_THROW(marshaller(attribute, -42)); // NOLINT

    ASSERT_TRUE(document.HasMember(key));
    ASSERT_TRUE(document[key].IsInt64());
    EXPECT_EQ(document[key].GetInt64(), -42);
}

TEST(Http_Json_Marshaller, Marshall_Integer_Unsigned)
{
    constexpr auto attribute = Venue::Attribute::VenueId;
    constexpr char const* key = "id";

    rapidjson::Document document{};
    Json::Marshaller marshaller{document};

    ASSERT_NO_THROW(marshaller(attribute, 42U)); // NOLINT

    ASSERT_TRUE(document.HasMember(key));
    ASSERT_TRUE(document[key].IsUint64());
    EXPECT_EQ(document[key].GetUint64(), 42U);
}

TEST(Http_Json_Marshaller, Marshall_Enumeration_BadValue)
{
    constexpr auto attribute = Venue::Attribute::VenueId;
    constexpr auto badEnum = static_cast<Venue::EngineType>(-1);

    rapidjson::Document document{};
    Json::Marshaller marshaller{document};

    // The exception is being thrown by an Enumeration resolver
    EXPECT_THROW(marshaller(attribute, badEnum), std::logic_error);
}

TEST(Http_Json_Marshaller, Marshall_Enumeration_CorrectValue)
{
    constexpr auto attribute = Venue::Attribute::VenueId;
    constexpr char const* key = "id";
    constexpr auto enumValue = Venue::EngineType::Matching;
    constexpr std::string_view expectedValue = "Matching";

    rapidjson::Document document{};
    Json::Marshaller marshaller{document};

    ASSERT_NO_THROW(marshaller(attribute, enumValue)); // NOLINT

    ASSERT_TRUE(document.HasMember(key));
    ASSERT_TRUE(document[key].IsString());
    EXPECT_EQ(document[key].GetStringLength(), expectedValue.size());
    EXPECT_EQ(document[key].GetString(), expectedValue);
}