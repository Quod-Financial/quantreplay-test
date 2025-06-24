#ifndef SIMULATOR_HTTP_IH_CONTROLLERS_LISTING_CONTROLLER_HPP_
#define SIMULATOR_HTTP_IH_CONTROLLERS_LISTING_CONTROLLER_HPP_

#include <cstdint>
#include <functional>
#include <string>
#include <utility>

#include <pistache/http_defs.h>

#include "ih/data_bridge/listing_accessor.hpp"

namespace Simulator::Http {

class ListingController {
  public:
    using Result = std::pair<Pistache::Http::Code, std::string>;

    explicit ListingController(
        DataBridge::ListingAccessor& _dataAccessor
    ) noexcept;


    [[nodiscard]]
    auto selectListing(std::string const& _key) const -> Result;

    [[nodiscard]]
    auto selectAllListings() const -> Result;


    [[nodiscard]]
    auto insertListing(std::string const& _body) const -> Result;


    [[nodiscard]]
    auto updateListing(
        std::string const& _key,
        std::string const& _body
    ) const -> Result;

  private:
    static auto formatErrorResponse(DataBridge::Failure _failure)
        -> std::string;

    std::reference_wrapper<DataBridge::ListingAccessor const> mDataAccessor;
};


} // namespace Simulator::Http


#endif // SIMULATOR_HTTP_IH_CONTROLLERS_LISTING_CONTROLLER_HPP_
