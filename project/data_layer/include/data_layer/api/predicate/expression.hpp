#ifndef SIMULATOR_DATA_LAYER_INCLUDE_DATA_LAYER_API_PREDICATE_EXPRESSION_HPP_
#define SIMULATOR_DATA_LAYER_INCLUDE_DATA_LAYER_API_PREDICATE_EXPRESSION_HPP_

#include <algorithm>
#include <cassert>
#include <utility>
#include <variant>
#include <vector>

#include "data_layer/api/predicate/definitions.hpp"
#include "data_layer/api/predicate/lexeme.hpp"
#include "data_layer/api/predicate/traits.hpp"

namespace Simulator::DataLayer::Predicate {

template<typename Model>
class Expression {
  public:
    using BasicLexeme = BasicPredicate<Model>;
    using CompositeLexeme = CompositePredicate;
    using SubExpressionBeginLexeme = SubExpressionBegin;
    using SubExpressionEndLexeme = SubExpressionEnd;

  private:
    using Lexeme = std::variant<
        BasicLexeme,
        CompositeLexeme,
        SubExpressionBeginLexeme,
        SubExpressionEndLexeme>;

  public:
    Expression() = delete; // Empty predicate expressions are not allowed

    // Creates a basic expression with a single BasicPredicate lexeme
    explicit Expression(BasicLexeme _basicLexeme) :
        mLexemes{std::move(_basicLexeme)}
    {}

    template<typename Formatter>
    auto accept(Formatter& _formatter) const -> void
    {
        auto const lexemeVisitor = [&](auto const& _concreteLexeme) {
            _concreteLexeme.accept(_formatter);
        };

        for (Lexeme const& lexeme : mLexemes) {
            std::visit(lexemeVisitor, lexeme);
        }
    }

    [[nodiscard]]
    auto isBasic() const noexcept -> bool
    {
        assert(!mLexemes.empty());
        return mLexemes.size() == 1 &&
               std::holds_alternative<BasicLexeme>(mLexemes.front());
    }

    [[nodiscard]]
    auto isComposite() const noexcept -> bool
    {
        return !isBasic();
    }

    static auto compose(
        Expression _left,
        CompositeOperation _operation,
        Expression _right
    ) -> Expression
    {
        std::size_t const composedSize = calculateExtendedSize(_left, _right);
        std::vector<Lexeme> composedLexemes;
        composedLexemes.reserve(composedSize);

        composeInto(std::move(_left), composedLexemes);
        composedLexemes.emplace_back(CompositeLexeme{_operation});
        composeInto(std::move(_right), composedLexemes);

        return Expression{std::move(composedLexemes)};
    }

  private:
    explicit Expression(std::vector<Lexeme> _lexemes) noexcept :
        mLexemes(std::move(_lexemes))
    {}

    static auto calculateExtendedSize(
        Expression const& _left,
        Expression const& _right
    ) -> std::size_t
    {
        // An extended size is size(left) + size(right) + 1
        // for a composite lexeme
        std::size_t extendedExpressionSize =
            _left.mLexemes.size() + _right.mLexemes.size() + 1;

        if (_left.isComposite()) {
            extendedExpressionSize += 2;
        }

        if (_right.isComposite()) {
            extendedExpressionSize += 2;
        }

        return extendedExpressionSize;
    }

    static auto composeInto(
        Expression _expression,
        std::vector<Lexeme>& _composedLexemes
    ) -> void
    {
        bool const isComposite = _expression.isComposite();
        auto& oldLexemes = _expression.mLexemes;

        if (isComposite) {
            _composedLexemes.emplace_back(SubExpressionBeginLexeme{});
        }

        std::copy(
            std::make_move_iterator(std::begin(oldLexemes)),
            std::make_move_iterator(std::end(oldLexemes)),
            std::back_inserter(_composedLexemes)
        );

        if (isComposite) {
            _composedLexemes.emplace_back(SubExpressionEndLexeme{});
        }
    }

    std::vector<Lexeme> mLexemes;
};


// Boolean-type expressions

template<typename Model, typename Traits = ModelTraits<Model>>
inline auto eq(typename Traits::AttributeType _field, bool _value)
    -> Expression<Model>
{
    using Expression = Expression<Model>;
    using Lexeme = typename Expression::BasicLexeme;

    return Expression{Lexeme{_field, BasicOperation::Eq, _value}};
}

template<typename Model, typename Traits = ModelTraits<Model>>
inline auto neq(typename Traits::AttributeType _field, bool _value)
    -> Expression<Model>
{
    using Expression = Expression<Model>;
    using Lexeme = typename Expression::BasicLexeme;

    return Expression{Lexeme{_field, BasicOperation::Neq, _value}};
}


// UnsignedInt-type expressions

template<typename Model, typename T, typename Traits = ModelTraits<Model>>
inline auto eq(typename Traits::AttributeType _field, T _value)
    -> std::enable_if_t<is_unsigned_int_v<T>, Expression<Model>>
{
    using Expression = Expression<Model>;
    using Lexeme = typename Expression::BasicLexeme;

    return Expression{
        Lexeme{_field, BasicOperation::Eq, static_cast<std::uint64_t>(_value)}};
}

template<typename Model, typename T, typename Traits = ModelTraits<Model>>
inline auto neq(typename Traits::AttributeType _field, T _value)
    -> std::enable_if_t<is_unsigned_int_v<T>, Expression<Model>>
{
    using Expression = Expression<Model>;
    using Lexeme = typename Expression::BasicLexeme;

    return Expression{Lexeme{
        _field,
        BasicOperation::Neq,
        static_cast<std::uint64_t>(_value)}};
}

template<typename Model, typename T, typename Traits = ModelTraits<Model>>
inline auto less(typename Traits::AttributeType _field, T _value)
    -> std::enable_if_t<is_unsigned_int_v<T>, Expression<Model>>
{
    using Expression = Expression<Model>;
    using Lexeme = typename Expression::BasicLexeme;

    return Expression{Lexeme{
        _field,
        BasicOperation::Less,
        static_cast<std::uint64_t>(_value)}};
}

template<typename Model, typename T, typename Traits = ModelTraits<Model>>
inline auto greater(typename Traits::AttributeType _field, T _value)
    -> std::enable_if_t<is_unsigned_int_v<T>, Expression<Model>>
{
    using Expression = Expression<Model>;
    using Lexeme = typename Expression::BasicLexeme;

    return Expression{Lexeme{
        _field,
        BasicOperation::Greater,
        static_cast<std::uint64_t>(_value)}};
}

template<typename Model, typename T, typename Traits = ModelTraits<Model>>
inline auto lessEq(typename Traits::AttributeType _field, T _value)
    -> std::enable_if_t<is_unsigned_int_v<T>, Expression<Model>>
{
    using Expression = Expression<Model>;
    using Lexeme = typename Expression::BasicLexeme;

    return Expression{Lexeme{
        _field,
        BasicOperation::LessEq,
        static_cast<std::uint64_t>(_value)}};
}

template<typename Model, typename T, typename Traits = ModelTraits<Model>>
inline auto greaterEq(typename Traits::AttributeType _field, T _value)
    -> std::enable_if_t<is_unsigned_int_v<T>, Expression<Model>>
{
    using Expression = Expression<Model>;
    using Lexeme = typename Expression::BasicLexeme;

    return Expression{Lexeme{
        _field,
        BasicOperation::GreaterEq,
        static_cast<std::uint64_t>(_value)}};
}


// UnsignedInt-type expressions

template<typename Model, typename T, typename Traits = ModelTraits<Model>>
inline auto eq(typename Traits::AttributeType _field, T _value)
    -> std::enable_if_t<is_signed_int_v<T>, Expression<Model>>
{
    using Expression = Expression<Model>;
    using Lexeme = typename Expression::BasicLexeme;

    return Expression{
        Lexeme{_field, BasicOperation::Eq, static_cast<std::int64_t>(_value)}};
}

template<typename Model, typename T, typename Traits = ModelTraits<Model>>
inline auto neq(typename Traits::AttributeType _field, T _value)
    -> std::enable_if_t<is_signed_int_v<T>, Expression<Model>>
{
    using Expression = Expression<Model>;
    using Lexeme = typename Expression::BasicLexeme;

    return Expression{
        Lexeme{_field, BasicOperation::Neq, static_cast<std::int64_t>(_value)}};
}

template<typename Model, typename T, typename Traits = ModelTraits<Model>>
inline auto less(typename Traits::AttributeType _field, T _value)
    -> std::enable_if_t<is_signed_int_v<T>, Expression<Model>>
{
    using Expression = Expression<Model>;
    using Lexeme = typename Expression::BasicLexeme;

    return Expression{Lexeme{
        _field,
        BasicOperation::Less,
        static_cast<std::int64_t>(_value)}};
}

template<typename Model, typename T, typename Traits = ModelTraits<Model>>
inline auto greater(typename Traits::AttributeType _field, T _value)
    -> std::enable_if_t<is_signed_int_v<T>, Expression<Model>>
{
    using Expression = Expression<Model>;
    using Lexeme = typename Expression::BasicLexeme;

    return Expression{Lexeme{
        _field,
        BasicOperation::Greater,
        static_cast<std::int64_t>(_value)}};
}

template<typename Model, typename T, typename Traits = ModelTraits<Model>>
inline auto lessEq(typename Traits::AttributeType _field, T _value)
    -> std::enable_if_t<is_signed_int_v<T>, Expression<Model>>
{
    using Expression = Expression<Model>;
    using Lexeme = typename Expression::BasicLexeme;

    return Expression{Lexeme{
        _field,
        BasicOperation::LessEq,
        static_cast<std::int64_t>(_value)}};
}

template<typename Model, typename T, typename Traits = ModelTraits<Model>>
inline auto greaterEq(typename Traits::AttributeType _field, T _value)
    -> std::enable_if_t<is_signed_int_v<T>, Expression<Model>>
{
    using Expression = Expression<Model>;
    using Lexeme = typename Expression::BasicLexeme;

    return Expression{Lexeme{
        _field,
        BasicOperation::GreaterEq,
        static_cast<std::int64_t>(_value)}};
}


// Floating-point-type expressions

template<typename Model, typename T, typename Traits = ModelTraits<Model>>
inline auto eq(typename Traits::AttributeType _field, T _value)
    -> std::enable_if_t<std::is_floating_point_v<T>, Expression<Model>>
{
    using Expression = Expression<Model>;
    using Lexeme = typename Expression::BasicLexeme;

    return Expression{
        Lexeme{_field, BasicOperation::Eq, static_cast<std::double_t>(_value)}};
}

template<typename Model, typename T, typename Traits = ModelTraits<Model>>
inline auto neq(typename Traits::AttributeType _field, T _value)
    -> std::enable_if_t<std::is_floating_point_v<T>, Expression<Model>>
{
    using Expression = Expression<Model>;
    using Lexeme = typename Expression::BasicLexeme;

    return Expression{Lexeme{
        _field,
        BasicOperation::Neq,
        static_cast<std::double_t>(_value)}};
}

template<typename Model, typename T, typename Traits = ModelTraits<Model>>
inline auto less(typename Traits::AttributeType _field, T _value)
    -> std::enable_if_t<std::is_floating_point_v<T>, Expression<Model>>
{
    using Expression = Expression<Model>;
    using Lexeme = typename Expression::BasicLexeme;

    return Expression{Lexeme{
        _field,
        BasicOperation::Less,
        static_cast<std::double_t>(_value)}};
}

template<typename Model, typename T, typename Traits = ModelTraits<Model>>
inline auto greater(typename Traits::AttributeType _field, T _value)
    -> std::enable_if_t<std::is_floating_point_v<T>, Expression<Model>>
{
    using Expression = Expression<Model>;
    using Lexeme = typename Expression::BasicLexeme;

    return Expression{Lexeme{
        _field,
        BasicOperation::Greater,
        static_cast<std::double_t>(_value)}};
}

template<typename Model, typename T, typename Traits = ModelTraits<Model>>
inline auto lessEq(typename Traits::AttributeType _field, T _value)
    -> std::enable_if_t<std::is_floating_point_v<T>, Expression<Model>>
{
    using Expression = Expression<Model>;
    using Lexeme = typename Expression::BasicLexeme;

    return Expression{Lexeme{
        _field,
        BasicOperation::LessEq,
        static_cast<std::double_t>(_value)}};
}

template<typename Model, typename T, typename Traits = ModelTraits<Model>>
inline auto greaterEq(typename Traits::AttributeType _field, T _value)
    -> std::enable_if_t<std::is_floating_point_v<T>, Expression<Model>>
{
    using Expression = Expression<Model>;
    using Lexeme = typename Expression::BasicLexeme;

    return Expression{Lexeme{
        _field,
        BasicOperation::GreaterEq,
        static_cast<std::double_t>(_value)}};
}


// String-type expressions

template<typename Model, typename Traits = ModelTraits<Model>>
inline auto eq(typename Traits::AttributeType _field, std::string _value)
    -> Expression<Model>
{
    using Expression = Expression<Model>;
    using Lexeme = typename Expression::BasicLexeme;

    return Expression{Lexeme{_field, BasicOperation::Eq, std::move(_value)}};
}

template<typename Model, typename Traits = ModelTraits<Model>>
inline auto neq(typename Traits::AttributeType _field, std::string _value)
    -> Expression<Model>
{
    using Expression = Expression<Model>;
    using Lexeme = typename Expression::BasicLexeme;

    return Expression{Lexeme{_field, BasicOperation::Neq, std::move(_value)}};
}

// Custom-type (enumeration) expressions

template<typename Model, typename T, typename Traits = ModelTraits<Model>>
inline auto eq(typename Traits::AttributeType _field, T _value)
    -> std::enable_if_t<std::is_enum_v<T>, Expression<Model>>
{
    using Expression = Expression<Model>;
    using Lexeme = typename Expression::BasicLexeme;

    return Expression{Lexeme{_field, BasicOperation::Eq, _value}};
}

template<typename Model, typename T, typename Traits = ModelTraits<Model>>
inline auto neq(typename Traits::AttributeType _field, T _value)
    -> std::enable_if_t<std::is_enum_v<T>, Expression<Model>>
{
    using Expression = Expression<Model>;
    using Lexeme = typename Expression::BasicLexeme;

    return Expression{Lexeme{_field, BasicOperation::Neq, _value}};
}

// Deleted functions (to avoid bugs with implicit conversions)

template<typename Model, typename Traits = ModelTraits<Model>>
inline auto eq(typename Traits::AttributeType _field, char const* _value) =
    delete;

template<typename Model, typename Traits = ModelTraits<Model>>
inline auto neq(typename Traits::AttributeType _field, char const* _value) =
    delete;


// Composite operations on expressions

template<typename Model>
auto operator&&(Expression<Model> _left, Expression<Model> _right)
    -> Expression<Model>
{
    return Expression<Model>::compose(
        std::move(_left),
        CompositeOperation::AND,
        std::move(_right)
    );
}

template<typename Model>
auto operator||(Expression<Model> _left, Expression<Model> _right)
    -> Expression<Model>
{
    return Expression<Model>::compose(
        std::move(_left),
        CompositeOperation::OR,
        std::move(_right)
    );
}

} // namespace Simulator::DataLayer::Predicate

#endif // SIMULATOR_DATA_LAYER_INCLUDE_DATA_LAYER_API_PREDICATE_EXPRESSION_HPP_
