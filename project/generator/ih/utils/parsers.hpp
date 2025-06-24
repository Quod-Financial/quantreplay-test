#ifndef SIMULATOR_PROJECT_GENERATOR_SRC_UTILS_PARSERS_HPP_
#define SIMULATOR_PROJECT_GENERATOR_SRC_UTILS_PARSERS_HPP_

#include <chrono>
#include <string_view>
#include <sstream>

namespace Simulator::Generator::Parsing {

// YYYY-mm-dd HH:MM:SS.MS
constexpr std::string_view DefaultTimestampFormat { "%F %T" };


bool parse(
        std::stringstream & _buffer
    ,   std::chrono::system_clock::time_point & _dest
    ,   std::string_view _format = DefaultTimestampFormat
);


bool parse(
        std::string_view _timestamp
    ,   std::chrono::system_clock::time_point & _dest
    ,   std::stringstream & _buffer
    ,   std::string_view _format = DefaultTimestampFormat
);


bool parse(
        std::string_view _timestamp
    ,   std::chrono::system_clock::time_point & _dest
    ,   std::string_view _format = DefaultTimestampFormat
);

} // namespace Simulator::Generator::Parsing

#endif // SIMULATOR_PROJECT_GENERATOR_SRC_UTILS_PARSERS_HPP_
