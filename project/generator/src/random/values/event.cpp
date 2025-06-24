#include "ih/random/values/event.hpp"

#include <stdexcept>

#include <fmt/format.h>

namespace Simulator::Generator::Random {

Event::Event(RandomInteger _integer) noexcept
    : m_type{ toType(_integer) }
{}


bool Event::isNoop() const noexcept
{
    return m_type == Type::NoOperation || m_type == Type::EventsCount;
}


bool Event::isBuyEvent() const noexcept
{
    return m_type == Type::RestingBuy || m_type == Type::AggressiveBuy;
}


bool Event::isSellEvent() const noexcept
{
    return m_type == Type::RestingSell || m_type == Type::AggressiveSell;
}


bool Event::isRestingOrderEvent() const noexcept
{
    return m_type == Type::RestingBuy || m_type == Type::RestingSell;
}


bool Event::isAggressiveOrderEvent() const noexcept
{
    return m_type == Type::AggressiveBuy || m_type == Type::AggressiveSell;
}


Event::Type Event::getType() const noexcept
{
    return m_type;
}


std::string Event::toString() const
{
    if (isNoop())
    {
        return "no-operation random order event";
    }

    return fmt::format(
            "{type} {side} random order generation event"
        ,   fmt::arg("type", isRestingOrderEvent() ? "resting" : "aggressive")
        ,   fmt::arg("side", isBuyEvent() ? "buy" : "sell")
    );
}


simulator::Side Event::targetSide() const
{
    if (isBuyEvent())
    {
        return simulator::Side::Option::Buy;
    }
    if (isSellEvent())
    {
        return simulator::Side::Option::Sell;
    }

    throw std::logic_error {
        "unable to deduce an associated side for a no-operation random event"
    };
}

} // namespace Simulator::Generator::Random
