#include <string>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "api/predicate/lexeme.hpp"
#include "common/model.hpp"

using namespace Simulator;
using namespace Simulator::DataLayer;

using ::testing::_; // NOLINT
using ::testing::Eq;
using ::testing::DoubleEq;

namespace {

class MockFormatter {
  public:
    using Field = TestModel::Attribute;
    using Operation = Simulator::DataLayer::Predicate::BasicOperation;
    using CustomType = TestModel::CustomFieldType;

    using CompositeOperation =
        Simulator::DataLayer::Predicate::CompositeOperation;

    using SubExpressionBegin =
        Simulator::DataLayer::Predicate::SubExpressionBegin;

    using SubExpressionEnd = Simulator::DataLayer::Predicate::SubExpressionEnd;

    auto operator()(Field _field, Operation _op, bool _val) -> void
    {
        formatBoolean(_field, _op, _val);
    }

    auto operator()(Field _field, Operation _op, std::int64_t _val) -> void
    {
        formatInt(_field, _op, _val);
    }

    auto operator()(Field _field, Operation _op, std::uint64_t _val) -> void
    {
        formatUint(_field, _op, _val);
    }

    auto operator()(Field _field, Operation _op, std::double_t _val) -> void
    {
        formatDouble(_field, _op, _val);
    }

    auto operator()(Field _field, Operation _op, std::string const& _val)
        -> void
    {
        formatString(_field, _op, _val);
    }

    auto operator()(Field _field, Operation _op, CustomType _val) -> void
    {
        formatCustom(_field, _op, _val);
    }


    auto operator()(CompositeOperation _op) -> void { formatComposite(_op); }


    auto operator()([[maybe_unused]] SubExpressionBegin _exp) -> void
    {
        formatSubExpressionBegin();
    }

    auto operator()([[maybe_unused]] SubExpressionEnd _exp) -> void
    {
        formatSubExpressionEnd();
    }

    MOCK_METHOD(void, formatBoolean, (Field, Operation, bool));
    MOCK_METHOD(void, formatInt, (Field, Operation, std::int64_t));
    MOCK_METHOD(void, formatUint, (Field, Operation, std::uint64_t));
    MOCK_METHOD(void, formatDouble, (Field, Operation, std::double_t));
    MOCK_METHOD(void, formatString, (Field, Operation, std::string const&));
    MOCK_METHOD(void, formatCustom, (Field, Operation, CustomType));

    MOCK_METHOD(void, formatComposite, (CompositeOperation));

    MOCK_METHOD(void, formatSubExpressionBegin, ());
    MOCK_METHOD(void, formatSubExpressionEnd, ());
};

} // namespace

class DataLayer_Predicate_BasicPredicateLexeme : public ::testing::Test {
  public:
    using Field = TestModel::Attribute;
    using Operation = Predicate::BasicOperation;
    using BasicPredicate = Predicate::BasicPredicate<TestModel>;

    static_assert(std::is_copy_constructible_v<BasicPredicate>);
    static_assert(std::is_move_constructible_v<BasicPredicate>);

    template<typename... Args>
    static auto makePredicate(Args&&... _args) -> BasicPredicate
    {
        return BasicPredicate{std::forward<Args>(_args)...};
    }
};

class DataLayer_Predicate_CompositePredicateLexeme : public ::testing::Test {
  public:
    using Operation = Predicate::CompositeOperation;
    using CompositePredicate = Predicate::CompositePredicate;

    static_assert(std::is_copy_constructible_v<CompositePredicate>);
    static_assert(std::is_move_constructible_v<CompositePredicate>);

    static auto makePredicate(Operation _op) -> CompositePredicate
    {
        return CompositePredicate{_op};
    }
};

class DataLayer_Predicate_SubExpressionLexeme : public ::testing::Test {
  public:
    using SubExpressionBegin = Predicate::SubExpressionBegin;
    using SubExpressionEnd = Predicate::SubExpressionEnd;

    static_assert(std::is_copy_constructible_v<SubExpressionBegin>);
    static_assert(std::is_move_constructible_v<SubExpressionBegin>);

    static_assert(std::is_copy_constructible_v<SubExpressionEnd>);
    static_assert(std::is_move_constructible_v<SubExpressionEnd>);

    static auto makeSubExpressionBegin() -> SubExpressionBegin { return {}; }
    static auto makeSubExpressionEnd() -> SubExpressionEnd { return {}; }
};


TEST_F(DataLayer_Predicate_BasicPredicateLexeme, Accept_Attribute_BooleanField)
{
    constexpr bool value = true;
    BasicPredicate const predicate =
        makePredicate(Field::BooleanField, Operation::Eq, value);

    MockFormatter formatter{};
    EXPECT_CALL(formatter, formatBoolean(Eq(Field::BooleanField), _, _))
        .Times(1);

    predicate.accept(formatter);
}


TEST_F(DataLayer_Predicate_BasicPredicateLexeme, Accept_Attribute_IntegerField)
{
    constexpr std::int64_t value = 1;
    BasicPredicate const predicate =
        makePredicate(Field::IntegerField, Operation::Eq, value);

    MockFormatter formatter{};
    EXPECT_CALL(formatter, formatInt(Eq(Field::IntegerField), _, _)).Times(1);

    predicate.accept(formatter);
}

TEST_F(
    DataLayer_Predicate_BasicPredicateLexeme,
    Accept_Attribute_UnsIntegerField
)
{
    constexpr std::uint64_t value = 1;
    BasicPredicate const predicate =
        makePredicate(Field::UnsignedIntegerField, Operation::Eq, value);

    MockFormatter formatter{};
    EXPECT_CALL(formatter, formatUint(Eq(Field::UnsignedIntegerField), _, _))
        .Times(1);

    predicate.accept(formatter);
}

TEST_F(DataLayer_Predicate_BasicPredicateLexeme, Accept_Attribute_DecimalField)
{
    constexpr std::double_t value = 1.;
    BasicPredicate const predicate =
        makePredicate(Field::DecimalField, Operation::Eq, value);

    MockFormatter formatter{};
    EXPECT_CALL(formatter, formatDouble(Eq(Field::DecimalField), _, _))
        .Times(1);

    predicate.accept(formatter);
}

TEST_F(DataLayer_Predicate_BasicPredicateLexeme, Accept_Attribute_StringField)
{
    std::string const value = "ABC";
    BasicPredicate const predicate =
        makePredicate(Field::StringField, Operation::Eq, value);

    MockFormatter formatter{};
    EXPECT_CALL(formatter, formatString(Eq(Field::StringField), _, _)).Times(1);

    predicate.accept(formatter);
}

TEST_F(DataLayer_Predicate_BasicPredicateLexeme, Accept_Attribute_CustomField)
{
    constexpr auto value = TestModel::CustomFieldType::Value1;
    BasicPredicate const predicate =
        makePredicate(Field::CustomField, Operation::Eq, value);

    MockFormatter formatter{};
    EXPECT_CALL(formatter, formatCustom(Eq(Field::CustomField), _, _)).Times(1);

    predicate.accept(formatter);
}

TEST_F(DataLayer_Predicate_BasicPredicateLexeme, Accept_Operation_Eq)
{
    constexpr bool value = true;
    BasicPredicate const predicate =
        makePredicate(Field::BooleanField, Operation::Eq, value);

    MockFormatter formatter{};
    EXPECT_CALL(formatter, formatBoolean(_, Eq(Operation::Eq), _)).Times(1);

    predicate.accept(formatter);
}

TEST_F(DataLayer_Predicate_BasicPredicateLexeme, Accept_Operation_Neq)
{
    constexpr bool value = true;
    BasicPredicate const predicate =
        makePredicate(Field::BooleanField, Operation::Neq, value);

    MockFormatter formatter{};
    EXPECT_CALL(formatter, formatBoolean(_, Eq(Operation::Neq), _)).Times(1);

    predicate.accept(formatter);
}

TEST_F(DataLayer_Predicate_BasicPredicateLexeme, Accept_Operation_Less)
{
    constexpr bool value = true;
    BasicPredicate const predicate =
        makePredicate(Field::BooleanField, Operation::Less, value);

    MockFormatter formatter{};
    EXPECT_CALL(formatter, formatBoolean(_, Eq(Operation::Less), _)).Times(1);

    predicate.accept(formatter);
}

TEST_F(DataLayer_Predicate_BasicPredicateLexeme, Accept_Operation_Greater)
{
    constexpr bool value = true;
    BasicPredicate const predicate =
        makePredicate(Field::BooleanField, Operation::Greater, value);

    MockFormatter formatter{};
    EXPECT_CALL(formatter, formatBoolean(_, Eq(Operation::Greater), _))
        .Times(1);

    predicate.accept(formatter);
}

TEST_F(DataLayer_Predicate_BasicPredicateLexeme, Accept_Operation_LessEq)
{
    constexpr bool value = true;
    BasicPredicate const predicate =
        makePredicate(Field::BooleanField, Operation::LessEq, value);

    MockFormatter formatter{};
    EXPECT_CALL(formatter, formatBoolean(_, Eq(Operation::LessEq), _)).Times(1);

    predicate.accept(formatter);
}

TEST_F(DataLayer_Predicate_BasicPredicateLexeme, Accept_Operation_GreaterEq)
{
    constexpr bool value = true;
    BasicPredicate const predicate =
        makePredicate(Field::BooleanField, Operation::GreaterEq, value);

    MockFormatter formatter{};
    EXPECT_CALL(formatter, formatBoolean(_, Eq(Operation::GreaterEq), _))
        .Times(1);

    predicate.accept(formatter);
}

TEST_F(DataLayer_Predicate_BasicPredicateLexeme, Accept_Value_Boolean)
{
    constexpr bool value = true;
    BasicPredicate const predicate =
        makePredicate(Field::BooleanField, Operation::Eq, value);

    MockFormatter formatter{};
    EXPECT_CALL(formatter, formatBoolean(_, _, Eq(value))).Times(1);

    predicate.accept(formatter);
}

TEST_F(DataLayer_Predicate_BasicPredicateLexeme, Accept_Value_Integer)
{
    constexpr std::int64_t value = 42;
    BasicPredicate const predicate =
        makePredicate(Field::IntegerField, Operation::Eq, value);

    MockFormatter formatter{};
    EXPECT_CALL(formatter, formatInt(_, _, Eq(value))).Times(1);

    predicate.accept(formatter);
}

TEST_F(DataLayer_Predicate_BasicPredicateLexeme, Accept_Value_UnsignedInteger)
{
    constexpr std::uint64_t value = 42;
    BasicPredicate const predicate =
        makePredicate(Field::UnsignedIntegerField, Operation::Eq, value);

    MockFormatter formatter{};
    EXPECT_CALL(formatter, formatUint(_, _, Eq(value))).Times(1);

    predicate.accept(formatter);
}

TEST_F(DataLayer_Predicate_BasicPredicateLexeme, Accept_Value_Double)
{
    constexpr std::double_t value = 42.42;
    BasicPredicate const predicate =
        makePredicate(Field::DecimalField, Operation::Eq, value);

    MockFormatter formatter{};
    EXPECT_CALL(formatter, formatDouble(_, _, DoubleEq(value))).Times(1);

    predicate.accept(formatter);
}

TEST_F(DataLayer_Predicate_BasicPredicateLexeme, Accept_Value_String)
{
    std::string const value = "ABC123";
    BasicPredicate const predicate =
        makePredicate(Field::StringField, Operation::Eq, value);

    MockFormatter formatter{};
    EXPECT_CALL(formatter, formatString(_, _, Eq(value))).Times(1);

    predicate.accept(formatter);
}

TEST_F(DataLayer_Predicate_BasicPredicateLexeme, Accept_Value_CustomType_Value1)
{
    constexpr auto value = TestModel::CustomFieldType::Value1;
    BasicPredicate const predicate =
        makePredicate(Field::CustomField, Operation::Eq, value);

    MockFormatter formatter{};
    EXPECT_CALL(formatter, formatCustom(_, _, Eq(value))).Times(1);

    predicate.accept(formatter);
}

TEST_F(DataLayer_Predicate_BasicPredicateLexeme, Accept_Value_CustomType_Value2)
{
    constexpr auto value = TestModel::CustomFieldType::Value2;
    BasicPredicate const predicate =
        makePredicate(Field::CustomField, Operation::Eq, value);

    MockFormatter formatter{};
    EXPECT_CALL(formatter, formatCustom(_, _, Eq(value))).Times(1);

    predicate.accept(formatter);
}

TEST_F(DataLayer_Predicate_BasicPredicateLexeme, Accept_Value_CustomType_Value3)
{
    constexpr auto value = TestModel::CustomFieldType::Value3;
    BasicPredicate const predicate =
        makePredicate(Field::CustomField, Operation::Eq, value);

    MockFormatter formatter{};
    EXPECT_CALL(formatter, formatCustom(_, _, Eq(value))).Times(1);

    predicate.accept(formatter);
}

TEST_F(DataLayer_Predicate_CompositePredicateLexeme, Accept_Operation_AND)
{
    CompositePredicate const predicate = makePredicate(Operation::AND);

    MockFormatter formatter{};
    EXPECT_CALL(formatter, formatComposite(Eq(Operation::AND)));

    predicate.accept(formatter);
}

TEST_F(DataLayer_Predicate_CompositePredicateLexeme, Accept_Operation_OR)
{
    CompositePredicate const predicate = makePredicate(Operation::OR);

    MockFormatter formatter{};
    EXPECT_CALL(formatter, formatComposite(Eq(Operation::OR)));

    predicate.accept(formatter);
}

TEST_F(DataLayer_Predicate_SubExpressionLexeme, Accept_SubExpressionBegin)
{
    SubExpressionBegin const expression = makeSubExpressionBegin();

    MockFormatter formatter{};
    EXPECT_CALL(formatter, formatSubExpressionBegin).Times(1);

    expression.accept(formatter);
}

TEST_F(DataLayer_Predicate_SubExpressionLexeme, Accept_SubExpressionEnd)
{
    SubExpressionEnd const expression = makeSubExpressionEnd();

    MockFormatter formatter{};
    EXPECT_CALL(formatter, formatSubExpressionEnd).Times(1);

    expression.accept(formatter);
}