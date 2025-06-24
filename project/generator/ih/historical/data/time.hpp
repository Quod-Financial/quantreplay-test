#ifndef SIMULATOR_GENERATOR_IH_HISTORICAL_DATA_TIME_HPP_
#define SIMULATOR_GENERATOR_IH_HISTORICAL_DATA_TIME_HPP_

#include <chrono>
#include <sstream>
#include <string>
#include <string_view>

#include <date/date.h>

namespace Simulator::Generator::Historical {

using Timepoint = std::chrono::system_clock::time_point;
using Duration = std::chrono::milliseconds;

namespace Time {

constexpr std::string_view DefaultFormat{"%F %T"};

constexpr inline auto makeOffset(
    Historical::Timepoint _historicalTime,
    Historical::Timepoint _currentTime
) -> Historical::Duration
{
    return std::chrono::duration_cast<Historical::Duration>(
        _currentTime - _historicalTime
    );
}

inline auto makeOffset(Historical::Timepoint _historicalTime) noexcept
    -> Historical::Duration
{
    auto const now = std::chrono::system_clock::now();
    return makeOffset(_historicalTime, now);
}

inline auto from(std::stringstream& _stream, Historical::Timepoint& _dest)
    -> bool
{
    if (_stream.bad() || _stream.fail()) {
        return false;
    }

    _stream >> date::parse(Time::DefaultFormat.data(), _dest);

    bool const converted = !_stream.fail();
    if (converted) {
        _dest = std::chrono::time_point_cast<Historical::Duration>(_dest);
    }

    return converted;
}

inline auto from(std::string_view _stringView, Historical::Timepoint& _dest)
    -> bool
{
    std::stringstream stream{};
    stream << _stringView;
    return from(stream, _dest);
}

inline auto from(std::string const& _string, Historical::Timepoint& _dest)
    -> bool
{
    return from(std::string_view{_string}, _dest);
}

inline auto toString(Historical::Timepoint _timepoint, std::string& _dest)
    -> bool
{
    std::stringstream stream{};
    try {
        stream << date::format(Time::DefaultFormat.data(), _timepoint);
    } catch (...) {
        return false;
    }

    _dest = stream.str();
    return true;
}

} // namespace Time

} // namespace Simulator::Generator::Historical

#endif // SIMULATOR_GENERATOR_IH_HISTORICAL_DATA_TIME_HPP_
