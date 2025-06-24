#include "ih/redirect/request_redirector.hpp"

#include <map>
#include <stdexcept>
#include <httplib.h>
#include <pistache/http.h>

#include "log/logging.hpp"
#include "ih/formatters/redirect.hpp"

namespace Simulator::Http::Redirect {

using MethodHandler = std::function<
    httplib::Result(httplib::Client &, Redirect::Request const &)
>;


static const std::map<Pistache::Http::Method, MethodHandler> Handlers {
    {
        Pistache::Http::Method::Get,
        [] (auto & _client, auto const & _request)
        {
            return _client.Get(_request.getUrl());
        }
    },
    {
        Pistache::Http::Method::Post,
        [] (auto & _client, auto const & _request)
        {
            return _client.Post(_request.getUrl());
        }
    },
    {
        Pistache::Http::Method::Put,
        [] (auto & _client, auto const & _request)
        {
            return _client.Put(_request.getUrl());
        }
    },
};


static MethodHandler const & findHandler(Pistache::Http::Method _method)
{
    auto handlerIt = Handlers.find(_method);
    if (handlerIt == std::end(Handlers))
    {
        throw std::logic_error { fmt::format(
            "request can not be redirected, {} method is not supported "
            "for redirection", Pistache::Http::methodString(_method)
        ) };
    }
    return handlerIt->second;
}


static Redirector::RedirectionResult processResponse(
        httplib::Result _response
    ,   Redirect::Request const & _request
)
{
    auto & receivedResponse = _response.value();
    int const responseStatus = receivedResponse.status;
    auto const responseCode = static_cast<Pistache::Http::Code>(responseStatus);

    Redirect::Result response { responseCode };
    response.setBodyContent(std::move(receivedResponse.body));

    simulator::log::debug("received a {} on a {}", response, _request);
    return std::make_pair(std::move(response), Redirector::Status::Success);
}


static Redirector::RedirectionResult processError(
        httplib::Error _error
    ,   Redirect::Request const & _request
)
{
    using Status = Redirector::Status;

    if (_error == httplib::Error::ConnectionTimeout ||
        _error == httplib::Error::Connection)
    {
        simulator::log::err("{} failed - connection failed", _request);
        return std::make_pair(std::nullopt, Status::ConnectionFailed);
    }

    simulator::log::err(
        "{} failed - httplib reported about error that can not be handled",
        _request);
    return std::make_pair(std::nullopt, Status::UnknownError);
}


Redirector::RedirectionResult
RequestRedirector::redirect(Redirect::Request const & _request) const noexcept
{
    auto const & destination = _request.getDestination();
    try
    {
        simulator::log::debug("forwarding {}", _request);
        httplib::Client client { destination.getHost(), destination.getPort() };
        auto const & methodHandler = findHandler(_request.getMethod());
        auto requestResult = methodHandler(client, _request);

        if (requestResult)
        {
            return processResponse(std::move(requestResult), _request);
        }

        return processError(requestResult.error(), _request);
    }
    catch (std::exception const & _ex)
    {
        simulator::log::err(
            "failed to forward {} - an error occurred: {}",
            _request,
            _ex.what());
      return std::make_pair(std::nullopt, Redirector::Status::UnknownError);
    }
    catch (...)
    {
        simulator::log::err("failed to redirect {} - unknown error occurred",
                            _request);
        return std::make_pair(std::nullopt, Redirector::Status::UnknownError);
    }
}


std::shared_ptr<RequestRedirector> RequestRedirector::create()
{
    return std::make_shared<RequestRedirector>();
}

} // namespace Simulator::Http::Redirect