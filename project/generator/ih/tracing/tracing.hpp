#ifndef SIMULATOR_GENERATOR_IH_TRACING_TRACING_HPP_
#define SIMULATOR_GENERATOR_IH_TRACING_TRACING_HPP_

#include <type_traits>
#include <utility>
#include <variant>

#include "cfg/api/cfg.hpp"

#include "ih/tracing/json_tracer.hpp"
#include "ih/tracing/null_tracer.hpp"
#include "ih/tracing/trace_logger.hpp"
#include "ih/tracing/trace_value.hpp"

namespace Simulator::Generator::Trace {

namespace Detail {

template<typename Algorithm>
class TracingWrapperBase
{
public:

    virtual ~TracingWrapperBase() = default;

    TracingWrapperBase() = delete;

    explicit TracingWrapperBase(Algorithm && _algorithm)
        :   m_wrappedAlgorithm { std::forward<Algorithm>(_algorithm) }
    {}

    Algorithm & algorithm() { return m_wrappedAlgorithm; }

private:

    Algorithm m_wrappedAlgorithm;
};


template<typename Algorithm, bool AlgorithmReturnsValue>
class TracingWrapper;


template<typename Algorithm>
class TracingWrapper<Algorithm, true>
    :   TracingWrapperBase<Algorithm>
{
    using Base = TracingWrapperBase<Algorithm>;

public:

    TracingWrapper() = delete;

    explicit TracingWrapper(Algorithm && _algorithm)
        :   Base(std::forward<Algorithm>(_algorithm))
    {}

    template<typename Tracer>
    auto operator() (Tracer & _tracer)
    {
        auto result = Base::algorithm()(_tracer);
        Trace::TraceLogger::log(std::move(_tracer));

        return result;
    }
};


template<typename Algorithm>
class TracingWrapper<Algorithm, false>
    :   TracingWrapperBase<Algorithm>
{
    using Base = TracingWrapperBase<Algorithm>;

public:

    TracingWrapper() = delete;

    explicit TracingWrapper(Algorithm && _algorithm)
        :   Base(std::forward<Algorithm>(_algorithm))
    {}

    template<typename Tracer>
    void operator() (Tracer & _tracer)
    {
        Base::algorithm()(_tracer);
        Trace::TraceLogger::log(std::move(_tracer));
    }
};

} // namespace Detail


template<typename Algorithm>
[[maybe_unused]]
auto trace(Algorithm && _algorithm)
{
    using DefaultTracer = NullTracer;
    using ReturnType = std::invoke_result_t<
            Algorithm
        ,   std::add_lvalue_reference_t<DefaultTracer>
    >;

    constexpr bool isNoreturn = std::is_void_v<ReturnType>;
    constexpr bool isReturning = !isNoreturn && !std::is_reference_v<ReturnType>;
    static_assert(
        isNoreturn ^ isReturning,
        "Unsupported 'Algorithm' type. An instance of an 'Algorithm' type "
        "has to have an 'operator()' defined with an single 'Tracer &' "
        "argument (where 'Tracer' is a template parameter) and must return "
        "a non-reference type or must not return anything."
    );

    std::variant<DefaultTracer, JsonTracer> tracer { DefaultTracer {} };
    if (Cfg::generator().enableTracing)
    {
        tracer = JsonTracer {};
    }

    using TracingWrapper = Detail::TracingWrapper<Algorithm, isReturning>;
    return std::visit(
            TracingWrapper { std::forward<Algorithm>(_algorithm) }
        ,   tracer
    );
}


[[maybe_unused]]
inline auto makeStep(
        NullTracer /*_tracer*/
    ,   std::string_view /*_action*/
) noexcept
{
    return NullTracer::makeStep();
}


[[maybe_unused]] inline auto
makeStep(JsonTracer & _tracer, std::string_view _action)
{
    return _tracer.makeStep(_action);
}


[[maybe_unused]] inline void
traceStep(NullTracer::Step /*_step*/, NullTracer /*_tracer*/) noexcept
{}


[[maybe_unused]] inline void
traceStep(JsonTracer::Step &&_step, JsonTracer &_tracer)
{
    _tracer.trace(std::move(_step));
}


template<typename T>
[[maybe_unused]] inline void
traceInput(NullTracer::Step /*_step*/, TraceValue<T> const & /*_value*/)
{}


template<typename T>
[[maybe_unused]] inline void
traceInput(JsonTracer::Step & _step, TraceValue<T> const & _value)
{
    _step.traceInput(_value);
}


template<typename T>
[[maybe_unused]] inline void
traceOutput(NullTracer::Step /*_step*/, TraceValue<T> const & /*_value*/)
{}


template<typename T>
[[maybe_unused]] inline void
traceOutput(JsonTracer::Step & _step, TraceValue<T> const & _value)
{
    _step.traceOutput(_value);
}


template<typename T>
[[maybe_unused]] auto
makeValue(std::string_view _valueName, T && _value)
{
    return TraceValue<T> { _valueName, std::forward<T>(_value) };
}


template<typename T>
[[maybe_unused]]
auto makeCommentedValue(
    std::string_view _valueName
    ,   T && _value
    ,   std::string_view _comment
)
{
    auto value = makeValue(_valueName, std::forward<T>(_value));
    value.setComment(_comment);
    return value;
}

} // namespace Simulator::GeneratorImpl::Trace

#endif // SIMULATOR_GENERATOR_IH_TRACING_TRACING_HPP_
