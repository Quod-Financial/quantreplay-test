#ifndef SIMULATOR_GENERATOR_IH_TRACING_JSON_TRACER_HPP_
#define SIMULATOR_GENERATOR_IH_TRACING_JSON_TRACER_HPP_

#include <rapidjson/stringbuffer.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>

#include "ih/tracing/trace_value.hpp"

namespace Simulator::Generator::Trace {

class JsonTracer
{
    template<typename T>
    friend class FormattingHelper;

    class TracedAlgorithm
    {
        friend class JsonTracer;

    public:

        TracedAlgorithm();

        decltype(auto) takeAllocator() { return m_trace.GetAllocator(); }

        [[nodiscard]] rapidjson::Document & composeAlgorithmTrace();

        void addStep(rapidjson::Document & _stepDoc);

    private:

        rapidjson::Document m_trace;
        rapidjson::Document m_steps;
    };

public:

    class Step
    {
        friend class JsonTracer;

    public:

        Step() = delete;

        template<typename T>
        void traceInput(TraceValue<T> const & _entry);

        template<typename T>
        void traceOutput(TraceValue<T> const & _entry);

    private:

        template<typename JsonAllocator>
        Step(std::string_view _action, JsonAllocator & _allocator);

        rapidjson::Document & takeTracedInput() noexcept;

        rapidjson::Document & takeTracedOutput() noexcept;

        [[nodiscard]] std::string_view getAction() const noexcept;

        template<typename T>
        static void traceEntryInto(
                rapidjson::Document & _destDoc
            ,   TraceValue<T> const & _entry
        );


        rapidjson::Document m_inputTrace;
        rapidjson::Document m_outputTrace;

        std::string_view m_stepAction;
    };


    JsonTracer();

    Step makeStep(std::string_view _action);

    void trace(Step && _step);

private:

    [[maybe_unused]] std::string composeTrace();

    TracedAlgorithm m_tracedData;
    std::size_t m_nextStep { 0 };
};


inline
JsonTracer::TracedAlgorithm::TracedAlgorithm()
    :   m_steps { std::addressof(m_trace.GetAllocator()) }
{
    m_trace.SetObject();
    m_steps.SetObject().SetArray();
}


inline rapidjson::Document &
JsonTracer::TracedAlgorithm::composeAlgorithmTrace()
{
    assert(m_trace.IsObject());
    m_trace.AddMember("algorithmTrace", m_steps, takeAllocator());
    return m_trace;
}


inline void
JsonTracer::TracedAlgorithm::addStep(rapidjson::Document & _stepDoc)
{
    assert(m_steps.IsArray());
    assert(_stepDoc.IsObject());
    m_steps.PushBack(_stepDoc, takeAllocator());
}


template<typename T>
void JsonTracer::Step::traceInput(TraceValue<T> const & _entry)
{
    traceEntryInto(m_inputTrace, _entry);
}


template<typename T>
void JsonTracer::Step::traceOutput(TraceValue<T> const & _entry)
{
    traceEntryInto(m_outputTrace, _entry);
}


template<typename JsonAllocator>
JsonTracer::Step::Step(std::string_view _action, JsonAllocator & _allocator)
    :   m_inputTrace { std::addressof(_allocator) }
    ,   m_outputTrace { std::addressof(_allocator) }
    ,   m_stepAction { _action }
{
    m_inputTrace.SetObject().SetArray();
    m_outputTrace.SetObject().SetArray();
}


inline rapidjson::Document &
JsonTracer::Step::takeTracedInput() noexcept
{
    return m_inputTrace;
}


inline rapidjson::Document &
JsonTracer::Step::takeTracedOutput() noexcept
{
    return m_outputTrace;
}


inline std::string_view
JsonTracer::Step::getAction() const noexcept
{
    return m_stepAction;
}


template<typename T>
void JsonTracer::Step::traceEntryInto(
        rapidjson::Document & _destDoc
    ,   TraceValue<T> const & _entry
)
{
    using Entry = TraceValue<T>;
    using rapidjson::Value;
    using rapidjson::SizeType;

    auto & allocator = _destDoc.GetAllocator();
    rapidjson::Document entryJson { std::addressof(allocator) };
    entryJson.SetObject();

    std::string_view const key = _entry.getKey();
    assert(!key.empty());
    auto const keySize = static_cast<SizeType>(key.size());
    Value jsonKey { key.data(), keySize, allocator };

    Value jsonValue {};
    if constexpr (Entry::holdsString() || Entry::holdsStringView())
    {
        auto const & value = _entry.getValue();
        auto const valueSize = static_cast<SizeType>(value.size());
        if constexpr (Entry::holdsString())
        {
            jsonValue = Value { value.c_str(), valueSize, allocator };
        }
        else // Entry holds string_view
        {
            jsonValue = Value { value.data(), valueSize, allocator };
        }
    }
    else // Entry holds arithmetic type
    {
        jsonValue = Value { _entry.getValue() };
    }

    entryJson.AddMember(jsonKey, jsonValue, allocator);

    if (_entry.holdsComment())
    {
        std::string_view const comment = _entry.getComment();
        Value jsonComment {
                comment.data()
            ,   static_cast<SizeType>(comment.size())
            ,   allocator
        };
        entryJson.AddMember("comment", jsonComment, allocator);
    }

    assert(_destDoc.IsArray());
    _destDoc.PushBack(entryJson, allocator);
}


inline
JsonTracer::JsonTracer()
    :   m_tracedData { TracedAlgorithm{} }
{}


inline JsonTracer::Step
JsonTracer::makeStep(std::string_view _action)
{
    return JsonTracer::Step { _action, m_tracedData.takeAllocator() };
}


inline void
JsonTracer::trace(JsonTracer::Step && _step)
{
    auto & allocator = m_tracedData.takeAllocator();

    rapidjson::Document stepDoc { std::addressof(allocator) };
    stepDoc.SetObject();

    std::string_view const action = _step.getAction();
    rapidjson::Value actionJson {
            action.data()
        ,   static_cast<rapidjson::SizeType>(action.size())
        ,   allocator
    };

    stepDoc.AddMember("step", rapidjson::Value { ++m_nextStep }, allocator);
    stepDoc.AddMember("action", actionJson, allocator);
    stepDoc.AddMember("input", _step.takeTracedInput(), allocator);
    stepDoc.AddMember("output", _step.takeTracedOutput(), allocator);

    m_tracedData.addStep(stepDoc);
}


[[maybe_unused]] inline std::string
JsonTracer::composeTrace()
{
    auto & jsonTrace = m_tracedData.composeAlgorithmTrace();
    assert(jsonTrace.IsObject());

    rapidjson::StringBuffer buffer {};
    rapidjson::Writer<rapidjson::StringBuffer> writer { buffer };
    jsonTrace.Accept(writer);

    return buffer.GetString();
}

} // namespace Simulator::Generator::Trace

#endif // SIMULATOR_GENERATOR_IH_TRACING_JSON_TRACER_HPP_
