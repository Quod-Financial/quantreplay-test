#ifndef SIMULATOR_HTTP_API_HTTP_HANDLER_HPP_
#define SIMULATOR_HTTP_API_HTTP_HANDLER_HPP_

#include <memory>

#include "mediator/api/services/generator_service.hpp"
#include "mediator/api/services/orderbook_service.hpp"

namespace Simulator::Http {

class IHttpHandler {
  public:
    using GeneratorService = Mediator::Communicators::GeneratorService;
    using OrderBookService = Mediator::Communicators::OrderBookService;

    virtual ~IHttpHandler() = default;

    virtual void setOrderBookService(
        std::shared_ptr<OrderBookService> _pOrderBookService
    ) = 0;

    virtual void setGeneratorService(
        std::shared_ptr<GeneratorService> _pGeneratorService
    ) = 0;

    virtual void launch() = 0;

    virtual void shutdown() noexcept = 0;
};

} // namespace Simulator::Http

#endif // SIMULATOR_HTTP_API_HTTP_HANDLER_HPP_