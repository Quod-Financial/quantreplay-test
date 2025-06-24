#ifndef SIMULATOR_DATA_LAYER_IH_PQXX_DAO_PRICE_SEED_DAO_HPP_
#define SIMULATOR_DATA_LAYER_IH_PQXX_DAO_PRICE_SEED_DAO_HPP_

#include <pqxx/connection>
#include <pqxx/row>
#include <pqxx/transaction>

#include "api/models/price_seed.hpp"
#include "ih/common/command/commands.hpp"
#include "ih/pqxx/context.hpp"
#include "ih/pqxx/database/transaction.hpp"

namespace Simulator::DataLayer::Pqxx {

class PriceSeedDao {
  public:
    using Predicate = Predicate::Expression<DataLayer::PriceSeed>;

    using InsertCommand = Command::Insert<DataLayer::PriceSeed>;
    using SelectOneCommand = Command::SelectOne<DataLayer::PriceSeed>;
    using SelectAllCommand = Command::SelectAll<DataLayer::PriceSeed>;
    using UpdateOneCommand = Command::UpdateOne<DataLayer::PriceSeed>;
    using UpdateAllCommand = Command::UpdateAll<DataLayer::PriceSeed>;
    using DeleteOneCommand = Command::DeleteOne<DataLayer::PriceSeed>;
    using DeleteAllCommand = Command::DeleteAll<DataLayer::PriceSeed>;

    PriceSeedDao() = delete;
    PriceSeedDao(PriceSeedDao const&) = delete;
    auto operator=(PriceSeedDao const&) -> PriceSeedDao& = delete;

    PriceSeedDao(PriceSeedDao&&) noexcept = default;
    auto operator=(PriceSeedDao&&) noexcept -> PriceSeedDao& = default;
    ~PriceSeedDao() = default;

    [[nodiscard]]
    static auto setupWith(Pqxx::Context const& _context) -> PriceSeedDao;

    auto execute(InsertCommand& _command) -> void;

    auto execute(SelectOneCommand& _command) -> void;

    auto execute(SelectAllCommand& _command) -> void;

    auto execute(UpdateOneCommand& _command) -> void;

    auto execute(UpdateAllCommand& _command) -> void;

    auto execute(DeleteOneCommand& _command) -> void;

    auto execute(DeleteAllCommand& _command) -> void;

  private:
    explicit PriceSeedDao(pqxx::connection _pqxxConnection) noexcept;

    [[nodiscard]]
    auto insert(PriceSeed::Patch const& _snapshot, Transaction::Handler _trh)
        -> PriceSeed;

    [[nodiscard]]
    auto selectSingle(Predicate const& _pred, Transaction::Handler _trh)
        -> PriceSeed;

    [[nodiscard]]
    auto selectSingle(std::uint64_t _seedId, Transaction::Handler _trh)
        -> PriceSeed;

    [[nodiscard]]
    auto selectAll(
        std::optional<Predicate> const& _pred,
        Transaction::Handler _trh
    ) -> std::vector<PriceSeed>;

    [[nodiscard]]
    auto update(
        PriceSeed::Patch const& _patch,
        Predicate const& _pred,
        Transaction::Handler _trh
    ) -> PriceSeed;

    [[nodiscard]]
    auto updateAll(
        PriceSeed::Patch const& _patch,
        std::optional<Predicate> const& _pred,
        Transaction::Handler _trh
    ) -> std::vector<PriceSeed>;

    auto deleteSingle(Predicate const& _pred, Transaction::Handler _trh)
        -> void;

    auto deleteAll(
        std::optional<Predicate> const& _pred,
        Transaction::Handler _trh
    ) -> void;

    [[nodiscard]]
    static auto decodePriceSeed(pqxx::row const& _row) -> PriceSeed;


    pqxx::connection mConnection;
};

} // namespace Simulator::DataLayer::Pqxx

#endif // SIMULATOR_DATA_LAYER_IH_PQXX_DAO_PRICE_SEED_DAO_HPP_
