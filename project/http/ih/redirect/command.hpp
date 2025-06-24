#ifndef SIMULATOR_HTTP_IH_REDIRECT_COMMAND_HPP_
#define SIMULATOR_HTTP_IH_REDIRECT_COMMAND_HPP_

#include <optional>

#include "ih/redirect/result.hpp"

namespace Simulator::Http::Redirect {

class Command
{
public:

    enum class Status
    {
        Success,
        ResolvingFailed,  // Venue entry can not be used to redirect a request
        ConnectionFailed, // Connection to the resolved destination failed
        UnknownError
    };

    virtual ~Command() = default;

    std::optional<Result> const & getCommandResult() const noexcept
    {
        return m_commandResult;
    }

    void setCommandResult(Result _commandResult) noexcept
    {
        m_commandResult = std::move(_commandResult);
    }

private:

    std::optional<Result> m_commandResult;
};

} // namespace Simulator::Http::Redirect

#endif // SIMULATOR_HTTP_IH_REDIRECT_DESTINATION_HPP_
