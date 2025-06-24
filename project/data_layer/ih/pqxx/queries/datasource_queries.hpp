#ifndef SIMULATOR_DATA_LAYER_IH_PQXX_QUERIES_DATASOURCE_QUERIES_HPP_
#define SIMULATOR_DATA_LAYER_IH_PQXX_QUERIES_DATASOURCE_QUERIES_HPP_

#include <cstdint>
#include <string>

#include "api/inspectors/column_mapping.hpp"
#include "api/inspectors/datasource.hpp"
#include "api/models/column_mapping.hpp"
#include "api/models/datasource.hpp"
#include "ih/pqxx/common/names/database_entries.hpp"
#include "ih/pqxx/queries/detail/delete_query_builder.hpp"
#include "ih/pqxx/queries/detail/insert_query_builder.hpp"
#include "ih/pqxx/queries/detail/select_query_builder.hpp"
#include "ih/pqxx/queries/detail/update_query_builder.hpp"

namespace Simulator::DataLayer::Pqxx {
namespace DatasourceQuery {

class Insert {
  public:
    using PatchType = DataLayer::Datasource::Patch;

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
        mBuilder.withReturning(Datasource::Attribute::DatasourceId);
        return *this;
    }

    [[nodiscard]]
    auto compose() const -> std::string
    {
        return mBuilder.compose();
    }

  private:
    Insert() :
        mBuilder(std::string{Table::Datasource})
    {}

    template<typename Sanitizer>
    void build(PatchType const& _snapshot, Sanitizer& _sanitizer)
    {
        auto dataExtractor = mBuilder.makeDataExtractor(_sanitizer);

        using PatchReader = DatasourcePatchReader<decltype(dataExtractor)>;
        PatchReader reader{dataExtractor};
        reader.read(_snapshot);

        mBuilder.build(dataExtractor);
    }

    Detail::InsertQueryBuilder mBuilder;
};

class Select {
  public:
    using Predicate = Predicate::Expression<Datasource>;

    static auto prepare() -> Select { return Select{}; }

    template<typename Sanitizer>
    auto by(Predicate const& _predicate, Sanitizer& _sanitizer) -> Select&
    {
        mBuilder.withPredicateExpression(_predicate, _sanitizer);
        return *this;
    }

    template<typename Sanitizer>
    auto byDatasourceId(std::uint64_t _datasourceId, Sanitizer& _sanitizer)
        -> Select&
    {
        constexpr auto column = Datasource::Attribute::DatasourceId;
        mBuilder.withEqPredicate(column, _datasourceId, _sanitizer);
        return *this;
    }

    [[nodiscard]]
    auto compose() const -> std::string
    {
        return mBuilder.compose();
    }

  private:
    Select() :
        mBuilder(std::string{Table::Datasource})
    {}

    Detail::SelectQueryBuilder mBuilder;
};

class Update {
  public:
    using PatchType = Datasource::Patch;
    using Predicate = Predicate::Expression<Datasource>;

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
        mBuilder.withReturning(Datasource::Attribute::DatasourceId);
        return *this;
    }

    [[nodiscard]]
    auto compose() const -> std::string
    {
        return mBuilder.compose();
    }

  private:
    Update() :
        mBuilder(std::string{Table::Datasource})
    {}

    template<typename Sanitizer>
    void build(PatchType const& _snapshot, Sanitizer& _sanitizer)
    {
        auto dataExtractor = mBuilder.makeDataExtractor(_sanitizer);

        using PatchReader = DatasourcePatchReader<decltype(dataExtractor)>;
        PatchReader reader{dataExtractor};
        reader.read(_snapshot);

        mBuilder.build(dataExtractor);
    }

    Detail::UpdateQueryBuilder mBuilder;
};

} // namespace DatasourceQuery

namespace ColumnMappingQuery {

class Insert {
  public:
    template<typename Sanitizer>
    static auto prepare(ColumnMapping const& _mapping, Sanitizer& _sanitizer)
        -> Insert
    {
        Insert query{};
        query.build(_mapping, _sanitizer);
        return query;
    }

    [[nodiscard]]
    auto compose() const -> std::string
    {
        return mBuilder.compose();
    }

  private:
    Insert() :
        mBuilder(std::string{Table::ColumnMapping})
    {}

    template<typename Sanitizer>
    void build(ColumnMapping const& _mapping, Sanitizer& _sanitizer)
    {
        auto dataExtractor = mBuilder.makeDataExtractor(_sanitizer);

        using PatchReader = ColumnMappingReader<decltype(dataExtractor)>;
        PatchReader reader{dataExtractor};
        reader.read(_mapping);

        mBuilder.build(dataExtractor);
    }

    Detail::InsertQueryBuilder mBuilder;
};

class Select {
  public:
    static auto prepare() -> Select { return Select{}; }

    template<typename Sanitizer>
    auto byDatasourceId(std::uint64_t _datasourceId, Sanitizer& _sanitizer)
        -> Select&
    {
        constexpr auto column = ColumnMapping::Attribute::DatasourceId;
        mBuilder.withEqPredicate(column, _datasourceId, _sanitizer);
        return *this;
    }

    [[nodiscard]]
    auto compose() const -> std::string
    {
        return mBuilder.compose();
    }

  private:
    Select() :
        mBuilder(std::string{Table::ColumnMapping})
    {}

    Detail::SelectQueryBuilder mBuilder;
};

class Delete {
  public:
    static auto prepare() -> Delete { return Delete{}; }

    template<typename Sanitizer>
    auto byDatasourceId(std::uint64_t _datasourceId, Sanitizer& _sanitizer)
        -> Delete&
    {
        constexpr auto column = ColumnMapping::Attribute::DatasourceId;
        mBuilder.withEqPredicate(column, _datasourceId, _sanitizer);
        return *this;
    }

    [[nodiscard]]
    auto compose() const -> std::string
    {
        return mBuilder.compose();
    }

  private:
    Delete() :
        mBuilder(std::string{Table::ColumnMapping})
    {}

    Detail::DeleteQueryBuilder mBuilder;
};

} // namespace ColumnMappingQuery
} // namespace Simulator::DataLayer::Pqxx

#endif // SIMULATOR_DATA_LAYER_IH_PQXX_QUERIES_DATASOURCE_QUERIES_HPP_
