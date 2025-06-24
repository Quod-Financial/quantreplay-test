#ifndef SIMULATOR_GENERATOR_SRC_VALUES_RANDOM_EVENT_HPP_
#define SIMULATOR_GENERATOR_SRC_VALUES_RANDOM_EVENT_HPP_

#include <string>
#include <type_traits>

#include "core/domain/attributes.hpp"

namespace Simulator::Generator::Random {

class Event
{
public:

    enum class Type : unsigned int
    {
        RestingBuy = 0,
        RestingSell = 8,
        AggressiveSell = 16,
        AggressiveBuy = 18,
        NoOperation = 20,
        EventsCount = 30
    };

    using RandomInteger = std::underlying_type_t<Type>;

    static constexpr auto minRandomInteger()
    {
        return toUnderlying(Type::RestingBuy);
    }

    static constexpr auto maxRandomInteger()
    {
        return toUnderlying(Type::EventsCount) - 1;
    }


    Event() = delete;

    explicit Event(RandomInteger _integer) noexcept;


    [[nodiscard]]
    bool isNoop() const noexcept;

    [[nodiscard]]
    bool isBuyEvent() const noexcept;

    [[nodiscard]]
    bool isSellEvent() const noexcept;

    [[nodiscard]]
    bool isRestingOrderEvent() const noexcept;

    [[nodiscard]]
    bool isAggressiveOrderEvent() const noexcept;

    [[nodiscard]]
    Event::Type getType() const noexcept;

    [[nodiscard]]
    std::string toString() const;

    // May throw logic error in case it's not possible to deduce side -
    [[nodiscard]]
    simulator::Side targetSide() const;

private:

    static constexpr RandomInteger toUnderlying(Type _type)
    {
        return static_cast<RandomInteger>(_type);
    }

    static constexpr Type toType(RandomInteger _value)
    {
        Type convertedType { Type::EventsCount };
        if (_value < toUnderlying(Type::RestingSell))
        {
            convertedType = Type::RestingBuy;
        }
        else if (_value < toUnderlying(Type::AggressiveSell))
        {
            convertedType = Type::RestingSell;
        }
        else if (_value < toUnderlying(Type::AggressiveBuy))
        {
            convertedType = Type::AggressiveSell;
        }
        else if (_value < toUnderlying(Type::NoOperation))
        {
            convertedType = Type::AggressiveBuy;
        }
        else if (_value < toUnderlying(Type::EventsCount))
        {
            convertedType = Type::NoOperation;
        }
        return convertedType;
    }

    Type m_type { Type::EventsCount };
};

} // namespace Simulator::Generator::Random

#endif // SIMULATOR_GENERATOR_SRC_VALUES_RANDOM_EVENT_HPP_
