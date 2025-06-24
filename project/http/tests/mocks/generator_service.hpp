#ifndef SIMULATOR_HTTP_TESTS_MOCKS_GENERATOR_SERVICE_HPP_
#define SIMULATOR_HTTP_TESTS_MOCKS_GENERATOR_SERVICE_HPP_

#include <gmock/gmock.h>

#include "mediator/api/requests/generator_requests.hpp"
#include "mediator/api/services/generator_service.hpp"

namespace Simulator::Http::Mock {

class GeneratorService
    :   public Mediator::Communicators::GeneratorService
{
public:

    using StatusRequest = Generator::Request::GetGenerationStatus;
    using StartRequest = Generator::Request::StartGeneration;
    using StopRequest = Generator::Request::StopGeneration;

    MOCK_METHOD(void, process, (StatusRequest &), (override));

    MOCK_METHOD(void, process, (StartRequest &), (override));

    MOCK_METHOD(void, process, (StopRequest &), (override));
};

} // namespace Simulator::Http::Mock

#endif // SIMULATOR_HTTP_TESTS_MOCKS_GENERATOR_SERVICE_HPP_
