#ifndef SIMULATOR_HTTP_IH_REDIRECT_REQUEST_HPP_
#define SIMULATOR_HTTP_IH_REDIRECT_REQUEST_HPP_

#include <string>
#include <utility>

#include <pistache/http.h>

#include "ih/redirect/destination.hpp"
#include "ih/redirect/result.hpp"

namespace Simulator::Http::Redirect {

class Request
{
public:

    Request() = delete;

    Request(
            Destination _destination
        ,   Pistache::Http::Method _method
        ,   std::string _url
    ) noexcept
        :   m_requestDestination { std::move(_destination) }
        ,   m_requestUrl { std::move(_url) }
        ,   m_requestMethod { _method }
    {}

    Destination const & getDestination() const noexcept
    {
        return m_requestDestination;
    }

    std::string const & getUrl() const noexcept
    {
        return m_requestUrl;
    }

    Pistache::Http::Method getMethod() const noexcept
    {
        return m_requestMethod;
    }

private:

    Destination m_requestDestination;
    std::string m_requestUrl;
    Pistache::Http::Method m_requestMethod;
};

} // Simulator::Http::Redirect

#endif // SIMULATOR_HTTP_IH_REDIRECT_REQUEST_HPP_
