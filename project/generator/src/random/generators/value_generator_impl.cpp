#include "ih/random/generators/value_generator_impl.hpp"

#include <cassert>
#include <memory>
#include <random>

#include "ih/random/generators/value_generator.hpp"

namespace Simulator::Generator::Random {

ValueGeneratorImpl::ValueGeneratorImpl()
    :   m_engine { std::random_device {}() }
{}


std::shared_ptr<ValueGeneratorImpl> ValueGeneratorImpl::create()
{
    return std::make_shared<ValueGeneratorImpl>();
}


ValueGeneratorImpl::RandomInt ValueGeneratorImpl::generateUniform(
        ValueGeneratorImpl::RandomInt _min
    ,   ValueGeneratorImpl::RandomInt _max
)
{
    using Distribution = std::uniform_int_distribution<RandomInt>;

    assert(_min <= _max);
    return Distribution { _min, _max }(m_engine);
}


ValueGeneratorImpl::RandomUnsignedInt
ValueGeneratorImpl::generateUniform(
        RandomUnsignedInt _min
    ,   RandomUnsignedInt _max
)
{
    using Distribution = std::uniform_int_distribution<RandomUnsignedInt>;

    assert(_min <= _max);
    return Distribution { _min, _max }(m_engine);
}


ValueGeneratorImpl::RandomFloat
ValueGeneratorImpl::generateUniform(RandomFloat _min, RandomFloat _max)
{
    using Distribution = std::uniform_real_distribution<RandomFloat>;

    assert(_min <= _max);
    return Distribution { _min, _max }(m_engine);
}

} // namespace Simulator::Generator::Random
