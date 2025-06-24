#ifndef SIMULATOR_HTTP_API_HTTP_HANDLER_FACTORY_HPP_
#define SIMULATOR_HTTP_API_HTTP_HANDLER_FACTORY_HPP_

#include <cstdint>
#include <memory>

#include "data_layer/api/database/context.hpp"

namespace Simulator::Http {

class IHttpHandler;

class IHttpHandlerFactory {
  public:
    virtual ~IHttpHandlerFactory() = default;

    virtual std::unique_ptr<IHttpHandler> createHttpHandler(
        std::uint16_t _port,
        DataLayer::Database::Context _databaseContext
    ) = 0;

    static std::unique_ptr<IHttpHandlerFactory> createFactory();
};

} // namespace Simulator::Http

#endif // SIMULATOR_HTTP_API_HTTP_HANDLER_FACTORY_HPP_