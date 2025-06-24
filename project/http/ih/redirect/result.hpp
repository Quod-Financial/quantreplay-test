#ifndef SIMULATOR_HTTP_IH_REDIRECT_RESPONSE_HPP_
#define SIMULATOR_HTTP_IH_REDIRECT_RESPONSE_HPP_

#include <string>
#include <utility>

#include <pistache/http.h>

namespace Simulator::Http::Redirect {

class Result
{
public:

    Result() = delete;

    explicit Result(Pistache::Http::Code _code) noexcept
        :   m_code { _code }
    {}

    auto getHttpCode() const noexcept
    {
        return m_code;
    }

    bool hasBodyContent() const noexcept
    {
        return !m_body.empty();
    }

    std::string const & getBodyContent() const noexcept
    {
        return m_body;
    }

    void setBodyContent(std::string _bodyContent) noexcept
    {
        m_body = std::move(_bodyContent);
    }

private:

    std::string m_body;
    Pistache::Http::Code m_code;
};

} // Simulator::Http::Redirect

#endif // SIMULATOR_HTTP_IH_REDIRECT_RESPONSE_HPP_
