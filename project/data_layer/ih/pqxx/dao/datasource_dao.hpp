#ifndef SIMULATOR_DATA_LAYER_IH_PQXX_DAO_DATASOURCE_DAO_HPP_
#define SIMULATOR_DATA_LAYER_IH_PQXX_DAO_DATASOURCE_DAO_HPP_

#include <pqxx/connection>
#include <pqxx/row>
#include <pqxx/transaction>

#include "api/models/column_mapping.hpp"
#include "api/models/datasource.hpp"
#include "ih/common/command/commands.hpp"
#include "ih/pqxx/context.hpp"
#include "ih/pqxx/database/transaction.hpp"

namespace Simulator::DataLayer::Pqxx {

class DatasourceDao {
  public:
    using Predicate = Predicate::Expression<DataLayer::Datasource>;

    using InsertCommand = Command::Insert<DataLayer::Datasource>;
    using SelectOneCommand = Command::SelectOne<DataLayer::Datasource>;
    using SelectAllCommand = Command::SelectAll<DataLayer::Datasource>;
    using UpdateOneCommand = Command::UpdateOne<DataLayer::Datasource>;

    DatasourceDao() = delete;
    DatasourceDao(DatasourceDao const&) = delete;
    auto operator=(DatasourceDao const&) -> DatasourceDao& = delete;

    DatasourceDao(DatasourceDao&&) noexcept = default;
    auto operator=(DatasourceDao&&) noexcept -> DatasourceDao& = default;
    ~DatasourceDao() = default;

    [[nodiscard]]
    static auto setupWith(Pqxx::Context const& _context) -> DatasourceDao;

    auto execute(InsertCommand& _command) -> void;

    auto execute(SelectOneCommand& _command) -> void;

    auto execute(SelectAllCommand& _command) -> void;

    auto execute(UpdateOneCommand& _command) -> void;

  private:
    explicit DatasourceDao(pqxx::connection _pqxxConnection) noexcept;

    [[nodiscard]]
    auto insert(Datasource::Patch const& _snapshot, Transaction::Handler _trh)
        -> Datasource;

    [[nodiscard]]
    auto selectSingle(Predicate const& _pred, Transaction::Handler _trh)
        -> Datasource;

    [[nodiscard]]
    auto selectSingle(std::uint64_t _datasourceId, Transaction::Handler _trh)
        -> Datasource;

    [[nodiscard]]
    auto selectAll(
        std::optional<Predicate> const& _pred,
        Transaction::Handler _trh
    ) -> std::vector<Datasource>;

    [[nodiscard]]
    auto update(
        Datasource::Patch const& _patch,
        Predicate const& _pred,
        Transaction::Handler _trh
    ) -> Datasource;


    [[nodiscard]]
    auto decodeDatasource(
        pqxx::row const& _row,
        Transaction::Handler _trh
    ) const -> Datasource;

    auto insertColumnsMapping(
        std::uint64_t _datasourceId,
        std::vector<ColumnMapping::Patch> _patches,
        Transaction::Handler _trh
    ) const -> void;

    auto insertColumnsMapping(
        std::vector<ColumnMapping> const& _columnMappings,
        Transaction::Handler _trh
    ) const -> void;

    [[nodiscard]]
    auto selectColumnsMapping(
        std::uint64_t _datasourceId,
        Transaction::Handler _trh
    ) const -> std::vector<ColumnMapping::Patch>;

    auto dropColumnsMapping(
        std::uint64_t _datasourceId,
        Transaction::Handler _trh
    ) const -> void;


    pqxx::connection mConnection;
};

} // namespace Simulator::DataLayer::Pqxx

#endif // SIMULATOR_DATA_LAYER_IH_PQXX_DAO_DATASOURCE_DAO_HPP_
