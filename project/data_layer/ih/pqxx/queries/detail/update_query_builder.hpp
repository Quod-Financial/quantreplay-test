#ifndef SIMULATOR_DATA_LAYER_IH_PQXX_QUERIES_DETAIL_UPDATE_QUERY_BUILDER_HPP_
#define SIMULATOR_DATA_LAYER_IH_PQXX_QUERIES_DETAIL_UPDATE_QUERY_BUILDER_HPP_

#include <cassert>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include <fmt/ranges.h>

#include "api/exceptions/exceptions.hpp"
#include "ih/common/queries/data_extractor.hpp"
#include "ih/pqxx/common/column_resolver.hpp"
#include "ih/pqxx/common/enumeration_resolver.hpp"
#include "ih/pqxx/queries/detail/predicate_formatter.hpp"

namespace Simulator::DataLayer::Pqxx::Detail {

class UpdateQueryBuilder {
    template<typename Sanitizer>
    using CustomExtractorType = DataLayer::DataExtractor<
        Pqxx::ColumnResolver,
        Pqxx::EnumerationResolver,
        Sanitizer>;

    template<typename Sanitizer>
    using PredicateBuilder = Pqxx::Detail::PredicateFormatter<
        Pqxx::ColumnResolver,
        Pqxx::EnumerationResolver,
        Sanitizer>;

    template<typename T>
    constexpr static bool can_be_resolved_as_column_v =
        std::is_invocable_r_v<std::string, Pqxx::ColumnResolver, T>;

  public:
    UpdateQueryBuilder() = delete;

    explicit UpdateQueryBuilder(std::string _tableName);

    template<typename Sanitizer>
    auto makeDataExtractor(Sanitizer& _valueSanitizer)
        -> CustomExtractorType<Sanitizer>;

    template<typename Sanitizer>
    auto build(CustomExtractorType<Sanitizer> const& _extractor) -> void;

    template<typename Column>
    auto withReturning(Column _column) -> void;

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
    std::string mReturning;
    std::string mPredicate;
    std::vector<std::string> mColumnValueUpdates;

    Pqxx::ColumnResolver mColumnResolver;
    Pqxx::EnumerationResolver mEnumResolver;
};

inline UpdateQueryBuilder::UpdateQueryBuilder(std::string _tableName) :
    mTableName(std::move(_tableName))
{
    if (mTableName.empty()) {
        throw InternalError(
            "UPDATE query builder has not been supplied with a target "
            "table name"
        );
    }
}

template<typename Sanitizer>
inline auto UpdateQueryBuilder::makeDataExtractor(Sanitizer& _valueSanitizer)
    -> UpdateQueryBuilder::CustomExtractorType<Sanitizer>
{
    return CustomExtractorType<Sanitizer>{
        mColumnResolver,
        mEnumResolver,
        _valueSanitizer};
}

template<typename Sanitizer>
inline auto UpdateQueryBuilder::build(
    CustomExtractorType<Sanitizer> const& _extractor
) -> void
{
    auto const& extractedData = _extractor.getExtractedData();
    mColumnValueUpdates.reserve(extractedData.size());

    for (auto [column, value] : _extractor.getExtractedData()) {
        mColumnValueUpdates.emplace_back(fmt::format("{} = {}", column, value));
    }
}

template<typename Model, typename Sanitizer>
inline auto UpdateQueryBuilder::withPredicateExpression(
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
inline auto UpdateQueryBuilder::withEqPredicate(
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

inline auto UpdateQueryBuilder::compose() const -> std::string
{
    assert(!mTableName.empty()); // Must be validated in c-tor

    if (mColumnValueUpdates.empty()) {
        throw MalformedPatch(
            "a patch with no fields is passed into UPDATE query builder"
        );
    }

    std::string const predicate =
        mPredicate.empty() ? "" : fmt::format(" WHERE {}", mPredicate);
    std::string const returning =
        mReturning.empty() ? "" : fmt::format(" RETURNING {}", mReturning);

    constexpr std::string_view separator = ", ";
    return fmt::format(
        "UPDATE {} SET {}{}{}",
        mTableName,
        fmt::join(mColumnValueUpdates, separator),
        predicate,
        returning
    );
}
template<typename Column>
auto UpdateQueryBuilder::withReturning(Column _column)
    -> void
{
    static_assert(
        can_be_resolved_as_column_v<Column>,
        "Can not resolve a column name for a given type"
    );
    mReturning = mColumnResolver(_column);
}

} // namespace Simulator::DataLayer::Pqxx::Detail

#endif // SIMULATOR_DATA_LAYER_IH_PQXX_QUERIES_DETAIL_UPDATE_QUERY_BUILDER_HPP_
