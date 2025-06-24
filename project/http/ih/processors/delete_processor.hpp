#ifndef __SIMULATOR_HTTP_SRC_ROUTER__DELETE_PROCESSOR_HPP__
#define __SIMULATOR_HTTP_SRC_ROUTER__DELETE_PROCESSOR_HPP__

#include <functional>

#include <pistache/http.h>
#include <pistache/router.h>

#include "ih/controllers/price_seed_controller.hpp"

namespace Simulator::Http {

class DeleteProcessor {
  public:
    explicit DeleteProcessor(
        PriceSeedController const& _priceSeedController
    ) noexcept;

    void deletePriceSeed(
        Pistache::Rest::Request const& _request,
        Pistache::Http::ResponseWriter _response
    );

  private:
    static void respond(
        Pistache::Rest::Request const& _request,
        Pistache::Http::ResponseWriter& _response,
        Pistache::Http::Code _code,
        std::string const& _body
    );

    std::reference_wrapper<PriceSeedController const> mPriceSeedController;
};

} // namespace Simulator::Http

#endif // __SIMULATOR_HTTP_SRC_ROUTER__DELETE_PROCESSOR_HPP__
