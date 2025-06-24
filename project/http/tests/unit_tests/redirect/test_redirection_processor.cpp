#include <memory>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <pistache/http_defs.h>

#include "ih/redirect/destination.hpp"
#include "ih/redirect/redirector.hpp"
#include "ih/redirect/resolver.hpp"
#include "ih/redirect/result.hpp"
#include "mocks/redirector.hpp"
#include "mocks/resolver.hpp"
#include "ih/redirect/redirection_processor.hpp"

using namespace Simulator;
using namespace Simulator::Http;

using ::testing::Eq;
using ::testing::Return;

class Http_RedirectionProcessor
    : public testing::Test
{
public:

    using ResolveStatus = Redirect::Resolver::Status;
    using RedirectStatus = Redirect::Redirector::Status;

    auto resolver() -> Mock::Resolver & { return *m_pResolver; }

    auto redirector() -> Mock::Redirector & { return *m_pRedirector; }

    auto redirect(
            std::string const & _venueID
        ,   Pistache::Http::Method _method
        ,   std::string const & _endpointUri
    ) -> Redirect::Result
    {
        return m_pProcessor->redirectToVenue(_venueID, _method, _endpointUri);
    }

    static auto makeRedirectResult(
            Pistache::Http::Code _responseCode
        ,   std::string _body = ""
    ) -> Redirect::Result
    {
        Redirect::Result result{ _responseCode };
        if (!_body.empty())
        {
            result.setBodyContent(std::move(_body));
        }
        return result;
    }

    static inline const std::string TestVenueID = "LSE";
    static inline const std::string TestEndpoint = "/api/test/endpoint";
    static constexpr auto TestMethod = Pistache::Http::Method::Get;

protected:

    void SetUp() override
    {
        m_pResolver = std::make_shared<Mock::Resolver>();
        m_pRedirector = std::make_shared<Mock::Redirector>();
        m_pProcessor = std::make_unique<Redirect::RedirectionProcessor>(
                m_pResolver
            ,   m_pRedirector
        );
    }

private:

    std::shared_ptr<Mock::Resolver> m_pResolver;
    std::shared_ptr<Mock::Redirector> m_pRedirector;

    std::unique_ptr<Redirect::RedirectionProcessor> m_pProcessor;
};

TEST_F(Http_RedirectionProcessor, Redirect_ToNonexistentInstance)
{
    std::string const expectedResponse{ "{"
        R"("result":"Could not resolve destination instance with LSE identifier")"
    "}" };

    EXPECT_CALL(resolver(), resolveByVenueID(Eq(TestVenueID)))
        .Times(1)
        .WillOnce(Return(
            Mock::Resolver::makeOutput(ResolveStatus::NonexistentInstance)));

    Redirect::Result result = redirect(TestVenueID, TestMethod, TestEndpoint);

    EXPECT_EQ(result.getHttpCode(), Pistache::Http::Code::Bad_Gateway);
    EXPECT_TRUE(result.hasBodyContent());
    EXPECT_EQ(result.getBodyContent(), expectedResponse);
}

TEST_F(Http_RedirectionProcessor, Redirect_ToUnresolvableInstance)
{
    std::string const expectedResponse { "{"
        R"("result":"Could not access an instance with LSE identifier")"
    "}" };

    EXPECT_CALL(resolver(), resolveByVenueID(Eq(TestVenueID)))
        .Times(1)
        .WillOnce(Return(
            Mock::Resolver::makeOutput(ResolveStatus::ResolvingFailed)));

    Redirect::Result result = redirect(TestVenueID, TestMethod, TestEndpoint);

    EXPECT_EQ(result.getHttpCode(), Pistache::Http::Code::Bad_Gateway);
    EXPECT_TRUE(result.hasBodyContent());
    EXPECT_EQ(result.getBodyContent(), expectedResponse);
}

TEST_F(Http_RedirectionProcessor, Redirect_UnknowResolvingError)
{
    std::string const expectedResponse { "{"
        R"("result":"Request destination resolving failed with unknown error")"
    "}" };

    EXPECT_CALL(resolver(), resolveByVenueID(Eq(TestVenueID)))
        .Times(1)
        .WillOnce(Return(
            Mock::Resolver::makeOutput(ResolveStatus::UnknownError)));

    Redirect::Result result = redirect(TestVenueID, TestMethod, TestEndpoint);

    EXPECT_EQ(result.getHttpCode(), Pistache::Http::Code::Internal_Server_Error);
    EXPECT_TRUE(result.hasBodyContent());
    EXPECT_EQ(result.getBodyContent(), expectedResponse);
}

TEST_F(Http_RedirectionProcessor, Redirect_SuccessfullyResolved_NoDestinationReturned)
{
    std::string const expectedResponse { "{"
        R"("result":"Request destination resolving failed with unknown error")"
    "}" };

    EXPECT_CALL(resolver(), resolveByVenueID(Eq(TestVenueID)))
        .Times(1)
        .WillOnce(Return(
            Mock::Resolver::makeOutput(ResolveStatus::Success)));

    Redirect::Result result = redirect(TestVenueID, TestMethod, TestEndpoint);

    EXPECT_EQ(result.getHttpCode(), Pistache::Http::Code::Internal_Server_Error);
    EXPECT_TRUE(result.hasBodyContent());
    EXPECT_EQ(result.getBodyContent(), expectedResponse);
}

TEST_F(Http_RedirectionProcessor, Redirect_SuccessfullyResolved)
{
    EXPECT_CALL(resolver(), resolveByVenueID(Eq(TestVenueID)))
        .Times(1)
        .WillOnce(Return(
            Mock::Resolver::makeOutput(
                Redirect::Destination{ "localhost", 10001 })));

    EXPECT_CALL(redirector(), redirect)
        .Times(1)
        .WillOnce(Return(
            Mock::Redirector::makeOutput(RedirectStatus::ConnectionFailed)));

    redirect(TestVenueID, TestMethod, TestEndpoint);
}

TEST_F(Http_RedirectionProcessor, Redirect_ConnectionFailed)
{
    std::string const expectedResponse { "{"
        R"("result":"Could not access an instance with LSE identifier")"
    "}" };

    EXPECT_CALL(resolver(), resolveByVenueID(Eq(TestVenueID)))
        .Times(1)
        .WillOnce(Return(
            Mock::Resolver::makeOutput(
                Redirect::Destination{ "localhost", 10001 })));

    EXPECT_CALL(redirector(), redirect)
        .Times(1)
        .WillOnce(Return(
            Mock::Redirector::makeOutput(RedirectStatus::ConnectionFailed)));

    Redirect::Result result = redirect(TestVenueID, TestMethod, TestEndpoint);

    EXPECT_EQ(result.getHttpCode(), Pistache::Http::Code::Bad_Gateway);
    EXPECT_TRUE(result.hasBodyContent());
    EXPECT_EQ(result.getBodyContent(), expectedResponse);
}

TEST_F(Http_RedirectionProcessor, Redirect_UnknownRedirectionError)
{
    std::string const expectedResponse { "{"
        R"("result":"Request forwarding failed with unknown error")"
    "}" };

    EXPECT_CALL(resolver(), resolveByVenueID(Eq(TestVenueID)))
        .Times(1)
        .WillOnce(Return(
            Mock::Resolver::makeOutput(
                Redirect::Destination{ "localhost", 10001 })));

    EXPECT_CALL(redirector(), redirect)
        .Times(1)
        .WillOnce(Return(
            Mock::Redirector::makeOutput(RedirectStatus::UnknownError)));

    Redirect::Result result = redirect(TestVenueID, TestMethod, TestEndpoint);

    EXPECT_EQ(result.getHttpCode(), Pistache::Http::Code::Internal_Server_Error);
    EXPECT_TRUE(result.hasBodyContent());
    EXPECT_EQ(result.getBodyContent(), expectedResponse);
}

TEST_F(Http_RedirectionProcessor, Redirect_SuccessfullyRedirected_NoResponseReturned)
{
    std::string const expectedResponse { "{"
        R"("result":"Request forwarding failed with unknown error")"
    "}" };

    EXPECT_CALL(resolver(), resolveByVenueID(Eq(TestVenueID)))
        .Times(1)
        .WillOnce(Return(
            Mock::Resolver::makeOutput(
                Redirect::Destination{ "localhost", 10001 })));

    EXPECT_CALL(redirector(), redirect)
        .Times(1)
        .WillOnce(Return(
            Mock::Redirector::makeOutput(RedirectStatus::Success)));

    Redirect::Result result = redirect(TestVenueID, TestMethod, TestEndpoint);

    EXPECT_EQ(result.getHttpCode(), Pistache::Http::Code::Internal_Server_Error);
    EXPECT_TRUE(result.hasBodyContent());
    EXPECT_EQ(result.getBodyContent(), expectedResponse);
}

TEST_F(Http_RedirectionProcessor, Redirect_SuccessfullyRedirected)
{
    constexpr auto responseCode = Pistache::Http::Code::Ok;
    std::string const responseBody { "{"
        R"("result":"Successful test request response")"
    "}" };

    EXPECT_CALL(resolver(), resolveByVenueID(Eq(TestVenueID)))
        .Times(1)
        .WillOnce(Return(
            Mock::Resolver::makeOutput(
                Redirect::Destination{ "localhost", 10001 })));

    EXPECT_CALL(redirector(), redirect)
        .Times(1)
        .WillOnce(Return(
            Mock::Redirector::makeOutput(
                makeRedirectResult(Pistache::Http::Code::Ok, responseBody))));

    Redirect::Result result = redirect(TestVenueID, TestMethod, TestEndpoint);

    EXPECT_EQ(result.getHttpCode(), responseCode);
    EXPECT_TRUE(result.hasBodyContent());
    EXPECT_EQ(result.getBodyContent(), responseBody);
}
