#ifndef SIMULATOR_DATA_LAYER_IH_PQXX_RESULT_DETAIL_BASIC_ROW_PARSER_HPP_
#define SIMULATOR_DATA_LAYER_IH_PQXX_RESULT_DETAIL_BASIC_ROW_PARSER_HPP_

#include <functional>
#include <string>

#include <pqxx/row>

#include "ih/pqxx/common/column_resolver.hpp"
#include "ih/pqxx/common/enumeration_resolver.hpp"

namespace Simulator::DataLayer::Pqxx::Detail {

class BasicRowParser {
    template<typename T>
    constexpr inline static bool is_character_v =
        std::is_same_v<std::remove_cv_t<std::remove_reference_t<T>>, char>;

    template<typename T>
    constexpr inline static bool is_enumerable_v = std::is_enum_v<T>;

    template<typename T>
    constexpr inline static bool is_default_decodable_v =
        !is_character_v<T> && !is_enumerable_v<T>;

    template<typename T>
    constexpr inline static bool is_resolvable_column_v =
        std::is_invocable_r_v<std::string, Pqxx::ColumnResolver, T>;

    template<typename T>
    constexpr inline static bool is_resolvable_enum_v =
        std::is_invocable_r_v<std::string, Pqxx::EnumerationResolver, T>;

  public:
    explicit BasicRowParser(pqxx::row const& _dbRow) noexcept :
        mRow(_dbRow)
    {}

    template<typename ColumnType, typename Value>
    auto operator()(ColumnType _column, Value& _value)
        -> std::enable_if_t<is_default_decodable_v<Value>, bool>
    {
        static_assert(
            is_resolvable_column_v<ColumnType>,
            "Given ColumnType can not be resolved by Pqxx::ColumnResolver"
        );

        auto const idx = getColumnIdx(_column);
        if (!idx.has_value()) {
            return false;
        }

        pqxx::field const& field = row().at(*idx);
        std::optional<Value> parsedValue = field.get<Value>();
        if (!parsedValue.has_value()) {
            return false;
        }

        _value = std::move(*parsedValue);
        return true;
    }

    template<typename ColumnType, typename Value>
    auto operator()(ColumnType _column, Value& _value)
        -> std::enable_if_t<is_character_v<Value>, bool>
    {
        static_assert(
            is_resolvable_column_v<ColumnType>,
            "Given ColumnType can not be resolved by Pqxx::ColumnResolver"
        );

        auto const idx = getColumnIdx(_column);
        if (!idx.has_value()) {
            return false;
        }

        pqxx::field const& field = row().at(*idx);
        std::optional<std::string> parsedValue = field.get<std::string>();
        if (!parsedValue.has_value() || parsedValue->empty()) {
            return false;
        }

        _value = parsedValue->front();
        return true;
    }

    template<typename ColumnType, typename Value>
    auto operator()(ColumnType _column, Value& _enumValue)
        -> std::enable_if_t<is_enumerable_v<Value>, bool>
    {
        static_assert(
            is_resolvable_column_v<ColumnType>,
            "Given ColumnType can not be resolved by Pqxx::ColumnResolver"
        );

        static_assert(
            is_resolvable_enum_v<Value>,
            "Given enumerable Value type can not be resolved by "
            "Pqxx::EnumerationResolver"
        );

        std::string encodedEnum{};
        if ((*this)(_column, encodedEnum)) {
            mEnumResolver(encodedEnum, _enumValue);
            return true;
        }
        return false;
    }

  private:
    template<typename ColumnType>
    [[nodiscard]]
    auto getColumnIdx(ColumnType _column) const noexcept
        -> std::optional<pqxx::row::size_type>
    {
        try {
            std::string const columnName = mColumnResolver(_column);
            return row().column_number(columnName);
        } catch (...) {
            return std::nullopt;
        }
    }

    [[nodiscard]]
    auto row() const noexcept -> pqxx::row const&
    {
        return mRow;
    }

    std::reference_wrapper<pqxx::row const> mRow;

    Pqxx::ColumnResolver mColumnResolver;
    Pqxx::EnumerationResolver mEnumResolver;
};

} // namespace Simulator::DataLayer::Pqxx::Detail

#endif // SIMULATOR_DATA_LAYER_IH_PQXX_RESULT_DETAIL_BASIC_ROW_PARSER_HPP_
