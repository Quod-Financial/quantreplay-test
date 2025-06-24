#include "ih/processors/delete_processor.hpp"

#include <pistache/http_defs.h>
#include <pistache/router.h>

#include "log/logging.hpp"

#include "ih/utils/response_formatters.hpp"

namespace Simulator::Http {

DeleteProcessor::DeleteProcessor(
    Simulator::Http::PriceSeedController const& _priceSeedController
) noexcept :
    mPriceSeedController(_priceSeedController)
{}

void DeleteProcessor::deletePriceSeed(
    Pistache::Rest::Request const& _request,
    Pistache::Http::ResponseWriter _response
)
{
    auto id = _request.param(":id").as<std::uint64_t>();
    simulator::log::info("requested delete of the price seed - {}", id);

    auto [code, body] = mPriceSeedController.get().deletePriceSeed(id);
    respond(_request, _response, code, body);
}

void DeleteProcessor::respond(
    Pistache::Rest::Request const& _request,
    Pistache::Http::ResponseWriter& _response,
    Pistache::Http::Code _code,
    std::string const& _body
)
{
    simulator::log::debug(
        "sending response on {} {} from {}:{} with code: {} ({})",
        Pistache::Http::methodString(_request.method()),
        _request.resource(),
        _request.address().host(),
        static_cast<std::uint16_t>(_request.address().port()),
        Pistache::Http::codeString(_code),
        static_cast<int>(_code));

    _response.send(_code, _body);
}

} // namespace Simulator::Http
