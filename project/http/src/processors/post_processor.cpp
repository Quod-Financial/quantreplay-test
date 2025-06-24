#include "ih/processors/post_processor.hpp"

#include <pistache/http_defs.h>

#include "ih/redirect/redirection_processor.hpp"
#include "ih/utils/response_formatters.hpp"
#include "log/logging.hpp"

namespace Simulator::Http {

PostProcessor::PostProcessor(
    DataBridge::VenueAccessor const& _venueAccessor,
    DatasourceController const& _datasourceController,
    ListingController const& _listingController,
    PriceSeedController const& _priceSeedController,
    SettingController const& _settingsController,
    TradingController const& _tradingController,
    VenueController const& _venueController
) : m_pRedirector(Redirect::RedirectionProcessor::create(_venueAccessor)),
    mDatasourceController(_datasourceController),
    mListingController(_listingController),
    mPriceSeedController(_priceSeedController),
    mSettingController(_settingsController),
    mTradingController(_tradingController),
    mVenueController(_venueController)
{}

void PostProcessor::addVenue (
        Pistache::Rest::Request const & _request
    ,   Pistache::Http::ResponseWriter _response
)
{
    simulator::log::info("requested insert of the new venue");

    auto [code, body] = mVenueController.get().insertVenue(_request.body());
    respond(_request, _response, code, body);
}

void PostProcessor::addListing (
        Pistache::Rest::Request const & _request
    ,   Pistache::Http::ResponseWriter _response
)
{
    simulator::log::info("requested insert of the new listing");

    auto [code, body] = mListingController.get().insertListing(_request.body());
    respond(_request, _response, code, body);
}


void PostProcessor::addDataSource (
        Pistache::Rest::Request const & _request
    ,   Pistache::Http::ResponseWriter _response
)
{
    simulator::log::info("requested insert of the new datasource");

    auto [code, body] =
        mDatasourceController.get().insertDatasource(_request.body());
    respond(_request, _response, code, body);
}


void PostProcessor::addPriceSeed (
        Pistache::Rest::Request const & _request
    ,   Pistache::Http::ResponseWriter _response
)
{
    simulator::log::info("requested insert of the new price seed");

    auto [code, body] = mPriceSeedController.get().insertPriceSeed(_request.body());
    respond(_request, _response, code, body);
}

auto PostProcessor::handleStoreRequest(
    const Pistache::Rest::Request& request,
    Pistache::Http::ResponseWriter response) -> void {
  const auto instanceID = request.hasParam(":venueId")
                              ? request.param(":venueId").as<std::string>()
                              : std::string{};

  if (instanceID.empty() || instanceID == Cfg::venue().name) {
    const auto [code, body] = mTradingController.get().store_market_state();
    respond(request, response, code, body);
  } else {
    const auto redirectResponse = redirect(request, instanceID);
    respond(request,
            response,
            redirectResponse.getHttpCode(),
            redirectResponse.getBodyContent());
  }
}

auto PostProcessor::handleRecoverRequest(
    const Pistache::Rest::Request& request,
    Pistache::Http::ResponseWriter response) -> void {
  const auto instanceID = request.hasParam(":venueId")
                              ? request.param(":venueId").as<std::string>()
                              : std::string{};

  if (instanceID.empty() || instanceID == Cfg::venue().name) {
    const auto [code, body] = mTradingController.get().recover_market_state();
    respond(request, response, code, body);
  } else {
    const auto redirectResponse = redirect(request, instanceID);
    respond(request,
            response,
            redirectResponse.getHttpCode(),
            redirectResponse.getBodyContent());
  }
}

void PostProcessor::respond(
        const Pistache::Rest::Request & _request
    ,   Pistache::Http::ResponseWriter & _response
    ,   Pistache::Http::Code _code
    ,   const std::string & _body
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


Redirect::Result PostProcessor::redirect(
        Pistache::Rest::Request const & _request
    ,   std::string const & _instanceID
) const
{
    assert(m_pRedirector);
    return m_pRedirector->redirectToVenue(
            _instanceID
        ,   _request.method()
        ,   _request.resource()
    );
}

} // namespace Simulator::Http
