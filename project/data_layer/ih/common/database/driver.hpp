#ifndef SIMULATOR_DATA_LAYER_IH_COMMON_DRIVER_HPP_
#define SIMULATOR_DATA_LAYER_IH_COMMON_DRIVER_HPP_

#include "cfg/api/cfg.hpp"

#include "api/database/context.hpp"
#include "ih/common/database/context_resolver.hpp"
#include "ih/pqxx/context.hpp"

namespace Simulator::DataLayer::Database {

class Driver final {
  public:
    [[nodiscard]]
    static auto configure(Cfg::DbConfiguration const& _cfg)
        -> Database::Context;

    [[nodiscard]]
    static auto makeDatabaseContext(Pqxx::Context _pqxx) -> Database::Context;

  private:
    Driver() = default;

    [[nodiscard]]
    static auto setupPqxxBackend(Cfg::DbConfiguration const& _cfg)
        -> Database::Context;
};

} // namespace Simulator::DataLayer::Database

#endif // SIMULATOR_DATA_LAYER_IH_COMMON_DRIVER_HPP_
