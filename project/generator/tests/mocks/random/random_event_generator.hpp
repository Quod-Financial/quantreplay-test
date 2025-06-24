#ifndef SIMULATOR_GENERATOR_TESTS_MOCKS_RANDOM_EVENT_GENERATOR_HPP_
#define SIMULATOR_GENERATOR_TESTS_MOCKS_RANDOM_EVENT_GENERATOR_HPP_

#include <gmock/gmock.h>

#include "ih/random/generators/event_generator.hpp"
#include "ih/random/values/event.hpp"

namespace Simulator::Generator::Mock {

class EventGenerator
    :   public Generator::Random::EventGenerator
{
public:

    MOCK_METHOD(Random::Event::RandomInteger, generateChoiceInteger, (), (override));
};

} // namespace Simulator::Generator::Mock

#endif// SIMULATOR_GENERATOR_TESTS_MOCKS_MESSAGE_CHOICE_GENERATOR_HPP_
