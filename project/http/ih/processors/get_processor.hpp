#ifndef __SIMULATOR_HTTP_SRC_ROUTER__GET_PROCESSOR_HPP__
#define __SIMULATOR_HTTP_SRC_ROUTER__GET_PROCESSOR_HPP__

#include <functional>
#include <memory>

#include <pistache/http.h>
#include <pistache/router.h>

#include "data_layer/api/models/venue.hpp"

#include "ih/controllers/datasource_controller.hpp"
#include "ih/controllers/listing_controller.hpp"
#include "ih/controllers/price_seed_controller.hpp"
#include "ih/controllers/setting_controller.hpp"
#include "ih/controllers/venue_controller.hpp"
#include "ih/data_bridge/venue_accessor.hpp"
#include "ih/redirect/result.hpp"

#include "ih/redirect/redirection_processor.hpp"

namespace Simulator::Http {

class GetProcessor {
  public:
    explicit GetProcessor(
        DataBridge::VenueAccessor const& _venueAccessor,
        DatasourceController const& _datasourceController,
        ListingController const& _listingController,
        PriceSeedController const& _priceSeedController,
        SettingController const& _settingController,
        VenueController const& _venueController
    );

    void getVenue(
        Pistache::Rest::Request const& _request,
        Pistache::Http::ResponseWriter _response
    );

    void getVenues(
        Pistache::Rest::Request const& _request,
        Pistache::Http::ResponseWriter _response
    );


    void getListing(
        Pistache::Rest::Request const& _request,
        Pistache::Http::ResponseWriter _response
    );

    void getListings(
        Pistache::Rest::Request const& _request,
        Pistache::Http::ResponseWriter _response
    );


    void getDataSource(
        Pistache::Rest::Request const& _request,
        Pistache::Http::ResponseWriter _response
    );

    void getDataSources(
        Pistache::Rest::Request const& _request,
        Pistache::Http::ResponseWriter _response
    );


    void getPriceSeed(
        Pistache::Rest::Request const& _request,
        Pistache::Http::ResponseWriter _response
    );

    void getPriceSeeds(
        Pistache::Rest::Request const& _request,
        Pistache::Http::ResponseWriter _response
    );


    void getStatus(
        Pistache::Rest::Request const& _request,
        Pistache::Http::ResponseWriter _response
    );


    void getVenueStatus(
        Pistache::Rest::Request const& _request,
        Pistache::Http::ResponseWriter _response
    );

    void getVenueStatuses(
        Pistache::Rest::Request const& _request,
        Pistache::Http::ResponseWriter _response
    );


    void getSettings(
        Pistache::Rest::Request const& _request,
        Pistache::Http::ResponseWriter _response
    );


    void getOrderGenStatus(
        Pistache::Rest::Request const& _request,
        Pistache::Http::ResponseWriter _response
    );


    auto getVenueStatusStr(
        DataLayer::Venue const& _venue,
        bool _sendResponseCode,
        bool& _available
    ) const -> std::string;

  private:
    void handleGenerationStatusRequest(
        Pistache::Rest::Request const& _request,
        Pistache::Http::ResponseWriter _response
    );

    static void respond(
        Pistache::Rest::Request const& _request,
        Pistache::Http::ResponseWriter& _response,
        Pistache::Http::Code _code,
        std::string const& _body
    );

    Redirect::Result redirect(
        Pistache::Rest::Request const& _request,
        std::string const& _instanceID
    ) const;

    std::shared_ptr<Redirect::RedirectionProcessor> m_pRedirector;

    std::reference_wrapper<DataBridge::VenueAccessor const> mVenueAccessor;

    std::reference_wrapper<DatasourceController const> mDatasourceController;
    std::reference_wrapper<ListingController const> mListingController;
    std::reference_wrapper<PriceSeedController const> mPriceSeedController;
    std::reference_wrapper<SettingController const> mSettingController;
    std::reference_wrapper<VenueController const> mVenueController;
};

} // namespace Simulator::Http

#endif // __SIMULATOR_HTTP_SRC_ROUTER__GET_PROCESSOR_HPP__
