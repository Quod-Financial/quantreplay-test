#ifndef SIMULATOR_GENERATOR_TESTS_MOCKS_COUNTERPARTY_GENERATOR_HPP_
#define SIMULATOR_GENERATOR_TESTS_MOCKS_COUNTERPARTY_GENERATOR_HPP_

#include <memory>

#include <gmock/gmock.h>

#include "ih/random/generators/counterparty_generator.hpp"

namespace Simulator::Generator::Mock {

class CounterpartyGenerator
    :   public Generator::Random::CounterpartyGenerator
{
public:

    MOCK_METHOD(unsigned int, generateCounterpartyNumber, (), (override));
};

} // namespace Simulator::Generator::Mock

#endif // SIMULATOR_GENERATOR_TESTS_MOCKS_COUNTERPARTY_GENERATOR_HPP_
