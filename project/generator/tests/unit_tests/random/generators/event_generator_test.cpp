#include <memory>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ih/random/generators/event_generator.hpp"
#include "ih/random/values/event.hpp"
#include "mocks/random/value_generator.hpp"

namespace Simulator::Generator::Random {
namespace {

using ::testing::A;
using ::testing::Eq;
using ::testing::Matcher;
using ::testing::Return;

class Generator_Random_EventGenerator
    :   public testing::Test
{
public:

    static constexpr auto toInteger(Random::Event::Type _event) -> std::uint64_t
    {
        return static_cast<std::uint64_t>(_event);
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

TEST_F(Generator_Random_EventGenerator, CheckRequestedLimit)
{
    constexpr std::uint64_t expectedMin = 0;
    constexpr std::uint64_t expectedMax = 29;

    auto pGenerator = Random::EventGeneratorImpl::create(valueGeneratorPtr());
    EXPECT_CALL(valueGenerator(), generateUniform(
        Matcher<std::uint64_t>(Eq(expectedMin)),
        Matcher<std::uint64_t>(Eq(expectedMax))))
        .Times(1)
        .WillOnce(Return(0));

    [[maybe_unused]] auto const event = pGenerator->generateEvent();
}

TEST_F(Generator_Random_EventGenerator, Generate_RestingBuy)
{
    constexpr auto random = toInteger(Random::Event::Type::RestingBuy);

    auto pGenerator = Random::EventGeneratorImpl::create(valueGeneratorPtr());
    EXPECT_CALL(valueGenerator(), generateUniform(
        A<std::uint64_t>(), A<std::uint64_t>()))
        .Times(1)
        .WillOnce(Return(random));

    Random::Event const event = pGenerator->generateEvent();
    EXPECT_EQ(event.getType(), Random::Event::Type::RestingBuy);
}

TEST_F(Generator_Random_EventGenerator, Generate_RestingSell)
{
    constexpr auto random = toInteger(Random::Event::Type::RestingSell);

    auto pGenerator = Random::EventGeneratorImpl::create(valueGeneratorPtr());
    EXPECT_CALL(valueGenerator(),
        generateUniform(A<std::uint64_t>(), A<std::uint64_t>()))
        .Times(1)
        .WillOnce(Return(random));

    Random::Event const event = pGenerator->generateEvent();
    EXPECT_EQ(event.getType(), Random::Event::Type::RestingSell);
}

TEST_F(Generator_Random_EventGenerator, Generate_AggressiveBuy)
{
    constexpr auto random = toInteger(Random::Event::Type::AggressiveBuy);

    auto pGenerator = Random::EventGeneratorImpl::create(valueGeneratorPtr());
    EXPECT_CALL(valueGenerator(),
        generateUniform(A<std::uint64_t>(), A<std::uint64_t>()))
        .Times(1)
        .WillOnce(Return(random));

    Random::Event const event = pGenerator->generateEvent();
    EXPECT_EQ(event.getType(), Random::Event::Type::AggressiveBuy);
}

TEST_F(Generator_Random_EventGenerator, Generate_AggressiveSell)
{
    constexpr auto random = toInteger(Random::Event::Type::AggressiveSell);

    auto pGenerator = Random::EventGeneratorImpl::create(valueGeneratorPtr());
    EXPECT_CALL(valueGenerator(),
        generateUniform(A<std::uint64_t>(), A<std::uint64_t>()))
        .Times(1)
        .WillOnce(Return(random));

    Random::Event const event = pGenerator->generateEvent();
    EXPECT_EQ(event.getType(), Random::Event::Type::AggressiveSell);
}

TEST_F(Generator_Random_EventGenerator, Generate_NoOperation)
{
    constexpr auto random = toInteger(Random::Event::Type::NoOperation);

    auto pGenerator = Random::EventGeneratorImpl::create(valueGeneratorPtr());
    EXPECT_CALL(valueGenerator(),
        generateUniform(A<std::uint64_t>(), A<std::uint64_t>()))
        .Times(1)
        .WillOnce(Return(random));

    Random::Event const event = pGenerator->generateEvent();
    EXPECT_EQ(event.getType(), Random::Event::Type::NoOperation);
}

}  // namespace
}  // namespace Simulator::Generator::Random