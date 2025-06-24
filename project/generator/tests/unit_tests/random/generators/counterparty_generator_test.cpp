#include <memory>
#include <string_view>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ih/random/generators/counterparty_generator.hpp"
#include "mocks/random/value_generator.hpp"

namespace Simulator::Generator::Random {
namespace {

using ::testing::A;
using ::testing::Eq;
using ::testing::Matcher;
using ::testing::Return;

class Generator_Random_CounterpartyGenerator
    :   public testing::Test
{
public:

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

TEST_F(Generator_Random_CounterpartyGenerator, CheckRequestedLimit)
{
    using RandomInt = std::uint64_t;

    constexpr RandomInt minCounterpartyNumber = 1;
    constexpr RandomInt maxCounterparties = 121;

    auto pGenerator = Random::CounterpartyGeneratorImpl::create(
            maxCounterparties
        ,   valueGeneratorPtr()
    );

    EXPECT_CALL(valueGenerator(), generateUniform(
        Matcher<RandomInt>(Eq(minCounterpartyNumber)),
        Matcher<RandomInt>(Eq(maxCounterparties))))
        .Times(1)
        .WillOnce(Return(1));

    [[maybe_unused]] auto const party = pGenerator->generateCounterparty();
}

TEST_F(Generator_Random_CounterpartyGenerator, GenerateParty)
{
    using RandomInt = std::uint64_t;

    constexpr RandomInt maxCounterparties = 121;
    constexpr RandomInt randomInteger = 21;
    const simulator::PartyId expectedPartyID{"CP21"};

    auto pGenerator = Random::CounterpartyGeneratorImpl::create(
            maxCounterparties
        ,   valueGeneratorPtr()
    );

    EXPECT_CALL(valueGenerator(), generateUniform(A<RandomInt>(), A<RandomInt>()))
        .Times(1)
        .WillOnce(Return(randomInteger));

    const auto party_id = pGenerator->generateCounterparty();
    EXPECT_EQ(party_id, expectedPartyID);
}

}  // namespace
}  // namespace Simulator::Generator::Random