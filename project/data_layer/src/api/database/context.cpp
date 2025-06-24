#include "api/database/context.hpp"

#include <variant>

#include "ih/common/database/context_resolver.hpp"
#include "ih/common/database/driver.hpp"
#include "ih/pqxx/context.hpp"

namespace Simulator::DataLayer::Database {

class Context::Implementation {
    using GenericDmbsContext = std::variant<Pqxx::Context>;

  public:
    Implementation() = delete;

    explicit Implementation(Pqxx::Context _pqxxContext) noexcept;

    static auto create(Pqxx::Context _pqxx) -> Database::Context;

    void accept(Database::ContextResolver& _resolver) const;

  private:
    GenericDmbsContext mDbmsContext;
};

Context::Implementation::Implementation(Pqxx::Context _pqxxContext) noexcept :
    mDbmsContext(std::move(_pqxxContext))
{}

auto Context::Implementation::create(Pqxx::Context _pqxxContext)
    -> Database::Context
{
    Database::Context dbmsContext{};
    auto impl = std::make_shared<Implementation>(std::move(_pqxxContext));
    dbmsContext.mImplementation = std::move(impl);
    return dbmsContext;
}

void Context::Implementation::accept(Database::ContextResolver& _resolver) const
{
    std::visit(
        [&_resolver](auto const& _concreteContext) {
            _resolver.executeWith(_concreteContext);
        },
        mDbmsContext
    );
}

} // namespace Simulator::DataLayer::Database


namespace Simulator::DataLayer::Database {

void Context::accept(Database::ContextResolver& _resolver) const
{
    assert(mImplementation);
    mImplementation->accept(_resolver);
}

} // namespace Simulator::DataLayer::Database


namespace Simulator::DataLayer::Database {

auto Driver::configure(Cfg::DbConfiguration const& _cfg) -> Database::Context
{
    // For the time being, only PostgreSQL is supported by the project
    return Driver::setupPqxxBackend(_cfg);
}

auto Driver::makeDatabaseContext(Pqxx::Context _pqxx) -> Database::Context
{
    return Context::Implementation::create(std::move(_pqxx));
}

auto Driver::setupPqxxBackend(Cfg::DbConfiguration const& _cfg)
    -> Database::Context
{
    Pqxx::Context pqxxContext{};
    pqxxContext.configure(_cfg);
    return Driver::makeDatabaseContext(std::move(pqxxContext));
}

} // namespace Simulator::DataLayer::Database