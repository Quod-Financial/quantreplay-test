#include "ih/redirect/redirection_processor.hpp"

#include <utility>

#include <fmt/format.h>

#include "ih/data_bridge/venue_accessor.hpp"
#include "ih/redirect/redirector.hpp"
#include "ih/utils/response_formatters.hpp"
#include "ih/redirect/destination_resolver.hpp"
#include "ih/redirect/request_redirector.hpp"

namespace Simulator::Http::Redirect {

RedirectionProcessor::RedirectionProcessor(
    DataBridge::VenueAccessor const& _venueAccessor
)
    :   RedirectionProcessor(
                DestinationResolver::create(_venueAccessor)
            ,   RequestRedirector::create()
        )
{}


RedirectionProcessor::RedirectionProcessor(
        std::shared_ptr<Redirect::Resolver> _pResolver
    ,   std::shared_ptr<Redirect::Redirector> _pRedirector
) noexcept
    :   m_pResolver { std::move(_pResolver) }
    ,   m_pRedirector { std::move(_pRedirector) }
{}


Redirect::Result RedirectionProcessor::redirectToVenue(
        std::string const & _venueID
    ,   Pistache::Http::Method _method
    ,   std::string const & _url
) const
{
    auto [optDestination, resolveSt] = m_pResolver->resolveByVenueID(_venueID);
    if (!optDestination.has_value() || resolveSt != Resolver::Status::Success)
    {
        return RedirectionProcessor::processResolveError(resolveSt, _venueID);
    }

    Redirect::Request const request { *optDestination, _method, _url };
    auto [optResponse, redirectSt] = m_pRedirector->redirect(request);
    if (!optResponse.has_value() || redirectSt != Redirector::Status::Success)
    {
        return RedirectionProcessor::processRedirectError(redirectSt, _venueID);
    }

    return *optResponse;
}


std::shared_ptr<RedirectionProcessor> RedirectionProcessor::create(
    DataBridge::VenueAccessor const& _venueAccessor
)
{
    return std::make_shared<RedirectionProcessor>(_venueAccessor);
}


Redirect::Result RedirectionProcessor::processResolveError(
        Redirect::Resolver::Status _status
    ,   std::string_view _venueID
)
{
    auto responseCode = Pistache::Http::Code::Ok;
    std::string message {};

    switch (_status)
    {
        case Resolver::Status::Success:
        case Resolver::Status::UnknownError:
        {
            responseCode = Pistache::Http::Code::Internal_Server_Error;
            message = formatResultResponse(
                "Request destination resolving failed with unknown error"
            );
            break;
        }
        case Resolver::Status::NonexistentInstance:
        {
            responseCode = Pistache::Http::Code::Bad_Gateway;
            message = formatResultResponse(fmt::format(
                "Could not resolve destination instance with {} identifier",
                _venueID
            ));
            break;
        }
        case Resolver::Status::ResolvingFailed:
        {
            responseCode = Pistache::Http::Code::Bad_Gateway;
            message = formatResultResponse(fmt::format(
                "Could not access an instance with {} identifier",
                _venueID
            ));
            break;
        }
    }

    Redirect::Result response { responseCode };
    response.setBodyContent(std::move(message));
    return response;
}


Redirect::Result RedirectionProcessor::processRedirectError(
        Redirect::Redirector::Status _status
    ,   std::string_view _venueID
)
{
    auto responseCode = Pistache::Http::Code::Ok;
    std::string message {};

    switch (_status)
    {
        case Redirector::Status::Success:
        case Redirector::Status::UnknownError:
        {
            responseCode = Pistache::Http::Code::Internal_Server_Error;
            message = formatResultResponse(
                "Request forwarding failed with unknown error"
            );
            break;
        }
        case Redirector::Status::ConnectionFailed:
        {
            responseCode = Pistache::Http::Code::Bad_Gateway;
            message = formatResultResponse(fmt::format(
                "Could not access an instance with {} identifier",
                _venueID
            ));
            break;
        }
    }

    Redirect::Result response { responseCode };
    response.setBodyContent(std::move(message));
    return response;
}

} // namespace Simulator::Http::Redirect