#ifndef SIMULATOR_GENERATOR_TESTS_MOCKS_VALUE_GENERATOR_HPP_
#define SIMULATOR_GENERATOR_TESTS_MOCKS_VALUE_GENERATOR_HPP_

#include <gmock/gmock.h>

#include "ih/random/generators/value_generator.hpp"

namespace Simulator::Generator::Mock {

class ValueGenerator
    :   public Generator::Random::ValueGenerator
{
public:

    MOCK_METHOD(RandomInt, generateUniform, (RandomInt, RandomInt), (override));

    MOCK_METHOD(RandomUnsignedInt, generateUniform, (RandomUnsignedInt, RandomUnsignedInt), (override));

    MOCK_METHOD(RandomFloat, generateUniform, (RandomFloat, RandomFloat), (override));
};

} // namespace Simulator::Generator::Mock

#endif // SIMULATOR_GENERATOR_TESTS_MOCKS_VALUE_GENERATOR_HPP_
