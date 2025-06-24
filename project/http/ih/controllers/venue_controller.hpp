#ifndef SIMULATOR_HTTP_IH_CONTROLLERS_VENUE_CONTROLLER_HPP_
#define SIMULATOR_HTTP_IH_CONTROLLERS_VENUE_CONTROLLER_HPP_

#include <functional>
#include <string>
#include <utility>

#include <pistache/http_defs.h>

#include "ih/data_bridge/venue_accessor.hpp"

namespace Simulator::Http {

class VenueController {
  public:
    using Result = std::pair<Pistache::Http::Code, std::string>;

    explicit VenueController(DataBridge::VenueAccessor& _dataAccessor) noexcept;


    [[nodiscard]]
    auto selectVenue(std::string const& _venueId) const -> Result;

    [[nodiscard]]
    auto selectAllVenues() const -> Result;


    [[nodiscard]]
    auto insertVenue(std::string const& _body) const -> Result;

    [[nodiscard]]
    auto updateVenue(
        std::string const& _venueId,
        std::string const& _body
    ) const -> Result;

  private:
    static auto formatErrorResponse(DataBridge::Failure _failure)
        -> std::string;

    std::reference_wrapper<DataBridge::VenueAccessor const> mDataAccessor;
};

} // namespace Simulator::Http

#endif // SIMULATOR_HTTP_IH_CONTROLLERS_VENUE_CONTROLLER_HPP_
