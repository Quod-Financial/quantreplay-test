#include <cstdint>

#include <gtest/gtest.h>
#include <pistache/http_defs.h>

#include "cfg/api/cfg.hpp"
#include "ih/redirect/destination.hpp"
#include "ih/redirect/redirector.hpp"
#include "ih/redirect/request.hpp"
#include "ih/redirect/request_redirector.hpp"
#include "utils/test_server.hpp"

using namespace Simulator;
using namespace Simulator::Http;
using namespace Simulator::Http::Redirect;

class Http_RequestRedirector
    :   public testing::Test
{
public:

    using RedirectStatus = Redirector::Status;
    using HttpCode = Pistache::Http::Code;

    auto serverResponder() -> Http::Test::Util::Responder &
    {
        auto pRouter = m_pTestHttpServer->getRouter();
        return *pRouter;
    }

    auto serverPort() -> std::uint16_t { return m_pTestHttpServer->getPort(); }

    auto redirect(Redirect::Request const & _request)
        -> Redirector::RedirectionResult
    {
        return m_pRequestRedirector->redirect(_request);
    }

    static auto makeDestination(std::uint16_t _port) -> Redirect::Destination
    {
        return Redirect::Destination{ "localhost", _port };
    }

    static auto makeGetRequest(Destination _destination, std::string _endpint)
        -> Redirect::Request
    {
        return makeRequest(
                std::move(_destination)
            ,   Pistache::Http::Method::Get
            ,   std::move(_endpint)
        );
    }

    static auto makePostRequest(Destination _destination, std::string _endpint)
        -> Redirect::Request
    {
        return makeRequest(
                std::move(_destination)
            ,   Pistache::Http::Method::Post
            ,   std::move(_endpint)
        );
    }

    static auto makePutRequest(Destination _destination, std::string _endpint)
        -> Redirect::Request
    {
        return makeRequest(
                std::move(_destination)
            ,   Pistache::Http::Method::Put
            ,   std::move(_endpint)
        );
    }

    static auto makeDeleteRequest(Destination _destination, std::string _endpint)
        -> Redirect::Request
    {
        return makeRequest(
                std::move(_destination)
            ,   Pistache::Http::Method::Delete
            ,   std::move(_endpint)
        );
    }

protected:

    void SetUp() override
    {
        Cfg::init();
        m_pRequestRedirector = Redirect::RequestRedirector::create();
        m_pTestHttpServer = Http::Test::Util::Server::create();
    }

private:

    static auto makeRequest(
            Destination _destination
        ,   Pistache::Http::Method _method
        ,   std::string _endpoint
    ) -> Redirect::Request
    {
        return Redirect::Request {
                std::move(_destination)
            ,   _method
            ,   std::move(_endpoint)
        };
    }

    std::shared_ptr<Redirect::RequestRedirector> m_pRequestRedirector;

    std::shared_ptr<Http::Test::Util::Server> m_pTestHttpServer;
};

TEST_F(Http_RequestRedirector, Redirect_Destination_NotAccessible)
{
    std::uint16_t const invalidPort = 0;
    ASSERT_NE(invalidPort, serverPort());

    Redirect::Request const invalidRequest = makeGetRequest(
        makeDestination(invalidPort),
        "/test/get/request"
    );

    auto [optResponse, status] = redirect(invalidRequest);

    EXPECT_EQ(status, RedirectStatus::ConnectionFailed);
    EXPECT_FALSE(optResponse.has_value());
}

TEST_F(Http_RequestRedirector, Redirect_Destination_Accessible)
{
    Redirect::Request const request = makeGetRequest(
        makeDestination(serverPort()),
        "/test/get/request"
    );

    auto [optResponse, status] = redirect(request);

    EXPECT_EQ(status, RedirectStatus::Success);
    EXPECT_TRUE(optResponse.has_value());
}

TEST_F(Http_RequestRedirector, Redirect_Method_Unsupported)
{
    Redirect::Request const request = makeDeleteRequest(
        makeDestination(serverPort()),
        "/test/delete/request"
    );

    auto [response, status] = redirect(request);
    EXPECT_EQ(status, RedirectStatus::UnknownError);
    EXPECT_FALSE(response.has_value());
}

TEST_F(Http_RequestRedirector, Redirect_Method_Invalid)
{
    Redirect::Request const request = makeGetRequest(
        makeDestination(serverPort()),
        "/test/post/request"
    );

    auto [response, status] = redirect(request);
    EXPECT_EQ(status, RedirectStatus::Success);

    ASSERT_TRUE(response.has_value());
    EXPECT_EQ(response->getHttpCode(), Pistache::Http::Code::Method_Not_Allowed);
}

TEST_F(Http_RequestRedirector, Redirect_Method_Get)
{
    serverResponder().setResponseData(Pistache::Http::Code::Ok);

    Redirect::Request const request = makeGetRequest(
        makeDestination(serverPort()),
        "/test/get/request"
    );

    auto [response, status] = redirect(request);
    EXPECT_EQ(status, RedirectStatus::Success);

    ASSERT_TRUE(response.has_value());
    EXPECT_EQ(response->getHttpCode(), Pistache::Http::Code::Ok);
}

TEST_F(Http_RequestRedirector, Redirect_Method_Post)
{
    serverResponder().setResponseData(Pistache::Http::Code::Created);

    Redirect::Request const request = makePostRequest(
        makeDestination(serverPort()),
        "/test/post/request"
    );

    auto [response, status] = redirect(request);
    EXPECT_EQ(status, RedirectStatus::Success);

    ASSERT_TRUE(response.has_value());
    EXPECT_EQ(response->getHttpCode(), Pistache::Http::Code::Created);
}

TEST_F(Http_RequestRedirector, Redirect_Method_Put)
{
    serverResponder().setResponseData(Pistache::Http::Code::Ok);

    Redirect::Request const request = makePutRequest(
        makeDestination(serverPort()),
        "/test/put/request"
    );

    auto [response, status] = redirect(request);
    EXPECT_EQ(status, RedirectStatus::Success);

    ASSERT_TRUE(response.has_value());
    EXPECT_EQ(response->getHttpCode(), Pistache::Http::Code::Ok);
}

TEST_F(Http_RequestRedirector, Redirect_EndpointURI_Invalid)
{
    serverResponder().setResponseData(Pistache::Http::Code::Ok);

    Redirect::Request const request = makeGetRequest(
        makeDestination(serverPort()),
        "/an/invalid/url"
    );

    auto [response, status] = redirect(request);
    EXPECT_EQ(status, RedirectStatus::Success);

    ASSERT_TRUE(response.has_value());
    EXPECT_EQ(response->getHttpCode(), Pistache::Http::Code::Not_Found);
}

TEST_F(Http_RequestRedirector, Redirect_EndpointURI_Valid)
{
    serverResponder().setResponseData(Pistache::Http::Code::Forbidden);

    Redirect::Request const request = makeGetRequest(
        makeDestination(serverPort()),
        "/test/get/request"
    );

    auto [response, status] = redirect(request);
    EXPECT_EQ(status, RedirectStatus::Success);

    ASSERT_TRUE(response.has_value());
    EXPECT_EQ(response->getHttpCode(), Pistache::Http::Code::Forbidden);
}

TEST_F(Http_RequestRedirector, Redirect_Response_EmptyBody)
{
    std::string body = "";
    serverResponder().setResponseData(Pistache::Http::Code::Ok, body);

    Redirect::Request const request = makeGetRequest(
        makeDestination(serverPort()),
        "/test/get/request"
    );

    auto [response, status] = redirect(request);
    EXPECT_EQ(status, RedirectStatus::Success);

    ASSERT_TRUE(response.has_value());
    EXPECT_FALSE(response->hasBodyContent());
    EXPECT_TRUE(response->getBodyContent().empty());
}

TEST_F(Http_RequestRedirector, Redirect_Response_WithBody)
{
    std::string body = "a content inside the response body";
    serverResponder().setResponseData(Pistache::Http::Code::Ok, body);

    Redirect::Request const request = makeGetRequest(
        makeDestination(serverPort()),
        "/test/get/request"
    );

    auto [response, status] = redirect(request);
    EXPECT_EQ(status, RedirectStatus::Success);

    ASSERT_TRUE(response.has_value());
    EXPECT_TRUE(response->hasBodyContent());
    EXPECT_EQ(response->getBodyContent(), body);
}
