#include "ih/pqxx/dao/setting_dao.hpp"

#include <pqxx/connection>

#include <string>
#include <utility>
#include <vector>

#include "ih/pqxx/database/connector.hpp"
#include "ih/pqxx/database/transaction.hpp"
#include "ih/pqxx/database/value_sanitizer.hpp"
#include "ih/pqxx/queries/setting_queries.hpp"
#include "ih/pqxx/result/setting_parser.hpp"
#include "log/logging.hpp"

namespace Simulator::DataLayer::Pqxx {

SettingDao::SettingDao(pqxx::connection _pqxxConnection) noexcept :
    mConnection(std::move(_pqxxConnection))
{}

auto SettingDao::setupWith(Pqxx::Context const& _context) -> SettingDao
{
    return SettingDao{Pqxx::Connector::connect(_context)};
}

auto SettingDao::execute(InsertCommand& _command) -> void
{
    Transaction transaction{mConnection};
    Setting::Patch const& snapshot = _command.getInitialPatch();

    Setting inserted = insert(snapshot, transaction.handler());

    transaction.commit();
    _command.setResult(std::move(inserted));
}

auto SettingDao::execute(SelectOneCommand& _command) -> void
{
    Transaction transaction{mConnection};
    Predicate const& predicate = _command.getPredicate();

    Setting selected = selectSingle(predicate, transaction.handler());

    transaction.commit();
    _command.setResult(std::move(selected));
}

auto SettingDao::execute(SelectAllCommand& _command) -> void
{
    Transaction transaction{mConnection};

    std::vector<Setting> selected =
        selectAll(_command.getPredicate(), transaction.handler());

    transaction.commit();
    _command.setResult(std::move(selected));
}

auto SettingDao::execute(UpdateOneCommand& _command) -> void
{
    Transaction transaction{mConnection};
    Setting::Patch const& patch = _command.getPatch();
    Predicate const& predicate = _command.getPredicate();

    Setting updated = update(patch, predicate, transaction.handler());

    transaction.commit();
    _command.setResult(std::move(updated));
}

auto SettingDao::insert(
    Setting::Patch const& _snapshot,
    Transaction::Handler _trh
) -> Setting
{
    using Query = SettingQuery::Insert;

    ValueSanitizer sanitizer{mConnection};
    std::string const query =
        Query::prepare(_snapshot, sanitizer).returningKey().compose();

    simulator::log::debug("executing `{}'", query);
    pqxx::row const result = [&] {
        try {
            return _trh.exec1(query);
        } catch (std::exception const& _ex) {
            simulator::log::warn(
                "setting insertion failed, error: `{}'",
                _ex.what());
            throw;
        }
    }();
    simulator::log::debug("setting insertion query executed");

    simulator::log::debug("decoding an inserted setting key");
    std::string const insertedKey = SettingParser::parseKey(result);
    simulator::log::debug("new setting key was decoded - `{}'", insertedKey);

    return selectSingle(insertedKey, _trh);
}

auto SettingDao::selectSingle(Predicate const& _pred, Transaction::Handler _trh)
    -> Setting
{
    using Query = SettingQuery::Select;
    ValueSanitizer sanitizer{mConnection};
    std::string const query = Query::prepare().by(_pred, sanitizer).compose();

    simulator::log::debug("executing `{}'", query);
    pqxx::row const selected = _trh.exec1(query);
    simulator::log::debug("setting selection query executed");

    Setting selectedSetting = decodeSetting(selected);
    return selectedSetting;
}

auto SettingDao::selectSingle(
    std::string const& _key,
    Transaction::Handler _trh
) -> Setting
{
    using Query = SettingQuery::Select;
    ValueSanitizer sanitizer{mConnection};
    std::string const query = Query::prepare().byKey(_key, sanitizer).compose();

    simulator::log::debug("executing `{}'", query);
    pqxx::row const selected = _trh.exec1(query);
    simulator::log::debug("setting selection query executed");

    Setting selectedSetting = decodeSetting(selected);
    simulator::log::info("selected setting with `{}' key", _key);
    return selectedSetting;
}

auto SettingDao::selectAll(
    std::optional<Predicate> const& _pred,
    Transaction::Handler _trh
) -> std::vector<Setting>
{
    using Query = SettingQuery::Select;
    ValueSanitizer sanitizer{mConnection};
    std::string const query = [&] {
        if (_pred.has_value()) {
            return Query::prepare().by(*_pred, sanitizer).compose();
        }
        return Query::prepare().compose();
    }();

    simulator::log::debug("executing `{}'", query);
    pqxx::result const selected = _trh.exec(query);
    simulator::log::debug(
        "setting selection query executed, {} records retrieved",
        selected.size());

    std::vector<Setting> selectedSettings{};
    selectedSettings.reserve(static_cast<std::size_t>(selected.size()));
    for (pqxx::row const& row : selected) {
        selectedSettings.emplace_back(decodeSetting(row));
    }

    return selectedSettings;
}

auto SettingDao::update(
    Setting::Patch const& _patch,
    Predicate const& _pred,
    Transaction::Handler _trh
) -> Setting
{
    using Query = SettingQuery::Update;

    ValueSanitizer sanitizer{mConnection};
    std::string const query = Query::prepare(_patch, sanitizer)
                                  .by(_pred, sanitizer)
                                  .returningKey()
                                  .compose();

    simulator::log::debug("executing `{}'", query);
    pqxx::row const result = _trh.exec1(query);
    simulator::log::debug("setting update query was executed");

    simulator::log::debug("decoding an updated setting key");
    std::string const updatedKey = SettingParser::parseKey(result);
    simulator::log::debug("updated setting key was decoded - `{}'", updatedKey);

    return selectSingle(updatedKey, _trh);
}

auto SettingDao::decodeSetting(pqxx::row const& _row) -> Setting
{
    Setting::Patch parsed = SettingParser::parse(_row);
    return Setting::create(std::move(parsed));
}

} // namespace Simulator::DataLayer::Pqxx
