#ifndef SIMULATOR_GENERATOR_SRC_ALGORITHM_UTILS_ATTRIBUTES_SETTER_HPP_
#define SIMULATOR_GENERATOR_SRC_ALGORITHM_UTILS_ATTRIBUTES_SETTER_HPP_

#include <fmt/format.h>

#include "ih/adaptation/generated_message.hpp"
#include "ih/tracing/null_tracer.hpp"
#include "ih/tracing/tracing.hpp"

namespace Simulator::Generator::Random {

class AttributesSetter
{
public:

    template<typename Tracer = Trace::NullTracer>
    static void set(
            GeneratedMessage & _msg
        ,   MessageType _messageType
        ,   Tracer && _tracer = Tracer {}
    );

    template<typename Tracer = Trace::NullTracer>
    static void set(
            GeneratedMessage & _msg
        ,   simulator::OrderType _orderType
        ,   Tracer && _tracer = Tracer {}
    );

    template<typename Tracer = Trace::NullTracer>
    static void set(
            GeneratedMessage & _msg
        ,   simulator::Side _orderSide
        ,   Tracer && _tracer = Tracer {}
    );

    template<typename Tracer = Trace::NullTracer>
    static void set(
            GeneratedMessage & _msg
        ,   simulator::TimeInForce _timeInForce
        ,   Tracer && _tracer = Tracer {}
    );
};


template<typename Tracer>
inline void AttributesSetter::set(
        GeneratedMessage & _msg
    ,   MessageType _messageType
    ,   Tracer && _tracer
)
{
    using namespace Trace;
    _msg.message_type = _messageType;

    auto step = makeStep(_tracer, "specifying MessageType");
    traceOutput(step, makeValue(
            "message_type"
        ,   fmt::format("{}", _msg.message_type)
    ));
    traceStep(std::move(step), _tracer);
}


template<typename Tracer>
inline void AttributesSetter::set(
        GeneratedMessage & _msg
    ,   simulator::OrderType _orderType
    ,   Tracer && _tracer
)
{
    using namespace Trace;
    _msg.order_type = _orderType;

    auto step = makeStep(_tracer, "specifying OrderType");
    traceOutput(step, makeValue("order_type", fmt::format("{}", _msg.order_type)));
    traceStep(std::move(step), _tracer);
}


template<typename Tracer>
void AttributesSetter::set(
        GeneratedMessage & _msg
    ,   simulator::Side _orderSide
    ,   Tracer && _tracer
)
{
    using namespace Trace;
    _msg.side = _orderSide;

    auto step = makeStep(_tracer, "specifying Side");
    traceOutput(step, makeValue("side", fmt::format("{}", _msg.side)));
    traceStep(std::move(step), _tracer);
}


template<typename Tracer>
inline void AttributesSetter::set(
        GeneratedMessage & _msg
    ,   simulator::TimeInForce _timeInForce
    ,   Tracer && _tracer
)
{
    using namespace Trace;
    _msg.time_in_force = _timeInForce;

    auto step = makeStep(_tracer, "specifying TimeInForce");
    traceOutput(step, makeValue(
            "time_in_force"
        ,   fmt::format("{}", _msg.time_in_force)
    ));
    traceStep(std::move(step), _tracer);
}

} // namespace Simulator::Generator::Random

#endif // SIMULATOR_GENERATOR_SRC_UTILS_GENERATED_ATTRIBUTES_SETTERS_HPP_
