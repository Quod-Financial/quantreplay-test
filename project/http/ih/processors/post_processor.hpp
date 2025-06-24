#ifndef __SIMULATOR_HTTP_SRC_ROUTER__POST_PROCESSOR_HPP__
#define __SIMULATOR_HTTP_SRC_ROUTER__POST_PROCESSOR_HPP__

/*----------------------------------------------------------------------------*/

#include <memory>
#include <string>

#include <pistache/http.h>
#include <pistache/router.h>

#include "ih/controllers/datasource_controller.hpp"
#include "ih/controllers/listing_controller.hpp"
#include "ih/controllers/price_seed_controller.hpp"
#include "ih/controllers/setting_controller.hpp"
#include "ih/controllers/trading_controller.hpp"
#include "ih/controllers/venue_controller.hpp"
#include "ih/data_bridge/venue_accessor.hpp"
#include "ih/redirect/redirection_processor.hpp"
#include "ih/redirect/result.hpp"

/*----------------------------------------------------------------------------*/

namespace Simulator {

/*----------------------------------------------------------------------------*/

namespace Mediator {
namespace Communicators {

class OrderBookService;

} // namespace Communicators

class IMediator;

} // namespace Mediator

/*----------------------------------------------------------------------------*/
namespace Http {

/*----------------------------------------------------------------------------*/

/* TODO: There can be more than one entity in one request */
class PostProcessor {
  public:
    PostProcessor(
        DataBridge::VenueAccessor const& _venueAccessor,
        DatasourceController const& _datasourceController,
        ListingController const& _listingController,
        PriceSeedController const& _priceSeedController,
        SettingController const& _settingController,
        TradingController const& _tradingController,
        VenueController const& _venueController
    );


    void addVenue(
        Pistache::Rest::Request const& _request,
        Pistache::Http::ResponseWriter _response
    );

    void addListing(
        Pistache::Rest::Request const& _request,
        Pistache::Http::ResponseWriter _response
    );

    void addDataSource(
        Pistache::Rest::Request const& _request,
        Pistache::Http::ResponseWriter _response
    );

    void addPriceSeed(
        Pistache::Rest::Request const& _request,
        Pistache::Http::ResponseWriter _response
    );

    auto handleStoreRequest(Pistache::Rest::Request const& request,
                            Pistache::Http::ResponseWriter response) -> void;

    auto handleRecoverRequest(Pistache::Rest::Request const& request,
                              Pistache::Http::ResponseWriter response) -> void;

  private:
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


    std::shared_ptr<Mediator::Communicators::OrderBookService> m_pOrderBook;

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

#endif // __SIMULATOR_HTTP_SRC_ROUTER__POST_PROCESSOR_HPP__