#ifndef SIMULATOR_HTTP_TESTS_MOCKS_REDIRECTOR_HPP_
#define SIMULATOR_HTTP_TESTS_MOCKS_REDIRECTOR_HPP_

#include <optional>
#include <utility>

#include <gmock/gmock.h>

#include <pistache/http.h>

#include "ih/redirect/result.hpp"
#include "ih/redirect/redirector.hpp"

namespace Simulator::Http::Mock {

class Redirector
    :   public Redirect::Redirector
{
public:

    MOCK_METHOD(
        RedirectionResult,
        redirect,
        (Redirect::Request const &),
        (const, noexcept, override));

    static auto makeOutput(Redirect::Result _result) -> RedirectionResult
    {
        return std::make_pair(
                std::make_optional(std::move(_result))
            ,   Redirector::Status::Success
        );
    }

    static auto makeOutput(Redirector::Status _status) -> RedirectionResult
    {
        return std::make_pair(std::nullopt, _status);
    }
};

} // namespace Simulator::Http::Mock

#endif // SIMULATOR_HTTP_TESTS_MOCKS_REDIRECTOR_HPP_
