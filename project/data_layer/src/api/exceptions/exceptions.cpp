#include "api/exceptions/exceptions.hpp"

#include <algorithm>
#include <string>
#include <string_view>

#include <fmt/format.h>

namespace Simulator::DataLayer {

ConfigurationError::ConfigurationError(std::string const& _message) noexcept :
    std::runtime_error{_message}
{}

ConnectionFailure::ConnectionFailure(std::string_view _driverMessage) noexcept :
    std::runtime_error{formatMessage(_driverMessage)}
{}

auto ConnectionFailure::formatMessage(std::string_view _error) noexcept
    -> std::string
{
    std::string escaped{_error};
    constexpr char space = ' ';
    std::replace_if(
        escaped.begin(),
        escaped.end(),
        [](auto _ch) { return _ch == '\n' || _ch == '\r'; },
        space
    );
    return fmt::format("database connection error occurred: {}", escaped);
}

DataDecodingError::DataDecodingError(std::string const& _message) noexcept :
    std::runtime_error(_message)
{}

MalformedPatch::MalformedPatch(std::string const& _message) noexcept :
    std::invalid_argument(_message)
{}

CardinalityViolationError::CardinalityViolationError() noexcept :
    std::out_of_range(
        "database operation affected an unexpected number of records"
    )
{}

DataIntegrityError::DataIntegrityError(std::string_view _constraint) noexcept :
    std::runtime_error(formatMessage(_constraint))
{}

auto DataIntegrityError::formatMessage(std::string_view _constraint) noexcept
    -> std::string
{
    return fmt::format(
        "requested operation violates {} constraint",
        _constraint
    );
}

InternalError::InternalError(std::string_view _message) noexcept :
    std::logic_error(formatMessage(_message))
{}

auto InternalError::formatMessage(std::string_view _message) noexcept
    -> std::string
{
    return fmt::format(
        "data access level internal error occurred: {}",
        _message
    );
}

} // namespace Simulator::DataLayer