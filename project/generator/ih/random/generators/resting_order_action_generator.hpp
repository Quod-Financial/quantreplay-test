#ifndef SIMULATOR_GENERATOR_SRC_GENERATORS_RESTING_ORDER_ACTION_GENERATOR_HPP_
#define SIMULATOR_GENERATOR_SRC_GENERATORS_RESTING_ORDER_ACTION_GENERATOR_HPP_

#include <utility>

#include "ih/random/generators/value_generator.hpp"
#include "ih/random/values/resting_order_action.hpp"
#include "ih/tracing/tracing.hpp"

namespace Simulator::Generator::Random {

class RestingOrderActionGenerator
{
public:

    virtual ~RestingOrderActionGenerator() = default;

    template<typename Tracer = Trace::NullTracer>
    RestingOrderAction generateAction(Tracer && _tracer = Tracer {});

private:

    virtual RestingOrderAction::RandomInteger generateInteger() = 0;
};


template<typename Tracer>
RestingOrderAction RestingOrderActionGenerator::generateAction(Tracer && _tracer)
{
    using namespace Trace;
    auto step = makeStep(_tracer, "generating random resting order action");

    auto const randomInteger = generateInteger();
    traceInput(step, makeValue("randomValue", randomInteger));

    RestingOrderAction action { randomInteger };
    traceOutput(step, makeValue("restingOrderAction", action.toString()));

    traceStep(std::move(step), _tracer);
    return action;
}

} // namespace Simulator::Generator::Random


namespace Simulator::Generator::Random {

class RestingOrderActionGeneratorImpl final
    :   public Random::RestingOrderActionGenerator
{
public:

    explicit RestingOrderActionGeneratorImpl(
        std::shared_ptr<Random::ValueGenerator> _pRandomIntGenerator
    ) noexcept;

    static std::unique_ptr<RestingOrderActionGeneratorImpl> create(
        std::shared_ptr<Random::ValueGenerator> _pRandomIntGenerator
    );

private:

    RestingOrderAction::RandomInteger generateInteger() override;


    std::shared_ptr<Random::ValueGenerator> m_pRandomIntGenerator;
};

} // namespace Simulator::Generator

#endif // SIMULATOR_GENERATOR_SRC_GENERATORS_RESTING_ORDER_ACTION_GENERATOR_HPP_
