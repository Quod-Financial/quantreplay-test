#ifndef SIMULATOR_DATA_LAYER_IH_COMMON_DATABASE_PING_AGENT_HPP_
#define SIMULATOR_DATA_LAYER_IH_COMMON_DATABASE_PING_AGENT_HPP_

#include <functional>

#include "api/database/context.hpp"
#include "ih/common/database/context_resolver.hpp"
#include "ih/pqxx/database/connector.hpp"

namespace Simulator::DataLayer::Database {

class PingAgent final : public ContextResolver {
  public:
    using PqxxAgentStrategy = std::function<void(Pqxx::Context const&)>;

    static auto ping(Database::Context const& _context) noexcept -> bool;


    PingAgent() = delete;

    explicit PingAgent(PqxxAgentStrategy _pqxxAgent) noexcept;


    auto pingWithContext(Database::Context const& _context) noexcept
        -> bool;

  private:
    void executeWith(Pqxx::Context const& _pqxxContext) override;

    PqxxAgentStrategy mPqxxAgent;
};

} // namespace Simulator::DataLayer::Database


#endif // SIMULATOR_DATA_LAYER_IH_COMMON_DATABASE_PING_AGENT_HPP_
