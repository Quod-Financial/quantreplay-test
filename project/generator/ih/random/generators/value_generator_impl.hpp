#ifndef SIMULATOR_GENERATOR_SRC_GENERATORS_RANDOM_VALUE_GENERATOR_HPP_
#define SIMULATOR_GENERATOR_SRC_GENERATORS_RANDOM_VALUE_GENERATOR_HPP_

#include <memory>
#include <random>

#include "ih/random/generators/value_generator.hpp"

namespace Simulator::Generator::Random {

class ValueGeneratorImpl final
    :   public Random::ValueGenerator
{
public:

    ValueGeneratorImpl();

    static std::shared_ptr<ValueGeneratorImpl> create();

private:

    RandomInt generateUniform(RandomInt _min, RandomInt _max) override;

    RandomUnsignedInt generateUniform(
            RandomUnsignedInt _min
        ,   RandomUnsignedInt _max
    ) override;

    RandomFloat generateUniform(RandomFloat _min, RandomFloat _max) override;


    std::mt19937 m_engine;
};

} // namespace Simulator::Generator::Random

#endif // SIMULATOR_GENERATOR_SRC_GENERATORS_RANDOM_VALUE_GENERATOR_HPP_
