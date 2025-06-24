#ifndef SIMULATOR_HTTP_IH_FORMATTERS_REQUEST_FORMATTER_HPP_
#define SIMULATOR_HTTP_IH_FORMATTERS_REQUEST_FORMATTER_HPP_

#include <string>

#include <fmt/format.h>
#include <pistache/http.h>

namespace Simulator::Http {

class RequestFormatter
{
public:
    auto method() const -> std::string
    {
        return Pistache::Http::methodString(m_request.get().method());
    }

    auto resource() const -> std::string const &
    {
        return m_request.get().resource();
    }

    auto address() const -> std::string
    {
        Pistache::Address const & address = m_request.get().address();
        return fmt::format(
            "{}:{}",
            address.host(),
            static_cast<std::uint16_t>(address.port())
        );
    }

    auto hasBody() const -> bool
    {
        return !m_request.get().body().empty();
    }

    auto body() const -> std::string
    {
        return m_request.get().body();
    }

    static auto format(Pistache::Http::Request const & _request)
        -> RequestFormatter
    {
        return RequestFormatter{_request};
    }

private:
    explicit RequestFormatter(Pistache::Http::Request const & _request)
        :   m_request{_request} {}

    std::reference_wrapper<Pistache::Http::Request const> m_request;
};

} // namespace Simulator::Http


namespace fmt {

template<>
class formatter<Simulator::Http::RequestFormatter> {

    using RequestFormatter = Simulator::Http::RequestFormatter;

public:

    static auto parse(format_parse_context & _ctx) -> decltype(_ctx.begin())
    {
        return _ctx.begin();
    }

    template<typename Context>
    auto format(RequestFormatter const & _formatter, Context & _ctx) const
        -> decltype(_ctx.out())
    {
        format_to(_ctx.out(),
            "RestRequest={{ Method={} URL=\"{}\" Source={} ",
            _formatter.method(), _formatter.resource(), _formatter.address()
        );
        if (_formatter.hasBody())
        {
            format_to(_ctx.out(), "Body=\"{}\" ", _formatter.body());
        }
        return format_to(_ctx.out(), "}}");;
    }
};

} // namespace fmt

#endif // SIMULATOR_HTTP_IH_FORMATTERS_REQUEST_FORMATTER_HPP_
