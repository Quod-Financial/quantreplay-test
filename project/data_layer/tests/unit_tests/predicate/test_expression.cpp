#include <sstream>
#include <string>
#include <type_traits>

#include <gtest/gtest.h>

#include "api/predicate/definitions.hpp"
#include "api/predicate/expression.hpp"
#include "api/predicate/lexeme.hpp"
#include "common/model.hpp"

using namespace Simulator;
using namespace Simulator::DataLayer;

namespace {

class MockFormatter {
    using BasicOperation = Predicate::BasicOperation;
    using CompositeOperation = Predicate::CompositeOperation;
    using Field = TestModel::Attribute;

  public:
    template<typename T>
    auto operator()(Field _field, BasicOperation _op, T _value)
        -> std::enable_if_t<std::is_arithmetic_v<T>>
    {
        format(_field);
        format(_op);
        mStream << _value;
    }

    auto operator()(Field _field, BasicOperation _op, bool _value) -> void
    {
        format(_field);
        format(_op);
        mStream << std::boolalpha << _value << std::noboolalpha;
    }

    auto operator()(Field _field, BasicOperation _op, std::string const& _value)
        -> void
    {
        format(_field);
        format(_op);
        mStream << "'" << _value << "'";
    }

    auto operator()(
        Field _field,
        BasicOperation _op,
        TestModel::CustomFieldType _value
    ) -> void
    {
        format(_field);
        format(_op);
        format(_value);
    }

    auto operator()(CompositeOperation _op) -> void { format(_op); }

    auto operator()([[maybe_unused]] Predicate::SubExpressionBegin _lexeme)
        -> void
    {
        mStream << "(";
    }

    auto operator()([[maybe_unused]] Predicate::SubExpressionEnd _lexeme)
        -> void
    {
        mStream << ")";
    }

    auto string() const -> std::string { return mStream.str(); }

  private:
    auto format(TestModel::Attribute _field) -> void
    {
        switch (_field) {
            case TestModel::Attribute::BooleanField:
                mStream << "BooleanField";
                break;
            case TestModel::Attribute::IntegerField:
                mStream << "IntegerField";
                break;
            case TestModel::Attribute::UnsignedIntegerField:
                mStream << "UnsignedIntegerField";
                break;
            case TestModel::Attribute::DecimalField:
                mStream << "DecimalField";
                break;
            case TestModel::Attribute::StringField:
                mStream << "StringField";
                break;
            case TestModel::Attribute::CustomField:
                mStream << "CustomField";
                break;
        }
    }

    auto format(TestModel::CustomFieldType _value) -> void
    {
        switch (_value) {
            case TestModel::CustomFieldType::Value1:
                mStream << "Value1";
                break;
            case TestModel::CustomFieldType::Value2:
                mStream << "Value2";
                break;
            case TestModel::CustomFieldType::Value3:
                mStream << "Value3";
                break;
        }
    }

    auto format(Predicate::BasicOperation _op) -> void
    {
        switch (_op) {
            case Predicate::BasicOperation::Eq:
                mStream << " = ";
                break;
            case Predicate::BasicOperation::Neq:
                mStream << " != ";
                break;
            case Predicate::BasicOperation::Less:
                mStream << " < ";
                break;
            case Predicate::BasicOperation::Greater:
                mStream << " > ";
                break;
            case Predicate::BasicOperation::LessEq:
                mStream << " <= ";
                break;
            case Predicate::BasicOperation::GreaterEq:
                mStream << " >= ";
                break;
        }
    }

    auto format(Predicate::CompositeOperation _op) -> void
    {
        switch (_op) {
            case Predicate::CompositeOperation::AND:
                mStream << " AND ";
                break;
            case Predicate::CompositeOperation::OR:
                mStream << " OR ";
                break;
        }
    }

    std::stringstream mStream;
};

} // namespace

class DataLayer_Predicate_Expression : public ::testing::Test {
  public:
    using Expression = Predicate::Expression<TestModel>;
    using BasicLexeme = Expression::BasicLexeme;

    using Field = TestModel::Attribute;
    using BasicOperation = Predicate::BasicOperation;
    using CompositeOperation = Predicate::CompositeOperation;

    template<typename... Args>
    static auto makeBasicLexeme(Args&&... _args) -> BasicLexeme
    {
        return BasicLexeme{std::forward<Args>(_args)...};
    }

    static auto makeExpression(BasicLexeme _lexeme) -> Expression
    {
        return Expression{std::move(_lexeme)};
    }
};


TEST_F(DataLayer_Predicate_Expression, Basic)
{
    using Predicate::eq;

    std::string const value = "ABC";
    Expression const expression = eq<TestModel>(Field::StringField, value);

    EXPECT_TRUE(expression.isBasic());
    EXPECT_FALSE(expression.isComposite());
}

TEST_F(DataLayer_Predicate_Expression, Composite)
{
    using Predicate::eq;

    std::string const leftValue = "ABC";
    std::string const rightValue = "DEF";

    Expression const expression =
        eq<TestModel>(Field::StringField, leftValue) ||
        eq<TestModel>(Field::StringField, rightValue);

    EXPECT_FALSE(expression.isBasic());
    EXPECT_TRUE(expression.isComposite());
}

TEST_F(DataLayer_Predicate_Expression, Format_Basic_Boolean)
{
    using Predicate::neq;

    constexpr bool value = false;
    Expression const expression = neq<TestModel>(Field::BooleanField, value);

    MockFormatter formatter;
    ASSERT_NO_THROW(expression.accept(formatter));

    EXPECT_EQ(formatter.string(), "BooleanField != false");
}

TEST_F(DataLayer_Predicate_Expression, Format_Basic_Integer)
{
    using Predicate::less;

    constexpr std::int64_t value = 42;
    Expression const expression = less<TestModel>(Field::IntegerField, value);

    MockFormatter formatter;
    ASSERT_NO_THROW(expression.accept(formatter));

    EXPECT_EQ(formatter.string(), "IntegerField < 42");
}

TEST_F(DataLayer_Predicate_Expression, Format_Basic_UnsignedInteger)
{
    using Predicate::greater;

    constexpr std::uint64_t value = 142;
    Expression const expression =
        greater<TestModel>(Field::UnsignedIntegerField, value);

    MockFormatter formatter;
    ASSERT_NO_THROW(expression.accept(formatter));

    EXPECT_EQ(formatter.string(), "UnsignedIntegerField > 142");
}

TEST_F(DataLayer_Predicate_Expression, Format_Basic_Double)
{
    using Predicate::lessEq;

    constexpr double value = 42.42;
    Expression const expression = lessEq<TestModel>(Field::DecimalField, value);

    MockFormatter formatter;
    ASSERT_NO_THROW(expression.accept(formatter));

    EXPECT_EQ(formatter.string(), "DecimalField <= 42.42");
}

TEST_F(DataLayer_Predicate_Expression, Format_Basic_String)
{
    using Predicate::eq;

    std::string const value = "A Name";
    Expression const expression = eq<TestModel>(Field::StringField, value);

    MockFormatter formatter;
    ASSERT_NO_THROW(expression.accept(formatter));

    EXPECT_EQ(formatter.string(), "StringField = 'A Name'");
}

TEST_F(DataLayer_Predicate_Expression, Format_Basic_CustomField_Value1)
{
    using Predicate::eq;

    constexpr auto value = TestModel::CustomFieldType::Value1;
    Expression const expression = eq<TestModel>(Field::CustomField, value);

    MockFormatter formatter;
    ASSERT_NO_THROW(expression.accept(formatter));

    EXPECT_EQ(formatter.string(), "CustomField = Value1");
}

TEST_F(DataLayer_Predicate_Expression, Format_Basic_CustomField_Value2)
{
    using Predicate::neq;

    constexpr auto value = TestModel::CustomFieldType::Value2;
    Expression const expression = neq<TestModel>(Field::CustomField, value);

    MockFormatter formatter;
    ASSERT_NO_THROW(expression.accept(formatter));

    EXPECT_EQ(formatter.string(), "CustomField != Value2");
}

TEST_F(DataLayer_Predicate_Expression, Format_Basic_CustomField_Value3)
{
    using Predicate::eq;

    constexpr auto value = TestModel::CustomFieldType::Value3;
    Expression const expression = eq<TestModel>(Field::CustomField, value);

    MockFormatter formatter;
    ASSERT_NO_THROW(expression.accept(formatter));

    EXPECT_EQ(formatter.string(), "CustomField = Value3");
}

TEST_F(DataLayer_Predicate_Expression, Format_Composite_BasicLexemes_AND)
{
    using Predicate::lessEq;
    using Predicate::greaterEq;

    constexpr double fDecimalValue = 42.42;
    constexpr double sDecimalValue = 43.43;

    Expression const expression =
        greaterEq<TestModel>(Field::DecimalField, fDecimalValue) &&
        lessEq<TestModel>(Field::DecimalField, sDecimalValue);

    MockFormatter formatter;
    ASSERT_NO_THROW(expression.accept(formatter));

    EXPECT_EQ(
        formatter.string(),
        "DecimalField >= 42.42 AND DecimalField <= 43.43"
    );
}

TEST_F(DataLayer_Predicate_Expression, Format_Composite_BasicOnly_OR)
{
    using Predicate::eq;

    std::string const fString = "XETRA";
    std::string const sString = "FASTMATCH";

    Expression const expression = eq<TestModel>(Field::StringField, fString) ||
                                  eq<TestModel>(Field::StringField, sString);

    MockFormatter formatter;
    ASSERT_NO_THROW(expression.accept(formatter));

    EXPECT_EQ(
        formatter.string(),
        "StringField = 'XETRA' OR StringField = 'FASTMATCH'"
    );
}

TEST_F(DataLayer_Predicate_Expression, Format_Composite_BasicAndComposite_AND)
{
    using Predicate::neq;
    using Predicate::lessEq;
    using Predicate::greaterEq;

    constexpr double decimalValue = 42.42;
    constexpr std::uint64_t uintValue = 42;
    std::string const stringValue = "A STRING";

    Expression const expression =
        (lessEq<TestModel>(Field::DecimalField, decimalValue) ||
         greaterEq<TestModel>(Field::UnsignedIntegerField, uintValue)) &&
        neq<TestModel>(Field::StringField, stringValue);

    MockFormatter formatter;
    ASSERT_NO_THROW(expression.accept(formatter));

    EXPECT_EQ(
        formatter.string(),
        "(DecimalField <= 42.42 OR UnsignedIntegerField >= 42) AND "
        "StringField != 'A STRING'"
    );
}

TEST_F(DataLayer_Predicate_Expression, Format_Composite_BasicAndComposite_OR)
{
    using Predicate::neq;
    using Predicate::lessEq;
    using Predicate::greaterEq;

    constexpr double decimalValue = 42.42;
    constexpr std::uint64_t uintValue = 42;
    std::string const stringValue = "A STRING";

    Expression const expression =
        neq<TestModel>(Field::StringField, stringValue) ||
        (lessEq<TestModel>(Field::DecimalField, decimalValue) &&
         greaterEq<TestModel>(Field::UnsignedIntegerField, uintValue));

    MockFormatter formatter;
    ASSERT_NO_THROW(expression.accept(formatter));

    EXPECT_EQ(
        formatter.string(),
        "StringField != 'A STRING' OR "
        "(DecimalField <= 42.42 AND UnsignedIntegerField >= 42)"
    );
}

TEST_F(DataLayer_Predicate_Expression, Format_Composite_CompositesOnly_AND)
{
    using Predicate::eq;
    using Predicate::neq;
    using Predicate::lessEq;
    using Predicate::greaterEq;

    constexpr bool booleanValue = true;
    constexpr double decimalValue = 42.42;
    constexpr std::int64_t intValue = 42;
    std::string const stringValue = "A STRING";

    Expression const expression =
        (eq<TestModel>(Field::StringField, stringValue) ||
         neq<TestModel>(Field::BooleanField, booleanValue)) &&
        (lessEq<TestModel>(Field::IntegerField, intValue) ||
         greaterEq<TestModel>(Field::DecimalField, decimalValue));

    MockFormatter formatter;
    ASSERT_NO_THROW(expression.accept(formatter));

    EXPECT_EQ(
        formatter.string(),
        "(StringField = 'A STRING' OR BooleanField != true) AND "
        "(IntegerField <= 42 OR DecimalField >= 42.42)"
    );
}

TEST_F(DataLayer_Predicate_Expression, Format_Composite_CompositesOnly_OR)
{
    using Predicate::eq;
    using Predicate::neq;
    using Predicate::lessEq;
    using Predicate::greaterEq;

    constexpr bool booleanValue = true;
    constexpr double decimalValue = 42.42;
    constexpr std::int64_t intValue = 42;
    std::string const stringValue = "A STRING";

    Expression const expression =
        (eq<TestModel>(Field::StringField, stringValue) &&
         neq<TestModel>(Field::BooleanField, booleanValue)) ||
        (lessEq<TestModel>(Field::IntegerField, intValue) &&
         greaterEq<TestModel>(Field::DecimalField, decimalValue));

    MockFormatter formatter;
    ASSERT_NO_THROW(expression.accept(formatter));

    EXPECT_EQ(
        formatter.string(),
        "(StringField = 'A STRING' AND BooleanField != true) OR "
        "(IntegerField <= 42 AND DecimalField >= 42.42)"
    );
}
