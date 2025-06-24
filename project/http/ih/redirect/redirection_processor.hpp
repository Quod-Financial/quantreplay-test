#ifndef SIMULATOR_HTTP_IH_REDIRECT_REDIRECTION_PROCESSOR_HPP_
#define SIMULATOR_HTTP_IH_REDIRECT_REDIRECTION_PROCESSOR_HPP_

#include <memory>
#include <string>

#include <pistache/http.h>

#include "ih/data_bridge/venue_accessor.hpp"
#include "ih/redirect/result.hpp"
#include "ih/redirect/redirector.hpp"
#include "ih/redirect/resolver.hpp"

namespace Simulator::Http::Redirect {

class RedirectionProcessor
{
public:

    RedirectionProcessor() = delete;

    explicit RedirectionProcessor(
        DataBridge::VenueAccessor const& _venueAccessor
    );

    RedirectionProcessor(
            std::shared_ptr<Redirect::Resolver> _pResolver
        ,   std::shared_ptr<Redirect::Redirector> _pRedirector
    ) noexcept;

    Redirect::Result redirectToVenue(
            std::string const & _venueID
        ,   Pistache::Http::Method _method
        ,   std::string const & _url
    ) const;

    static std::shared_ptr<RedirectionProcessor> create(
        DataBridge::VenueAccessor const& _venueAccessor
    );

private:

    static Redirect::Result processResolveError(
            Redirect::Resolver::Status _status
        ,   std::string_view _venueID
    );

    static Redirect::Result processRedirectError(
            Redirect::Redirector::Status _status
        ,   std::string_view _venueID
    );


    std::shared_ptr<Redirect::Resolver> m_pResolver;
    std::shared_ptr<Redirect::Redirector> m_pRedirector;
};

} // namespace Simulator::Http::Redirect

#endif // SIMULATOR_HTTP_SRC_REDIRECT_REDIRECTION_PROCESSOR_HPP_
