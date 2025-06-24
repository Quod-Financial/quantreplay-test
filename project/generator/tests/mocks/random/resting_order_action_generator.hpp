#ifndef SIMULATOR_GENERATOR_TESTS_MOCKS_RESTING_ORDER_ACTION_GENERATOR_HPP_
#define SIMULATOR_GENERATOR_TESTS_MOCKS_RESTING_ORDER_ACTION_GENERATOR_HPP_

#include <gmock/gmock.h>

#include "ih/random/generators/resting_order_action_generator.hpp"
#include "ih/random/values/resting_order_action.hpp"

namespace Simulator::Generator::Mock {

class RestingOrderActionGenerator
    :   public Generator::Random::RestingOrderActionGenerator
{
public:

    MOCK_METHOD(Random::RestingOrderAction::RandomInteger, generateInteger, (), (override));
};

} // namespace Simulator::Generator::Mock

#endif// SIMULATOR_GENERATOR_TESTS_MOCKS_RESTING_ORDER_ACTION_GENERATOR_HPP_
