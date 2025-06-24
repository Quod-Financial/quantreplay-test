#ifndef SIMULATOR_DATA_LAYER_IH_PQXX_QUERIES_DETAIL_PREDICATE_FORMATTER_HPP_
#define SIMULATOR_DATA_LAYER_IH_PQXX_QUERIES_DETAIL_PREDICATE_FORMATTER_HPP_

#include <functional>

#include <fmt/ranges.h>

#include "api/predicate/predicate.hpp"

namespace Simulator::DataLayer::Pqxx::Detail {

template<
    typename ColumnResolver,
    typename EnumerationResolver,
    typename ValueSanitizer>
class PredicateFormatter {
    template<typename T>
    constexpr inline static bool is_enum_value_v =
        std::is_enum_v<std::remove_cv_t<std::remove_reference_t<T>>>;

  public:
    PredicateFormatter(
        ColumnResolver& _columnResolver,
        EnumerationResolver& _enumValuesResolver,
        ValueSanitizer& _sanitizer
    ) noexcept :
        mColumnResolver(_columnResolver),
        mEnumResolver(_enumValuesResolver),
        mSanitizer(_sanitizer)
    {}

    template<typename Model>
    auto accept(Predicate::Expression<Model> const& _expression)
    {
        mLexemes.clear();
        _expression.accept(*this);
    }

    [[nodiscard]]
    auto compose() const -> std::string
    {
        constexpr std::string_view separator{" "};
        return fmt::format("{}", fmt::join(mLexemes, separator));
    }

    template<typename Column, typename Value>
    auto operator()(Column _col, Predicate::BasicOperation _op, Value&& _value)
        -> std::enable_if_t<!is_enum_value_v<Value>>
    {
        std::string const col = mColumnResolver(_col);
        std::string_view const operation = format(_op);
        std::string const val = mSanitizer(std::forward<Value>(_value));
        mLexemes.emplace_back(fmt::format("{} {} {}", col, operation, val));
    }

    template<typename Column, typename Value>
    auto operator()(Column _col, Predicate::BasicOperation _op, Value _value)
        -> std::enable_if_t<is_enum_value_v<Value>>
    {
        std::string const value = mEnumResolver(_value);
        (*this)(_col, _op, value);
    }

    auto operator()([[maybe_unused]] Predicate::SubExpressionBegin _lexeme)
        -> void
    {
        mLexemes.emplace_back("(");
    }

    auto operator()([[maybe_unused]] Predicate::SubExpressionEnd _lexeme)
        -> void
    {
        mLexemes.emplace_back(")");
    }

    auto operator()(Predicate::CompositeOperation _op) -> void
    {
        std::string_view const operation = format(_op);
        mLexemes.emplace_back(fmt::format("{}", operation));
    }

  private:
    constexpr static auto format(Predicate::BasicOperation _op)
        -> std::string_view
    {
        std::string_view value{};
        switch (_op) {
            case Predicate::BasicOperation::Eq:
                value = "=";
                break;
            case Predicate::BasicOperation::Neq:
                value = "!=";
                break;
            case Predicate::BasicOperation::Less:
                value = "<";
                break;
            case Predicate::BasicOperation::Greater:
                value = ">";
                break;
            case Predicate::BasicOperation::LessEq:
                value = "<=";
                break;
            case Predicate::BasicOperation::GreaterEq:
                value = ">=";
                break;
        }
        return value;
    }

    constexpr static auto format(Predicate::CompositeOperation _op)
        -> std::string_view
    {
        std::string_view value{};
        switch (_op) {
            case Predicate::CompositeOperation::AND:
                value = "AND";
                break;
            case Predicate::CompositeOperation::OR:
                value = "OR";
                break;
        }
        return value;
    }

    std::vector<std::string> mLexemes;

    std::reference_wrapper<ColumnResolver> mColumnResolver;
    std::reference_wrapper<EnumerationResolver> mEnumResolver;
    std::reference_wrapper<ValueSanitizer> mSanitizer;
};

} // namespace Simulator::DataLayer::Pqxx::Detail

#endif // SIMULATOR_DATA_LAYER_IH_PQXX_QUERIES_DETAIL_PREDICATE_FORMATTER_HPP_
