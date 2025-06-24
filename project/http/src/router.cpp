#include "ih/router.hpp"

#include <fmt/format.h>
#include <fmt/ranges.h>
#include <pistache/http.h>
#include <pistache/router.h>

#include "cfg/api/cfg.hpp"
#include "core/version.hpp"
#include "ih/endpoint.hpp"
#include "ih/headers/x_api_version.hpp"
#include "ih/utils/response_formatters.hpp"
#include "log/logging.hpp"

template <>
struct fmt::formatter<Pistache::Address> : fmt::formatter<std::string_view> {
  using formattable = Pistache::Address;

  auto format(const formattable& address, format_context& context) const
      -> decltype(context.out()) {
    return format_to(context.out(),
                     "{}:{}",
                     address.host(),
                     static_cast<std::uint16_t>(address.port()));
  }
};

template <>
struct fmt::formatter<Pistache::Http::Header::Raw>
    : fmt::formatter<std::string_view> {
  using formattable = Pistache::Http::Header::Raw;

  auto format(const formattable& raw, format_context& context) const
      -> decltype(context.out()) {
    return format_to(context.out(), "\"{}\"", raw.value());
  }
};

template <>
struct fmt::formatter<Pistache::Http::Request>
    : fmt::formatter<std::string_view> {
  using formattable = Pistache::Http::Request;

  auto format(const formattable& request, format_context& context) const
      -> decltype(context.out()) {
    using Pistache::Http::methodString;
    format_to(context.out(), "Request={{ ");
    format_to(context.out(), "Method={}, ", methodString(request.method()));
    format_to(context.out(), "Resource='{}', ", request.resource());
    format_to(context.out(), "Origin='{}', ", request.address());
    format_to(context.out(), "Body='{}', ", beautify_body(request.body()));
    format_to(context.out(), "Header='{}'", request.headers().rawList());
    return format_to(context.out(), " }}");
  }

 private:
  static auto beautify_body(std::string body) -> std::string {
    // That's not a fully correct approach, as we can drop spaces inside
    // JSON body attributes.
    constexpr auto remover = [](char character) -> bool {
      return character == ' ' || character == '\n';
    };

    body.erase(std::remove_if(std::begin(body), std::end(body), remover),
               std::end(body));

    return body;
  }
};

namespace database = Simulator::DataLayer::Database;

namespace simulator::http {

namespace {
auto check_api_version(const Pistache::Http::Request& request) -> bool {
  if (!Simulator::Cfg::http().check_api_version) {
    return true;
  }

  const auto api_version = request.headers().tryGet<XApiVersion>();
  if (!api_version) {
    return true;
  }

  if (const auto version = api_version->version(); version) {
    return *version == core::major_version();
  }
  return false;
}
}  // namespace

// TODO(avovchen): Temporal routes handler, that must be removed
//                 as soon as proper controllers are written
static const auto dummy_handler =
    [](const auto& /*request*/,
       auto /*response*/) -> Pistache::Rest::Route::Result {
  throw std::runtime_error("request handling is not implemented");
};

Router::Router([[maybe_unused]] database::Context database)
    : datasource_accessor_(database),
      listing_accessor_(database),
      price_seed_accessor_(database),
      settings_accessor_(database),
      venue_accessor_(database),
      datasource_controller_(datasource_accessor_),
      listing_controller_(listing_accessor_),
      price_seed_controller_(price_seed_accessor_, settings_accessor_),
      setting_controller_(settings_accessor_),
      trading_controller_(),
      venue_controller_(venue_accessor_),
      get_processor_(venue_accessor_,
                     datasource_controller_,
                     listing_controller_,
                     price_seed_controller_,
                     setting_controller_,
                     venue_controller_),
      post_processor_(venue_accessor_,
                      datasource_controller_,
                      listing_controller_,
                      price_seed_controller_,
                      setting_controller_,
                      trading_controller_,
                      venue_controller_),
      put_processor_(venue_accessor_,
                     datasource_controller_,
                     listing_controller_,
                     price_seed_controller_,
                     setting_controller_,
                     trading_controller_,
                     venue_controller_),
      delete_processor_(price_seed_controller_) {
  init_generator_admin_routers();
  init_matching_engine_admin_routes();
  init_admin_routes();
  init_venue_routes();
  init_listing_routes();
  init_price_seed_routes();
  init_datasource_routes();
  init_setting_routes();
}

void Router::onRequest(const Pistache::Http::Request& request,
                       Pistache::Http::ResponseWriter response) {
  log::info("received request: {}", request);
  if (check_api_version(request)) {
    router_.route(request, std::move(response));
  } else {
    static const auto error_message =
        fmt::format("API version mismatch. API version '{}' is supported.",
                    core::major_version());
    log::warn("received request was rejected due to API version mismatch");
    response.send(Pistache::Http::Code::Precondition_Failed,
                  Simulator::Http::formatResultResponse(error_message));
  }
}

auto Router::init_generator_admin_routers() -> void {
  using Simulator::Http::GetProcessor;
  using Simulator::Http::PutProcessor;

  Pistache::Rest::Routes::Get(
      router_,
      endpoint::GenStatus,
      Pistache::Rest::Routes::bind(&GetProcessor::getOrderGenStatus,
                                   &get_processor_));

  Pistache::Rest::Routes::Put(
      router_,
      endpoint::GenStart,
      Pistache::Rest::Routes::bind(&PutProcessor::startOrderGen,
                                   &put_processor_));

  Pistache::Rest::Routes::Put(
      router_,
      endpoint::GenStop,
      Pistache::Rest::Routes::bind(&PutProcessor::stopOrderGen,
                                   &put_processor_));
}

auto Router::init_matching_engine_admin_routes() -> void {
  using Simulator::Http::PostProcessor;
  using Simulator::Http::PutProcessor;

  Pistache::Rest::Routes::Post(
      router_,
      endpoint::Store,
      Pistache::Rest::Routes::bind(&PostProcessor::handleStoreRequest,
                                   &post_processor_));

  Pistache::Rest::Routes::Post(
      router_,
      endpoint::StoreById,
      Pistache::Rest::Routes::bind(&PostProcessor::handleStoreRequest,
                                   &post_processor_));

  Pistache::Rest::Routes::Post(
      router_,
      endpoint::Recover,
      Pistache::Rest::Routes::bind(&PostProcessor::handleRecoverRequest,
                                   &post_processor_));

  Pistache::Rest::Routes::Post(
      router_,
      endpoint::RecoverById,
      Pistache::Rest::Routes::bind(&PostProcessor::handleRecoverRequest,
                                   &post_processor_));

  Pistache::Rest::Routes::Put(
      router_,
      endpoint::Halt,
      Pistache::Rest::Routes::bind(&PutProcessor::halt_phase, &put_processor_));

  Pistache::Rest::Routes::Put(
      router_,
      endpoint::Resume,
      Pistache::Rest::Routes::bind(&PutProcessor::resume_phase,
                                   &put_processor_));
}

auto Router::init_admin_routes() -> void {
  using Simulator::Http::GetProcessor;

  Pistache::Rest::Routes::Get(
      router_,
      endpoint::Status,
      Pistache::Rest::Routes::bind(&GetProcessor::getStatus, &get_processor_));

  Pistache::Rest::Routes::Get(
      router_,
      endpoint::VenueStatusByVenueId,
      Pistache::Rest::Routes::bind(&GetProcessor::getVenueStatus,
                                   &get_processor_));

  Pistache::Rest::Routes::Get(
      router_,
      endpoint::VenueStatus,
      Pistache::Rest::Routes::bind(&GetProcessor::getVenueStatuses,
                                   &get_processor_));
}

auto Router::init_venue_routes() -> void {
  using Simulator::Http::GetProcessor;
  using Simulator::Http::PostProcessor;
  using Simulator::Http::PutProcessor;

  Pistache::Rest::Routes::Get(
      router_,
      endpoint::VenuesById,
      Pistache::Rest::Routes::bind(&GetProcessor::getVenue, &get_processor_));

  Pistache::Rest::Routes::Get(
      router_,
      endpoint::Venues,
      Pistache::Rest::Routes::bind(&GetProcessor::getVenues, &get_processor_));

  Pistache::Rest::Routes::Post(
      router_,
      endpoint::Venues,
      Pistache::Rest::Routes::bind(&PostProcessor::addVenue, &post_processor_));

  Pistache::Rest::Routes::Put(router_,
                              endpoint::VenuesById,
                              Pistache::Rest::Routes::bind(
                                  &PutProcessor::updateVenue, &put_processor_));
}

auto Router::init_listing_routes() -> void {
  using Simulator::Http::GetProcessor;
  using Simulator::Http::PostProcessor;
  using Simulator::Http::PutProcessor;

  Pistache::Rest::Routes::Get(
      router_,
      endpoint::ListingsBySymbol,
      Pistache::Rest::Routes::bind(&GetProcessor::getListing, &get_processor_));

  Pistache::Rest::Routes::Get(router_,
                              endpoint::Listings,
                              Pistache::Rest::Routes::bind(
                                  &GetProcessor::getListings, &get_processor_));

  Pistache::Rest::Routes::Post(
      router_,
      endpoint::Listings,
      Pistache::Rest::Routes::bind(&PostProcessor::addListing,
                                   &post_processor_));

  Pistache::Rest::Routes::Put(
      router_,
      endpoint::ListingsBySymbol,
      Pistache::Rest::Routes::bind(&PutProcessor::updateListing,
                                   &put_processor_));
}

auto Router::init_price_seed_routes() -> void {
  using Simulator::Http::DeleteProcessor;
  using Simulator::Http::GetProcessor;
  using Simulator::Http::PostProcessor;
  using Simulator::Http::PutProcessor;

  Pistache::Rest::Routes::Get(
      router_,
      endpoint::PriceSeedsById,
      Pistache::Rest::Routes::bind(&GetProcessor::getPriceSeed,
                                   &get_processor_));

  Pistache::Rest::Routes::Get(
      router_,
      endpoint::PriceSeeds,
      Pistache::Rest::Routes::bind(&GetProcessor::getPriceSeeds,
                                   &get_processor_));

  Pistache::Rest::Routes::Post(
      router_,
      endpoint::PriceSeeds,
      Pistache::Rest::Routes::bind(&PostProcessor::addPriceSeed,
                                   &post_processor_));

  Pistache::Rest::Routes::Put(
      router_,
      endpoint::PriceSeedsById,
      Pistache::Rest::Routes::bind(&PutProcessor::updatePriceSeed,
                                   &put_processor_));

  Pistache::Rest::Routes::Put(
      router_,
      endpoint::SyncPriceSeeds,
      Pistache::Rest::Routes::bind(&PutProcessor::syncPriceSeeds,
                                   &put_processor_));

  Pistache::Rest::Routes::Delete(
      router_,
      endpoint::PriceSeedsById,
      Pistache::Rest::Routes::bind(&DeleteProcessor::deletePriceSeed,
                                   &delete_processor_));
}

auto Router::init_datasource_routes() -> void {
  using Simulator::Http::GetProcessor;
  using Simulator::Http::PostProcessor;
  using Simulator::Http::PutProcessor;

  Pistache::Rest::Routes::Get(
      router_,
      endpoint::DataSourcesById,
      Pistache::Rest::Routes::bind(&GetProcessor::getDataSource,
                                   &get_processor_));

  Pistache::Rest::Routes::Get(
      router_,
      endpoint::DataSources,
      Pistache::Rest::Routes::bind(&GetProcessor::getDataSources,
                                   &get_processor_));

  Pistache::Rest::Routes::Post(
      router_,
      endpoint::DataSources,
      Pistache::Rest::Routes::bind(&PostProcessor::addDataSource,
                                   &post_processor_));

  Pistache::Rest::Routes::Put(
      router_,
      endpoint::DataSourcesById,
      Pistache::Rest::Routes::bind(&PutProcessor::updateDataSource,
                                   &put_processor_));
}

auto Router::init_setting_routes() -> void {
  using Simulator::Http::GetProcessor;
  using Simulator::Http::PutProcessor;

  Pistache::Rest::Routes::Get(router_,
                              endpoint::Settings,
                              Pistache::Rest::Routes::bind(
                                  &GetProcessor::getSettings, &get_processor_));

  Pistache::Rest::Routes::Put(
      router_,
      endpoint::Settings,
      Pistache::Rest::Routes::bind(&PutProcessor::updateSettings,
                                   &put_processor_));
}

}  // namespace simulator::http