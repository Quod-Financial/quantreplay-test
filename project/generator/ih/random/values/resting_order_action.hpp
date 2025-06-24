#ifndef SIMULATOR_GENERATOR_SRC_VALUES_RESTING_ORDER_ACTION_HPP_
#define SIMULATOR_GENERATOR_SRC_VALUES_RESTING_ORDER_ACTION_HPP_

#include <cstdint>
#include <string>

namespace Simulator::Generator::Random {

class RestingOrderAction
{
public:

    enum class Type
        :   std::uint8_t
    {
        QuantityModification = 0,
        PriceModification = 9,
        Cancellation = 18,
        ActionsCount = 20
    };

    using RandomInteger = std::underlying_type_t<Type>;

    static constexpr auto minRandomInteger()
    {
        return toUnderlying(Type::QuantityModification);
    }

    static constexpr auto maxRandomInteger()
    {
        return static_cast<RandomInteger>(toUnderlying(Type::ActionsCount) - 1);
    }


    RestingOrderAction() = delete;

    constexpr explicit RestingOrderAction(RandomInteger _integer)
        :   m_actionType { toType(_integer) }
    {}


    [[nodiscard]]
    bool isQuantityModification() const noexcept;

    [[nodiscard]]
    bool isPriceModification() const noexcept;

    [[nodiscard]]
    bool isCancellation() const noexcept;


    [[nodiscard]]
    Type getActionType() const noexcept;

    [[nodiscard]]
    std::string toString() const noexcept;

private:

    static constexpr RandomInteger toUnderlying(Type _type)
    {
        return static_cast<RandomInteger>(_type);
    }

    static constexpr Type toType(RandomInteger _value)
    {
        Type convertedType { Type::ActionsCount };
        if (_value < toUnderlying(Type::PriceModification))
        {
            convertedType = Type::QuantityModification;
        }
        else if (_value < toUnderlying(Type::Cancellation))
        {
            convertedType = Type::PriceModification;
        }
        else if (_value < toUnderlying(Type::ActionsCount))
        {
            convertedType = Type::Cancellation;
        }
        return convertedType;
    }


    Type m_actionType { Type::ActionsCount };
};

} // namespace Simulator::Generator::Random

#endif // SIMULATOR_GENERATOR_SRC_VALUES_RESTING_ORDER_ACTION_HPP_
