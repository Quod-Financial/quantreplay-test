#ifndef SIMULATOR_HTTP_API_FORMATTERS_REDIRECT_HPP_
#define SIMULATOR_HTTP_API_FORMATTERS_REDIRECT_HPP_

#include <fmt/format.h>

#include "ih/redirect/destination.hpp"
#include "ih/redirect/request.hpp"
#include "ih/redirect/result.hpp"

namespace fmt {

template<>
class formatter<Simulator::Http::Redirect::Destination>
{
    using FormatableType = Simulator::Http::Redirect::Destination;

public:

    static constexpr auto parse(format_parse_context & _ctx)
    {
        return _ctx.begin();
    }

    template<typename FormattingContext>
    auto format(FormatableType const & _destination, FormattingContext & _ctx) const
    {
        return format_to(_ctx.out(),
            "Redirect::Destination: \"{}:{}\"",
            _destination.getHost(), _destination.getPort()
        );
    }
};


template<>
class formatter<Simulator::Http::Redirect::Request>
{
    using FormatableType = Simulator::Http::Redirect::Request;

public:

    static constexpr auto parse(format_parse_context & _ctx)
    {
        return _ctx.begin();
    }

    template<typename FormattingContext>
    auto format(FormatableType const & _request, FormattingContext & _ctx) const
    {
        using Pistache::Http::methodString;

        return format_to( _ctx.out(),
            "Redirect::Request: {{ Resource: '{}:{}', {} }}",
            methodString(_request.getMethod()), _request.getUrl(),
            _request.getDestination()
        );
    }
};


template<>
class formatter<Simulator::Http::Redirect::Result>
{
    using FormatableType = Simulator::Http::Redirect::Result;

public:

    static constexpr auto parse(format_parse_context & _ctx)
    {
        return _ctx.begin();
    }

    template<typename FormattingContext>
    auto format(FormatableType const & _result, FormattingContext & _ctx) const
    {
        using Pistache::Http::codeString;

        return format_to(_ctx.out(),
            "Redirect::Response: {{ "
                "Status: \"{}\", Code: {}, Body: \"{}-byte providers\" "
            "}}",
            codeString(_result.getHttpCode()),
            static_cast<int>(_result.getHttpCode()),
            _result.getBodyContent().size()
        );
    }
};

} // namespace fmt

#endif // SIMULATOR_HTTP_API_FORMATTERS_REDIRECT_HPP_
