#include <string_view>
#include <type_traits>

#include <gtest/gtest.h>

#include "ih/random/values/resting_order_action.hpp"

using namespace Simulator;
using namespace Simulator::Generator;
using namespace Simulator::Generator::Random;

TEST(Generator_Random_RestingOrderAction, CheckBounds)
{
    static_assert(
        std::is_same_v<
                std::underlying_type_t<RestingOrderAction::Type>
            ,   std::uint8_t
        >
    );

    auto checkEnum =
    [] (RestingOrderAction::Type _enumType, std::uint8_t _expValue) {
        auto const underlying = static_cast<std::uint8_t>(_enumType);
        EXPECT_EQ(underlying, _expValue);
    };

    EXPECT_EQ(RestingOrderAction::minRandomInteger(), 0);
    EXPECT_EQ(RestingOrderAction::maxRandomInteger(), 19);

    constexpr std::uint8_t quantityModification = 0;
    constexpr std::uint8_t priceModification = 9;
    constexpr std::uint8_t cancellation = 18;
    constexpr std::uint8_t actionsCount = 20;

    checkEnum(
        RestingOrderAction::Type::QuantityModification, quantityModification
    );

    checkEnum(
        RestingOrderAction::Type::PriceModification, priceModification
    );

    checkEnum(
        RestingOrderAction::Type::Cancellation, cancellation
    );

    checkEnum(
        RestingOrderAction::Type::ActionsCount, actionsCount
    );
}

TEST(Generator_Random_RestingOrderAction, Create_QuantityModification)
{
    constexpr std::uint8_t minValue = 0;
    constexpr std::uint8_t midValue = 5;
    constexpr std::uint8_t maxValue = 8;

    auto checkAction = [] (RestingOrderAction _action) {
        using Type = RestingOrderAction::Type;

        EXPECT_TRUE(_action.isQuantityModification());
        EXPECT_FALSE(_action.isPriceModification());
        EXPECT_FALSE(_action.isCancellation());
        EXPECT_EQ(_action.getActionType(), Type::QuantityModification);
    };

    checkAction(RestingOrderAction { minValue });
    checkAction(RestingOrderAction { midValue });
    checkAction(RestingOrderAction { maxValue });
}

TEST(Generator_Random_RestingOrderAction, Create_PriceModification)
{
    constexpr std::uint8_t minValue = 9;
    constexpr std::uint8_t midValue = 11;
    constexpr std::uint8_t maxValue = 17;

    auto checkAction = [] (RestingOrderAction _action) {
        using Type = RestingOrderAction::Type;

        EXPECT_FALSE(_action.isQuantityModification());
        EXPECT_TRUE(_action.isPriceModification());
        EXPECT_FALSE(_action.isCancellation());
        EXPECT_EQ(_action.getActionType(), Type::PriceModification);
    };

    checkAction(RestingOrderAction { minValue });
    checkAction(RestingOrderAction { midValue });
    checkAction(RestingOrderAction { maxValue });
}

TEST(Generator_Random_RestingOrderAction, Create_Cancellation)
{
    constexpr std::uint8_t minValue = 18;
    constexpr std::uint8_t maxValue = 19;

    auto checkAction = [] (RestingOrderAction _action) {
        using Type = RestingOrderAction::Type;

        EXPECT_FALSE(_action.isQuantityModification());
        EXPECT_FALSE(_action.isPriceModification());
        EXPECT_TRUE(_action.isCancellation());
        EXPECT_EQ(_action.getActionType(), Type::Cancellation);
    };

    checkAction(RestingOrderAction { minValue });
    checkAction(RestingOrderAction { maxValue });
}

TEST(Generator_Random_RestingOrderAction, Create_OutOfAllowedRange)
{
    constexpr std::uint8_t minValue = 20;
    constexpr std::uint8_t miscValue = 45;

    auto checkAction = [] (RestingOrderAction _action)
    {
        using Type = RestingOrderAction::Type;

        EXPECT_FALSE(_action.isQuantityModification());
        EXPECT_FALSE(_action.isPriceModification());
        EXPECT_FALSE(_action.isCancellation());
        EXPECT_EQ(_action.getActionType(), Type::ActionsCount);
    };

    checkAction(RestingOrderAction { minValue });
    checkAction(RestingOrderAction { miscValue });
}

TEST(Generator_Random_RestingOrderAction, Format)
{
    auto checkFormat =
    [] (RestingOrderAction::Type _actionType, std::string_view _format) {
        RestingOrderAction const action{ static_cast<std::uint8_t>(_actionType) };
        EXPECT_EQ(action.toString(), _format);
    };

    checkFormat(
        RestingOrderAction::Type::QuantityModification,
        "QuantityModification action"
    );

    checkFormat(
        RestingOrderAction::Type::PriceModification,
        "PriceModification action"
    );

    checkFormat(
        RestingOrderAction::Type::Cancellation,
        "Cancellation action"
    );

    checkFormat(
        RestingOrderAction::Type::ActionsCount,
        "undefined resting order action"
    );
}
