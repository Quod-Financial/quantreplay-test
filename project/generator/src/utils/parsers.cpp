#include "ih/utils/parsers.hpp"

#include <string>

#include <date/date.h>

namespace Simulator::Generator::Parsing {

bool parse(
        std::stringstream & _buffer
    ,   std::chrono::system_clock::time_point & _dest
    ,   std::string_view _format
)
{
    _buffer >> date::parse(std::string { _format }, _dest);
    return !_buffer.fail() && !_buffer.bad();
}


bool parse(
        std::string_view _timestamp
    ,   std::chrono::system_clock::time_point & _dest
    ,   std::stringstream & _buffer
    ,   std::string_view _format
)
{
    _buffer.clear();
    _buffer << _timestamp;

    bool const parsed = parse(_buffer, _dest, _format);

    _buffer.clear();
    return parsed;
}


bool parse(
        std::string_view _timestamp
    ,   std::chrono::system_clock::time_point & _dest
    ,   std::string_view _format
)
{
    std::stringstream buffer {};
    return parse(_timestamp, _dest, buffer, _format);
}

} // namespace Simulator::Generator::Parsing
