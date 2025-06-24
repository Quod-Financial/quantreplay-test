#include <string>
#include <utility>

#include <gtest/gtest.h>
#include <fmt/format.h>

#include "api/exceptions/exceptions.hpp"
#include "api/models/venue.hpp"
#include "api/predicate/predicate.hpp"
#include "ih/pqxx/queries/detail/select_query_builder.hpp"

using namespace Simulator::DataLayer;

namespace {

struct SanitizerStub {
    template<typename T>
    auto operator()(T&& _value) -> std::string
    {
        return fmt::format("`{}`", std::forward<T>(_value));
    }
};

} // namespace


TEST(DataLayer_Pqxx_SelectQueryBuilder, Create_EmptyTableName)
{
    EXPECT_THROW(Pqxx::Detail::SelectQueryBuilder{""}, InternalError);
}

TEST(DataLayer_Pqxx_SelectQueryBuilder, Create_NonEmptyTable)
{
    EXPECT_NO_THROW(Pqxx::Detail::SelectQueryBuilder{"A"});
}

TEST(DataLayer_Pqxx_SelectQueryBuilder, Compose_WithoutPredicate)
{
    Pqxx::Detail::SelectQueryBuilder const builder{"table_name"};
    EXPECT_EQ(builder.compose(), "SELECT * FROM table_name");
}

TEST(DataLayer_Pqxx_SelectQueryBuilder, Compose_WithPredicateExpression)
{
    using Field = Venue::Attribute;

    SanitizerStub sanitizer{};
    auto const predicate = VenueCmp::eq(Field::Timezone, std::string{"GMT+3"});
    Pqxx::Detail::SelectQueryBuilder builder{"table_name"};
    builder.withPredicateExpression(predicate, sanitizer);

    EXPECT_EQ(
        builder.compose(),
        "SELECT * FROM table_name WHERE timezone = `GMT+3`"
    );
}

TEST(DataLayer_Pqxx_SelectQueryBuilder, Compose_WithEqPredicate)
{
    using Field = Venue::Attribute;

    SanitizerStub sanitizer{};
    Pqxx::Detail::SelectQueryBuilder builder{"table_name"};
    builder.withEqPredicate(Field::VenueId, "LSE", sanitizer);

    EXPECT_EQ(
        builder.compose(),
        "SELECT * FROM table_name WHERE venue_id = `LSE`"
    );
}