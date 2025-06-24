#ifndef SIMULATOR_GENERATOR_IH_GENERATORS_RANDOM_VALUE_GENERATOR_HPP_
#define SIMULATOR_GENERATOR_IH_GENERATORS_RANDOM_VALUE_GENERATOR_HPP_

#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace Simulator::Generator::Random {

class ValueGenerator
{
protected:

    using RandomInt         = std::int64_t;
    using RandomUnsignedInt = std::uint64_t;
    using RandomFloat       = std::double_t;

private:

    template<typename T>
    constexpr static bool is_signed_integer_v =
        std::is_integral_v<T> && std::is_signed_v<T>;

    template<typename T>
    constexpr static bool is_unsigned_integer_v =
        std::is_integral_v<T> && std::is_unsigned_v<T>;

    template<typename F, typename S>
    constexpr static bool both_are_signed =
        std::is_signed_v<F> && std::is_signed_v<S>;

    template<typename F, typename S>
    constexpr static bool both_are_unsigned =
        std::is_unsigned_v<F> && std::is_unsigned_v<S>;

public:

    virtual ~ValueGenerator() = default;

    template<typename T, std::enable_if_t<is_signed_integer_v<T>> * = nullptr>
    T generateUniformValue(T _min, T _max)
    {
        assert(_min <= _max);
        return getRandomUniformValue<RandomInt, T>(_min, _max);
    }

    template<typename T, std::enable_if_t<is_unsigned_integer_v<T>> * = nullptr>
    T generateUniformValue(T _min, T _max)
    {
        assert(_min <= _max);
        return getRandomUniformValue<RandomUnsignedInt, T>(_min, _max);
    }

    template<
            typename T
        ,   std::enable_if_t<std::is_floating_point_v<T>> * = nullptr
    >
    T generateUniformValue(T _min, T _max)
    {
        assert(_min <= _max);
        return getRandomUniformValue<RandomFloat, T>(_min, _max);
    }

private:

    template<typename SupportedRandomType, typename TargetType>
    TargetType getRandomUniformValue(TargetType _min, TargetType _max)
    {
        static_assert(sizeof(SupportedRandomType) >= sizeof(TargetType));
        static_assert(
            both_are_signed<SupportedRandomType, TargetType> ||
            both_are_unsigned<SupportedRandomType, TargetType>
        );

        auto const generated = generateUniform(
                static_cast<SupportedRandomType>(_min)
            ,   static_cast<SupportedRandomType>(_max)
        );

        return static_cast<TargetType>(generated);
    }

    virtual RandomInt generateUniform(RandomInt _min, RandomInt _max) = 0;

    virtual RandomUnsignedInt generateUniform(
            RandomUnsignedInt _min
        ,   RandomUnsignedInt _max
    ) = 0;

    virtual RandomFloat generateUniform(RandomFloat _min, RandomFloat _max) = 0;
};

} // namespace Simulator::Generator::Random

#endif // SIMULATOR_GENERATOR_IH_GENERATORS_RANDOM_VALUE_GENERATOR_HPP_
