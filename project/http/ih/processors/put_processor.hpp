#ifndef __SIMULATOR_HTTP_SRC_ROUTER__PUT_PROCESSOR_HPP__
#define __SIMULATOR_HTTP_SRC_ROUTER__PUT_PROCESSOR_HPP__

/*----------------------------------------------------------------------------*/

#include <functional>

#include <pistache/http.h>
#include <pistache/router.h>

#include "ih/controllers/datasource_controller.hpp"
#include "ih/controllers/listing_controller.hpp"
#include "ih/controllers/price_seed_controller.hpp"
#include "ih/controllers/setting_controller.hpp"
#include "ih/controllers/trading_controller.hpp"
#include "ih/controllers/venue_controller.hpp"
#include "ih/data_bridge/venue_accessor.hpp"
#include "ih/redirect/result.hpp"
#include "ih/redirect/redirection_processor.hpp"

/*----------------------------------------------------------------------------*/

namespace Simulator {

/*----------------------------------------------------------------------------*/

namespace Mediator {

class IMediator;

} // namespace Mediator

/*----------------------------------------------------------------------------*/

namespace Http {

/*----------------------------------------------------------------------------*/

class PutProcessor {
  public:
    PutProcessor(
        DataBridge::VenueAccessor const& _venueAccessor,
        DatasourceController const& _datasourceController,
        ListingController const& _listingController,
        PriceSeedController const& _priceSeedController,
        SettingController const& _settingController,
        TradingController const& _tradingController,
        VenueController const& _venueController
    );

    void updateVenue(
        Pistache::Rest::Request const& _request,
        Pistache::Http::ResponseWriter _response
    );

    void updateListing(
        Pistache::Rest::Request const& _request,
        Pistache::Http::ResponseWriter _response
    );

    void updateDataSource(
        Pistache::Rest::Request const& _request,
        Pistache::Http::ResponseWriter _response
    );

    void updatePriceSeed(
        Pistache::Rest::Request const& _request,
        Pistache::Http::ResponseWriter _response
    );

    void syncPriceSeeds(
        Pistache::Rest::Request const& _request,
        Pistache::Http::ResponseWriter _response
    );

    void updateSettings(
        Pistache::Rest::Request const& _request,
        Pistache::Http::ResponseWriter _response
    );


    void stopOrderGen(
        Pistache::Rest::Request const& _request,
        Pistache::Http::ResponseWriter _response
    );

    void startOrderGen(
        Pistache::Rest::Request const& _request,
        Pistache::Http::ResponseWriter _response
    );

    void halt_phase(const Pistache::Rest::Request& request,
                    Pistache::Http::ResponseWriter response);

    void resume_phase(const Pistache::Rest::Request& request,
                      Pistache::Http::ResponseWriter response);

  private:
    void handleGenerationStartRequest(
        Pistache::Rest::Request const& _request,
        Pistache::Http::ResponseWriter _response
    );

    void handleGenerationStopRequest(
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

    std::reference_wrapper<DatasourceController const> mDatasourceController;
    std::reference_wrapper<ListingController const> mListingController;
    std::reference_wrapper<PriceSeedController const> mPriceSeedController;
    std::reference_wrapper<SettingController const> mSettingController;
    std::reference_wrapper<TradingController const> mTradingController;
    std::reference_wrapper<VenueController const> mVenueController;
};

/*----------------------------------------------------------------------------*/

} // namespace Http
} // namespace Simulator

/*----------------------------------------------------------------------------*/

#endif // __SIMULATOR_HTTP_SRC_ROUTER__PUT_PROCESSOR_HPP__
