#include <stdexcept>
#include <string>
#include <string_view>

#include <fmt/format.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "common/model.hpp"
#include "ih/common/queries/data_extractor.hpp"

using namespace Simulator::DataLayer;

using ::testing::ElementsAre;
using ::testing::Pair;

namespace {

struct SanitizerStub {
    template<typename T>
    auto operator()(T&& _value) -> std::string
    {
        return fmt::format("`{}`", std::forward<T>(_value));
    }
};

} // namespace

class DataLayer_Queries_DataExtractor : public ::testing::Test {
  public:
    using Column = TestModel::Attribute;
    using CustomField = TestModel::CustomFieldType;

    using ExtractorType = DataExtractor<
        FakeColumnResolver,
        FakeEnumerationResolver,
        SanitizerStub>;

    auto makeExtractor() -> ExtractorType
    {
        return ExtractorType{
            mColumnResolver,
            mEnumerationResolver,
            mSanitizerStub};
    }

  private:
    FakeColumnResolver mColumnResolver;
    FakeEnumerationResolver mEnumerationResolver;
    SanitizerStub mSanitizerStub;
};


TEST_F(DataLayer_Queries_DataExtractor, Extract_Field_Boolean)
{
    ExtractorType extractor = makeExtractor();
    ASSERT_NO_THROW(extractor(Column::BooleanField, true));

    EXPECT_THAT(
        extractor.getExtractedData(),
        ElementsAre(Pair("BooleanField", "`true`"))
    );
}

TEST_F(DataLayer_Queries_DataExtractor, Extract_Field_Integer)
{
    ExtractorType extractor = makeExtractor();
    ASSERT_NO_THROW(extractor(Column::IntegerField, 42));

    EXPECT_THAT(
        extractor.getExtractedData(),
        ElementsAre(Pair("IntegerField", "`42`"))
    );
}

TEST_F(DataLayer_Queries_DataExtractor, Extract_Field_UnsignedInteger)
{
    ExtractorType extractor = makeExtractor();
    ASSERT_NO_THROW(extractor(Column::UnsignedIntegerField, 4200U));

    EXPECT_THAT(
        extractor.getExtractedData(),
        ElementsAre(Pair("UnsignedIntegerField", "`4200`"))
    );
}

TEST_F(DataLayer_Queries_DataExtractor, Extract_Field_Double)
{
    ExtractorType extractor = makeExtractor();
    ASSERT_NO_THROW(extractor(Column::DecimalField, 42.42));

    EXPECT_THAT(
        extractor.getExtractedData(),
        ElementsAre(Pair("DecimalField", "`42.42`"))
    );
}

TEST_F(DataLayer_Queries_DataExtractor, Extract_Field_String)
{
    ExtractorType extractor = makeExtractor();
    ASSERT_NO_THROW(extractor(Column::StringField, "My cool value"));

    EXPECT_THAT(
        extractor.getExtractedData(),
        ElementsAre(Pair("StringField", "`My cool value`"))
    );
}

TEST_F(DataLayer_Queries_DataExtractor, Extract_Field_CustomType_Value1)
{
    ExtractorType extractor = makeExtractor();
    ASSERT_NO_THROW(extractor(Column::CustomField, CustomField::Value1));

    EXPECT_THAT(
        extractor.getExtractedData(),
        ElementsAre(Pair("CustomField", "`Value1`"))
    );
}

TEST_F(DataLayer_Queries_DataExtractor, Extract_Field_CustomType_Value2)
{
    ExtractorType extractor = makeExtractor();
    ASSERT_NO_THROW(extractor(Column::CustomField, CustomField::Value2));

    EXPECT_THAT(
        extractor.getExtractedData(),
        ElementsAre(Pair("CustomField", "`Value2`"))
    );
}

TEST_F(DataLayer_Queries_DataExtractor, Extract_Field_CustomType_Value3)
{
    ExtractorType extractor = makeExtractor();
    ASSERT_NO_THROW(extractor(Column::CustomField, CustomField::Value3));

    EXPECT_THAT(
        extractor.getExtractedData(),
        ElementsAre(Pair("CustomField", "`Value3`"))
    );
}

TEST_F(DataLayer_Queries_DataExtractor, Extract_Field_CustomType_UndefinedValue)
{
    ExtractorType extractor = makeExtractor();

    // Thrown by FakeEnumerationResolver
    EXPECT_THROW(
        extractor(Column::CustomField, static_cast<CustomField>(-1)),
        std::logic_error
    );

    EXPECT_TRUE(extractor.getExtractedData().empty());
}

TEST_F(DataLayer_Queries_DataExtractor, Extract_UndefinedColumnValue)
{
    ExtractorType extractor = makeExtractor();

    // Thrown by FakeColumnResolver
    EXPECT_THROW(extractor(static_cast<Column>(-1), 11), std::logic_error);
    EXPECT_TRUE(extractor.getExtractedData().empty());
}