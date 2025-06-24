#ifndef SIMULATOR_DATA_LAYER_IH_PQXX_DAO_SETTING_DAO_HPP_
#define SIMULATOR_DATA_LAYER_IH_PQXX_DAO_SETTING_DAO_HPP_

#include <string>
#include <vector>

#include <pqxx/connection>
#include <pqxx/row>
#include <pqxx/transaction>

#include "api/models/setting.hpp"
#include "ih/common/command/commands.hpp"
#include "ih/pqxx/context.hpp"
#include "ih/pqxx/database/transaction.hpp"

namespace Simulator::DataLayer::Pqxx {

class SettingDao {
  public:
    using Predicate = Predicate::Expression<DataLayer::Setting>;

    using InsertCommand = Command::Insert<DataLayer::Setting>;
    using SelectOneCommand = Command::SelectOne<DataLayer::Setting>;
    using SelectAllCommand = Command::SelectAll<DataLayer::Setting>;
    using UpdateOneCommand = Command::UpdateOne<DataLayer::Setting>;

    SettingDao() = delete;
    SettingDao(SettingDao const&) = delete;
    auto operator=(SettingDao const&) -> SettingDao& = delete;

    SettingDao(SettingDao&&) noexcept = default;
    auto operator=(SettingDao&&) noexcept -> SettingDao& = default;
    ~SettingDao() = default;

    [[nodiscard]]
    static auto setupWith(Pqxx::Context const& _context) -> SettingDao;

    auto execute(InsertCommand& _command) -> void;

    auto execute(SelectOneCommand& _command) -> void;

    auto execute(SelectAllCommand& _command) -> void;

    auto execute(UpdateOneCommand& _command) -> void;

  private:
    explicit SettingDao(pqxx::connection _pqxxConnection) noexcept;

    [[nodiscard]]
    auto insert(Setting::Patch const& _snapshot, Transaction::Handler _trh)
        -> Setting;

    [[nodiscard]]
    auto selectSingle(Predicate const& _pred, Transaction::Handler _trh)
        -> Setting;

    [[nodiscard]]
    auto selectSingle(std::string const& _key, Transaction::Handler _trh)
        -> Setting;

    [[nodiscard]]
    auto selectAll(
        std::optional<Predicate> const& _pred,
        Transaction::Handler _trh
    ) -> std::vector<Setting>;

    [[nodiscard]]
    auto update(
        Setting::Patch const& _patch,
        Predicate const& _pred,
        Transaction::Handler _trh
    ) -> Setting;

    [[nodiscard]]
    static auto decodeSetting(pqxx::row const& _row) -> Setting;


    pqxx::connection mConnection;
};

} // namespace Simulator::DataLayer::Pqxx

#endif // SIMULATOR_DATA_LAYER_IH_PQXX_DAO_SETTING_DAO_HPP_
