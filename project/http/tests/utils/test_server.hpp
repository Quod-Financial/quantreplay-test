#ifndef SIMULATOR_HTTP_TEST_UTILS_TEST_SERVER_HPP_
#define SIMULATOR_HTTP_TEST_UTILS_TEST_SERVER_HPP_

#include <utility>

#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>

namespace Simulator::Http::Test::Util {

class Responder
{
public:

    virtual ~Responder() = default;

    void setResponseData(
            Pistache::Http::Code _responseCode
        ,   std::string _responseBody = ""
    ) noexcept
    {
        m_code = _responseCode;
        m_body = std::move(_responseBody);
    }

protected:

    std::string m_body;
    Pistache::Http::Code m_code { Pistache::Http::Code::Ok };
};


class Router
    :   public Pistache::Http::Handler
    ,   public Responder
{
public:

    HTTP_PROTOTYPE( Router )

    Router()
    {
        init();
    }

    void onRequest (
            Pistache::Http::Request const & _request
        ,   Pistache::Http::ResponseWriter _response
    ) override
    {
        m_router.route(_request, std::move(_response));
    }

private:

    void init()
    {
        Pistache::Rest::Routes::Get(m_router, "/test/get/request",
            Pistache::Rest::Routes::bind(&Router::respond, this)
        );

        Pistache::Rest::Routes::Post(m_router, "/test/post/request",
            Pistache::Rest::Routes::bind(&Router::respond, this)
        );

        Pistache::Rest::Routes::Put(m_router, "/test/put/request",
            Pistache::Rest::Routes::bind(&Router::respond, this)
        );

        Pistache::Rest::Routes::Delete(m_router, "/test/delete/request",
            Pistache::Rest::Routes::bind(&Router::respond, this)
        );
    }

    void respond(
            Pistache::Rest::Request const & /*_request*/
        ,   Pistache::Http::ResponseWriter _response
    )
    {
        _response.send(m_code, m_body);
    }

    Pistache::Rest::Router m_router;
};


class Server
{
public:

    Server()
        :   m_endpoint { Pistache::Address {
                Pistache::Ipv4::any(), Pistache::Port { 0 }
            } }
        ,   m_pRouter { Pistache::Http::make_handler<Router>() }
    {
        auto opts = Pistache::Http::Endpoint::options().threads( 1 );
        m_endpoint.init(opts);
        m_endpoint.setHandler(m_pRouter);

        m_endpoint.serveThreaded();
    }

    Server(Server const &) = delete;
    Server operator= (Server const &) = delete;

    Server(Server &&) noexcept = delete;
    Server operator= (Server &&) noexcept = delete;

    ~Server()
    {
        m_endpoint.shutdown();
    }

    std::uint16_t getPort() const
    {
        return m_endpoint.getPort();
    }

    std::shared_ptr<Router> getRouter() const noexcept { return m_pRouter; }

    static std::shared_ptr<Server> create()
    {
        return std::make_shared<Server>();
    }

private:

    Pistache::Http::Endpoint m_endpoint;

    std::shared_ptr<Router> m_pRouter;
};

} // namespace Simulator::Http::Test::Util

#endif // SIMULATOR_HTTP_TEST_UTILS_TEST_SERVER_HPP_
