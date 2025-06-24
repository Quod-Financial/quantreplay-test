#ifndef SIMULATOR_DATA_LAYER_IH_PQXX_QUERIES_VENUE_QUERIES_HPP_
#define SIMULATOR_DATA_LAYER_IH_PQXX_QUERIES_VENUE_QUERIES_HPP_

#include <string>

#include "api/inspectors/market_phase.hpp"
#include "api/inspectors/venue.hpp"
#include "api/models/market_phase.hpp"
#include "api/models/venue.hpp"
#include "ih/pqxx/common/names/database_entries.hpp"
#include "ih/pqxx/queries/detail/delete_query_builder.hpp"
#include "ih/pqxx/queries/detail/insert_query_builder.hpp"
#include "ih/pqxx/queries/detail/select_query_builder.hpp"
#include "ih/pqxx/queries/detail/update_query_builder.hpp"

namespace Simulator::DataLayer::Pqxx {
namespace VenueQuery {

class Insert {
  public:
    using PatchType = DataLayer::Venue::Patch;

    template<typename Sanitizer>
    static auto prepare(PatchType const& _snapshot, Sanitizer& _sanitizer)
        -> Insert
    {
        Insert query{};
        query.build(_snapshot, _sanitizer);
        return query;
    }

    auto returningId() -> Insert&
    {
        constexpr auto column = Venue::Attribute::VenueId;
        mBuilder.withReturning(column);
        return *this;
    }

    [[nodiscard]]
    auto compose() const -> std::string
    {
        return mBuilder.compose();
    }

  private:
    Insert() :
        mBuilder(std::string{Table::Venue})
    {}

    template<typename Sanitizer>
    void build(PatchType const& _snapshot, Sanitizer& _sanitizer)
    {
        auto dataExtractor = mBuilder.makeDataExtractor(_sanitizer);

        using PatchReader = VenuePatchReader<decltype(dataExtractor)>;
        PatchReader reader{dataExtractor};
        reader.read(_snapshot);

        mBuilder.build(dataExtractor);
    }

    Detail::InsertQueryBuilder mBuilder;
};

class Select {
  public:
    using Predicate = Predicate::Expression<Venue>;

    static auto prepare() -> Select { return Select{}; }

    template<typename Sanitizer>
    auto by(Predicate const& _predicate, Sanitizer& _sanitizer) -> Select&
    {
        mBuilder.withPredicateExpression(_predicate, _sanitizer);
        return *this;
    }

    template<typename Sanitizer>
    auto byVenueId(std::string const& _venueId, Sanitizer& _sanitizer)
        -> Select&
    {
        using Column = Venue::Attribute;
        mBuilder.withEqPredicate(Column::VenueId, _venueId, _sanitizer);
        return *this;
    }

    [[nodiscard]]
    auto compose() const -> std::string
    {
        return mBuilder.compose();
    }

  private:
    Select() :
        mBuilder(std::string{Table::Venue})
    {}

    Detail::SelectQueryBuilder mBuilder;
};

class Update {
  public:
    using PatchType = DataLayer::Venue::Patch;
    using Predicate = Predicate::Expression<Venue>;

    template<typename Sanitizer>
    static auto prepare(PatchType const& _snapshot, Sanitizer& _sanitizer)
        -> Update
    {
        Update query{};
        query.build(_snapshot, _sanitizer);
        return query;
    }

    template<typename Sanitizer>
    auto by(Predicate const& _predicate, Sanitizer& _sanitizer) -> Update&
    {
        mBuilder.withPredicateExpression(_predicate, _sanitizer);
        return *this;
    }

    auto returningId() -> Update&
    {
        mBuilder.withReturning(Venue::Attribute::VenueId);
        return *this;
    }

    [[nodiscard]]
    auto compose() const -> std::string
    {
        return mBuilder.compose();
    }

  private:
    Update() :
        mBuilder(std::string{Table::Venue})
    {}

    template<typename Sanitizer>
    void build(PatchType const& _snapshot, Sanitizer& _sanitizer)
    {
        auto dataExtractor = mBuilder.makeDataExtractor(_sanitizer);

        using PatchReader = VenuePatchReader<decltype(dataExtractor)>;
        PatchReader reader{dataExtractor};
        reader.read(_snapshot);

        mBuilder.build(dataExtractor);
    }

    Detail::UpdateQueryBuilder mBuilder;
};

} // namespace VenueQuery

namespace MarketPhaseQuery {

class Insert {
  public:
    template<typename Sanitizer>
    static auto prepare(MarketPhase const& _phase, Sanitizer& _sanitizer)
        -> Insert
    {
        Insert query{};
        query.build(_phase, _sanitizer);
        return query;
    }

    [[nodiscard]]
    auto compose() const -> std::string
    {
        return mBuilder.compose();
    }

  private:
    Insert() :
        mBuilder(std::string{Table::MarketPhase})
    {}

    template<typename Sanitizer>
    void build(MarketPhase const& _marketPhase, Sanitizer& _sanitizer)
    {
        auto dataExtractor = mBuilder.makeDataExtractor(_sanitizer);

        using PatchReader = MarketPhaseReader<decltype(dataExtractor)>;
        PatchReader reader{dataExtractor};
        reader.read(_marketPhase);

        mBuilder.build(dataExtractor);
    }

    Detail::InsertQueryBuilder mBuilder;
};

class Select {
  public:
    static auto prepare() -> Select { return Select{}; }

    template<typename Sanitizer>
    auto byVenueId(std::string const& _venueId, Sanitizer& _sanitizer)
        -> Select&
    {
        using Column = MarketPhase::Attribute;
        mBuilder.withEqPredicate(Column::VenueId, _venueId, _sanitizer);
        return *this;
    }

    [[nodiscard]]
    auto compose() const -> std::string
    {
        return mBuilder.compose();
    }

  private:
    Select() :
        mBuilder(std::string{Table::MarketPhase})
    {}

    Detail::SelectQueryBuilder mBuilder;
};

class Delete {
  public:
    static auto prepare() -> Delete { return Delete{}; }

    template<typename Sanitizer>
    auto byVenueId(std::string const& _venueId, Sanitizer& _sanitizer)
        -> Delete&
    {
        using Column = MarketPhase::Attribute;
        mBuilder.withEqPredicate(Column::VenueId, _venueId, _sanitizer);
        return *this;
    }

    [[nodiscard]]
    auto compose() const -> std::string
    {
        return mBuilder.compose();
    }

  private:
    Delete() :
        mBuilder(std::string{Table::MarketPhase})
    {}

    Detail::DeleteQueryBuilder mBuilder;
};

} // namespace MarketPhaseQuery
} // namespace Simulator::DataLayer::Pqxx

#endif // SIMULATOR_DATA_LAYER_IH_PQXX_QUERIES_VENUE_QUERIES_HPP_
