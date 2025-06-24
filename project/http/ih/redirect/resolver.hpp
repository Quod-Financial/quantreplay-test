#ifndef SIMULATOR_HTTP_IH_REDIRECT_RESOLVER_HPP_
#define SIMULATOR_HTTP_IH_REDIRECT_RESOLVER_HPP_

#include <stdexcept>
#include <string>

#include "ih/redirect/command.hpp"
#include "ih/redirect/destination.hpp"

namespace Simulator::Http::Redirect {

class Resolver
{
public:

    enum class Status
    {
        Success,
        NonexistentInstance,
        ResolvingFailed,
        UnknownError
    };

    using ResolvingResult = std::pair<std::optional<Destination>, Status>;

    virtual ~Resolver() = default;

    virtual ResolvingResult resolveByVenueID(
        std::string const & _venueID
    ) const noexcept = 0;
};

} // namespace Simulator::Http::Redirect

#endif // SIMULATOR_HTTP_IH_REDIRECT_RESOLVER_HPP_
