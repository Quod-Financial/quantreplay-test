#include <memory>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ih/random/generators/resting_order_action_generator.hpp"
#include "ih/random/values/resting_order_action.hpp"
#include "mocks/random/value_generator.hpp"

using namespace Simulator;
using namespace Simulator::Generator;
using namespace Simulator::Generator::Random;

using ::testing::A;
using ::testing::Eq;
using ::testing::Matcher;
using ::testing::Return;

class Generator_Random_RestingOrderActionGenerator
    :   public testing::Test
{
public:

    using Action = Random::RestingOrderAction::Type;

    static constexpr auto toInteger(Random::RestingOrderAction::Type _action)
        -> std::uint64_t
    {
        return static_cast<std::uint64_t>(_action);
    }

    auto valueGeneratorPtr() -> std::shared_ptr<Mock::ValueGenerator>
    {
        return m_pValueGenerator;
    }

    auto valueGenerator() -> Mock::ValueGenerator &
    {
        return *m_pValueGenerator;
    }

protected:

    void SetUp() override
    {
        m_pValueGenerator = std::make_shared<Mock::ValueGenerator>();
    }

private:

    std::shared_ptr<Mock::ValueGenerator> m_pValueGenerator;
};

TEST_F(Generator_Random_RestingOrderActionGenerator, CheckRequestedLimit)
{
    constexpr std::uint64_t expectedMin = 0;
    constexpr std::uint64_t expectedMax = 19;

    auto pGenerator = Random::RestingOrderActionGeneratorImpl::create(
        valueGeneratorPtr()
    );

    EXPECT_CALL(valueGenerator(), generateUniform(
        Matcher<std::uint64_t>(Eq(expectedMin)),
        Matcher<std::uint64_t>(Eq(expectedMax))))
        .Times(1)
        .WillOnce(Return(0));

    [[maybe_unused]] auto const event = pGenerator->generateAction();
}

TEST_F(Generator_Random_RestingOrderActionGenerator, Generate_PriceModification)
{
    constexpr auto random = toInteger(Action::PriceModification);

    auto pGenerator = Random::RestingOrderActionGeneratorImpl::create(
        valueGeneratorPtr()
    );

    EXPECT_CALL(valueGenerator(), generateUniform(
        A<std::uint64_t>(), A<std::uint64_t>()))
        .Times(1)
        .WillOnce(Return(random));

    Random::RestingOrderAction const event = pGenerator->generateAction();
    EXPECT_EQ(event.getActionType(), Action::PriceModification);
}

TEST_F(Generator_Random_RestingOrderActionGenerator, Generate_QuantityModification)
{
    constexpr auto random = toInteger(Action::QuantityModification);

    auto pGenerator = Random::RestingOrderActionGeneratorImpl::create(
        valueGeneratorPtr()
    );

    EXPECT_CALL(valueGenerator(), generateUniform(
        A<std::uint64_t>(), A<std::uint64_t>()))
        .Times(1)
        .WillOnce(Return(random));

    Random::RestingOrderAction const event = pGenerator->generateAction();
    EXPECT_EQ(event.getActionType(), Action::QuantityModification);
}

TEST_F(Generator_Random_RestingOrderActionGenerator, Generate_Cancellation)
{
    constexpr auto random = toInteger(Action::Cancellation);

    auto pGenerator = Random::RestingOrderActionGeneratorImpl::create(
        valueGeneratorPtr()
    );

    EXPECT_CALL(valueGenerator(), generateUniform(
        A<std::uint64_t>(), A<std::uint64_t>()))
        .Times(1)
        .WillOnce(Return(random));

    Random::RestingOrderAction const event = pGenerator->generateAction();
    EXPECT_EQ(event.getActionType(), Action::Cancellation);
}
