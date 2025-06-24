#include <stdexcept>
#include <string_view>
#include <type_traits>

#include <gtest/gtest.h>

#include "ih/random/values/event.hpp"

namespace Simulator::Generator::Random {
namespace {

TEST(Generator_Random_Event, CheckBounds)
{
    static_assert(
        std::is_same_v<std::underlying_type_t<Event::Type>, unsigned int>
    );

    auto checkEnum = [] (Event::Type _enumType, unsigned int _expValue)
    {
        auto const underlying = static_cast<unsigned int>(_enumType);
        EXPECT_EQ(underlying, _expValue);
    };

    EXPECT_EQ(Event::minRandomInteger(), 0);
    EXPECT_EQ(Event::maxRandomInteger(), 29);

    constexpr unsigned int restingBuy = 0;
    constexpr unsigned int restingSell = 8;
    constexpr unsigned int aggressiveSell = 16;
    constexpr unsigned int aggressiveBuy = 18;
    constexpr unsigned int noOperation = 20;
    constexpr unsigned int eventsCount = 30;

    checkEnum(Event::Type::RestingBuy, restingBuy);
    checkEnum(Event::Type::RestingSell, restingSell);
    checkEnum(Event::Type::AggressiveSell, aggressiveSell);
    checkEnum(Event::Type::AggressiveBuy, aggressiveBuy);
    checkEnum(Event::Type::NoOperation, noOperation);
    checkEnum(Event::Type::EventsCount, eventsCount);
}

TEST(Generator_Random_Event, Create_RestingBuy)
{
    constexpr unsigned int minValue = 0;
    constexpr unsigned int midValue = 4;
    constexpr unsigned int maxValue = 7;

    auto checkEvent = [] (Event _event)
    {
        EXPECT_TRUE(_event.isBuyEvent());
        EXPECT_FALSE(_event.isSellEvent());
        EXPECT_EQ(_event.targetSide(), simulator::Side::Option::Buy);

        EXPECT_TRUE(_event.isRestingOrderEvent());
        EXPECT_FALSE(_event.isAggressiveOrderEvent());

        EXPECT_FALSE(_event.isNoop());
        EXPECT_EQ(_event.getType(), Event::Type::RestingBuy);
    };

    checkEvent(Event{ minValue });
    checkEvent(Event{ midValue });
    checkEvent(Event{ maxValue });
}

TEST(Generator_Random_Event, Create_RestingSell)
{
    constexpr unsigned int minValue = 8;
    constexpr unsigned int midValue = 10;
    constexpr unsigned int maxValue = 15;

    auto checkEvent = [] (Event _event)
    {
        EXPECT_FALSE(_event.isBuyEvent());
        EXPECT_TRUE(_event.isSellEvent());
        EXPECT_EQ(_event.targetSide(), simulator::Side::Option::Sell);

        EXPECT_TRUE(_event.isRestingOrderEvent());
        EXPECT_FALSE(_event.isAggressiveOrderEvent());

        EXPECT_FALSE(_event.isNoop());
        EXPECT_EQ(_event.getType(), Event::Type::RestingSell);
    };

    checkEvent(Event{ minValue });
    checkEvent(Event{ midValue });
    checkEvent(Event{ maxValue });
}

TEST(Generator_Random_Event, Create_AggressiveSell)
{
    constexpr unsigned int minValue = 16;
    constexpr unsigned int maxValue = 17;

    auto checkEvent = [] (Event _event)
    {
        EXPECT_FALSE(_event.isBuyEvent());
        EXPECT_TRUE(_event.isSellEvent());
        EXPECT_EQ(_event.targetSide(), simulator::Side::Option::Sell);

        EXPECT_FALSE(_event.isRestingOrderEvent());
        EXPECT_TRUE(_event.isAggressiveOrderEvent());

        EXPECT_FALSE(_event.isNoop());
        EXPECT_EQ(_event.getType(), Event::Type::AggressiveSell);
    };

    checkEvent(Event{ minValue });
    checkEvent(Event{ maxValue });
}

TEST(Generator_Random_Event, Create_AggressiveBuy)
{
    constexpr unsigned int minValue = 18;
    constexpr unsigned int maxValue = 19;

    auto checkEvent = [] (Event _event)
    {
        EXPECT_TRUE(_event.isBuyEvent());
        EXPECT_FALSE(_event.isSellEvent());
        EXPECT_EQ(_event.targetSide(), simulator::Side::Option::Buy);

        EXPECT_FALSE(_event.isRestingOrderEvent());
        EXPECT_TRUE(_event.isAggressiveOrderEvent());

        EXPECT_FALSE(_event.isNoop());
        EXPECT_EQ(_event.getType(), Event::Type::AggressiveBuy);
    };

    checkEvent(Event{ minValue });
    checkEvent(Event{ maxValue });
}

TEST(Generator_Random_Event, Create_NoOperation)
{
    constexpr unsigned int minValue = 20;
    constexpr unsigned int midValue = 25;
    constexpr unsigned int maxValue = 29;

    auto checkEvent = [] (Event _event)
    {
        EXPECT_FALSE(_event.isBuyEvent());
        EXPECT_FALSE(_event.isSellEvent());
        EXPECT_THROW(
            [[maybe_unused]] auto const type = _event.targetSide(),
            std::logic_error
        );

        EXPECT_FALSE(_event.isRestingOrderEvent());
        EXPECT_FALSE(_event.isAggressiveOrderEvent());

        EXPECT_TRUE(_event.isNoop());
        EXPECT_EQ(_event.getType(), Event::Type::NoOperation);
    };

    checkEvent(Event { minValue });
    checkEvent(Event { midValue });
    checkEvent(Event { maxValue });
}

TEST(Generator_Random_Event, Create_OutOfRange)
{
    constexpr unsigned int minValue = 30;
    constexpr unsigned int randomValue = 124;

    auto checkEvent = [] (Event _event)
    {
        EXPECT_FALSE(_event.isBuyEvent());
        EXPECT_FALSE(_event.isSellEvent());
        EXPECT_THROW(
            [[maybe_unused]] auto const type = _event.targetSide(),
            std::logic_error
        );

        EXPECT_FALSE(_event.isRestingOrderEvent());
        EXPECT_FALSE(_event.isAggressiveOrderEvent());

        EXPECT_TRUE(_event.isNoop());
        EXPECT_EQ(_event.getType(), Event::Type::EventsCount);
    };

    checkEvent(Event { minValue });
    checkEvent(Event { randomValue });
}

TEST(Generator_Random_Event, Format)
{
    auto checkFormat =
    [] (Event::Type _eventType, std::string_view _format)
    {
        Event const event{ static_cast<unsigned int>(_eventType) };
        EXPECT_EQ(event.toString(), _format);
    };

    checkFormat(
        Event::Type::RestingBuy,
        "resting buy random order generation event"
    );

    checkFormat(
        Event::Type::RestingSell,
        "resting sell random order generation event"
    );

    checkFormat(
        Event::Type::AggressiveBuy,
        "aggressive buy random order generation event"
    );

    checkFormat(
        Event::Type::AggressiveSell,
        "aggressive sell random order generation event"
    );

    checkFormat(
        Event::Type::NoOperation,
        "no-operation random order event"
    );

    checkFormat(
        Event::Type::EventsCount,
        "no-operation random order event"
    );
}

}  // namespace
}  // namespace Simulator::Generator::Random