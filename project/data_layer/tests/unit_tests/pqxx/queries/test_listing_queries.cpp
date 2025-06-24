#include <string>

#include <fmt/format.h>
#include <gtest/gtest.h>

#include "api/exceptions/exceptions.hpp"
#include "api/models/listing.hpp"
#include "api/predicate/predicate.hpp"
#include "ih/pqxx/queries/listing_queries.hpp"

using namespace Simulator::DataLayer;

namespace {

struct SanitizerStub {
    template<typename T>
    auto operator()(T&& _value) -> std::string
    {
        return fmt::format("`{}`", std::forward<T>(_value));
    }
};

TEST(DataLayer_Pqxx_ListingQuery_Insert, Compose_FromEmptyPatch)
{
    using Query = Pqxx::ListingQuery::Insert;

    Listing::Patch const patch{};
    SanitizerStub sanitizer{};

    auto const query = Query::prepare(patch, sanitizer);
    EXPECT_THROW((void)query.compose(), MalformedPatch);
}

TEST(DataLayer_Pqxx_ListingQuery_Insert, Compose_FromNonEmptyPatch)
{
    using Query = Pqxx::ListingQuery::Insert;

    SanitizerStub sanitizer{};
    Listing::Patch patch{};
    patch.withSymbol("AAPL").withVenueId("NASDAQ");

    auto const query = Query::prepare(patch, sanitizer).returningId();
    EXPECT_EQ(
        query.compose(),
        "INSERT INTO listing (symbol, venue_id) "
        "VALUES (`AAPL`, `NASDAQ`) "
        "RETURNING listing_id"
    );
}

TEST(DataLayer_Pqxx_ListingQuery_Select, Compose_WithPredicateExpression)
{
    using Column = Listing::Attribute;
    using Query = Pqxx::ListingQuery::Select;

    SanitizerStub sanitizer{};
    auto const predicate =
        ListingCmp::greater(Column::QtyMinimum, 32.32) &&
        ListingCmp::less(Column::QtyMaximum, 42.42);

    auto const query = Query::prepare().by(predicate, sanitizer);
    EXPECT_EQ(
        query.compose(),
        "SELECT * FROM listing "
        "WHERE qty_minimum > `32.32` AND qty_maximum < `42.42`"
    );
}

TEST(DataLayer_Pqxx_ListingQuery_Select, Compose_WithByListingIdPredicate)
{
    using Query = Pqxx::ListingQuery::Select;

    SanitizerStub sanitizer{};
    auto const query = Query::prepare().byListingId(42, sanitizer); // NOLINT
    EXPECT_EQ(
        query.compose(),
        "SELECT * FROM listing WHERE listing_id = `42`"
    );
}

TEST(DataLayer_Pqxx_ListingQuery_Update, Compose_FromEmptyPatch)
{
    using Query = Pqxx::ListingQuery::Update;

    Listing::Patch const patch{};
    SanitizerStub sanitizer{};

    auto const query = Query::prepare(patch, sanitizer);
    EXPECT_THROW((void)query.compose(), MalformedPatch);
}

TEST(DataLayer_Pqxx_ListingQuery_Update, Compose_FromNonEmptyPatch)
{
    using Query = Pqxx::ListingQuery::Update;

    SanitizerStub sanitizer{};
    Listing::Patch patch{};
    patch.withSymbol("AAPL").withVenueId("NASDAQ");

    auto const query = Query::prepare(patch, sanitizer);
    EXPECT_EQ(
        query.compose(),
        "UPDATE listing SET symbol = `AAPL`, venue_id = `NASDAQ`"
    );
}

TEST(DataLayer_Pqxx_ListingQuery_Update, Compose_WithPredicate)
{
    using Column = Listing::Attribute;
    using Query = Pqxx::ListingQuery::Update;

    SanitizerStub sanitizer{};
    Listing::Patch patch{};
    patch.withSymbol("AAPL").withVenueId("NASDAQ");
    auto const predicate =
        ListingCmp::greater(Column::QtyMinimum, 32.32) &&
        ListingCmp::less(Column::QtyMaximum, 42.42);

    auto const query =
        Query::prepare(patch, sanitizer).by(predicate, sanitizer);

    EXPECT_EQ(
        query.compose(),
        "UPDATE listing "
        "SET symbol = `AAPL`, venue_id = `NASDAQ` "
        "WHERE qty_minimum > `32.32` AND qty_maximum < `42.42`"
    );
}

TEST(DataLayer_Pqxx_ListingQuery_Update, Compose_WithListingIdReturning)
{
    using Query = Pqxx::ListingQuery::Update;

    SanitizerStub sanitizer{};
    Listing::Patch patch{};
    patch.withSymbol("AAPL").withVenueId("NASDAQ");

    auto const query = Query::prepare(patch, sanitizer).returningId();
    EXPECT_EQ(
        query.compose(),
        "UPDATE listing "
        "SET symbol = `AAPL`, venue_id = `NASDAQ` "
        "RETURNING listing_id"
    );
}

} // namespace
