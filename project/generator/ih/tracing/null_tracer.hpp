#ifndef SIMULATOR_GENERATOR_IH_TRACING_NULL_TRACER_HPP_
#define SIMULATOR_GENERATOR_IH_TRACING_NULL_TRACER_HPP_

namespace Simulator::Generator::Trace {

class NullTracer
{
public:

    class Step {};

    template<typename ... Ts>
    static NullTracer::Step makeStep(Ts && ... /*_args*/) noexcept
    {
        return NullTracer::Step {};
    }
};

} // namespace Simulator::Generator::Trace

#endif // SIMULATOR_GENERATOR_IH_TRACING_NULL_TRACER_HPP_
