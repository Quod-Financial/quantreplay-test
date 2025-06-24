#ifndef SIMULATOR_DATA_LAYER_IH_PQXX_QUERIES_DETAIL_DELETE_QUERY_BUILDER_HPP_
#define SIMULATOR_DATA_LAYER_IH_PQXX_QUERIES_DETAIL_DELETE_QUERY_BUILDER_HPP_

#include <string>

#include <fmt/format.h>

#include "api/exceptions/exceptions.hpp"
#include "ih/pqxx/common/column_resolver.hpp"
#include "ih/pqxx/common/enumeration_resolver.hpp"
#include "ih/pqxx/queries/detail/predicate_formatter.hpp"

namespace Simulator::DataLayer::Pqxx::Detail {

class DeleteQueryBuilder {
    template<typename Sanitizer>
    using PredicateBuilder = Pqxx::Detail::PredicateFormatter<
        Pqxx::ColumnResolver,
        Pqxx::EnumerationResolver,
        Sanitizer>;

  public:
    DeleteQueryBuilder() = delete;

    explicit DeleteQueryBuilder(std::string _tableName);

    template<typename Model, typename Sanitizer>
    auto withPredicateExpression(
        Predicate::Expression<Model> const& _predicate,
        Sanitizer& _sanitizer
    ) -> void;

    template<typename Column, typename Value, typename Sanitizer>
    auto withEqPredicate(Column _column, Value&& _value, Sanitizer& _sanitizer)
        -> void;

    [[nodiscard]]
    auto compose() const -> std::string;

  private:
    std::string mTableName;
    std::string mPredicate;

    Pqxx::ColumnResolver mColumnResolver;
    Pqxx::EnumerationResolver mEnumResolver;
};

inline DeleteQueryBuilder::DeleteQueryBuilder(std::string _tableName) :
    mTableName(std::move(_tableName))
{
    if (mTableName.empty()) {
        throw InternalError(
            "DELETE query builder has not been supplied with a target "
            "table name"
        );
    }
}

template<typename Model, typename Sanitizer>
inline auto DeleteQueryBuilder::withPredicateExpression(
    Predicate::Expression<Model> const& _predicate,
    Sanitizer& _sanitizer
) -> void
{
    PredicateBuilder<Sanitizer> builder{
        mColumnResolver,
        mEnumResolver,
        _sanitizer};

    builder.accept(_predicate);
    mPredicate = builder.compose();
}

template<typename Column, typename Value, typename Sanitizer>
inline auto DeleteQueryBuilder::withEqPredicate(
    Column _column,
    Value&& _value,
    Sanitizer& _sanitizer
) -> void
{
    mPredicate = fmt::format(
        "{} = {}",
        mColumnResolver(_column),
        _sanitizer(std::forward<Value>(_value))
    );
}

inline auto DeleteQueryBuilder::compose() const -> std::string
{
    std::string res{};
    if (mPredicate.empty()) {
        res = fmt::format("DELETE FROM {}", mTableName);
    } else {
        res = fmt::format("DELETE FROM {} WHERE {}", mTableName, mPredicate);
    }
    return res;
}

} // namespace Simulator::DataLayer::Pqxx::Detail

#endif // SIMULATOR_DATA_LAYER_IH_PQXX_QUERIES_DETAIL_DELETE_QUERY_BUILDER_HPP_
