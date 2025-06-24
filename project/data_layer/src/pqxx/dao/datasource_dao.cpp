#include "ih/pqxx/dao/datasource_dao.hpp"

#include <pqxx/connection>

#include <algorithm>
#include <utility>
#include <vector>

#include "api/validations/datasource.hpp"

#include "ih/pqxx/database/connector.hpp"
#include "ih/pqxx/database/transaction.hpp"
#include "ih/pqxx/database/value_sanitizer.hpp"
#include "ih/pqxx/queries/datasource_queries.hpp"
#include "ih/pqxx/result/datasource_parser.hpp"
#include "log/logging.hpp"

namespace Simulator::DataLayer::Pqxx {

DatasourceDao::DatasourceDao(pqxx::connection _pqxxConnection) noexcept :
    mConnection(std::move(_pqxxConnection))
{}

auto DatasourceDao::setupWith(Pqxx::Context const& _context) -> DatasourceDao
{
    return DatasourceDao{Pqxx::Connector::connect(_context)};
}

auto DatasourceDao::execute(InsertCommand& _command) -> void
{
    const Datasource::Patch& snapshot = _command.getInitialPatch();
    if (const auto valid = Validation::valid(snapshot); !valid.has_value()) {
        throw MalformedPatch{valid.error()};
    }

    Transaction transaction{mConnection};

    Datasource inserted = insert(snapshot, transaction.handler());

    transaction.commit();
    _command.setResult(std::move(inserted));
}

auto DatasourceDao::execute(SelectOneCommand& _command) -> void
{
    Transaction transaction{mConnection};
    Predicate const& predicate = _command.getPredicate();

    Datasource selected = selectSingle(predicate, transaction.handler());

    transaction.commit();
    _command.setResult(std::move(selected));
}

auto DatasourceDao::execute(SelectAllCommand& _command) -> void
{
    Transaction transaction{mConnection};

    std::vector<Datasource> selected =
        selectAll(_command.getPredicate(), transaction.handler());

    transaction.commit();
    _command.setResult(std::move(selected));
}

auto DatasourceDao::execute(UpdateOneCommand& _command) -> void
{
    const Datasource::Patch& patch = _command.getPatch();
    if (const auto valid = Validation::valid(patch); !valid.has_value()) {
        throw MalformedPatch{valid.error()};
    }

    Transaction transaction{mConnection};
    Predicate const& predicate = _command.getPredicate();

    Datasource updated = update(patch, predicate, transaction.handler());

    transaction.commit();
    _command.setResult(std::move(updated));
}

auto DatasourceDao::insert(
    Datasource::Patch const& _snapshot,
    Transaction::Handler _trh
) -> Datasource
{
    using Query = DatasourceQuery::Insert;

    ValueSanitizer sanitizer{mConnection};
    std::string const query =
        Query::prepare(_snapshot, sanitizer).returningId().compose();

    simulator::log::debug("executing `{}'", query);
    pqxx::row const result = [&] {
        try {
            return _trh.exec1(query);
        } catch (std::exception const& _ex) {
            simulator::log::warn(
              "datasource insertion failed, error: `{}'", _ex.what());
            throw;
        }
    }();
    simulator::log::debug("datasource insertion query executed");

    simulator::log::debug("decoding an inserted datasource record identifier");
    std::uint64_t const insertedId = DatasourceParser::parseIdentifier(result);
    simulator::log::debug(
        "inserted datasource identifier was decoded - `{}'", insertedId);

    if (const auto& column_mapping = _snapshot.columns_mapping()) {
        insertColumnsMapping(insertedId, *column_mapping, _trh);
    }

    return selectSingle(insertedId, _trh);
}

auto DatasourceDao::selectSingle(
    Predicate const& _pred,
    Transaction::Handler _trh
) -> Datasource
{
    using Query = DatasourceQuery::Select;
    ValueSanitizer sanitizer{mConnection};
    std::string const query = Query::prepare().by(_pred, sanitizer).compose();

    simulator::log::debug("executing `{}'", query);
    pqxx::row const selected = _trh.exec1(query);
    simulator::log::debug("datasource selection query executed");

    Datasource selectedDatasource = decodeDatasource(selected, _trh);
    return selectedDatasource;
}

auto DatasourceDao::selectSingle(
    std::uint64_t _datasourceId,
    Transaction::Handler _trh
) -> Datasource
{
    using Query = DatasourceQuery::Select;
    ValueSanitizer sanitizer{mConnection};
    std::string const query =
        Query::prepare().byDatasourceId(_datasourceId, sanitizer).compose();

    simulator::log::debug("executing `{}'", query);
    pqxx::row const selected = _trh.exec1(query);
    simulator::log::debug("datasource selection query executed");

    Datasource selectedDatasource = decodeDatasource(selected, _trh);
    simulator::log::info(
        "selected a datasource with `{}' identifier", _datasourceId);
    return selectedDatasource;
}

auto DatasourceDao::selectAll(
    std::optional<Predicate> const& _pred,
    Transaction::Handler _trh
) -> std::vector<Datasource>
{
    using Query = DatasourceQuery::Select;
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
        "Datasource selection query executed, {} record retrieved",
        selected.size());

    std::vector<Datasource> selectedDatasources{};
    selectedDatasources.reserve(static_cast<std::size_t>(selected.size()));
    for (pqxx::row const& row : selected) {
        selectedDatasources.emplace_back(decodeDatasource(row, _trh));
    }

    return selectedDatasources;
}

auto DatasourceDao::update(
    Datasource::Patch const& _patch,
    Predicate const& _pred,
    Transaction::Handler _trh
) -> Datasource
{
    using Query = DatasourceQuery::Update;

    ValueSanitizer sanitizer{mConnection};
    std::string const query = Query::prepare(_patch, sanitizer)
                                  .by(_pred, sanitizer)
                                  .returningId()
                                  .compose();

    simulator::log::debug("executing `{}'", query);
    pqxx::row const result = _trh.exec1(query);
    simulator::log::debug("datasource update query was executed");

    simulator::log::debug("decoding an updated datasource record identifier");
    std::uint64_t const updatedId = DatasourceParser::parseIdentifier(result);
    simulator::log::debug(
        "updated datasource identifier was decoded - `{}'", updatedId);

    if (auto const& patchedMapping = _patch.columns_mapping()) {
        dropColumnsMapping(updatedId, _trh);
        insertColumnsMapping(updatedId, *patchedMapping, _trh);
    }

    return selectSingle(updatedId, _trh);
}

auto DatasourceDao::decodeDatasource(
    pqxx::row const& _row,
    Transaction::Handler _trh
) const -> Datasource
{
    std::uint64_t const selectedId = DatasourceParser::parseIdentifier(_row);
    Datasource::Patch selected = DatasourceParser::parse(_row);

    auto columnsMapping = selectColumnsMapping(selectedId, _trh);
    std::for_each(
        std::make_move_iterator(columnsMapping.begin()),
        std::make_move_iterator(columnsMapping.end()),
        [&](ColumnMapping::Patch&& _entry) {
            selected.with_column_mapping(std::move(_entry));
        }
    );

    return Datasource::create(std::move(selected), selectedId);
}

auto DatasourceDao::insertColumnsMapping(
    std::uint64_t _datasourceId,
    std::vector<ColumnMapping::Patch> _patches,
    Transaction::Handler _trh
) const -> void
{
    std::vector<ColumnMapping> mapping{};
    mapping.reserve(_patches.size());
    std::transform(
        std::make_move_iterator(_patches.begin()),
        std::make_move_iterator(_patches.end()),
        std::back_inserter(mapping),
        [_datasourceId](ColumnMapping::Patch&& _patch) {
            return ColumnMapping::create(std::move(_patch), _datasourceId);
        }
    );

    insertColumnsMapping(mapping, _trh);
}

auto DatasourceDao::insertColumnsMapping(
    std::vector<ColumnMapping> const& _columnMappings,
    Transaction::Handler _trh
) const -> void
{
    using Query = ColumnMappingQuery::Insert;

    ValueSanitizer sanitizer{mConnection};
    simulator::log::debug(
        "inserting {} column mapping records", _columnMappings.size());

    for (auto const& mapping : _columnMappings) {
        std::string const query = Query::prepare(mapping, sanitizer).compose();
        simulator::log::debug("executing `{}", query);
        try {
            _trh.exec0(query);
            simulator::log::debug("column mapping insertion query executed");
        } catch (std::exception const& _ex) {
            simulator::log::warn(
                "failed to inset a column mapping, error occurred: `{}'",
                _ex.what()
            );
        }
    }
}

auto DatasourceDao::selectColumnsMapping(
    std::uint64_t _datasourceId,
    Transaction::Handler _trh
) const -> std::vector<ColumnMapping::Patch>
{
    using Query = ColumnMappingQuery::Select;

    ValueSanitizer sanitizer{mConnection};
    std::string const query =
        Query::prepare().byDatasourceId(_datasourceId, sanitizer).compose();

    simulator::log::debug("executing `{}'", query);
    pqxx::result const selected = _trh.exec(query);
    simulator::log::debug(
        "{} column mapping records selected with `{}' datasource identifier",
        selected.size(),
        _datasourceId);

    std::vector<ColumnMapping::Patch> patches{};
    patches.reserve(static_cast<std::size_t>(selected.size()));

    for (pqxx::row const& row : selected) {
        try {
            patches.emplace_back(ColumnMappingParser::parse(row));
        } catch (std::exception const& _ex) {
            simulator::log::warn(
                "failed to decode a column mapping, error occurred: `{}'",
                _ex.what()
            );
        }
    }

    simulator::log::debug("{} column mapping records decoded", patches.size());
    return patches;
}

auto DatasourceDao::dropColumnsMapping(
    std::uint64_t _datasourceId,
    Transaction::Handler _trh
) const -> void
{
    using Query = ColumnMappingQuery::Delete;

    ValueSanitizer sanitizer{mConnection};
    std::string const query =
        Query::prepare().byDatasourceId(_datasourceId, sanitizer).compose();

    simulator::log::debug("executing `{}'", query);
    pqxx::result const result = _trh.exec0(query);
    simulator::log::debug(
        "{} column mapping records deleted with `{}' datasource identifier",
        result.affected_rows(),
        _datasourceId);
}

} // namespace Simulator::DataLayer::Pqxx
