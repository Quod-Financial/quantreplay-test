#ifndef SIMULATOR_GENERATOR_SRC_GENERATORS_RANDOM_EVENT_GENERATOR_HPP_
#define SIMULATOR_GENERATOR_SRC_GENERATORS_RANDOM_EVENT_GENERATOR_HPP_

#include <functional>
#include <memory>
#include <string_view>
#include <utility>

#include "ih/random/generators/value_generator.hpp"
#include "ih/random/values/event.hpp"
#include "ih/tracing/null_tracer.hpp"
#include "ih/tracing/tracing.hpp"

namespace Simulator::Generator::Random {

class EventGenerator
{
public:

    virtual ~EventGenerator() = default;

    template<typename GenerationTracer = Trace::NullTracer>
    Event generateEvent(GenerationTracer && _tracer = GenerationTracer {});

private:

    virtual Event::RandomInteger generateChoiceInteger() = 0;
};


template<typename GenerationTracer>
Event EventGenerator::generateEvent(GenerationTracer && _tracer)
{
    using namespace Trace;

    auto stepTracer = makeStep(_tracer, "generating random event");

    auto const randomInt = generateChoiceInteger();
    traceInput(stepTracer, makeValue("randomValue", randomInt));

    Event event { randomInt };
    traceOutput(stepTracer, makeValue("generatedEvent", event.toString()));

    traceStep(std::move(stepTracer), _tracer);
    return event;
}

} // namespace Simulator::Generator::Random


namespace Simulator::Generator::Random {

class EventGeneratorImpl final
    :   public Random::EventGenerator
{
public:

    explicit EventGeneratorImpl(
        std::shared_ptr<Random::ValueGenerator> _pRandomIntGenerator
    ) noexcept;

    static std::unique_ptr<EventGeneratorImpl> create(
        std::shared_ptr<Random::ValueGenerator> _pRandomIntGenerator
    );

private:

    Event::RandomInteger generateChoiceInteger() override;


    std::shared_ptr<Random::ValueGenerator> m_pRandomIntGenerator;
};

} // namespace Simulator::Generator::Random

#endif // SIMULATOR_GENERATOR_SRC_GENERATORS_RANDOM_EVENT_GENERATOR_HPP_
