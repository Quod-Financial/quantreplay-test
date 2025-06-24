#include <gtest/gtest.h>

#include "api/predicate/expression.hpp"
#include "common/model.hpp"
#include "ih/pqxx/queries/detail/predicate_formatter.hpp"

using namespace Simulator::DataLayer;
using namespace Simulator::DataLayer::Predicate;

namespace {

struct SanitizerStub {
    template<typename T>
    auto operator()(T&& _value) -> std::string
    {
        return fmt::format("`{}`", std::forward<T>(_value));
    }
};

} // namespace

class DataLayer_Pqxx_PredicateFormatter : public ::testing::Test {
  public:
    using Model = TestModel;
    using Field = typename Model::Attribute;
    using Predicate = Predicate::Expression<Model>;
    using Formatter = Pqxx::Detail::PredicateFormatter<
        FakeColumnResolver,
        FakeEnumerationResolver,
        SanitizerStub>;

    auto makeFormatter() -> Formatter
    {
        return Formatter{mColumnResolver, mEnumResolver, mSanitizer};
    }

  private:
    FakeColumnResolver mColumnResolver;
    FakeEnumerationResolver mEnumResolver;
    SanitizerStub mSanitizer;
};

TEST_F(DataLayer_Pqxx_PredicateFormatter, Format_BasicOperation_Eq)
{
    Predicate const pred = eq<Model>(Field::IntegerField, 1);

    Formatter formatter = makeFormatter();
    formatter.accept(pred);

    EXPECT_EQ(formatter.compose(), "IntegerField = `1`");
}

TEST_F(DataLayer_Pqxx_PredicateFormatter, Format_BasicOperation_Neq)
{
    Predicate const pred = neq<Model>(Field::IntegerField, 1);

    Formatter formatter = makeFormatter();
    formatter.accept(pred);

    EXPECT_EQ(formatter.compose(), "IntegerField != `1`");
}

TEST_F(DataLayer_Pqxx_PredicateFormatter, Format_BasicOperation_Less)
{
    Predicate const pred = less<Model>(Field::IntegerField, 1);

    Formatter formatter = makeFormatter();
    formatter.accept(pred);

    EXPECT_EQ(formatter.compose(), "IntegerField < `1`");
}

TEST_F(DataLayer_Pqxx_PredicateFormatter, Format_BasicOperation_Greater)
{
    Predicate const pred = greater<Model>(Field::IntegerField, 1);

    Formatter formatter = makeFormatter();
    formatter.accept(pred);

    EXPECT_EQ(formatter.compose(), "IntegerField > `1`");
}

TEST_F(DataLayer_Pqxx_PredicateFormatter, Format_BasicOperation_LessEq)
{
    Predicate const pred = lessEq<Model>(Field::IntegerField, 1);

    Formatter formatter = makeFormatter();
    formatter.accept(pred);

    EXPECT_EQ(formatter.compose(), "IntegerField <= `1`");
}

TEST_F(DataLayer_Pqxx_PredicateFormatter, Format_BasicOperation_GreaterEq)
{
    Predicate const pred = greaterEq<Model>(Field::IntegerField, 1);

    Formatter formatter = makeFormatter();
    formatter.accept(pred);

    EXPECT_EQ(formatter.compose(), "IntegerField >= `1`");
}

TEST_F(DataLayer_Pqxx_PredicateFormatter, Format_BasicOperation_EnumerableValue)
{
    Predicate const pred =
        eq<Model>(Field::CustomField, TestModel::CustomFieldType::Value3);

    Formatter formatter = makeFormatter();
    formatter.accept(pred);

    EXPECT_EQ(formatter.compose(), "CustomField = `Value3`");
}

TEST_F(DataLayer_Pqxx_PredicateFormatter, Format_CompositeOperation_AND)
{
    Predicate const pred = eq<Model>(Field::IntegerField, 1) &&
                           eq<Model>(Field::BooleanField, true);

    Formatter formatter = makeFormatter();
    formatter.accept(pred);

    EXPECT_EQ(
        formatter.compose(),
        "IntegerField = `1` AND BooleanField = `true`"
    );
}

TEST_F(DataLayer_Pqxx_PredicateFormatter, Format_CompositeOperation_OR)
{
    Predicate const pred = eq<Model>(Field::IntegerField, 1) ||
                           eq<Model>(Field::BooleanField, true);

    Formatter formatter = makeFormatter();
    formatter.accept(pred);

    EXPECT_EQ(
        formatter.compose(),
        "IntegerField = `1` OR BooleanField = `true`"
    );
}

TEST_F(DataLayer_Pqxx_PredicateFormatter, Format_Subexpression)
{
    Predicate const pred =
        (eq<Model>(Field::IntegerField, 1) ||
         eq<Model>(Field::CustomField, TestModel::CustomFieldType::Value1)) &&
        (less<Model>(Field::DecimalField, 42.42) ||
         greater<Model>(Field::DecimalField, 32.32));

    Formatter formatter = makeFormatter();
    formatter.accept(pred);

    EXPECT_EQ(
        formatter.compose(),
        "( IntegerField = `1` OR CustomField = `Value1` ) AND "
        "( DecimalField < `42.42` OR DecimalField > `32.32` )"
    );
}