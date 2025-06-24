#include <gtest/gtest.h>

#include "ih/random/values/price_generation_params.hpp"

using namespace Simulator;
using namespace Simulator::Generator;
using namespace Simulator::Generator::Random;

TEST(Generator_Random_PriceGenerationParams, SpecifyPriceTickRange)
{
    constexpr int expectedPxTickRange = 23;

    PriceGenerationParams::Builder pxParamsBuilder{};
    pxParamsBuilder.setPriceTickRange(expectedPxTickRange);
    PriceGenerationParams const pxParams{ pxParamsBuilder };

    EXPECT_EQ(pxParams.getPriceTickRange(), expectedPxTickRange);

    EXPECT_DOUBLE_EQ(pxParams.getPriceTickSize(), 0.);
    EXPECT_DOUBLE_EQ(pxParams.getPriceSpread(), 0.);
}

TEST(Generator_Random_PriceGenerationParams, SpecifyPriceTickSize)
{
    constexpr double expectedPxTickSize = 32.235346642;

    PriceGenerationParams::Builder pxParamsBuilder{};
    pxParamsBuilder.setPriceTickSize(expectedPxTickSize);
    PriceGenerationParams const pxParams{ pxParamsBuilder };

    EXPECT_DOUBLE_EQ(pxParams.getPriceTickSize(), expectedPxTickSize);

    EXPECT_EQ(pxParams.getPriceTickRange(), 0);
    EXPECT_DOUBLE_EQ(pxParams.getPriceSpread(), 0.);
}

TEST(Generator_Random_PriceGenerationParams, SpecifyPriceSpread)
{
    constexpr double expectedPxSpread = 1.00000005;

    PriceGenerationParams::Builder pxParamsBuilder{};
    pxParamsBuilder.setPriceSpread(expectedPxSpread);
    PriceGenerationParams const pxParams{ pxParamsBuilder };

    EXPECT_DOUBLE_EQ(pxParams.getPriceSpread(), expectedPxSpread);

    EXPECT_EQ(pxParams.getPriceTickRange(), 0);
    EXPECT_DOUBLE_EQ(pxParams.getPriceTickSize(), 0.);
}

TEST(Generator_Random_PriceGenerationParams, SpecifyAll)
{
    constexpr int expectedPxTickRange = 2;
    constexpr double expectedPxTickSize = 2.000000001;
    constexpr double expectedPxSpread = 1.00000005;

    PriceGenerationParams::Builder pxParamsBuilder{};
    pxParamsBuilder.setPriceTickSize(expectedPxTickSize);
    pxParamsBuilder.setPriceSpread(expectedPxSpread);
    pxParamsBuilder.setPriceTickRange(expectedPxTickRange);

    PriceGenerationParams const pxParams{ pxParamsBuilder };

    EXPECT_EQ(pxParams.getPriceTickRange(), expectedPxTickRange);
    EXPECT_DOUBLE_EQ(pxParams.getPriceTickSize(), expectedPxTickSize);
    EXPECT_DOUBLE_EQ(pxParams.getPriceSpread(), expectedPxSpread);
}
