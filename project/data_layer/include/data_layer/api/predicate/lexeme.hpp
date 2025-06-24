#ifndef SIMULATOR_DATA_LAYER_INCLUDE_DATA_LAYER_API_PREDICATE_LEXEME_HPP_
#define SIMULATOR_DATA_LAYER_INCLUDE_DATA_LAYER_API_PREDICATE_LEXEME_HPP_

#include <variant>
#include <utility>

#include "data_layer/api/predicate/definitions.hpp"
#include "data_layer/api/predicate/traits.hpp"

namespace Simulator::DataLayer::Predicate {

// Represents a simple predicate expression, which matches given model attribute
// to a given value with a given binary operation, such as:
//     `listing_id = 10`, where:
//          'listing_id' represents an attribute name (enumeration)
//          '=' represents a binary logical operation (equal)
//          '10' represent a value
// Each attribute must be an enumeration, which is taken from model traits
// Each value must be represented by a type from StandardTypes tuple,
// or by an additional enumerable type specified in model traits in
// CustomFieldTypes tuple
template<typename Model, typename ModelTraits = ModelTraits<Model>>
class BasicPredicate {
    using AttributeType = typename ModelTraits::AttributeType;
    using ExtendedTypes = typename ModelTraits::CustomFieldTypes;
    using FieldTypes = aggregated_tuple_t<StandardTypes, ExtendedTypes>;
    using GenericValue = variant_from_tuple_t<FieldTypes>;

  public:
    BasicPredicate() = delete;

    template<
        typename V,
        std::enable_if_t<is_in_tuple_v<V, FieldTypes>>* = nullptr>
    BasicPredicate(AttributeType _field, BasicOperation _op, V&& _value) :
        mValue(std::forward<V>(_value)), mOperation(_op), mField(_field)
    {}

    template<typename Formatter>
    auto accept(Formatter& _formatter) const -> void
    {
        auto const valueVisitor = [&](auto const& _concreteValue) {
            _formatter(mField, mOperation, _concreteValue);
        };

        std::visit(valueVisitor, mValue);
    }

  private:
    GenericValue mValue;
    BasicOperation mOperation;
    AttributeType mField;
};


// Represents a logical binary operator applied to basic predicates and/or other
// composite structures, such as:
//     listing_id = 10 AND price <= 20:
//          AND is composite predicate applied on 2 basic predicates
//     (listing_id = 10 AND price <= 20) OR size > 100:
//          OR is a composite predicate applied on basic predicate and another
//          composite predicate
//     (listing_id = 10 AND price <= 20) OR (size > 100 AND name = 'my_name'):
//          OR is a composite predicate applied on 2 composite predicates
class CompositePredicate {
  public:
    CompositePredicate() = delete;

    explicit CompositePredicate(CompositeOperation _op) noexcept :
        mOperation(_op)
    {}

    template<typename Formatter>
    auto accept(Formatter& _formatter) const -> void
    {
        _formatter(mOperation);
    }

  private:
    CompositeOperation mOperation;
};


// Special type, which represents a predicate sub-expression begin.
// Usually represented as "("
class SubExpressionBegin {
  public:
    template<typename Formatter>
    auto accept(Formatter& _formatter) const -> void
    {
        _formatter(*this);
    }
};


// Special type, which represents a predicate sub-expression end.
// Usually represented as ")"
class SubExpressionEnd {
  public:
    template<typename Formatter>
    auto accept(Formatter& _formatter) const -> void
    {
        _formatter(*this);
    }
};

} // namespace Simulator::DataLayer::Predicate

#endif // SIMULATOR_DATA_LAYER_INCLUDE_DATA_LAYER_API_PREDICATE_LEXEME_HPP_
