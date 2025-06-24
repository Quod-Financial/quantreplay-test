#include <limits>
#include <stdexcept>
#include <string>

#include <gtest/gtest.h>
#include <rapidjson/document.h>

#include "ih/marshalling/json/detail/unmarshaller.hpp"

using namespace Simulator::Http;
using namespace Simulator::DataLayer;

class Http_Json_Unmarshaller : public ::testing::Test {
  public:
    constexpr inline static auto Attribute = Venue::Attribute::VenueId;
    constexpr inline static char const* Key = "id";

    template<typename T>
    auto addMember(char const* _key, T _value, rapidjson::Document& _document)
        -> void
    {
        if (!_document.IsObject()) {
            _document.SetObject();
        }

        rapidjson::Value key{_key, _document.GetAllocator()};

        rapidjson::Value value{std::forward<T>(_value)};

        _document.AddMember(key, value, _document.GetAllocator());
    }

    auto addMember(
        char const* _key,
        std::string const& _value,
        rapidjson::Document& _document
    ) -> void
    {
        if (!_document.IsObject()) {
            _document.SetObject();
        }

        rapidjson::Value key{_key, _document.GetAllocator()};
        rapidjson::Value value{_value.data(), _document.GetAllocator()};
        _document.AddMember(key, value, _document.GetAllocator());
    }
};


TEST_F(Http_Json_Unmarshaller, Create_WithNonObjectValue)
{
    rapidjson::Document const document{};
    ASSERT_FALSE(document.IsObject());

    EXPECT_THROW(Json::Unmarshaller{document}, std::logic_error);
}

TEST_F(Http_Json_Unmarshaller, Create_WithObjectValue)
{
    rapidjson::Document document{};
    document.SetObject();
    ASSERT_TRUE(document.IsObject());

    EXPECT_NO_THROW(Json::Unmarshaller{document});
}

TEST_F(Http_Json_Unmarshaller, Unmarshall_BadAttributeValue)
{
    constexpr auto badAttribute = static_cast<Venue::Attribute>(-1);

    rapidjson::Document document{};
    addMember(Key, true, document);

    Json::Unmarshaller unmarshaller{document};
    bool decoded{};

    // The exception is being thrown by an Attribute resolver
    EXPECT_THROW(unmarshaller(badAttribute, decoded), std::logic_error);
}

TEST_F(Http_Json_Unmarshaller, Unmarshall_Boolean_NotBooleanEncoded)
{
    rapidjson::Document document{};
    addMember(Key, 1, document);
    ASSERT_FALSE(document[Key].IsBool());

    Json::Unmarshaller unmarshaller{document};
    bool decoded{};

    EXPECT_THROW(unmarshaller(Attribute, decoded), std::runtime_error);
}

TEST_F(Http_Json_Unmarshaller, Unmarshall_Boolean_By_Key)
{
  rapidjson::Document document{};
  addMember(Key, true, document);
  ASSERT_TRUE(document[Key].IsBool());

  Json::Unmarshaller unmarshaller{document};
  bool decoded{};

  ASSERT_TRUE(unmarshaller(Key, decoded));
  EXPECT_EQ(decoded, true);
}

TEST_F(Http_Json_Unmarshaller, Unmarshall_Boolean_BooleanEncoded)
{
    rapidjson::Document document{};
    addMember(Key, true, document);
    ASSERT_TRUE(document[Key].IsBool());

    Json::Unmarshaller unmarshaller{document};
    bool decoded{};

    ASSERT_TRUE(unmarshaller(Attribute, decoded));
    EXPECT_EQ(decoded, true);
}

TEST_F(Http_Json_Unmarshaller, Unmarshall_Character_EmptyValue)
{
    std::string const value{};
    ASSERT_LT(value.size(), 1);
    rapidjson::Document document{};
    addMember(Key, value, document);

    Json::Unmarshaller unmarshaller{document};
    char decoded{};

    EXPECT_THROW(unmarshaller(Attribute, decoded), std::runtime_error);
}

TEST_F(Http_Json_Unmarshaller, Unmarshall_Character_ValueExceedsSize)
{
    std::string const value = "AB";
    ASSERT_GT(value.size(), 1);
    rapidjson::Document document{};
    addMember(Key, value, document);

    Json::Unmarshaller unmarshaller{document};
    char decoded{};

    EXPECT_THROW(unmarshaller(Attribute, decoded), std::runtime_error);
}

TEST_F(Http_Json_Unmarshaller, Unmarshall_Character_CorrectValue)
{
    std::string const value = ";";
    ASSERT_EQ(value.size(), 1);
    rapidjson::Document document{};
    addMember(Key, value, document);

    Json::Unmarshaller unmarshaller{document};
    char decoded{};

    ASSERT_TRUE(unmarshaller(Attribute, decoded));
    EXPECT_EQ(decoded, ';');
}

TEST_F(Http_Json_Unmarshaller, Unmarshall_Double_InvalidDataType)
{
    std::string const value = "bad_type";
    rapidjson::Document document{};
    addMember(Key, value, document);
    ASSERT_TRUE(document[Key].IsString());

    Json::Unmarshaller unmarshaller{document};
    double decoded{};

    EXPECT_THROW(unmarshaller(Attribute, decoded), std::runtime_error);
}

TEST_F(Http_Json_Unmarshaller, Unmarshall_Double_IntegerEncoded)
{
    rapidjson::Document document{};
    addMember(Key, 42, document); // NOLINT
    ASSERT_TRUE(document[Key].IsInt64());

    Json::Unmarshaller unmarshaller{document};
    double decoded{};

    ASSERT_TRUE(unmarshaller(Attribute, decoded));
    EXPECT_DOUBLE_EQ(decoded, 42.0);
}

TEST_F(Http_Json_Unmarshaller, Unmarshall_Double_UnsignedIntegerEncoded)
{
    rapidjson::Document document{};
    addMember(Key, 42U, document); // NOLINT
    ASSERT_TRUE(document[Key].IsUint64());

    Json::Unmarshaller unmarshaller{document};
    double decoded{};

    ASSERT_TRUE(unmarshaller(Attribute, decoded));
    EXPECT_DOUBLE_EQ(decoded, 42.0);
}

TEST_F(Http_Json_Unmarshaller, Unmarshall_Double_DoubleEncoded)
{
    rapidjson::Document document{};
    addMember(Key, 42.42, document); // NOLINT
    ASSERT_TRUE(document[Key].IsDouble());

    Json::Unmarshaller unmarshaller{document};
    double decoded{};

    ASSERT_TRUE(unmarshaller(Attribute, decoded));
    EXPECT_DOUBLE_EQ(decoded, 42.42);
}

TEST_F(Http_Json_Unmarshaller, Unmarshall_String_NotStringEncoded)
{
    rapidjson::Document document{};
    addMember(Key, false, document);
    ASSERT_FALSE(document[Key].IsString());

    Json::Unmarshaller unmarshaller{document};
    std::string decoded{};

    EXPECT_THROW(unmarshaller(Attribute, decoded), std::runtime_error);
}

TEST_F(Http_Json_Unmarshaller, Unmarshall_String_StringEncoded)
{
    std::string const value = "string_value";
    rapidjson::Document document{};
    addMember(Key, value, document);
    ASSERT_TRUE(document[Key].IsString());

    Json::Unmarshaller unmarshaller{document};
    std::string decoded{};

    ASSERT_TRUE(unmarshaller(Attribute, decoded));
    EXPECT_EQ(decoded, value);
}

TEST_F(Http_Json_Unmarshaller, Unmarshall_Integer_Signed_NotSignedEncoded)
{
    constexpr auto value = std::numeric_limits<std::uint64_t>::max();
    rapidjson::Document document{};
    addMember(Key, value, document);
    ASSERT_FALSE(document[Key].IsInt64());

    Json::Unmarshaller unmarshaller{document};
    std::int32_t decoded{};

    EXPECT_THROW(unmarshaller(Attribute, decoded), std::runtime_error);
}

TEST_F(Http_Json_Unmarshaller, Unmarshall_Integer_Signed_SignedEncoded)
{
    rapidjson::Document document{};
    addMember(Key, -42, document); // NOLINT
    ASSERT_TRUE(document[Key].IsInt64());

    Json::Unmarshaller unmarshaller{document};
    std::int32_t decoded{};

    ASSERT_TRUE(unmarshaller(Attribute, decoded));
    EXPECT_EQ(decoded, -42);
}

TEST_F(Http_Json_Unmarshaller, Unmarshall_Integer_Unsigned_NotUnsignedEncoded)
{
    rapidjson::Document document{};
    addMember(Key, -1, document);
    ASSERT_FALSE(document[Key].IsUint64());

    Json::Unmarshaller unmarshaller{document};
    std::uint32_t decoded{};

    EXPECT_THROW(unmarshaller(Attribute, decoded), std::runtime_error);
}

TEST_F(Http_Json_Unmarshaller, Unmarshall_Integer_Unsigned_UnsignedEncoded)
{
    constexpr auto value = std::numeric_limits<std::uint64_t>::max();
    rapidjson::Document document{};
    addMember(Key, value, document); // NOLINT
    ASSERT_TRUE(document[Key].IsUint64());

    Json::Unmarshaller unmarshaller{document};
    std::uint64_t decoded{};

    ASSERT_TRUE(unmarshaller(Attribute, decoded));
    EXPECT_EQ(decoded, value);
}

TEST_F(Http_Json_Unmarshaller, Unmarshall_Enum_NotStringEncoded)
{
    rapidjson::Document document{};
    addMember(Key, false, document);
    ASSERT_FALSE(document[Key].IsString());

    Json::Unmarshaller unmarshaller{document};
    Venue::EngineType decoded{};

    EXPECT_THROW(unmarshaller(Attribute, decoded), std::runtime_error);
}

TEST_F(Http_Json_Unmarshaller, Unmarshall_Enum_BadEnumStringEncoded)
{
    std::string const value = "bad_value";
    rapidjson::Document document{};
    addMember(Key, value, document);
    ASSERT_TRUE(document[Key].IsString());

    Json::Unmarshaller unmarshaller{document};
    Venue::EngineType decoded{};

    EXPECT_THROW(unmarshaller(Attribute, decoded), std::runtime_error);
}

TEST_F(Http_Json_Unmarshaller, Unmarshall_Enum_ValidEnumStringEncoded)
{
    std::string const value = "Quoting";
    rapidjson::Document document{};
    addMember(Key, value, document);
    ASSERT_TRUE(document[Key].IsString());

    Json::Unmarshaller unmarshaller{document};
    Venue::EngineType decoded{};

    ASSERT_TRUE(unmarshaller(Attribute, decoded));
    EXPECT_EQ(decoded, Venue::EngineType::Quoting);
}