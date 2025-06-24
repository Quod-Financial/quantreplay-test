#ifndef SIMULATOR_DATA_LAYER_IH_PQXX_QUERIES_PRICE_SEED_QUERIES_HPP_
#define SIMULATOR_DATA_LAYER_IH_PQXX_QUERIES_PRICE_SEED_QUERIES_HPP_

#include <string>

#include "api/inspectors/price_seed.hpp"
#include "api/models/price_seed.hpp"
#include "ih/pqxx/common/names/database_entries.hpp"
#include "ih/pqxx/queries/detail/delete_query_builder.hpp"
#include "ih/pqxx/queries/detail/insert_query_builder.hpp"
#include "ih/pqxx/queries/detail/select_query_builder.hpp"
#include "ih/pqxx/queries/detail/update_query_builder.hpp"

namespace Simulator::DataLayer::Pqxx::PriceSeedQuery {

class Insert {
  public:
    using PatchType = DataLayer::PriceSeed::Patch;

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
        constexpr auto column = PriceSeed::Attribute::PriceSeedId;
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
        mBuilder(std::string{Table::PriceSeed})
    {}

    template<typename Sanitizer>
    void build(PatchType const& _snapshot, Sanitizer& _sanitizer)
    {
        auto dataExtractor = mBuilder.makeDataExtractor(_sanitizer);

        using PatchReader = PriceSeedPatchReader<decltype(dataExtractor)>;
        PatchReader reader{dataExtractor};
        reader.read(_snapshot);

        mBuilder.build(dataExtractor);
    }

    Detail::InsertQueryBuilder mBuilder;
};

class Select {
  public:
    using Predicate = Predicate::Expression<PriceSeed>;

    static auto prepare() -> Select { return Select{}; }

    template<typename Sanitizer>
    auto by(Predicate const& _predicate, Sanitizer& _sanitizer) -> Select&
    {
        mBuilder.withPredicateExpression(_predicate, _sanitizer);
        return *this;
    }

    template<typename Sanitizer>
    auto byPriceSeedId(std::uint64_t _priceSeedId, Sanitizer& _sanitizer)
        -> Select&
    {
        constexpr auto column = PriceSeed::Attribute::PriceSeedId;
        mBuilder.withEqPredicate(column, _priceSeedId, _sanitizer);
        return *this;
    }

    [[nodiscard]]
    auto compose() const -> std::string
    {
        return mBuilder.compose();
    }

  private:
    Select() :
        mBuilder(std::string{Table::PriceSeed})
    {}

    Detail::SelectQueryBuilder mBuilder;
};

class Update {
  public:
    using PatchType = PriceSeed::Patch;
    using Predicate = Predicate::Expression<PriceSeed>;

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
        constexpr auto column = PriceSeed::Attribute::PriceSeedId;
        mBuilder.withReturning(column);
        return *this;
    }

    [[nodiscard]]
    auto compose() const -> std::string
    {
        return mBuilder.compose();
    }

  private:
    Update() :
        mBuilder(std::string{Table::PriceSeed})
    {}

    template<typename Sanitizer>
    void build(PatchType const& _snapshot, Sanitizer& _sanitizer)
    {
        auto dataExtractor = mBuilder.makeDataExtractor(_sanitizer);

        using PatchReader = PriceSeedPatchReader<decltype(dataExtractor)>;
        PatchReader reader{dataExtractor};
        reader.read(_snapshot);

        mBuilder.build(dataExtractor);
    }

    Detail::UpdateQueryBuilder mBuilder;
};

class Delete {
  public:
    using Predicate = Predicate::Expression<PriceSeed>;

    static auto prepare() -> Delete { return Delete{}; }

    template<typename Sanitizer>
    auto by(Predicate const& _predicate, Sanitizer& _sanitizer) -> Delete&
    {
        mBuilder.withPredicateExpression(_predicate, _sanitizer);
        return *this;
    }

    [[nodiscard]]
    auto compose() const -> std::string
    {
        return mBuilder.compose();
    }

  private:
    Delete() :
        mBuilder(std::string{Table::PriceSeed})
    {}

    Detail::DeleteQueryBuilder mBuilder;
};

} // namespace Simulator::DataLayer::Pqxx::PriceSeedQuery

#endif // SIMULATOR_DATA_LAYER_IH_PQXX_QUERIES_PRICE_SEED_QUERIES_HPP_
