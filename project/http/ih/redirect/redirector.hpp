#ifndef SIMULATOR_HTTP_IH_REDIRECT_REDIRECTOR_HPP_
#define SIMULATOR_HTTP_IH_REDIRECT_REDIRECTOR_HPP_

#include <optional>
#include <string>
#include <utility>

#include "ih/redirect/request.hpp"
#include "ih/redirect/result.hpp"

namespace Simulator::Http::Redirect {

class Redirector
{
public:

    enum class Status
    {
        Success,
        ConnectionFailed,
        UnknownError
    };

    virtual ~Redirector() = default;

    using RedirectionResult = std::pair<std::optional<Result>, Status>;

    virtual RedirectionResult redirect(
        Redirect::Request const & _request
    ) const noexcept = 0;
};

} // namespace Simulator::Http::Redirect

#endif // SIMULATOR_HTTP_IH_REDIRECT_RESOLVING_HPP_

