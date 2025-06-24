#ifndef SIMULATOR_GENERATOR_IH_TRACING_TRACE_LOGGER_HPP_
#define SIMULATOR_GENERATOR_IH_TRACING_TRACE_LOGGER_HPP_

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <functional>
#include <type_traits>

#include "ih/tracing/json_tracer.hpp"
#include "ih/tracing/null_tracer.hpp"
#include "log/logging.hpp"

namespace Simulator::Generator::Trace {

template<typename Tracer>
class FormattingHelper
{
    using TracerType = std::decay_t<Tracer>;

public:

    explicit FormattingHelper(Tracer & _tracer)
        :   m_tracerReference { _tracer }
    {}

    [[nodiscard]] std::string getTraceString() const
    {
        return m_tracerReference.get().composeTrace();
    }

private:

    mutable std::reference_wrapper<TracerType> m_tracerReference;
};

} // Simulator::Generator::Trace


namespace fmt {

template<typename Tracer>
class [[maybe_unused]]
formatter<Simulator::Generator::Trace::FormattingHelper<Tracer>>
{
    using Type = Simulator::Generator::Trace::FormattingHelper<Tracer>;

public:

    static constexpr auto parse(format_parse_context & _ctx)
    {
        return _ctx.begin();
    }

    template<typename FormattingContext>
    auto format(Type const & _formattingHelper, FormattingContext && _ctx) const
    {
        return format_to(_ctx.out(), "{}", _formattingHelper.getTraceString());
    }
};

} // namespace fmt


namespace Simulator::Generator::Trace {


class [[maybe_unused]] TraceLogger
{
public:

    static void log(NullTracer && /*_tracer*/) {}

    static void log(JsonTracer && _tracer)
    {
        simulator::log::debug("{}", FormattingHelper<JsonTracer> { _tracer });
    }
};


class [[maybe_unused]] TraceFormatter
{
public:

    static std::string format(NullTracer && /*_tracer*/) { return ""; }

    static std::string format(JsonTracer && _tracer)
    {
        return fmt::format("{}", FormattingHelper<JsonTracer> { _tracer });
    }
};

} // namespace Simulator::Generator::Trace

#endif // SIMULATOR_GENERATOR_IH_TRACING_TRACE_LOGGER_HPP_
