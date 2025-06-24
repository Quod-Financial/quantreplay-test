#ifndef SIMULATOR_GENERATOR_TESTS_MOCKS_QUANTITY_GENERATOR_HPP_
#define SIMULATOR_GENERATOR_TESTS_MOCKS_QUANTITY_GENERATOR_HPP_

#include <gmock/gmock.h>

#include "ih/random/generators/quantity_generator.hpp"

namespace Simulator::Generator::Mock {

class QuantityGenerator
    :   public Generator::Random::QuantityGenerator
{
public:

    static auto makeOutput(double _qty) -> GenerationOutput
    {
        return GenerationOutput{ _qty, 0 };
    }

    MOCK_METHOD(
        GenerationOutput, generateRandomQty,
        (Random::QuantityGenerationParams const &),
        (override)
    );
};

} // namespace Simulator::Generator::Mock

#endif // SIMULATOR_GENERATOR_TESTS_MOCKS_QUANTITY_GENERATOR_HPP_
