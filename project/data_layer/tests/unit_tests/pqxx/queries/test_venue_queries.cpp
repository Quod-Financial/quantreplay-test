#include <string>

#include <fmt/format.h>
#include <gtest/gtest.h>

#include "api/exceptions/exceptions.hpp"
#include "api/models/market_phase.hpp"
#include "api/models/venue.hpp"
#include "ih/pqxx/queries/venue_queries.hpp"

using namespace Simulator::DataLayer;

// NOLINTBEGIN(*magic-numbers*)

namespace {

struct SanitizerStub {
    template<typename T>
    auto operator()(T&& _value) -> std::string
    {
        return fmt::format("`{}`", std::forward<T>(_value));
    }
};

} // namespace

TEST(DataLayer_Pqxx_VenueQuery_Insert, Compose_FromEmptyPatch)
{
    Venue::Patch const patch{};
    SanitizerStub sanitizer{};

    auto const query = Pqxx::VenueQuery::Insert::prepare(patch, sanitizer);
    EXPECT_THROW((void)query.compose(), MalformedPatch);
}

TEST(DataLayer_Pqxx_VenueQuery_Insert, Compose_FromNonEmptyPatch)
{
    SanitizerStub sanitizer{};
    Venue::Patch patch{};
    patch.withVenueId("LSE").withName("London Stock Exchange");

    auto const query = Pqxx::VenueQuery::Insert::prepare(patch, sanitizer);
    EXPECT_EQ(
        query.compose(),
        "INSERT INTO venue (venue_id, name) "
        "VALUES (`LSE`, `London Stock Exchange`)"
    );
}

TEST(DataLayer_Pqxx_VenueQuery_Select, Compose_Empty)
{
    using Query = Pqxx::VenueQuery::Select;
    auto const query = Query::prepare();
    EXPECT_EQ(query.compose(), "SELECT * FROM venue");
}

TEST(DataLayer_Pqxx_VenueQuery_Select, Compose_WithPredicateExpression)
{
    using Column = Venue::Attribute;
    using Query = Pqxx::VenueQuery::Select;

    SanitizerStub sanitizer{};
    auto const predicate =
        VenueCmp::eq(Column::EngineType, Venue::EngineType::Quoting) &&
        VenueCmp::eq(Column::SupportTifDay, false);

    auto const query = Query::prepare().by(predicate, sanitizer);
    EXPECT_EQ(
        query.compose(),
        "SELECT * FROM venue "
        "WHERE engine_type = `Quoting` AND support_tif_day = `false`"
    );
}

TEST(DataLayer_Pqxx_VenueQuery_Select, Compose_WithByVenueIdPredicate)
{
    using Query = Pqxx::VenueQuery::Select;

    SanitizerStub sanitizer{};
    auto const query = Query::prepare().byVenueId("LSE", sanitizer);
    EXPECT_EQ(query.compose(), "SELECT * FROM venue WHERE venue_id = `LSE`");
}

TEST(DataLayer_Pqxx_VenueQuery_Update, Compose_FromEmptyPatch)
{
    Venue::Patch const patch{};
    SanitizerStub sanitizer{};

    auto const query = Pqxx::VenueQuery::Update::prepare(patch, sanitizer);
    EXPECT_THROW((void)query.compose(), MalformedPatch);
}

TEST(DataLayer_Pqxx_VenueQuery_Update, Compose_FromNonEmptyPatch)
{
    SanitizerStub sanitizer{};
    Venue::Patch patch{};
    patch.withVenueId("LSE").withName("London Stock Exchange");

    auto const query = Pqxx::VenueQuery::Update::prepare(patch, sanitizer);
    EXPECT_EQ(
        query.compose(),
        "UPDATE venue SET venue_id = `LSE`, name = `London Stock Exchange`"
    );
}

TEST(DataLayer_Pqxx_VenueQuery_Update, Compose_WithPredicate)
{
    using Column = Venue::Attribute;

    SanitizerStub sanitizer{};
    Venue::Patch patch{};
    patch.withVenueId("LSE").withName("London Stock Exchange");
    auto const predicate = VenueCmp::eq(Column::VenueId, std::string{"NSE"});

    auto const query = Pqxx::VenueQuery::Update::prepare(patch, sanitizer)
                           .by(predicate, sanitizer);

    EXPECT_EQ(
        query.compose(),
        "UPDATE venue "
        "SET venue_id = `LSE`, name = `London Stock Exchange` "
        "WHERE venue_id = `NSE`"
    );
}

TEST(DataLayer_Pqxx_VenueQuery_Update, Compose_WithVenueIdReturning)
{
    SanitizerStub sanitizer{};
    Venue::Patch patch{};
    patch.withVenueId("LSE").withName("London Stock Exchange");

    auto const query =
        Pqxx::VenueQuery::Update::prepare(patch, sanitizer).returningId();

    EXPECT_EQ(
        query.compose(),
        "UPDATE venue "
        "SET venue_id = `LSE`, name = `London Stock Exchange` "
        "RETURNING venue_id"
    );
}

TEST(DataLayer_Pqxx_MarketPhaseQuery_Insert, Compose_FromEmptyMarketPhase)
{
    SanitizerStub sanitizer{};
    MarketPhase::Patch patch{};
    patch.with_phase(MarketPhase::Phase::Open)
        .with_start_time("09:00:00")
        .with_end_time("18:00:00");
    MarketPhase const phase = MarketPhase::create(patch, "LSE");

    auto const query =
        Pqxx::MarketPhaseQuery::Insert::prepare(phase, sanitizer);

    EXPECT_EQ(
        query.compose(),
        "INSERT INTO "
        "market_phase (phase, start_time, end_time, venue_id) "
        "VALUES (`Open`, `09:00:00`, `18:00:00`, `LSE`)"
    );
}

TEST(DataLayer_Pqxx_MarketPhaseQuery_Insert, Compose_FromMarketPhaseWithAllowCancels)
{
  SanitizerStub sanitizer{};
  MarketPhase::Patch patch{};
  patch.with_phase(MarketPhase::Phase::Open)
      .with_start_time("09:00:00")
      .with_end_time("17:59:59")
      .with_allow_cancels(false);
  MarketPhase const phase = MarketPhase::create(patch, "LSE");

  auto const query =
      Pqxx::MarketPhaseQuery::Insert::prepare(phase, sanitizer);

  EXPECT_EQ(
      query.compose(),
      "INSERT INTO "
      "market_phase (phase, start_time, end_time, venue_id, allow_cancels) "
      "VALUES (`Open`, `09:00:00`, `17:59:59`, `LSE`, `false`)"
  );
}

TEST(DataLayer_Pqxx_MarketPhaseQuery_Insert, Compose_FromMarketPhase)
{
    SanitizerStub sanitizer{};
    MarketPhase::Patch patch{};
    patch.with_phase(MarketPhase::Phase::Open)
        .with_start_time("09:00:00")
        .with_end_time("17:59:59")
        .with_allow_cancels(true)
        .with_end_time_range(10);
    MarketPhase const phase = MarketPhase::create(patch, "LSE");

    auto const query =
        Pqxx::MarketPhaseQuery::Insert::prepare(phase, sanitizer);

    EXPECT_EQ(
        query.compose(),
        "INSERT INTO "
        "market_phase (phase, start_time, end_time, venue_id, allow_cancels, end_time_range) "
        "VALUES (`Open`, `09:00:00`, `17:59:59`, `LSE`, `true`, `10`)"
    );
}

TEST(DataLayer_Pqxx_MarketPhaseQuery_Select, Compose_WithoutPredicate)
{
    auto const query = Pqxx::MarketPhaseQuery::Select::prepare();
    EXPECT_EQ(query.compose(), "SELECT * FROM market_phase");
}

TEST(DataLayer_Pqxx_MarketPhaseQuery_Select, Compose_WithPredicate)
{
    SanitizerStub sanitizer{};
    auto query = Pqxx::MarketPhaseQuery::Select::prepare();
    query.byVenueId("LSE", sanitizer);

    EXPECT_EQ(
        query.compose(),
        "SELECT * FROM market_phase WHERE venue_id = `LSE`"
    );
}

TEST(DataLayer_Pqxx_MarketPhaseQuery_Delete, Compose_WithoutPredicate)
{
    auto const query = Pqxx::MarketPhaseQuery::Delete::prepare();
    EXPECT_EQ(query.compose(), "DELETE FROM market_phase");
}

TEST(DataLayer_Pqxx_MarketPhaseQuery_Delete, Compose_WithPredicate)
{
    SanitizerStub sanitizer{};
    auto query = Pqxx::MarketPhaseQuery::Delete::prepare();
    query.byVenueId("LSE", sanitizer);

    EXPECT_EQ(
        query.compose(),
        "DELETE FROM market_phase WHERE venue_id = `LSE`"
    );
}

// NOLINTEND(*magic-numbers*)