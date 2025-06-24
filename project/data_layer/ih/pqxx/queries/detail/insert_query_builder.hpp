#ifndef SIMULATOR_DATA_LAYER_IH_PQXX_QUERIES_DETAIL_INSERT_QUERY_BUILDER_HPP_
#define SIMULATOR_DATA_LAYER_IH_PQXX_QUERIES_DETAIL_INSERT_QUERY_BUILDER_HPP_

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

namespace Simulator::DataLayer::Pqxx::Detail {

class InsertQueryBuilder {
    template<typename Sanitizer>
    using CustomExtractorType = DataLayer::DataExtractor<
        Pqxx::ColumnResolver,
        Pqxx::EnumerationResolver,
        Sanitizer>;

    template<typename T>
    constexpr static bool can_be_resolved_as_column_v =
        std::is_invocable_r_v<std::string, Pqxx::ColumnResolver, T>;

  public:
    InsertQueryBuilder() = delete;

    explicit InsertQueryBuilder(std::string _tableName);

    template<typename Sanitizer>
    auto makeDataExtractor(Sanitizer& _valueSanitizer)
        -> CustomExtractorType<Sanitizer>;

    template<typename Sanitizer>
    auto build(CustomExtractorType<Sanitizer> const& _extractor) -> void;

    template<typename Column>
    auto withReturning(Column _column) -> void;

    [[nodiscard]]
    auto compose() const -> std::string;

  private:
    std::string mTableName;
    std::string mReturning;
    std::vector<std::string> mColumns;
    std::vector<std::string> mValues;

    Pqxx::ColumnResolver mColumnResolver;
    Pqxx::EnumerationResolver mEnumResolver;
};

inline InsertQueryBuilder::InsertQueryBuilder(std::string _tableName) :
    mTableName(std::move(_tableName))
{
    if (mTableName.empty()) {
        throw InternalError(
            "INSERT query builder has not been supplied with a target "
            "table name"
        );
    }
}

template<typename Sanitizer>
inline auto InsertQueryBuilder::makeDataExtractor(Sanitizer& _valueSanitizer)
    -> CustomExtractorType<Sanitizer>
{
    return CustomExtractorType<Sanitizer>{
        mColumnResolver,
        mEnumResolver,
        _valueSanitizer};
}

template<typename Column>
inline auto InsertQueryBuilder::withReturning(Column _column) -> void
{
    static_assert(
        can_be_resolved_as_column_v<Column>,
        "Can not resolve a column name for a given type"
    );
    mReturning = mColumnResolver(_column);
}

template<typename Sanitizer>
inline auto InsertQueryBuilder::build(
    CustomExtractorType<Sanitizer> const& _extractor
) -> void
{
    auto const& extractedData = _extractor.getExtractedData();
    mColumns.reserve(extractedData.size());
    mValues.reserve(extractedData.size());

    for (auto [column, value] : _extractor.getExtractedData()) {
        mColumns.emplace_back(column);
        mValues.emplace_back(value);
    }
}

inline auto InsertQueryBuilder::compose() const -> std::string
{
    assert(!mTableName.empty()); // Must be validated in c-tor

    if (mColumns.empty() && mValues.empty()) {
        throw MalformedPatch(
            "a patch with no fields is passed into INSERT query builder"
        );
    }

    std::string const returning =
        mReturning.empty() ? "" : fmt::format(" RETURNING {}", mReturning);

    constexpr std::string_view separator = ", ";
    return fmt::format(
        "INSERT INTO {} ({}) VALUES ({}){}",
        mTableName,
        fmt::join(mColumns, separator),
        fmt::join(mValues, separator),
        returning
    );
}

} // namespace Simulator::DataLayer::Pqxx::Detail

#endif // SIMULATOR_DATA_LAYER_IH_PQXX_QUERIES_DETAIL_INSERT_QUERY_BUILDER_HPP_
