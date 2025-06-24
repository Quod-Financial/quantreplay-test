#ifndef SIMULATOR_PROJECT_DATA_LAYER_IH_COMMON_QUERIES_DATA_EXTRACTOR_HPP_
#define SIMULATOR_PROJECT_DATA_LAYER_IH_COMMON_QUERIES_DATA_EXTRACTOR_HPP_

#include <functional>
#include <map>
#include <string>
#include <string_view>
#include <type_traits>

#include <fmt/format.h>

namespace Simulator::DataLayer {

template<
    typename ColumnResolver,
    typename EnumerationResolver,
    typename ValueSanitizer>
class DataExtractor {

    template<typename T>
    constexpr inline static bool can_represent_column_v =
        std::is_invocable_r_v<std::string, ColumnResolver, T>;

    template<typename T>
    constexpr inline static bool is_resolvable_enum_v =
        std::is_invocable_r_v<std::string, EnumerationResolver, T>;

    template<typename T>
    constexpr inline static bool is_boolean_v =
        std::is_same_v<bool, std::remove_cv_t<std::remove_reference_t<T>>>;

  public:
    using ColumnValuePair = std::pair<std::string, std::string>;
    using ExtractedData = std::vector<ColumnValuePair>;

    DataExtractor(
        ColumnResolver& _columnResolver,
        EnumerationResolver& _enumValuesResolver,
        ValueSanitizer& _sanitizer
    ) noexcept :
        mColumnResolver(_columnResolver),
        mEnumResolver(_enumValuesResolver),
        mSanitizer(_sanitizer)
    {}

    [[nodiscard]]
    auto getExtractedData() const noexcept -> ExtractedData const&
    {
        return mExtractedData;
    }

    template<typename Column, typename T>
    auto operator()(Column _column, T _value)
        -> std::enable_if_t<is_boolean_v<T>>
    {
        // It's critical to enable this function by `is_boolean` trait
        // to prevent implicit conversions (f.e. char const[]) to boolean
        std::string const str = _value ? "true" : "false";
        (*this)(_column, str);
    }

    template<typename Column, typename T>
    auto operator()(Column _column, T _value)
        -> std::enable_if_t<std::is_arithmetic_v<T> && !is_boolean_v<T>>
    {
        static_assert(
            can_represent_column_v<Column>,
            "Given Column type can not represent a column (attribute) or can "
            "not be resolved by given EnumerationResolver"
        );

        // fmt::to_string is used to drop trailing zeros for fp types,
        // std::to_string returns a str with 6-digit precision (filled with 0)
        std::string value = mSanitizer(fmt::to_string(_value));
        std::string columnName = mColumnResolver(_column);
        mExtractedData.emplace_back(
            std::make_pair(std::move(columnName), std::move(value))
        );
    }

    template<typename Column>
    auto operator()(Column _column, std::string const& _value) -> void
    {
        static_assert(
            can_represent_column_v<Column>,
            "Given Column type can not represent a column (attribute) or can "
            "not be resolved by given EnumerationResolver"
        );

        std::string columnName = mColumnResolver(_column);
        std::string value = mSanitizer(_value);
        mExtractedData.emplace_back(
            std::make_pair(std::move(columnName), std::move(value))
        );
    }

    template<typename Column, typename T>
    auto operator()(Column _column, T _value)
        -> std::enable_if_t<std::is_enum_v<T>>
    {
        static_assert(
            is_resolvable_enum_v<T>,
            "Passed enumeration value can not be resolved by given "
            "EnumerationResolver"
        );

        std::string const resolvedValue = mEnumResolver(_value);
        (*this)(_column, resolvedValue);
    }

  private:
    ExtractedData mExtractedData;

    std::reference_wrapper<ColumnResolver> mColumnResolver;
    std::reference_wrapper<EnumerationResolver> mEnumResolver;
    std::reference_wrapper<ValueSanitizer> mSanitizer;
};

} // namespace Simulator::DataLayer

#endif // SIMULATOR_PROJECT_DATA_LAYER_IH_COMMON_QUERIES_DATA_EXTRACTOR_HPP_
