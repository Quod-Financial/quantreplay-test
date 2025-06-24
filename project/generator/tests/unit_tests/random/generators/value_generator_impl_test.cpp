#include "gtest/gtest.h"
#include <cmath>
#include <limits>
#include <tuple>
#include <type_traits>

#include <gtest/gtest.h>

#include "ih/random/generators/value_generator_impl.hpp"

using namespace Simulator;
using namespace Simulator::Generator;
using namespace Simulator::Generator::Random;

using SignedTypes = testing::Types<
    std::int8_t, std::int16_t, std::int32_t, std::int64_t
>;

template<typename>
class Generator_Random_ValueGenerator_Signed
    :   public testing::Test
{};

TYPED_TEST_SUITE(Generator_Random_ValueGenerator_Signed, SignedTypes);

TYPED_TEST(Generator_Random_ValueGenerator_Signed, GenerateUniformInterval)
{
    constexpr auto min = static_cast<TypeParam>(-1);
    constexpr auto max = static_cast<TypeParam>(1);

    auto pGenerator = ValueGeneratorImpl::create();
    ASSERT_TRUE(pGenerator);

    auto const random = pGenerator->generateUniformValue(min, max);
    static_assert(std::is_same_v<std::decay_t<decltype(random)>, TypeParam>);

    EXPECT_GE(random, min);
    EXPECT_LE(random, max);
}

TYPED_TEST(Generator_Random_ValueGenerator_Signed, GenerateMin)
{
    constexpr std::int64_t min = std::numeric_limits<std::int64_t>::min();
    constexpr std::int64_t max = min + 1;

    auto pGenerator = ValueGeneratorImpl::create();
    ASSERT_TRUE(pGenerator);

    auto const random = pGenerator->generateUniformValue(min, max);

    EXPECT_TRUE(random == min || random == max);
}

TYPED_TEST(Generator_Random_ValueGenerator_Signed, GenerateMax)
{
    constexpr std::int64_t max = std::numeric_limits<std::int64_t>::max();
    constexpr std::int64_t min = max - 1;

    auto pGenerator = ValueGeneratorImpl::create();
    ASSERT_TRUE(pGenerator);

    auto const random = pGenerator->generateUniformValue(min, max);

    EXPECT_TRUE(random == max || random == min);
}


using UnsignedTypes = testing::Types<
    std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t
>;

template<typename>
class Generator_Random_ValueGenerator_Unsigned
    :   public testing::Test
{};

TYPED_TEST_SUITE(Generator_Random_ValueGenerator_Unsigned, UnsignedTypes);

TYPED_TEST(Generator_Random_ValueGenerator_Unsigned, GenerateUniformInterval)
{
    constexpr auto min = static_cast<TypeParam>(0);
    constexpr auto max = static_cast<TypeParam>(2);

    auto pGenerator = ValueGeneratorImpl::create();
    ASSERT_TRUE(pGenerator);

    auto const random = pGenerator->generateUniformValue(min, max);
    static_assert(std::is_same_v<std::decay_t<decltype(random)>, TypeParam>);

    EXPECT_LE(random, max);
    EXPECT_GE(random, min);
}

TYPED_TEST(Generator_Random_ValueGenerator_Unsigned, GenerateMin)
{
    constexpr std::uint64_t min = std::numeric_limits<std::uint64_t>::min();
    constexpr std::uint64_t max = min + 1;

    auto pGenerator = ValueGeneratorImpl::create();
    ASSERT_TRUE(pGenerator);

    auto const random = pGenerator->generateUniformValue(min, max);

    EXPECT_TRUE(random == min || random == max);
}

TYPED_TEST(Generator_Random_ValueGenerator_Unsigned, GenerateMax)
{
    constexpr std::uint64_t max = std::numeric_limits<std::uint64_t>::max();
    constexpr std::uint64_t min = max - 1;

    auto pGenerator = ValueGeneratorImpl::create();
    ASSERT_TRUE(pGenerator);

    auto const random = pGenerator->generateUniformValue(min, max);

    EXPECT_TRUE(random == min || random == max);
}


using FloatingPointTypes = testing::Types<std::float_t, std::double_t>;

template<typename>
class Generator_Random_ValueGenerator_FloatingPoint
    :   public testing::Test
{};

TYPED_TEST_SUITE(Generator_Random_ValueGenerator_FloatingPoint, FloatingPointTypes);

TYPED_TEST(Generator_Random_ValueGenerator_FloatingPoint, GenerateUniformInterval)
{
    constexpr auto min = static_cast<TypeParam>(-0.001);
    constexpr auto max = static_cast<TypeParam>(0.001);

    auto pGenerator = ValueGeneratorImpl::create();
    ASSERT_TRUE(pGenerator);

    auto const random = pGenerator->generateUniformValue(min, max);
    static_assert(std::is_same_v<std::decay_t<decltype(random)>, TypeParam>);

    EXPECT_LE(random, max);
    EXPECT_GE(random, min);
}
