#ifndef SIMULATOR_HTTP_TESTS_MOCKS_RESOLVER_HPP_
#define SIMULATOR_HTTP_TESTS_MOCKS_RESOLVER_HPP_

#include <optional>
#include <utility>

#include <gmock/gmock.h>

#include "ih/redirect/destination.hpp"
#include "ih/redirect/resolver.hpp"

namespace Simulator::Http::Mock {

class Resolver
    :   public Redirect::Resolver
{
public:

    MOCK_METHOD(
        ResolvingResult,
        resolveByVenueID,
        (std::string const &),
        (const, noexcept, override));

    static auto makeOutput(Redirect::Destination _destination)
        -> Redirect::Resolver::ResolvingResult
    {
        return std::make_pair(
                std::make_optional(std::move(_destination))
            ,   Redirect::Resolver::Status::Success
        );
    }

    static auto makeOutput(Redirect::Resolver::Status _status)
        -> Redirect::Resolver::ResolvingResult
    {
        return std::make_pair(std::nullopt, _status);
    }
};

} // namespace Simulator::Http::Mock

#endif // SIMULATOR_HTTP_TESTS_MOCKS_RESOLVER_HPP_
