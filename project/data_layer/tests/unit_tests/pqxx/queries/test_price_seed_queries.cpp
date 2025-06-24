#include <string>

#include <fmt/format.h>
#include <gtest/gtest.h>

#include "api/exceptions/exceptions.hpp"
#include "api/models/price_seed.hpp"
#include "api/predicate/predicate.hpp"
#include "ih/pqxx/queries/price_seed_queries.hpp"

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


TEST(DataLayer_Pqxx_PriceSeedQuery_Insert, Compose_FromEmptyPatch)
{
    using Query = Pqxx::PriceSeedQuery::Insert;

    PriceSeed::Patch const patch{};
    SanitizerStub sanitizer{};

    auto const query = Query::prepare(patch, sanitizer);
    EXPECT_THROW((void)query.compose(), MalformedPatch);
}

TEST(DataLayer_Pqxx_PriceSeedQuery_Insert, Compose_FromNonEmptyPatch)
{
    using Query = Pqxx::PriceSeedQuery::Insert;

    SanitizerStub sanitizer{};
    PriceSeed::Patch patch{};
    patch.withSymbol("AAPL").withMidPrice(42.42); // NOLINT: test value

    auto const query = Query::prepare(patch, sanitizer).returningId();
    EXPECT_EQ(
        query.compose(),
        "INSERT INTO price_seed (symbol, mid_price) "
        "VALUES (`AAPL`, `42.42`) "
        "RETURNING price_seed_id"
    );
}

TEST(DataLayer_Pqxx_PriceSeedQuery_Select, Compose_WithPredicateExpression)
{
    using Column = PriceSeed::Attribute;
    using Query = Pqxx::PriceSeedQuery::Select;

    SanitizerStub sanitizer{};
    auto const predicate =
        PriceSeedCmp::eq(Column::SecurityIdSource, std::string{"ISIN"}) &&
        PriceSeedCmp::greaterEq(Column::OfferPrice, 42.42);

    auto const query = Query::prepare().by(predicate, sanitizer);
    EXPECT_EQ(
        query.compose(),
        "SELECT * FROM price_seed "
        "WHERE security_id_source = `ISIN` AND offer_price >= `42.42`"
    );
}

TEST(DataLayer_Pqxx_PriceSeedQuery_Select, Compose_WithByPriceSeedIdPredicate)
{
    using Query = Pqxx::PriceSeedQuery::Select;

    SanitizerStub sanitizer{};
    auto const query = Query::prepare().byPriceSeedId(42, sanitizer); // NOLINT
    EXPECT_EQ(
        query.compose(),
        "SELECT * FROM price_seed WHERE price_seed_id = `42`"
    );
}

TEST(DataLayer_Pqxx_PriceSeedQuery_Update, Compose_FromEmptyPatch)
{
    using Query = Pqxx::PriceSeedQuery::Update;

    PriceSeed::Patch const patch{};
    SanitizerStub sanitizer{};

    auto const query = Query::prepare(patch, sanitizer);
    EXPECT_THROW((void)query.compose(), MalformedPatch);
}

TEST(DataLayer_Pqxx_PriceSeedQuery_Update, Compose_FromNonEmptyPatch)
{
    using Query = Pqxx::PriceSeedQuery::Update;

    SanitizerStub sanitizer{};
    PriceSeed::Patch patch{};
    patch.withSymbol("AAPL").withMidPrice(42.42); // NOLINT: test value

    auto const query = Query::prepare(patch, sanitizer);
    EXPECT_EQ(
        query.compose(),
        "UPDATE price_seed SET symbol = `AAPL`, mid_price = `42.42`"
    );
}

TEST(DataLayer_Pqxx_PriceSeedQuery_Update, Compose_WithPredicate)
{
    using Column = PriceSeed::Attribute;
    using Query = Pqxx::PriceSeedQuery::Update;

    SanitizerStub sanitizer{};
    PriceSeed::Patch patch{};
    patch.withSymbol("AAPL").withMidPrice(42.42); // NOLINT: test value
    auto const predicate =
        PriceSeedCmp::eq(Column::SecurityIdSource, std::string{"ISIN"}) &&
        PriceSeedCmp::greaterEq(Column::OfferPrice, 42.42);

    auto const query =
        Query::prepare(patch, sanitizer).by(predicate, sanitizer);

    EXPECT_EQ(
        query.compose(),
        "UPDATE price_seed "
        "SET symbol = `AAPL`, mid_price = `42.42` "
        "WHERE security_id_source = `ISIN` AND offer_price >= `42.42`"
    );
}

TEST(DataLayer_Pqxx_PriceSeedQuery_Update, Compose_WithPriceSeedIdReturning)
{
    using Query = Pqxx::PriceSeedQuery::Update;

    SanitizerStub sanitizer{};
    PriceSeed::Patch patch{};
    patch.withSymbol("AAPL").withMidPrice(42.42); // NOLINT: test value

    auto const query = Query::prepare(patch, sanitizer).returningId();
    EXPECT_EQ(
        query.compose(),
        "UPDATE price_seed "
        "SET symbol = `AAPL`, mid_price = `42.42` "
        "RETURNING price_seed_id"
    );
}

TEST(DataLayer_Pqxx_PriceSeedQuery_Delete, Compose_WithoutPredicate)
{
    using Query = Pqxx::PriceSeedQuery::Delete;

    auto const query = Query::prepare();
    EXPECT_EQ(query.compose(), "DELETE FROM price_seed");
}

TEST(DataLayer_Pqxx_PriceSeedQuery_Delete, Compose_WithPredicateExpression)
{
    using Column = PriceSeed::Attribute;
    using Query = Pqxx::PriceSeedQuery::Delete;

    SanitizerStub sanitizer{};
    auto const predicate =
        PriceSeedCmp::eq(Column::SecurityIdSource, std::string{"ISIN"}) &&
        PriceSeedCmp::greaterEq(Column::OfferPrice, 42.42);

    auto const query = Query::prepare().by(predicate, sanitizer);
    EXPECT_EQ(
        query.compose(),
        "DELETE FROM price_seed "
        "WHERE security_id_source = `ISIN` AND offer_price >= `42.42`"
    );
}