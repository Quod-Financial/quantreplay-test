#include <utility>

#include <gtest/gtest.h>
#include <fmt/format.h>

#include "api/exceptions/exceptions.hpp"
#include "api/models/venue.hpp"
#include "ih/pqxx/queries/detail/update_query_builder.hpp"

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


TEST(DataLayer_Pqxx_UpdateQueryBuilder, Create_EmptyTableName)
{
    EXPECT_THROW(Pqxx::Detail::UpdateQueryBuilder{""}, InternalError);
}

TEST(DataLayer_Pqxx_UpdateQueryBuilder, Create_NonEmptyTableName)
{
    EXPECT_NO_THROW(Pqxx::Detail::UpdateQueryBuilder{"A"});
}

TEST(DataLayer_Pqxx_UpdateQueryBuilder, Compose_Empty)
{
    Pqxx::Detail::UpdateQueryBuilder const builder{"table"};
    EXPECT_THROW((void)builder.compose(), MalformedPatch);
}

TEST(DataLayer_Pqxx_UpdateQueryBuilder, Compose_NotBuilt)
{
    SanitizerStub sanitizer{};
    Pqxx::Detail::UpdateQueryBuilder builder{"table"};

    auto dataExtractor = builder.makeDataExtractor(sanitizer);
    dataExtractor(Venue::Attribute::VenueId, "XETRA");

    // Note: builder.build(dataExtractor) is not called here

    EXPECT_THROW((void)builder.compose(), MalformedPatch);
}

TEST(DataLayer_Pqxx_UpdateQueryBuilder, Compose_SingleColumnValuePair)
{
    SanitizerStub sanitizer{};
    Pqxx::Detail::UpdateQueryBuilder builder{"table"};

    auto dataExtractor = builder.makeDataExtractor(sanitizer);
    dataExtractor(Venue::Attribute::VenueId, "XETRA");
    builder.build(dataExtractor);

    EXPECT_EQ(
        builder.compose(),
        "UPDATE table SET venue_id = `XETRA`"
    );
}

TEST(DataLayer_Pqxx_UpdateQueryBuilder, Compose_MultipleColumnValuePairs)
{
    SanitizerStub sanitizer{};
    Pqxx::Detail::UpdateQueryBuilder builder{"table"};

    auto dataExtractor = builder.makeDataExtractor(sanitizer);
    dataExtractor(Venue::Attribute::VenueId, "LSE");
    dataExtractor(Venue::Attribute::Name, "London Stock Exchange");
    dataExtractor(Venue::Attribute::Timezone, "GMT");
    builder.build(dataExtractor);

    EXPECT_EQ(
        builder.compose(),
        "UPDATE table "
        "SET venue_id = `LSE`, name = `London Stock Exchange`, timezone = `GMT`"
    );
}

TEST(DataLayer_Pqxx_UpdateQueryBuilder, Compose_WithPredicateExpression)
{
    using Field = Venue::Attribute;

    SanitizerStub sanitizer{};
    Pqxx::Detail::UpdateQueryBuilder builder{"table"};

    auto dataExtractor = builder.makeDataExtractor(sanitizer);
    dataExtractor(Field::VenueId, "XETRA");
    builder.build(dataExtractor);

    auto const predicate = VenueCmp::eq(Field::VenueId, std::string{"LSE"});
    builder.withPredicateExpression(predicate, sanitizer);

    EXPECT_EQ(
        builder.compose(),
        "UPDATE table SET venue_id = `XETRA` WHERE venue_id = `LSE`"
    );
}

TEST(DataLayer_Pqxx_UpdateQueryBuilder, Compose_WithEqPredicate)
{
    using Field = Venue::Attribute;

    SanitizerStub sanitizer{};
    Pqxx::Detail::UpdateQueryBuilder builder{"table"};

    auto dataExtractor = builder.makeDataExtractor(sanitizer);
    dataExtractor(Field::VenueId, "XETRA");
    builder.build(dataExtractor);

    builder.withEqPredicate(Field::VenueId, "LSE", sanitizer);

    EXPECT_EQ(
        builder.compose(),
        "UPDATE table SET venue_id = `XETRA` WHERE venue_id = `LSE`"
    );
}

TEST(DataLayer_Pqxx_UpdateQueryBuilder, Compose_WithReturning)
{
    using Field = Venue::Attribute;

    SanitizerStub sanitizer{};
    Pqxx::Detail::UpdateQueryBuilder builder{"table"};

    auto dataExtractor = builder.makeDataExtractor(sanitizer);
    dataExtractor(Field::VenueId, "XETRA");
    builder.build(dataExtractor);

    builder.withEqPredicate(Field::VenueId, "LSE", sanitizer);
    builder.withReturning(Field::VenueId);

    EXPECT_EQ(
        builder.compose(),
        "UPDATE table SET venue_id = `XETRA` "
        "WHERE venue_id = `LSE` "
        "RETURNING venue_id"
    );
}