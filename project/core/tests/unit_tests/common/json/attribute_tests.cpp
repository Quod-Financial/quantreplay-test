#include <gtest/gtest.h>

#include "core/common/json/type.hpp"
#include "core/tools/time.hpp"
#include "test_definitions/test_attributes.hpp"

namespace simulator::core::json::test {
namespace {

TEST(CoreJsonAttributeReader, ThrowsExceptionOnReadBoolAsIntAttribute) {
  using namespace core::test;
  const rapidjson::Value value{true};

  ASSERT_THROW((void)Type<IntAttribute>::read_json_value(value),
               std::runtime_error);
}

TEST(CoreJsonAttributeReader, ReadsIntAttribute) {
  using namespace core::test;
  const rapidjson::Value value{42};

  ASSERT_EQ(Type<IntAttribute>::read_json_value(value), IntAttribute{42});
}

TEST(CoreJsonAttributeReader, ThrowsExceptionOnReadBoolAsDecimalAttribute) {
  using namespace core::test;
  const rapidjson::Value value{true};

  ASSERT_THROW((void)Type<DecimalAttribute>::read_json_value(value),
               std::runtime_error);
}

TEST(CoreJsonAttributeReader, ReadsDecimalAttribute) {
  using namespace core::test;
  const rapidjson::Value value{42.2};

  ASSERT_EQ(Type<DecimalAttribute>::read_json_value(value),
            DecimalAttribute{42.2});
}

TEST(CoreJsonAttributeReader, ThrowsExceptionOnReadBoolAsEnumAttribute) {
  using namespace core::test;
  const rapidjson::Value value{true};

  ASSERT_THROW((void)Type<EnumAttribute>::read_json_value(value),
               std::runtime_error);
}

TEST(CoreJsonAttributeReader, ReadsEnumAttribute) {
  using namespace core::test;
  const rapidjson::Value value{"Option2"};

  ASSERT_EQ(Type<EnumAttribute>::read_json_value(value),
            EnumAttribute::Option::Option2);
}

TEST(CoreJsonAttributeReader,
     ThrowsExceptionOnReadBoolAsUtcTimestampAttribute) {
  using namespace core::test;
  const rapidjson::Value value{true};

  ASSERT_THROW((void)Type<UtcTimestampAttribute>::read_json_value(value),
               std::runtime_error);
}

TEST(CoreJsonAttributeReader, ReadsUtcTimestampAttribute) {
  using namespace core::test;
  using namespace std::chrono_literals;
  const rapidjson::Value value{"2025-04-21 14:47:59.270438"};

  ASSERT_EQ(Type<UtcTimestampAttribute>::read_json_value(value),
            UtcTimestampAttribute{core::sys_us{core::sys_days{2025y / 4 / 21} +
                                               14h + 47min + 59s + 270438us}});
}

TEST(CoreJsonAttributeReader, ThrowsExceptionOnReadBoolAsLocalDateAttribute) {
  using namespace core::test;
  const rapidjson::Value value{true};

  ASSERT_THROW((void)Type<LocalDateAttribute>::read_json_value(value),
               std::runtime_error);
}

TEST(CoreJsonAttributeReader, ReadsLocalDateAttribute) {
  using namespace core::test;
  using namespace std::chrono_literals;
  const rapidjson::Value value{"2025-04-21"};

  ASSERT_EQ(Type<LocalDateAttribute>::read_json_value(value),
            LocalDateAttribute{core::local_days{2025y / 4 / 21}});
}

TEST(CoreJsonAttributeReader, ThrowsExceptionOnReadBoolAsLiteralAttribute) {
  using namespace core::test;
  const rapidjson::Value value{true};

  ASSERT_THROW((void)Type<LiteralAttribute>::read_json_value(value),
               std::runtime_error);
}

TEST(CoreJsonAttributeReader, ReadsLiteralAttribute) {
  using namespace core::test;
  const rapidjson::Value value{"literal"};

  ASSERT_EQ(Type<LiteralAttribute>::read_json_value(value),
            LiteralAttribute{"literal"});
}

TEST(CoreJsonAttributeReader, ThrowsExceptionOnReadBoolAsDerivedIntAttribute) {
  using namespace core::test;
  const rapidjson::Value value{true};

  ASSERT_THROW((void)Type<DerivedIntAttribute>::read_json_value(value),
               std::runtime_error);
}

TEST(CoreJsonAttributeReader, ReadsDerivedIntAttribute) {
  using namespace core::test;
  const rapidjson::Value value{42};

  ASSERT_EQ(Type<DerivedIntAttribute>::read_json_value(value),
            DerivedIntAttribute{IntAttribute{42}});
}

TEST(CoreJsonAttributeReader, ThrowsExceptionOnReadBoolAsDerivedEnumAttribute) {
  using namespace core::test;
  const rapidjson::Value value{true};

  ASSERT_THROW((void)Type<DerivedEnumAttribute>::read_json_value(value),
               std::runtime_error);
}

TEST(CoreJsonAttributeReader, ReadsDerivedEnumAttribute) {
  using namespace core::test;
  const rapidjson::Value value{"Option2"};

  ASSERT_EQ(Type<DerivedEnumAttribute>::read_json_value(value),
            DerivedEnumAttribute{EnumAttribute::Option::Option2});
}

TEST(CoreJsonAttributeReader,
     ThrowsExceptionOnReadBoolAsDerivedLiteralAttribute) {
  using namespace core::test;
  const rapidjson::Value value{true};

  ASSERT_THROW((void)Type<DerivedLiteralAttribute>::read_json_value(value),
               std::runtime_error);
}

TEST(CoreJsonAttributeReader, ReadsDerivedLiteralAttribute) {
  using namespace core::test;
  const rapidjson::Value value{"literal"};

  ASSERT_EQ(Type<DerivedLiteralAttribute>::read_json_value(value),
            DerivedLiteralAttribute{LiteralAttribute{"literal"}});
}

struct CoreJsonAttributeWriter : public ::testing::Test {
  rapidjson::Document doc;
  rapidjson::Value value;
};

TEST_F(CoreJsonAttributeWriter, WritesIntAttribute) {
  using namespace core::test;
  Type<IntAttribute>::write_json_value(
      value, doc.GetAllocator(), IntAttribute{42});

  ASSERT_TRUE(value.IsInt());
  ASSERT_EQ(value.GetInt(), 42);
}

TEST_F(CoreJsonAttributeWriter, WritesDecimalAttribute) {
  using namespace core::test;
  Type<DecimalAttribute>::write_json_value(
      value, doc.GetAllocator(), DecimalAttribute{42.2});

  ASSERT_TRUE(value.IsDouble());
  ASSERT_DOUBLE_EQ(value.GetDouble(), 42.2);
}

TEST_F(CoreJsonAttributeWriter, WritesEnumAttribute) {
  using namespace core::test;
  Type<EnumAttribute>::write_json_value(
      value, doc.GetAllocator(), EnumAttribute::Option::Option3);

  ASSERT_TRUE(value.IsString());
  ASSERT_STREQ(value.GetString(), "Option3");
}

TEST_F(CoreJsonAttributeWriter, WritesUtcTimestampAttribute) {
  using namespace core::test;
  using namespace std::chrono_literals;
  Type<UtcTimestampAttribute>::write_json_value(
      value,
      doc.GetAllocator(),
      UtcTimestampAttribute{core::sys_us{core::sys_days{2025y / 4 / 21} + 14h +
                                         47min + 59s + 270438us}});

  ASSERT_TRUE(value.IsString());
  ASSERT_STREQ(value.GetString(), "2025-04-21 14:47:59.270438");
}

TEST_F(CoreJsonAttributeWriter, WritesLocalDateAttribute) {
  using namespace core::test;
  using namespace std::chrono_literals;
  Type<LocalDateAttribute>::write_json_value(
      value,
      doc.GetAllocator(),
      LocalDateAttribute{core::local_days{2025y / 4 / 21}});

  ASSERT_TRUE(value.IsString());
  ASSERT_STREQ(value.GetString(), "2025-04-21");
}

TEST_F(CoreJsonAttributeWriter, WritesLiteralAttribute) {
  using namespace core::test;
  Type<LiteralAttribute>::write_json_value(
      value, doc.GetAllocator(), LiteralAttribute{"literal"});

  ASSERT_TRUE(value.IsString());
  ASSERT_STREQ(value.GetString(), "literal");
}

TEST_F(CoreJsonAttributeWriter, WritesDerivedIntAttribute) {
  using namespace core::test;
  Type<DerivedIntAttribute>::write_json_value(
      value, doc.GetAllocator(), DerivedIntAttribute{IntAttribute{42}});

  ASSERT_TRUE(value.IsInt());
  ASSERT_EQ(value.GetInt(), 42);
}

TEST_F(CoreJsonAttributeWriter, WritesDerivedEnumAttribute) {
  using namespace core::test;
  Type<DerivedEnumAttribute>::write_json_value(
      value,
      doc.GetAllocator(),
      DerivedEnumAttribute{EnumAttribute::Option::Option2});

  ASSERT_TRUE(value.IsString());
  ASSERT_STREQ(value.GetString(), "Option2");
}

TEST_F(CoreJsonAttributeWriter, WritesDerivedLiteralAttribute) {
  using namespace core::test;
  Type<DerivedLiteralAttribute>::write_json_value(
      value, doc.GetAllocator(), DerivedLiteralAttribute{"literal"});

  ASSERT_TRUE(value.IsString());
  ASSERT_STREQ(value.GetString(), "literal");
}

}  // namespace
}  // namespace simulator::core::json::test