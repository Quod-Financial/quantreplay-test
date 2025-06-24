#include "ih/processors/get_processor.hpp"

#include <fmt/format.h>
#include <pistache/http_defs.h>
#include <pistache/router.h>

#include <cassert>
#include <regex>
#include <utility>

#include "cfg/api/cfg.hpp"
#include "ih/endpoint.hpp"
#include "ih/marshalling/json/venue.hpp"
#include "ih/utils/response_formatters.hpp"
#include "log/logging.hpp"
#include "middleware/routing/generator_admin_channel.hpp"
#include "protocol/admin/generator.hpp"

/*----------------------------------------------------------------------------*/

namespace Simulator::Http {

GetProcessor::GetProcessor(DataBridge::VenueAccessor const& _venueAccessor,
                           DatasourceController const& _datasourceController,
                           ListingController const& _listingController,
                           PriceSeedController const& _priceSeedController,
                           SettingController const& _settingController,
                           VenueController const& _venueController)
    : m_pRedirector(Redirect::RedirectionProcessor::create(_venueAccessor)),
      mVenueAccessor(_venueAccessor),
      mDatasourceController(_datasourceController),
      mListingController(_listingController),
      mPriceSeedController(_priceSeedController),
      mSettingController(_settingController),
      mVenueController(_venueController) {}

void GetProcessor::getVenue(Pistache::Rest::Request const& _request,
                            Pistache::Http::ResponseWriter _response) {
  auto const venueId = _request.param(":id").as<std::string>();
  simulator::log::info("requested venue - {}", venueId);

  auto [code, body] = mVenueController.get().selectVenue(venueId);
  respond(_request, _response, code, body);
}

void GetProcessor::getVenues(Pistache::Rest::Request const& _request,
                             Pistache::Http::ResponseWriter _response) {
  simulator::log::info("requested all venues");

  auto [code, body] = mVenueController.get().selectAllVenues();
  respond(_request, _response, code, body);
}

void GetProcessor::getListing(Pistache::Rest::Request const& _request,
                              Pistache::Http::ResponseWriter _response) {
  auto const symbol = _request.param(":symbol").as<std::string>();
  auto const key = std::regex_replace(symbol, std::regex("%2F"), "/");
  simulator::log::info("requested listing - {}", key);

  auto [code, body] = mListingController.get().selectListing(key);
  respond(_request, _response, code, body);
}

void GetProcessor::getListings(Pistache::Rest::Request const& _request,
                               Pistache::Http::ResponseWriter _response) {
  simulator::log::info("requested all listings");

  auto [code, body] = mListingController.get().selectAllListings();
  respond(_request, _response, code, body);
}

void GetProcessor::getDataSource(Pistache::Rest::Request const& _request,
                                 Pistache::Http::ResponseWriter _response) {
  auto const sourceId = _request.param(":id").as<std::uint64_t>();
  simulator::log::info("requested a data source with identifier - {}",
                       sourceId);

  auto [code, body] = mDatasourceController.get().selectDatasource(sourceId);
  respond(_request, _response, code, body);
}

void GetProcessor::getDataSources(Pistache::Rest::Request const& /*_request*/
                                  ,
                                  Pistache::Http::ResponseWriter _response) {
  simulator::log::info("requested all data sources");

  auto [code, body] = mDatasourceController.get().selectAllDatasources();
  _response.send(code, body);
}

void GetProcessor::getPriceSeed(Pistache::Rest::Request const& _request,
                                Pistache::Http::ResponseWriter _response) {
  auto const id = _request.param(":id").as<std::uint64_t>();
  simulator::log::info("requested price seed - {}", id);

  auto [code, body] = mPriceSeedController.get().selectPriceSeed(id);
  respond(_request, _response, code, body);
}

void GetProcessor::getPriceSeeds(Pistache::Rest::Request const& _request,
                                 Pistache::Http::ResponseWriter _response) {
  simulator::log::info("requested all price seeds");

  auto [code, body] = mPriceSeedController.get().selectAllPriceSeeds();
  respond(_request, _response, code, body);
}

void GetProcessor::getStatus(Pistache::Rest::Request const& _request,
                             Pistache::Http::ResponseWriter _response) {
  Pistache::Http::Code responseCode{};
  std::string responseBody{};

  const auto opResult = mVenueAccessor.get().selectSingle(Cfg::venue().name);
  if (opResult) {
    bool available = false;
    responseBody = getVenueStatusStr(opResult.value(), false, available);
    responseCode = available ? Pistache::Http::Code::Ok
                             : Pistache::Http::Code::Service_Unavailable;
  } else {
    responseCode = Pistache::Http::Code::Service_Unavailable;
    responseBody = formatResultResponse("failed to select venue");
  }

  respond(_request, _response, responseCode, responseBody);
}

void GetProcessor::getVenueStatus(Pistache::Rest::Request const& _request,
                                  Pistache::Http::ResponseWriter _response) {
  auto const venueId = _request.param(":id").as<std::string>();
  simulator::log::info("requested status of venue - {}", venueId);

  bool available = false;
  Pistache::Http::Code responseCode{};
  std::string responseBody{};

  const auto opResult = mVenueAccessor.get().selectSingle(venueId);
  if (opResult) {
    responseBody = getVenueStatusStr(opResult.value(), true, available);
    responseCode = available ? Pistache::Http::Code::Ok
                             : Pistache::Http::Code::Service_Unavailable;
  } else {
    responseCode = Pistache::Http::Code::Service_Unavailable;
    responseBody = formatResultResponse("failed to select venue");
  }

  respond(_request, _response, responseCode, responseBody);
}

void GetProcessor::getVenueStatuses(Pistache::Rest::Request const& _request,
                                    Pistache::Http::ResponseWriter _response) {
  simulator::log::info("requested statuses of all venues");

  Pistache::Http::Code responseCode{};
  std::string responseBody{};
  bool available = false;

  auto const opResult = mVenueAccessor.get().selectAll();
  if (!opResult) {
    responseCode = Pistache::Http::Code::Service_Unavailable;
    responseBody = formatResultResponse("failed to select venues");
    respond(_request, _response, responseCode, responseBody);
    return;
  }

  for (auto const& venue : opResult.value()) {
    if (!responseBody.empty()) {
      responseBody.append(",");
    }
    responseBody.append(getVenueStatusStr(venue, true, available));
  }

  responseCode = Pistache::Http::Code::Ok;
  responseBody =
      std::string("{\"venuestatus\": [").append(responseBody).append("]}");

  respond(_request, _response, responseCode, responseBody);
}

std::string GetProcessor::getVenueStatusStr(DataLayer::Venue const& _venue,
                                            bool _sendResponseCode,
                                            bool& _available) const {
  _available = false;
  std::string const& venueId = _venue.getVenueId();

  if (venueId == Cfg::venue().name) {
    _available = true;
  } else if (auto restPort = _venue.getRestPort()) {
    auto result = m_pRedirector->redirectToVenue(
        venueId,
        Pistache::Http::Method::Get,
        fmt::format(simulator::http::endpoint::VenueStatusByVenueIdFmt, venueId));
    _available = result.getHttpCode() == Pistache::Http::Code::Ok;
  }

  int const responseCode = _available ? 200 : 503;
  return formatVenueStatus(_venue, _sendResponseCode ? responseCode : 0);
}

void GetProcessor::getSettings(
    [[maybe_unused]] Pistache::Rest::Request const& _request,
    Pistache::Http::ResponseWriter _response) {
  simulator::log::info("requested all settings");

  auto [code, body] = mSettingController.get().selectAllSettings();
  respond(_request, _response, code, body);
}

void GetProcessor::getOrderGenStatus(Pistache::Rest::Request const& _request,
                                     Pistache::Http::ResponseWriter _response) {
  auto const instanceID = _request.param(":venueId").as<std::string>();
  simulator::log::info(
      "received request to retrieve random order generator status for {}",
      instanceID);

  if (instanceID == Cfg::venue().name) {
    handleGenerationStatusRequest(_request, std::move(_response));
  } else {
    auto const redirectResponse = redirect(_request, instanceID);
    respond(_request,
            _response,
            redirectResponse.getHttpCode(),
            redirectResponse.getBodyContent());
  }
}

void GetProcessor::handleGenerationStatusRequest(
    Pistache::Rest::Request const& _request,
    Pistache::Http::ResponseWriter _response) {
  simulator::protocol::GenerationStatusRequest request;
  simulator::protocol::GenerationStatusReply reply;

  try {
    simulator::middleware::send_admin_request(request, reply);
  } catch (const simulator::middleware::ChannelUnboundError&) {
  }

  if (reply.status ==
      simulator::protocol::GenerationStatusReply::Status::Stopped) {
    respond(_request,
            _response,
            Pistache::Http::Code::Ok,
            formatResultResponse("Stopped"));
  } else if (reply.status ==
             simulator::protocol::GenerationStatusReply::Status::Running) {
    respond(_request,
            _response,
            Pistache::Http::Code::Ok,
            formatResultResponse("Running"));
  } else {
    respond(_request,
            _response,
            Pistache::Http::Code::Internal_Server_Error,
            formatResultResponse("Failed to retrieve generation status"));
  }
}

void GetProcessor::respond(Pistache::Rest::Request const& _request,
                           Pistache::Http::ResponseWriter& _response,
                           Pistache::Http::Code _code,
                           std::string const& _body) {
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

Redirect::Result GetProcessor::redirect(Pistache::Rest::Request const& _request,
                                        std::string const& _instanceID) const {
  assert(m_pRedirector);
  return m_pRedirector->redirectToVenue(
      _instanceID, _request.method(), _request.resource());
}

}  // namespace Simulator::Http
