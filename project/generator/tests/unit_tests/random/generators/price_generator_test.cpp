#include <memory>
#include <optional>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ih/random/generators/price_generator.hpp"
#include "ih/random/values/event.hpp"
#include "ih/random/values/price_generation_params.hpp"
#include "mocks/random/value_generator.hpp"

namespace Simulator::Generator::Random {
namespace {

using ::testing::A;
using ::testing::Eq;
using ::testing::Matcher;
using ::testing::Return;

class Generator_Random_PriceGenerator
    :   public testing::Test
{
public:

    using RandomEvent = Random::Event::Type;

    auto valueGenerator() -> Mock::ValueGenerator &
    {
        return *m_pValueGenerator;
    }

    auto generate(
            PriceGenerationParams const & _params
        ,   Generator::MarketState const & _actualPrices
        ,   DataLayer::PriceSeed const & _configuredPrices
        ,   Random::Event _event
    ) -> double
    {
        return m_pPriceGenerator->generatePrice(
                _params
            ,   _actualPrices
            ,   _configuredPrices
            ,   _event
        );
    }

    static Random::Event makeEvent(Random::Event::Type _eventType)
    {
        return Random::Event {
            static_cast<Random::Event::RandomInteger>(_eventType)
        };
    }

    static auto makeGenerationParams(
            std::optional<std::uint32_t> const & _pxTickRange = std::nullopt
        ,   std::optional<double> const & _pxTickSize = std::nullopt
        ,   std::optional<double> const & _pxSpread = std::nullopt
    ) -> Random::PriceGenerationParams
    {
        PriceGenerationParams::Builder pxGenParamsBuilder {};
        if (_pxTickRange.has_value())
        {
            pxGenParamsBuilder.setPriceTickRange(_pxTickRange.value());
        }
        if (_pxTickSize.has_value())
        {
            pxGenParamsBuilder.setPriceTickSize(_pxTickSize.value());
        }
        if (_pxSpread.has_value())
        {
            pxGenParamsBuilder.setPriceSpread(_pxSpread.value());
        }
        return PriceGenerationParams { pxGenParamsBuilder };
    }

    static auto makePxSeed(
            std::optional<double> const & _bidPx = std::nullopt
        ,   std::optional<double> const & _offerPx = std::nullopt
    ) -> DataLayer::PriceSeed
    {
        DataLayer::PriceSeed::Patch patch;
        if (_bidPx.has_value())
        {
            patch.withBidPrice(_bidPx.value());
        }
        if (_offerPx.has_value())
        {
            patch.withOfferPrice(_offerPx.value());
        }
        return DataLayer::PriceSeed::create(std::move(patch), 1);
    }

    static auto makeMktState(
            std::optional<double> const & _bestBuyPx = std::nullopt
        ,   std::optional<double> const & _bestSellPx = std::nullopt
    ) -> Generator::MarketState
    {
        Generator::MarketState mktState {};
        mktState.bestBidPrice = _bestBuyPx;
        mktState.bestOfferPrice = _bestSellPx;
        return mktState;
    }

protected:

    void SetUp() override
    {
        m_pValueGenerator = std::make_shared<Mock::ValueGenerator>();
        m_pPriceGenerator = std::make_unique<Random::PriceGeneratorImpl>(
            m_pValueGenerator
        );
    }

private:

    std::shared_ptr<Mock::ValueGenerator> m_pValueGenerator;
    std::unique_ptr<Random::PriceGenerator> m_pPriceGenerator;
};

TEST_F(Generator_Random_PriceGenerator, Generate_EmptyMktState_BuyOrder)
{
    constexpr double seedBuyPx = 123.5;
    constexpr double seedSellPx = 321.5;

    Random::PriceGenerationParams const pxGenParams = makeGenerationParams();
    MarketState const mktState = makeMktState();
    DataLayer::PriceSeed const pxSeed = makePxSeed(seedBuyPx, seedSellPx);

    EXPECT_CALL(valueGenerator(),
        generateUniform(A<std::int64_t>(), A<std::int64_t>()))
        .Times(0);

    double const restingPx = generate(
        pxGenParams, mktState, pxSeed, makeEvent(RandomEvent::RestingBuy)
    );
    EXPECT_DOUBLE_EQ(restingPx, seedBuyPx);

    double const aggressivePx = generate(
        pxGenParams, mktState, pxSeed, makeEvent(RandomEvent::AggressiveBuy)
    );
    EXPECT_DOUBLE_EQ(aggressivePx, seedBuyPx);
}

TEST_F(Generator_Random_PriceGenerator, Generate_EmptyMktState_SellOrder)
{
    constexpr double seedBuyPx = 123.5;
    constexpr double seedSellPx = 321.5;

    Random::PriceGenerationParams const pxGenParams = makeGenerationParams();
    MarketState const mktState = makeMktState();
    DataLayer::PriceSeed const pxSeed = makePxSeed(seedBuyPx, seedSellPx);

    EXPECT_CALL(valueGenerator(),
        generateUniform(A<std::int64_t>(), A<std::int64_t>()))
        .Times(0);

    double const restingPx = generate(
        pxGenParams, mktState, pxSeed, makeEvent(RandomEvent::RestingSell)
    );
    EXPECT_DOUBLE_EQ(restingPx, seedSellPx);

    double const aggressivePx = generate(
        pxGenParams, mktState, pxSeed, makeEvent(RandomEvent::AggressiveSell)
    );
    EXPECT_DOUBLE_EQ(aggressivePx, seedSellPx);
}

TEST_F(Generator_Random_PriceGenerator, Generate_RestingBuy_SellSideEmpty)
{
    // Input values:
    //     BasePx (aka bestBuyPx)        = 201.05
    //     PxTickRange (aka pxTickRange) = 10
    //     PxTickSize (aka pxTickSize)   = 0.001
    //     PxSpread (aka pxSpread)       = 0.1
    //     RandomValue (aka randomValue) = 10
    // Steps:
    //     1. GeometricSum = 13 (as static_cast<int>(13.206787162326272))
    //     2. RandomMin = 0, RandomMax = 12 (as GeometricSum - 1)
    //     3. logBase = 1.05, logNum = 1.4761904761904767
    //        (as (RandomValue * (1.05 - 1)) / 1.05 + 1)
    //     4. PxDeviation = 8 (as ceil(log(logBase) / log(1.05))
    //     5. RandomTick = 2 (as PxTickRange - PxDeviation)
    //     6. PxIncrement = 0.002 (as RandomTick * PxTickSize)
    // Output:
    //      GeneratedPx = 201.048 (as BasePx - PxIncrement)

    constexpr double bestBuyPx = 201.05;
    constexpr std::uint32_t pxTickRange = 10;
    constexpr double pxTickSize = 0.001;
    constexpr double pxSpread = 0.1;
    constexpr int randomValue = 10;

    constexpr int expectedMaxRand = 12;
    constexpr int expectedMinRand = 0;
    constexpr double expectedPx = 201.048;

    // No bestSellPx - opposite side is empty, price spread is not used
    MarketState const mktState = makeMktState(bestBuyPx);
    Random::Event const event = makeEvent(RandomEvent::RestingBuy);
    DataLayer::PriceSeed const pxSeed = makePxSeed();
    Random::PriceGenerationParams const pxGenParams = makeGenerationParams(
            pxTickRange
        ,   pxTickSize
        ,   pxSpread
    );

    EXPECT_CALL(valueGenerator(), generateUniform(
        Matcher<std::int64_t>(Eq(expectedMinRand)),
        Matcher<std::int64_t>(Eq(expectedMaxRand))))
        .Times(1)
        .WillOnce(Return(randomValue));

    double const price = generate(pxGenParams, mktState, pxSeed, event);
    EXPECT_DOUBLE_EQ(price, expectedPx);
}

TEST_F(Generator_Random_PriceGenerator, Generate_RestingBuy_SellSideHasPrices)
{
    // Input values:
    //     BasePx (aka bestSellPx)       = 201.05
    //     PxTickRange (aka pxTickRange) = 10
    //     PxTickSize (aka pxTickSize)   = 0.001
    //     PxSpread (aka pxSpread)       = 0.1
    //     RandomValue (aka randomValue) = 10
    // Steps:
    //     1. GeometricSum = 13 (as static_cast<int>(13.206787162326272))
    //     2. RandomMin = 0, RandomMax = 12 (as GeometricSum - 1)
    //     3. logBase = 1.05, logNum = 1.4761904761904767
    //        (as (RandomValue * (1.05 - 1)) / 1.05 + 1)
    //     4. PxDeviation = 8 (as ceil(log(logBase) / log(1.05))
    //     5. RandomTick = 2 (as PxTickRange - PxDeviation)
    //     6. ScaledRandomTick = 0.002 (as RandomTick * PxTickSize)
    //     7. PxIncrement = 0.102 (as ScaledRandomTick + PxSpread)
    // Output:
    //      GeneratedPx = 200.948 (as BasePx - PxIncrement)

    constexpr double bestBuyPx = 200.05;
    constexpr double bestSellPx = 201.05;
    constexpr std::uint32_t pxTickRange = 10;
    constexpr double pxTickSize = 0.001;
    constexpr double pxSpread = 0.1; // Unused for resting px based on same side
    constexpr int randomValue = 10;

    constexpr int expectedMaxRand = 12;
    constexpr int expectedMinRand = 0;
    constexpr double expectedPx = 200.948;

    // bestSellPx (opposite) has to be taken as a base price
    MarketState const mktState = makeMktState(bestBuyPx, bestSellPx);
    DataLayer::PriceSeed const pxSeed = makePxSeed();
    Random::Event const event = makeEvent(Random::Event::Type::RestingBuy);
    Random::PriceGenerationParams const pxGenParams = makeGenerationParams(
            pxTickRange
        ,   pxTickSize
        ,   pxSpread
    );

    EXPECT_CALL(valueGenerator(), generateUniform(
        Matcher<std::int64_t>(Eq(expectedMinRand)),
        Matcher<std::int64_t>(Eq(expectedMaxRand))))
        .Times(1)
        .WillOnce(Return(randomValue));

    double const price = generate(pxGenParams, mktState, pxSeed, event);
    EXPECT_DOUBLE_EQ(price, expectedPx);
}

TEST_F(Generator_Random_PriceGenerator, Generate_RestingSell_BuySideEmpty)
{
    // Input values:
    //     BasePx (aka bestSellPx)       = 434.045
    //     PxTickRange (aka pxTickRange) = 7
    //     PxTickSize (aka pxTickSize)   = 0.1
    //     RandomValue (aka randomValue) = 3
    // Steps:
    //     1. GeometricSum = 8 (as static_cast<int>(8.549108875781252))
    //     2. RandomMin = 0, RandomMax = 7 (as GeometricSum - 1)
    //     3. logBase = 1.05, logNum = 1.142857142857143
    //        (as (RandomValue * (1.05 - 1)) / 1.05 + 1)
    //     4. PxDeviation = 3 (as ceil(log(logBase) / log(1.05))
    //     5. RandomTick = 4 (as PxTickRange - PxDeviation)
    //     6. ScaledRandomTick = 0.4 (as RandomTick * PxTickSize)
    // Output:
    //      GeneratedPx = 434.445 (as BasePx + PxIncrement)

    constexpr double bestSellPx = 434.045;
    constexpr std::uint32_t pxTickRange = 7;
    constexpr double pxTickSize = 0.1;
    constexpr double pxSpread = 2; // Unused for resting px based on same side
    constexpr int randomValue = 3;

    constexpr int expectedMinRand = 0;
    constexpr int expectedMaxRand = 7;
    constexpr double expectedPx = 434.445;

    // No bestBuyPx - opposite side is empty, price spread is not used
    MarketState const mktState = makeMktState(std::nullopt, bestSellPx);
    DataLayer::PriceSeed const pxSeed = makePxSeed();
    Random::Event const event = makeEvent(Random::Event::Type::RestingSell);
    Random::PriceGenerationParams const pxGenParams = makeGenerationParams(
            pxTickRange
        ,   pxTickSize
        ,   pxSpread
    );

    EXPECT_CALL(valueGenerator(), generateUniform(
         Matcher<std::int64_t>(Eq(expectedMinRand)),
         Matcher<std::int64_t>(Eq(expectedMaxRand))))
        .Times(1)
        .WillOnce(Return(randomValue));

    double const price = generate(pxGenParams, mktState, pxSeed, event);
    EXPECT_DOUBLE_EQ(price, expectedPx);
}

TEST_F(Generator_Random_PriceGenerator, Generate_RestingSell_BuySideHasPrices)
{
    // Input values:
    //     BasePx (aka bestBuyPx)        = 434.045
    //     PxTickRange (aka pxTickRange) = 7
    //     PxTickSize (aka pxTickSize)   = 0.1
    //     PxSpread (aka pxSpread)       = 2
    //     RandomValue (aka randomValue) = 3
    // Steps:
    //     1. GeometricSum = 8 (as static_cast<int>(8.549108875781252))
    //     2. RandomMin = 0, RandomMax = 7 (as GeometricSum - 1)
    //     3. logBase = 1.05, logNum = 1.142857142857143
    //        (as (RandomValue * (1.05 - 1)) / 1.05 + 1)
    //     4. PxDeviation = 3 (as ceil(log(logBase) / log(1.05))
    //     5. RandomTick = 4 (as PxTickRange - PxDeviation)
    //     6. ScaledRandomTick = 0.4 (as RandomTick * PxTickSize)
    //     7. PxIncrement = 2.4 (as ScaledRandomTick + PxSpread)
    // Output:
    //      GeneratedPx = 436.445 (as BasePx + PxIncrement)

    constexpr double bestBuyPx = 434.045;
    constexpr double bestSellPx = 435.045;
    constexpr std::uint32_t pxTickRange = 7;
    constexpr double pxTickSize = 0.1;
    constexpr double pxSpread = 2;
    constexpr int randomValue = 3;

    constexpr int expectedMinRand = 0;
    constexpr int expectedMaxRand = 7;
    constexpr double expectedPx = 436.445;

    MarketState const mktState = makeMktState(bestBuyPx, bestSellPx);
    DataLayer::PriceSeed const pxSeed = makePxSeed();
    Random::Event const event = makeEvent(Random::Event::Type::RestingSell);
    Random::PriceGenerationParams const pxGenParams = makeGenerationParams(
            pxTickRange
        ,   pxTickSize
        ,   pxSpread
    );

    EXPECT_CALL(valueGenerator(), generateUniform(
        Matcher<std::int64_t>(Eq(expectedMinRand)),
        Matcher<std::int64_t>(Eq(expectedMaxRand))))
        .Times(1)
        .WillOnce(Return(randomValue));


    double const price = generate(pxGenParams, mktState, pxSeed, event);
    EXPECT_DOUBLE_EQ(price, expectedPx);
}

TEST_F(Generator_Random_PriceGenerator, Generate_AggressiveBuy)
{
    // Input values:
    //     BasePx (aka bestSellPx)       = 434.045
    //     PxTickRange (aka pxTickRange) = 7
    //     PxTickSize (aka pxTickSize)   = 0.1
    //     RandomValue (aka randomValue) = 3
    // Steps:
    //     1. GeometricSum = 8 (as static_cast<int>(8.549108875781252))
    //     2. RandomMin = 0, RandomMax = 7 (as GeometricSum - 1)
    //     3. logBase = 1.05, logNum = 1.142857142857143
    //        (as (RandomValue * (1.05 - 1)) / 1.05 + 1)
    //     4. PxDeviation = 3 (as ceil(log(logBase) / log(1.05))
    //     5. RandomTick = 4 (as PxTickRange - PxDeviation)
    //     6. PxIncrement = 0.4 (as RandomTick * PxTickSize)
    // Output:
    //      GeneratedPx = 434.445 (as BasePx + PxIncrement)

    constexpr double bestBuyPx = 433.045;
    constexpr double bestSellPx = 434.045;
    constexpr std::uint32_t pxTickRange = 7;
    constexpr double pxTickSize = 0.1;
    constexpr double pxSpread = 2; // Not used for aggressive orders
    constexpr int randomValue = 3;

    constexpr int expectedMinRand = 0;
    constexpr int expectedMaxRand = 7;
    constexpr double expectedPx = 434.445;

    MarketState const mktState = makeMktState(bestBuyPx, bestSellPx);
    DataLayer::PriceSeed const pxSeed = makePxSeed();
    Random::Event const event = makeEvent(Random::Event::Type::AggressiveBuy);
    Random::PriceGenerationParams const pxGenParams = makeGenerationParams(
            pxTickRange
        ,   pxTickSize
        ,   pxSpread
    );

    EXPECT_CALL(valueGenerator(), generateUniform(
        Matcher<std::int64_t>(Eq(expectedMinRand)),
        Matcher<std::int64_t>(Eq(expectedMaxRand))))
        .Times(1)
        .WillOnce(Return(randomValue));

    double const price = generate(pxGenParams, mktState, pxSeed, event);
    EXPECT_DOUBLE_EQ(price, expectedPx);
}

TEST_F(Generator_Random_PriceGenerator, Generate_AggressiveSell)
{
    // Input values:
    //     BasePx (aka bestBuyPx)        = 201.05
    //     PxTickRange (aka pxTickRange) = 10
    //     PxTickSize (aka pxTickSize)   = 0.001
    //     RandomValue (aka randomValue) = 10
    // Steps:
    //     1. GeometricSum = 13 (as static_cast<int>(13.206787162326272))
    //     2. RandomMin = 0, RandomMax = 12 (as GeometricSum - 1)
    //     3. logBase = 1.05, logNum = 1.4761904761904767
    //        (as (RandomValue * (1.05 - 1)) / 1.05 + 1)
    //     4. PxDeviation = 8 (as ceil(log(logBase) / log(1.05))
    //     5. RandomTick = 2 (as PxTickRange - PxDeviation)
    //     6. PxIncrement = 0.002 (as RandomTick * PxTickSize)
    // Output:
    //      GeneratedPx = 201.048 (as BasePx - PxIncrement)

    constexpr double bestBuyPx = 201.05;
    constexpr double bestSellPx = 202.15;
    constexpr std::uint32_t pxTickRange = 10;
    constexpr double pxTickSize = 0.001;
    constexpr double pxSpread = 0.1; // Not used for aggressive orders
    constexpr int randomValue = 10;

    constexpr int expectedMaxRand = 12;
    constexpr int expectedMinRand = 0;
    constexpr double expectedPx = 201.048;

    MarketState const mktState = makeMktState(bestBuyPx, bestSellPx);
    DataLayer::PriceSeed const pxSeed = makePxSeed();
    Random::Event const event = makeEvent(Random::Event::Type::AggressiveSell);
    Random::PriceGenerationParams const pxGenParams = makeGenerationParams(
            pxTickRange
        ,   pxTickSize
        ,   pxSpread
    );

    EXPECT_CALL(valueGenerator(), generateUniform(
        Matcher<std::int64_t>(Eq(expectedMinRand)),
        Matcher<std::int64_t>(Eq(expectedMaxRand))))
        .Times(1)
        .WillOnce(Return(randomValue));

    double const price = generate(pxGenParams, mktState, pxSeed, event);
    EXPECT_DOUBLE_EQ(price, expectedPx);
}

}  // namespace
}  // namespace Simulator::Generator::Random