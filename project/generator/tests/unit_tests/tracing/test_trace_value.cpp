#include <gtest/gtest.h>

#include "ih/tracing/trace_value.hpp"
#include "ih/tracing/tracing.hpp"

using namespace Simulator;
using namespace Simulator::Generator;
using namespace Simulator::Generator::Trace;

TEST(Generator_Trace_TraceValue, ArithmeticType)
{
    constexpr std::string_view key = "DoubleValue";
    constexpr double value = 23.43;

    auto const traceValue = makeValue(key, value);
    static_assert(decltype(traceValue)::holdsArithmeticType());

    EXPECT_EQ(traceValue.getKey(), key);
    EXPECT_EQ(traceValue.getValue(), value);
    EXPECT_FALSE(traceValue.holdsComment());
}

TEST(Generator_Trace_TraceValue, String)
{
    constexpr std::string_view key = "StringValue";
    std::string const value = "An awesome string";

    auto const traceValue = makeValue(key, value);
    static_assert(decltype(traceValue)::holdsString());

    EXPECT_EQ(traceValue.getKey(), key);
    EXPECT_EQ(traceValue.getValue(), value);
    EXPECT_FALSE(traceValue.holdsComment());
}

TEST(Generator_Trace_TraceValue, StringView)
{
    constexpr std::string_view key = "StringViewValue";
    constexpr std::string_view value = "An awesome string view";

    auto const traceValue = makeValue(key, value);
    static_assert(decltype(traceValue)::holdsStringView());

    EXPECT_EQ(traceValue.getKey(), key);
    EXPECT_EQ(traceValue.getValue(), value);
    EXPECT_FALSE(traceValue.holdsComment() );
}

TEST(Generator_Trace_TraceValue, WithComment)
{
    constexpr std::string_view key = "DoubleValue";
    constexpr std::string_view value = "An awesome string view";
    constexpr std::string_view comment = "My cool comment to the value";

    auto const traceValue = makeCommentedValue(key, value, comment);

    ASSERT_TRUE(traceValue.holdsComment());
    EXPECT_EQ(traceValue.getComment(), comment);
}
