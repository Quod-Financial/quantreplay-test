#include "ih/random/values/resting_order_action.hpp"

namespace Simulator::Generator::Random {

bool RestingOrderAction::isQuantityModification() const noexcept
{
    return m_actionType == Type::QuantityModification;
}


bool RestingOrderAction::isPriceModification() const noexcept
{
    return m_actionType == Type::PriceModification;
}


bool RestingOrderAction::isCancellation() const noexcept
{
    return m_actionType == Type::Cancellation;
}


RestingOrderAction::Type RestingOrderAction::getActionType() const noexcept
{
    return m_actionType;
}


std::string RestingOrderAction::toString() const noexcept
{
    std::string action {};
    if (isQuantityModification())
    {
        action = "QuantityModification action";
    }
    else if (isPriceModification())
    {
        action = "PriceModification action";
    }
    else if (isCancellation())
    {
        action = "Cancellation action";
    }
    else
    {
        action = "undefined resting order action";
    }
    return action;
}

} // namespace Simulator::Generator::Random
