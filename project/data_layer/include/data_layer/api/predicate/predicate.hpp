#ifndef SIMULATOR_DATA_LAYER_INCLUDE_DATA_LAYER_API_PREDICATE_PREDICATE_HPP_
#define SIMULATOR_DATA_LAYER_INCLUDE_DATA_LAYER_API_PREDICATE_PREDICATE_HPP_

#include <tuple>

#include "data_layer/api/models/datasource.hpp"
#include "data_layer/api/models/listing.hpp"
#include "data_layer/api/models/price_seed.hpp"
#include "data_layer/api/models/setting.hpp"
#include "data_layer/api/models/venue.hpp"
#include "data_layer/api/predicate/expression.hpp"
#include "data_layer/api/predicate/traits.hpp"

namespace Simulator::DataLayer {
namespace Predicate {

template<>
struct ModelTraits<DataLayer::Datasource> {
    using AttributeType = Datasource::Attribute;
    using CustomFieldTypes = std::tuple<Datasource::Format, Datasource::Type>;
};

template<>
struct ModelTraits<DataLayer::Listing> {
    using AttributeType = Listing::Attribute;
    using CustomFieldTypes = std::tuple<>;
};

template<>
struct ModelTraits<DataLayer::PriceSeed> {
    using AttributeType = PriceSeed::Attribute;
    using CustomFieldTypes = std::tuple<>;
};

template<>
struct ModelTraits<DataLayer::Setting> {
    using AttributeType = Setting::Attribute;
    using CustomFieldTypes = std::tuple<>;
};

template<>
struct ModelTraits<DataLayer::Venue> {
    using AttributeType = Venue::Attribute;
    using CustomFieldTypes = std::tuple<Venue::EngineType>;
};

} // namespace Predicate

struct DatasourceCmp {
    using ExpressionType = Predicate::Expression<DataLayer::Datasource>;

    template<typename T>
    static auto eq(Datasource::Attribute _field, T&& _value) -> ExpressionType
    {
        return Predicate::eq<Datasource>(_field, std::forward<T>(_value));
    }

    template<typename T>
    static auto neq(Datasource::Attribute _field, T&& _value) -> ExpressionType
    {
        return Predicate::neq<Datasource>(_field, std::forward<T>(_value));
    }

    template<typename T>
    static auto less(Datasource::Attribute _field, T&& _value) -> ExpressionType
    {
        static_assert(Predicate::is_less_greater_comparable_v<T>);
        return Predicate::less<Datasource>(_field, std::forward<T>(_value));
    }

    template<typename T>
    static auto greater(Datasource::Attribute _field, T&& _value)
        -> ExpressionType
    {
        static_assert(Predicate::is_less_greater_comparable_v<T>);
        return Predicate::greater<Datasource>(_field, std::forward<T>(_value));
    }

    template<typename T>
    static auto lessEq(Datasource::Attribute _field, T&& _value)
        -> ExpressionType
    {
        static_assert(Predicate::is_less_greater_comparable_v<T>);
        return Predicate::lessEq<Datasource>(_field, std::forward<T>(_value));
    }

    template<typename T>
    static auto greaterEq(Datasource::Attribute _field, T&& _value)
        -> ExpressionType
    {
        static_assert(Predicate::is_less_greater_comparable_v<T>);
        return Predicate::greaterEq<Datasource>(
            _field,
            std::forward<T>(_value)
        );
    }
};

struct ListingCmp {
    using ExpressionType = Predicate::Expression<DataLayer::Listing>;

    template<typename T>
    static auto eq(Listing::Attribute _field, T&& _value) -> ExpressionType
    {
        return Predicate::eq<Listing>(_field, std::forward<T>(_value));
    }

    template<typename T>
    static auto neq(Listing::Attribute _field, T&& _value) -> ExpressionType
    {
        return Predicate::neq<Listing>(_field, std::forward<T>(_value));
    }

    template<typename T>
    static auto less(Listing::Attribute _field, T&& _value) -> ExpressionType
    {
        static_assert(Predicate::is_less_greater_comparable_v<T>);
        return Predicate::less<Listing>(_field, std::forward<T>(_value));
    }

    template<typename T>
    static auto greater(Listing::Attribute _field, T&& _value)
        -> ExpressionType
    {
        static_assert(Predicate::is_less_greater_comparable_v<T>);
        return Predicate::greater<Listing>(_field, std::forward<T>(_value));
    }

    template<typename T>
    static auto lessEq(Listing::Attribute _field, T&& _value)
        -> ExpressionType
    {
        static_assert(Predicate::is_less_greater_comparable_v<T>);
        return Predicate::lessEq<Listing>(_field, std::forward<T>(_value));
    }

    template<typename T>
    static auto greaterEq(Listing::Attribute _field, T&& _value)
        -> ExpressionType
    {
        static_assert(Predicate::is_less_greater_comparable_v<T>);
        return Predicate::greaterEq<Listing>(_field, std::forward<T>(_value));
    }
};

struct PriceSeedCmp {
    using ExpressionType = Predicate::Expression<DataLayer::PriceSeed>;

    template<typename T>
    static auto eq(PriceSeed::Attribute _field, T&& _value) -> ExpressionType
    {
        return Predicate::eq<PriceSeed>(_field, std::forward<T>(_value));
    }

    template<typename T>
    static auto neq(PriceSeed::Attribute _field, T&& _value) -> ExpressionType
    {
        return Predicate::neq<PriceSeed>(_field, std::forward<T>(_value));
    }

    template<typename T>
    static auto less(PriceSeed::Attribute _field, T&& _value) -> ExpressionType
    {
        static_assert(Predicate::is_less_greater_comparable_v<T>);
        return Predicate::less<PriceSeed>(_field, std::forward<T>(_value));
    }

    template<typename T>
    static auto greater(PriceSeed::Attribute _field, T&& _value)
    -> ExpressionType
    {
        static_assert(Predicate::is_less_greater_comparable_v<T>);
        return Predicate::greater<PriceSeed>(_field, std::forward<T>(_value));
    }

    template<typename T>
    static auto lessEq(PriceSeed::Attribute _field, T&& _value)
    -> ExpressionType
    {
        static_assert(Predicate::is_less_greater_comparable_v<T>);
        return Predicate::lessEq<PriceSeed>(_field, std::forward<T>(_value));
    }

    template<typename T>
    static auto greaterEq(PriceSeed::Attribute _field, T&& _value)
    -> ExpressionType
    {
        static_assert(Predicate::is_less_greater_comparable_v<T>);
        return Predicate::greaterEq<PriceSeed>(_field, std::forward<T>(_value));
    }
};

struct SettingCmp {
    using ExpressionType = Predicate::Expression<DataLayer::Setting>;

    template<typename T>
    static auto eq(Setting::Attribute _field, T&& _value) -> ExpressionType
    {
        return Predicate::eq<Setting>(_field, std::forward<T>(_value));
    }

    template<typename T>
    static auto neq(Setting::Attribute _field, T&& _value) -> ExpressionType
    {
        return Predicate::neq<Setting>(_field, std::forward<T>(_value));
    }

    template<typename T>
    static auto less(Setting::Attribute _field, T&& _value) -> ExpressionType
    {
        static_assert(Predicate::is_less_greater_comparable_v<T>);
        return Predicate::less<Setting>(_field, std::forward<T>(_value));
    }

    template<typename T>
    static auto greater(Setting::Attribute _field, T&& _value)
        -> ExpressionType
    {
        static_assert(Predicate::is_less_greater_comparable_v<T>);
        return Predicate::greater<Setting>(_field, std::forward<T>(_value));
    }

    template<typename T>
    static auto lessEq(Setting::Attribute _field, T&& _value)
        -> ExpressionType
    {
        static_assert(Predicate::is_less_greater_comparable_v<T>);
        return Predicate::lessEq<Setting>(_field, std::forward<T>(_value));
    }

    template<typename T>
    static auto greaterEq(Setting::Attribute _field, T&& _value)
        -> ExpressionType
    {
        static_assert(Predicate::is_less_greater_comparable_v<T>);
        return Predicate::greaterEq<Setting>(_field, std::forward<T>(_value));
    }
};

struct VenueCmp {
    using ExpressionType = Predicate::Expression<DataLayer::Venue>;

    template<typename T>
    static auto eq(Venue::Attribute _field, T&& _value) -> ExpressionType
    {
        return Predicate::eq<Venue>(_field, std::forward<T>(_value));
    }

    template<typename T>
    static auto neq(Venue::Attribute _field, T&& _value) -> ExpressionType
    {
        return Predicate::neq<Venue>(_field, std::forward<T>(_value));
    }

    template<typename T>
    static auto less(Venue::Attribute _field, T&& _value) -> ExpressionType
    {
        static_assert(Predicate::is_less_greater_comparable_v<T>);
        return Predicate::less<Venue>(_field, std::forward<T>(_value));
    }

    template<typename T>
    static auto greater(Venue::Attribute _field, T&& _value) -> ExpressionType
    {
        static_assert(Predicate::is_less_greater_comparable_v<T>);
        return Predicate::greater<Venue>(_field, std::forward<T>(_value));
    }

    template<typename T>
    static auto lessEq(Venue::Attribute _field, T&& _value) -> ExpressionType
    {
        static_assert(Predicate::is_less_greater_comparable_v<T>);
        return Predicate::lessEq<Venue>(_field, std::forward<T>(_value));
    }

    template<typename T>
    static auto greaterEq(Venue::Attribute _field, T&& _value) -> ExpressionType
    {
        static_assert(Predicate::is_less_greater_comparable_v<T>);
        return Predicate::greaterEq<Venue>(_field, std::forward<T>(_value));
    }
};

} // namespace Simulator::DataLayer

#endif // SIMULATOR_DATA_LAYER_INCLUDE_DATA_LAYER_API_PREDICATE_PREDICATE_HPP_
