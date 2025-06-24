#ifndef SIMULATOR_HTTP_IH_REDIRECT_REQUEST_REDIRECTOR_HPP_
#define SIMULATOR_HTTP_IH_REDIRECT_REQUEST_REDIRECTOR_HPP_

#include <memory>

#include "ih/redirect/redirector.hpp"

namespace Simulator::Http::Redirect {

class RequestRedirector final
    :   public Redirector
{
public:

    RedirectionResult redirect(
        Redirect::Request const & _request
    ) const noexcept override;

    static std::shared_ptr<RequestRedirector> create();
};

} // namespace Simulator::Http::Redirect

#endif // SIMULATOR_HTTP_IH_REDIRECT_REQUEST_REDIRECTOR_HPP_
