#ifndef SIMULATOR_HTTP_IH_REDIRECT_DESTINATION_RESOLVER_HPP_
#define SIMULATOR_HTTP_IH_REDIRECT_DESTINATION_RESOLVER_HPP_

#include <functional>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>

#include "ih/data_bridge/venue_accessor.hpp"
#include "ih/redirect/resolver.hpp"

namespace Simulator::Http::Redirect {

class DestinationResolver final
    :   public Resolver
{
public:

    DestinationResolver() = delete;

    explicit DestinationResolver(
        DataBridge::VenueAccessor const& _venueAccessor,
        bool useVenueIdAsPeerHost = false
    ) noexcept;

    ResolvingResult resolveByVenueID(
        std::string const & _venueID
    ) const noexcept override;

    static std::shared_ptr<DestinationResolver> create(
        DataBridge::VenueAccessor const& _venueAccessor
    );

private:

    std::reference_wrapper<DataBridge::VenueAccessor const> mVenueAccessor;
    bool mUseVenueIdAsPeerHost = false;
};

} // namespace Simulator::Http::Redirect

#endif // SIMULATOR_HTTP_SRC_REDIRECT_DESTINATION_RESOLVER_HPP_
