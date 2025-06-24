#include <utility>

#include <gtest/gtest.h>
#include <fmt/format.h>

#include "api/exceptions/exceptions.hpp"
#include "api/models/venue.hpp"
#include "ih/pqxx/queries/detail/insert_query_builder.hpp"

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


TEST(DataLayer_Pqxx_InsertQueryBuilder, Create_EmptyTableName)
{
    EXPECT_THROW(Pqxx::Detail::InsertQueryBuilder{""}, InternalError);
}

TEST(DataLayer_Pqxx_InsertQueryBuilder, Create_NonEmptyTableName)
{
    EXPECT_NO_THROW(Pqxx::Detail::InsertQueryBuilder{"A"});
}

TEST(DataLayer_Pqxx_InsertQueryBuilder, Compose_Empty)
{
    Pqxx::Detail::InsertQueryBuilder const builder{"table"};
    EXPECT_THROW((void)builder.compose(), MalformedPatch);
}

TEST(DataLayer_Pqxx_InsertQueryBuilder, Compose_NotBuilt)
{
    SanitizerStub sanitizer{};
    Pqxx::Detail::InsertQueryBuilder builder{"table"};

    auto dataExtractor = builder.makeDataExtractor(sanitizer);
    dataExtractor(Venue::Attribute::VenueId, "XETRA");

    // Note: builder.build(dataExtractor) is not called here

    EXPECT_THROW((void)builder.compose(), MalformedPatch);
}

TEST(DataLayer_Pqxx_InsertQueryBuilder, Compose_SingleColumnValuePair)
{
    SanitizerStub sanitizer{};
    Pqxx::Detail::InsertQueryBuilder builder{"table"};

    auto dataExtractor = builder.makeDataExtractor(sanitizer);
    dataExtractor(Venue::Attribute::VenueId, "XETRA");
    builder.build(dataExtractor);

    EXPECT_EQ(
        builder.compose(),
        "INSERT INTO table (venue_id) VALUES (`XETRA`)"
    );
}

TEST(DataLayer_Pqxx_InsertQueryBuilder, Compose_MultipleColumnValuePairs)
{
    SanitizerStub sanitizer{};
    Pqxx::Detail::InsertQueryBuilder builder{"table"};

    auto dataExtractor = builder.makeDataExtractor(sanitizer);
    dataExtractor(Venue::Attribute::VenueId, "LSE");
    dataExtractor(Venue::Attribute::Name, "London Stock Exchange");
    dataExtractor(Venue::Attribute::Timezone, "GMT");
    builder.build(dataExtractor);

    EXPECT_EQ(
        builder.compose(),
        "INSERT INTO table (venue_id, name, timezone) "
        "VALUES (`LSE`, `London Stock Exchange`, `GMT`)"
    );
}