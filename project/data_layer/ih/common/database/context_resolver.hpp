#ifndef SIMULATOR_PROJECT_DATA_LAYER_IH_COMMON_CONTEXT_RESOLVER_HPP_
#define SIMULATOR_PROJECT_DATA_LAYER_IH_COMMON_CONTEXT_RESOLVER_HPP_

#include "api/database/context.hpp"
#include "ih/pqxx/context.hpp"

namespace Simulator::DataLayer::Database {

class ContextResolver {
    friend class Database::Context::Implementation;

  public:
    ContextResolver() = default;

    ContextResolver(ContextResolver const&) = default;
    auto operator=(ContextResolver const&) -> ContextResolver& = default;

    ContextResolver(ContextResolver&&) = default;
    auto operator=(ContextResolver&&) -> ContextResolver& = default;

    virtual ~ContextResolver() = default;

  protected:
    void resolve(Database::Context const& _dbmsContext)
    {
        _dbmsContext.accept(*this);
    }

  private:
    virtual void executeWith(Pqxx::Context const& _pqxxContext) = 0;
};

} // namespace Simulator::DataLayer::Database

#endif // SIMULATOR_PROJECT_DATA_LAYER_IH_COMMON_CONTEXT_RESOLVER_HPP_
