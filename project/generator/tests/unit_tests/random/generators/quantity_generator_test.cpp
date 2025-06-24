#include <memory>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ih/random/generators/quantity_generator.hpp"
#include "ih/random/values/quantity_generation_params.hpp"
#include "mocks/random/value_generator.hpp"

using namespace Simulator;
using namespace Simulator::Generator;
using namespace Simulator::Generator::Random;

using ::testing::Eq;
using ::testing::Matcher;
using ::testing::Return;

class Generator_Random_QuantityGenerator
    :    public testing::Test
{
public:

    auto valueGenerator() -> Mock::ValueGenerator &
    {
        return *m_pValueGenerator;
    }

    auto generate(Random::QuantityGenerationParams const & _params) -> double
    {
        return m_pQtyGenerator->generateQty(_params);
    }

    static auto makeParams(double _multiplier, double _minQty, double _maxQty)
        -> Random::QuantityGenerationParams
    {
        QuantityGenerationParams::Builder qtyParamsBuilder {};
        qtyParamsBuilder.setQuantityMultiplier(_multiplier);
        qtyParamsBuilder.setMinimalQuantity(_minQty);
        qtyParamsBuilder.setMaximalQuantity(_maxQty);
        return QuantityGenerationParams { qtyParamsBuilder };
    }

protected:

    void SetUp() override
    {
        m_pValueGenerator = std::make_shared<Mock::ValueGenerator>();
        m_pQtyGenerator = std::make_unique<Random::QuantityGeneratorImpl>(
            m_pValueGenerator
        );
    }

private:

    std::shared_ptr<Mock::ValueGenerator> m_pValueGenerator;
    std::unique_ptr<QuantityGenerator> m_pQtyGenerator;
};

TEST_F(Generator_Random_QuantityGenerator, AllParamsZero)
{
    // In case all params are equal to zero, we'll use default multiplier as qty
    // Default multiplier is used when provided multiplier equals to 0
    // Default multiplier must be equal to 1
    constexpr double expectedGeneratedQty = 1.0;

    constexpr std::int32_t randomValue = 0;
    constexpr double expectedRandMin = 0.0;
    constexpr double expectedRandMax = 0.0;

    EXPECT_CALL(valueGenerator(), generateUniform(
         Matcher<std::int64_t>(Eq(expectedRandMin)),
         Matcher<std::int64_t>(Eq(expectedRandMax))))
        .Times(1)
        .WillOnce(Return(randomValue));

    double const generatedQty = generate(makeParams(0., 0., 0.));
    EXPECT_DOUBLE_EQ(generatedQty, expectedGeneratedQty);
}

TEST_F(Generator_Random_QuantityGenerator, MultiplierIsOne)
{
    constexpr double multiplier = 1.0;
    constexpr double minQty = 20.0;
    constexpr double maxQty = 1000.0;

    // (maxQty / multiplier) - (minQty / multiplier)
    constexpr std::int64_t expectedRandMax = 980;
    constexpr std::int64_t expectedRandMin = 0;
    constexpr std::int64_t randomValue = 700;

    // (randomValue + (minQty / multiplier)) * multiplier
    constexpr double expectedGeneratedQty = 720.0;

    EXPECT_CALL(valueGenerator(), generateUniform(
        Matcher<std::int64_t>(Eq(expectedRandMin)),
        Matcher<std::int64_t>(Eq(expectedRandMax))))
        .Times(1)
        .WillOnce(Return(randomValue));

    double const generatedQty = generate(makeParams(multiplier, minQty, maxQty));
    EXPECT_DOUBLE_EQ(generatedQty, expectedGeneratedQty);
}

TEST_F(Generator_Random_QuantityGenerator, MultiplierIsThree)
{
    constexpr double multiplier = 3.0;
    constexpr double minQty = 30.0;
    constexpr double maxQty = 12000.0;

    // (maxQty / multiplier) - (minQty / multiplier)
    constexpr std::int64_t expectedRandMax = 3990;
    constexpr std::int64_t expectedRandMin = 0;
    constexpr std::int64_t randomValue = 2100;

    // (randomValue + (minQty / multiplier)) * multiplier
    constexpr double expectedGeneratedQty = 6330.0;

    EXPECT_CALL(valueGenerator(), generateUniform(
        Matcher<std::int64_t>(Eq(expectedRandMin)),
        Matcher<std::int64_t>(Eq(expectedRandMax))))
        .Times(1)
        .WillOnce(Return(randomValue));

    double const generatedQty = generate(makeParams(multiplier, minQty, maxQty));
    EXPECT_DOUBLE_EQ(generatedQty, expectedGeneratedQty);
}
