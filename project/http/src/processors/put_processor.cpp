#include "ih/processors/put_processor.hpp"

#include <pistache/http_defs.h>

#include <regex>
#include <string>

#include "ih/controllers/trading_controller.hpp"
#include "ih/utils/response_formatters.hpp"
#include "log/logging.hpp"
#include "middleware/routing/generator_admin_channel.hpp"
#include "protocol/admin/generator.hpp"

/*----------------------------------------------------------------------------*/

namespace Simulator::Http {

/*----------------------------------------------------------------------------*/

PutProcessor::PutProcessor(
    DataBridge::VenueAccessor const& _venueAccessor,
    DatasourceController const& _datasourceController,
    ListingController const& _listingController,
    PriceSeedController const& _priceSeedController,
    SettingController const& _settingController,
    TradingController const& _tradingController,
    VenueController const& _venueController
) :
    m_pRedirector(Redirect::RedirectionProcessor::create(_venueAccessor)),
    mDatasourceController(_datasourceController),
    mListingController(_listingController),
    mPriceSeedController(_priceSeedController),
    mSettingController(_settingController),
    mTradingController(_tradingController),
    mVenueController(_venueController)
{}

void PutProcessor::updateVenue(
    Pistache::Rest::Request const& _request,
    Pistache::Http::ResponseWriter _response
)
{
    auto const venueId = _request.param(":id").as<std::string>();
    simulator::log::info("requested update of the venue - {}", venueId);

    auto [code, body] =
        mVenueController.get().updateVenue(venueId, _request.body());
    respond(_request, _response, code, body);
}

void PutProcessor::updateListing(
    Pistache::Rest::Request const& _request,
    Pistache::Http::ResponseWriter _response
)
{
    auto const symbol = _request.param(":symbol").as<std::string>();
    auto const key = std::regex_replace(symbol, std::regex("%2F"), "/");
    simulator::log::info("requested update of the listing - {}", key);

    auto [code, body] =
        mListingController.get().updateListing(key, _request.body());
    respond(_request, _response, code, body);
}

void PutProcessor::updateDataSource(
    Pistache::Rest::Request const& _request,
    Pistache::Http::ResponseWriter _response
)
{
    auto const sourceId = _request.param(":id").as<std::uint64_t>();
    simulator::log::info("requested update of the datasource - {}", sourceId);

    auto [code, body] =
        mDatasourceController.get().updateDatasource(sourceId, _request.body());
    respond(_request, _response, code, body);
}


void PutProcessor::updatePriceSeed(
    Pistache::Rest::Request const& _request,
    Pistache::Http::ResponseWriter _response
)
{
    auto const id = _request.param(":id").as<std::uint64_t>();
    simulator::log::info("requested update of the price seed - {}", id);

    auto [code, body] =
        mPriceSeedController.get().updatePriceSeed(id, _request.body());
    respond(_request, _response, code, body);
}

void PutProcessor::syncPriceSeeds(
    Pistache::Rest::Request const& _request,
    Pistache::Http::ResponseWriter _response
)
{
    simulator::log::info("received a request to synchronize price seeds");

    auto [code, body] = mPriceSeedController.get().syncPriceSeeds();
    respond(_request, _response, code, body);
}

void PutProcessor::updateSettings(
    Pistache::Rest::Request const& _request,
    Pistache::Http::ResponseWriter _response
)
{
    simulator::log::info("requested update of general settings");

    auto [code, body] =
        mSettingController.get().updateSettings(_request.body());
    respond(_request, _response, code, body);
}

void PutProcessor::stopOrderGen(
    Pistache::Rest::Request const& _request,
    Pistache::Http::ResponseWriter _response
)
{
    auto const instanceID = _request.param(":venueId").as<std::string>();
    simulator::log::info("requested to stop order generation for {}",
                         instanceID);

    if (instanceID == Cfg::venue().name) {
        handleGenerationStopRequest(_request, std::move(_response));
    } else {
        auto const redirectResponse = redirect(_request, instanceID);
        respond(
            _request,
            _response,
            redirectResponse.getHttpCode(),
            redirectResponse.getBodyContent()
        );
    }
}

/*----------------------------------------------------------------------------*/

void PutProcessor::startOrderGen(
    Pistache::Rest::Request const& _request,
    Pistache::Http::ResponseWriter _response
)
{
    auto const instanceID = _request.param(":venueId").as<std::string>();
    simulator::log::info("requested to start order generation for {}",
                         instanceID);

    if (instanceID == Cfg::venue().name) {
        handleGenerationStartRequest(_request, std::move(_response));
    } else {
        auto const redirectResponse = redirect(_request, instanceID);
        respond(
            _request,
            _response,
            redirectResponse.getHttpCode(),
            redirectResponse.getBodyContent()
        );
    }
}

/*----------------------------------------------------------------------------*/

auto PutProcessor::halt_phase(const Pistache::Rest::Request& request,
                              Pistache::Http::ResponseWriter response) -> void {
  const auto instanceID = request.param(":venueId").as<std::string>();
  if (instanceID == Cfg::venue().name) {
    const auto [code, body] = mTradingController.get().halt(request.body());
    respond(request, response, code, body);
  } else {
    const auto redirectResponse = redirect(request, instanceID);
    respond(request,
            response,
            redirectResponse.getHttpCode(),
            redirectResponse.getBodyContent());
  }
}

/*----------------------------------------------------------------------------*/

auto PutProcessor::resume_phase(const Pistache::Rest::Request& request,
                                Pistache::Http::ResponseWriter response) -> void {
  const auto instanceID = request.param(":venueId").as<std::string>();
  if (instanceID == Cfg::venue().name) {
    const auto [code, body] = mTradingController.get().resume();
    respond(request, response, code, body);
  } else {
    const auto redirectResponse = redirect(request, instanceID);
    respond(request,
            response,
            redirectResponse.getHttpCode(),
            redirectResponse.getBodyContent());
  }
}


void PutProcessor::handleGenerationStartRequest(
    Pistache::Rest::Request const& _request,
    Pistache::Http::ResponseWriter _response
)
{
  simulator::protocol::StartGenerationRequest request;
  simulator::protocol::StartGenerationReply reply;

  try {
    simulator::middleware::send_admin_request(request, reply);
  } catch (const simulator::middleware::ChannelUnboundError &) {}

  if (reply.result.has_value() &&
      *reply.result == simulator::protocol::StartGenerationReply::Result::Started) {
    respond(_request,
            _response,
            Pistache::Http::Code::Ok,
            formatResultResponse("Generator was started"));
  } else {
    respond(_request,
            _response,
            Pistache::Http::Code::Internal_Server_Error,
            formatResultResponse("Failed to start the generator"));
  }
}


void PutProcessor::handleGenerationStopRequest(
    Pistache::Rest::Request const& _request,
    Pistache::Http::ResponseWriter _response
)
{
  simulator::protocol::StopGenerationRequest request;
  simulator::protocol::StopGenerationReply reply;

  try {
    simulator::middleware::send_admin_request(request, reply);
  } catch (const simulator::middleware::ChannelUnboundError &) {}

  if (reply.result.has_value() &&
      *reply.result == simulator::protocol::StopGenerationReply::Result::Stopped) {
    respond(_request,
            _response,
            Pistache::Http::Code::Ok,
            formatResultResponse("Generator was stopped"));
  } else {
    respond(_request,
            _response,
            Pistache::Http::Code::Internal_Server_Error,
            formatResultResponse("Failed to stop the generator"));
  }
}

void PutProcessor::respond(
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


Redirect::Result PutProcessor::redirect(
    Pistache::Rest::Request const& _request,
    std::string const& _instanceID
) const
{
    assert(m_pRedirector);
    return m_pRedirector
        ->redirectToVenue(_instanceID, _request.method(), _request.resource());
}

/*----------------------------------------------------------------------------*/

} // namespace Simulator::Http

/*----------------------------------------------------------------------------*/
