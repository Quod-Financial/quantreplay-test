#ifndef SIMULATOR_DATA_LAYER_IH_PQXX_QUERIES_LISTING_QUERIES_HPP_
#define SIMULATOR_DATA_LAYER_IH_PQXX_QUERIES_LISTING_QUERIES_HPP_

#include <string>

#include "api/models/listing.hpp"
#include "api/inspectors/listing.hpp"
#include "ih/pqxx/common/names/database_entries.hpp"
#include "ih/pqxx/queries/detail/insert_query_builder.hpp"
#include "ih/pqxx/queries/detail/select_query_builder.hpp"
#include "ih/pqxx/queries/detail/update_query_builder.hpp"

namespace Simulator::DataLayer::Pqxx::ListingQuery {

class Insert {
  public:
    using PatchType = DataLayer::Listing::Patch;

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
        constexpr auto column = Listing::Attribute::ListingId;
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
        mBuilder(std::string{Table::Listing})
    {}

    template<typename Sanitizer>
    void build(PatchType const& _snapshot, Sanitizer& _sanitizer)
    {
        auto dataExtractor = mBuilder.makeDataExtractor(_sanitizer);

        using PatchReader = ListingPatchReader<decltype(dataExtractor)>;
        PatchReader reader{dataExtractor};
        reader.read(_snapshot);

        mBuilder.build(dataExtractor);
    }

    Detail::InsertQueryBuilder mBuilder;
};

class Select {
  public:
    using Predicate = Predicate::Expression<Listing>;

    static auto prepare() -> Select { return Select{}; }

    template<typename Sanitizer>
    auto by(Predicate const& _predicate, Sanitizer& _sanitizer) -> Select&
    {
        mBuilder.withPredicateExpression(_predicate, _sanitizer);
        return *this;
    }

    template<typename Sanitizer>
    auto byListingId(std::uint64_t _listingId, Sanitizer& _sanitizer)
        -> Select&
    {
        constexpr auto column = Listing::Attribute::ListingId;
        mBuilder.withEqPredicate(column, _listingId, _sanitizer);
        return *this;
    }

    [[nodiscard]]
    auto compose() const -> std::string
    {
        return mBuilder.compose();
    }

  private:
    Select() :
        mBuilder(std::string{Table::Listing})
    {}

    Detail::SelectQueryBuilder mBuilder;
};

class Update {
  public:
    using PatchType = Listing::Patch;
    using Predicate = Predicate::Expression<Listing>;

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
        constexpr auto column = Listing::Attribute::ListingId;
        mBuilder.withReturning(column);
        return *this;
    }

    [[nodiscard]]
    auto compose() const -> std::string { return mBuilder.compose(); }

  private:
    Update() :
        mBuilder(std::string{Table::Listing})
    {}

    template<typename Sanitizer>
    void build(PatchType const& _snapshot, Sanitizer& _sanitizer)
    {
        auto dataExtractor = mBuilder.makeDataExtractor(_sanitizer);

        using PatchReader = ListingPatchReader<decltype(dataExtractor)>;
        PatchReader reader{dataExtractor};
        reader.read(_snapshot);

        mBuilder.build(dataExtractor);
    }

    Detail::UpdateQueryBuilder mBuilder;
};

} // namespace Simulator::DataLayer::Pqxx::ListingQuery

#endif // SIMULATOR_DATA_LAYER_IH_PQXX_QUERIES_LISTING_QUERIES_HPP_
