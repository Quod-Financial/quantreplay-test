#include <gtest/gtest.h>

#include "ih/tracing/tracing.hpp"

using namespace Simulator;
using namespace Simulator::Generator;
using namespace Simulator::Generator::Trace;

TEST(Generator_Trace_JsonTracer, Trace_NoSteps)
{
    constexpr std::string_view expectedTrace{ R"({"algorithmTrace":[]})" };

    JsonTracer tracer {};
    std::string const actualTrace = TraceFormatter::format(std::move(tracer));

    EXPECT_EQ(actualTrace, expectedTrace);
}

TEST(Generator_Trace_JsonTracer, Trace_SingleStep)
{
    constexpr std::string_view expectedTrace {
        R"({"algorithmTrace":[)"
            R"({"step":1,"action":"firstStep","input":[],"output":[]})"
        "]}"
    };

    JsonTracer tracer {};
    auto step = makeStep(tracer, "firstStep");
    tracer.trace(std::move(step));
    std::string const actualTrace = TraceFormatter::format(std::move(tracer));

    EXPECT_EQ(actualTrace, expectedTrace);
}

TEST(Generator_Trace_JsonTracer, Trace_MultipleSteps)
{
    constexpr std::string_view expectedTrace {
        R"({"algorithmTrace":[)"
            R"({"step":1,"action":"firstStep","input":[],"output":[]},)"
            R"({"step":2,"action":"secondStep","input":[],"output":[]},)"
            R"({"step":3,"action":"thirdStep","input":[],"output":[]})"
        "]}"
    };

    JsonTracer tracer {};

    auto step = makeStep(tracer, "firstStep");
    tracer.trace(std::move(step));

    step = makeStep(tracer, "secondStep");
    tracer.trace(std::move(step));

    step = makeStep(tracer, "thirdStep");
    tracer.trace(std::move(step));

    std::string const actualTrace = TraceFormatter::format(std::move(tracer));

    EXPECT_EQ(actualTrace, expectedTrace);
}

TEST(Generator_Trace_JsonTracer, Trace_Step_SingeInput)
{
    constexpr int value = 12;

    constexpr std::string_view expectedTrace {
        R"({"algorithmTrace":[)"
            R"({"step":1,"action":"firstStep",)"
            R"("input":[)"
                R"({"someValue":12,"comment":"a comment"})"
            "],"
            R"("output":[]})"
        "]}"
    };

    JsonTracer tracer {};
    auto step = makeStep(tracer, "firstStep");
    step.traceInput(makeCommentedValue("someValue", value, "a comment"));
    tracer.trace(std::move(step));

    std::string const actualTrace = TraceFormatter::format(std::move(tracer));

    EXPECT_EQ(actualTrace, expectedTrace);
}

TEST(Generator_Trace_JsonTracer, Trace_Step_MultipleInputs)
{
    constexpr double doubleValue = 12.9;

    constexpr std::string_view expectedTrace {
        R"({"algorithmTrace":[)"
            R"({"step":1,"action":"firstStep",)"
                R"("input":[)"
                    R"({"doubleValue":12.9,"comment":"a comment"},)"
                    R"({"booleanValue":true,"comment":"a comment to boolean"},)"
                    R"({"stringViewValue":"string_view"})"
                "],"
                R"("output":[])"
            "}"
        "]}"
    };

    JsonTracer tracer {};
    auto step = makeStep(tracer, "firstStep");
    step.traceInput(makeCommentedValue(
            "doubleValue"
        ,   doubleValue
        ,   "a comment"
    ));
    step.traceInput(makeCommentedValue(
            "booleanValue"
        ,   true
        ,   "a comment to boolean"
    ));
    step.traceInput(makeValue(
            "stringViewValue"
        ,   std::string_view { "string_view" }
    ));

    tracer.trace(std::move(step));

    std::string const actualTrace = TraceFormatter::format(std::move(tracer));
    EXPECT_EQ(actualTrace, expectedTrace);
}

TEST(Generator_Trace_JsonTracer, Trace_Step_SingeOutput)
{
    constexpr std::string_view expectedTrace {
        R"({"algorithmTrace":[)"
            R"({"step":1,"action":"firstStep",)"
                R"("input":[],)"
                R"("output":[)"
                    R"({"stringValue":"string","comment":"a comment to string"})"
                "]"
            "}"
        "]}"
    };

    JsonTracer tracer {};
    auto step = makeStep(tracer, "firstStep");
    std::string const value = "string";
    step.traceOutput(makeCommentedValue(
            "stringValue"
        ,   value
        ,   "a comment to string"
    ));
    tracer.trace(std::move(step));

    std::string const actualTrace = TraceFormatter::format(std::move(tracer));
    EXPECT_EQ(actualTrace, expectedTrace);
}

TEST(Generator_Trace_JsonTracer, Trace_Step_MultipleOutputs)
{
    constexpr short shortValue = 2;
    std::string const stringValue = "string";

    constexpr std::string_view expectedTrace {
        R"({"algorithmTrace":[)"
            R"({"step":1,"action":"firstStep",)"
                R"("input":[],)"
                R"("output":[)"
                    R"({"stringValue":"string","comment":"a comment to string"},)"
                    R"({"shortValue":2})"
                "]"
            "}"
        "]}"
    };

    JsonTracer tracer {};
    auto step = makeStep(tracer, "firstStep");
    step.traceOutput(makeCommentedValue(
            "stringValue"
        ,   stringValue
        ,   "a comment to string"
    ));
    step.traceOutput(makeValue(
            "shortValue"
        ,   shortValue
    ));
    tracer.trace(std::move(step));

    std::string const actualTrace = TraceFormatter::format(std::move(tracer));
    EXPECT_EQ(actualTrace, expectedTrace);
}
