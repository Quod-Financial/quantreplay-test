#include "ih/pqxx/dao/price_seed_dao.hpp"

#include <pqxx/connection>

#include <utility>

#include "ih/pqxx/database/connector.hpp"
#include "ih/pqxx/database/transaction.hpp"
#include "ih/pqxx/database/value_sanitizer.hpp"
#include "ih/pqxx/queries/price_seed_queries.hpp"
#include "ih/pqxx/result/price_seed_parser.hpp"
#include "log/logging.hpp"

namespace Simulator::DataLayer::Pqxx {

PriceSeedDao::PriceSeedDao(pqxx::connection _pqxxConnection) noexcept :
    mConnection(std::move(_pqxxConnection))
{}

auto PriceSeedDao::setupWith(Pqxx::Context const& _context) -> PriceSeedDao
{
    return PriceSeedDao{Pqxx::Connector::connect(_context)};
}

auto PriceSeedDao::execute(InsertCommand& _command) -> void
{
    Transaction transaction{mConnection};
    PriceSeed::Patch const& snapshot = _command.getInitialPatch();

    PriceSeed inserted = insert(snapshot, transaction.handler());

    transaction.commit();
    _command.setResult(std::move(inserted));
}

auto PriceSeedDao::execute(SelectOneCommand& _command) -> void
{
    Transaction transaction{mConnection};
    Predicate const& predicate = _command.getPredicate();

    PriceSeed selected = selectSingle(predicate, transaction.handler());

    transaction.commit();
    _command.setResult(std::move(selected));
}

auto PriceSeedDao::execute(SelectAllCommand& _command) -> void
{
    Transaction transaction{mConnection};

    std::vector<PriceSeed> selected =
        selectAll(_command.getPredicate(), transaction.handler());

    transaction.commit();
    _command.setResult(std::move(selected));
}

auto PriceSeedDao::execute(UpdateOneCommand& _command) -> void
{
    Transaction transaction{mConnection};
    PriceSeed::Patch const& patch = _command.getPatch();
    Predicate const& predicate = _command.getPredicate();

    PriceSeed updated = update(patch, predicate, transaction.handler());

    transaction.commit();
    _command.setResult(std::move(updated));
}

auto PriceSeedDao::execute(UpdateAllCommand& _command) -> void
{
    Transaction transaction{mConnection};
    PriceSeed::Patch const& patch = _command.getPatch();
    std::optional<Predicate> const& predicate = _command.getPredicate();

    std::vector<PriceSeed> updated =
        updateAll(patch, predicate, transaction.handler());

    transaction.commit();
    _command.setResult(std::move(updated));
}

auto PriceSeedDao::execute(DeleteOneCommand& _command) -> void
{
    Transaction transaction{mConnection};
    Predicate const& predicate = _command.getPredicate();

    deleteSingle(predicate, transaction.handler());

    transaction.commit();
}

auto PriceSeedDao::execute(DeleteAllCommand& _command) -> void
{
    Transaction transaction{mConnection};
    std::optional<Predicate> const& predicate = _command.getPredicate();

    deleteAll(predicate, transaction.handler());

    transaction.commit();
}

auto PriceSeedDao::insert(
    PriceSeed::Patch const& _snapshot,
    Transaction::Handler _trh
) -> PriceSeed
{
    using Query = PriceSeedQuery::Insert;

    ValueSanitizer sanitizer{mConnection};
    std::string const query =
        Query::prepare(_snapshot, sanitizer).returningId().compose();

    simulator::log::debug("executing `{}'", query);
    pqxx::row const result = [&] {
        try {
            return _trh.exec1(query);
        } catch (std::exception const& _ex) {
            simulator::log::warn(
                "price seed insertion failed, error: `{}'",
                _ex.what());
            throw;
        }
    }();
    simulator::log::debug("price seed insertion query executed");

    simulator::log::debug("decoding an inserted price seed record identifier");
    std::uint64_t const insertedId = PriceSeedParser::parseIdentifier(result);
    simulator::log::debug(
        "inserted price seed identifier was decoded - `{}'",
        insertedId);

    return selectSingle(insertedId, _trh);
}

auto PriceSeedDao::selectSingle(
    Predicate const& _pred,
    Transaction::Handler _trh
) -> PriceSeed
{
    using Query = PriceSeedQuery::Select;
    ValueSanitizer sanitizer{mConnection};
    std::string const query = Query::prepare().by(_pred, sanitizer).compose();

    simulator::log::debug("executing `{}'", query);
    pqxx::row const selected = _trh.exec1(query);
    simulator::log::debug("price seed selection query executed");

    PriceSeed selectedSeed = decodePriceSeed(selected);
    return selectedSeed;
}

auto PriceSeedDao::selectSingle(
    std::uint64_t _seedId,
    Transaction::Handler _trh
) -> PriceSeed
{
    using Query = PriceSeedQuery::Select;
    ValueSanitizer sanitizer{mConnection};
    std::string const query =
        Query::prepare().byPriceSeedId(_seedId, sanitizer).compose();

    simulator::log::debug("executing `{}'", query);
    pqxx::row const selected = _trh.exec1(query);
    simulator::log::debug("price seed selection query executed");

    PriceSeed selectedSeed = decodePriceSeed(selected);
    simulator::log::info(
        "selected price seed record with `{}' identifier",
        _seedId);
    return selectedSeed;
}

auto PriceSeedDao::selectAll(
    std::optional<Predicate> const& _pred,
    Transaction::Handler _trh
) -> std::vector<PriceSeed>
{
    using Query = PriceSeedQuery::Select;
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
        "price seed selection query executed, {} records retrieved",
        selected.size());

    std::vector<PriceSeed> selectedSeeds{};
    selectedSeeds.reserve(static_cast<std::size_t>(selected.size()));
    for (pqxx::row const& row : selected) {
        selectedSeeds.emplace_back(decodePriceSeed(row));
    }

    return selectedSeeds;
}

auto PriceSeedDao::update(
    PriceSeed::Patch const& _patch,
    Predicate const& _pred,
    Transaction::Handler _trh
) -> PriceSeed
{
    using Query = PriceSeedQuery::Update;

    ValueSanitizer sanitizer{mConnection};
    std::string const query = Query::prepare(_patch, sanitizer)
                                  .by(_pred, sanitizer)
                                  .returningId()
                                  .compose();

    simulator::log::debug("executing `{}'", query);
    pqxx::row const result = _trh.exec1(query);
    simulator::log::debug("price seed update query was executed");

    simulator::log::debug("decoding an updated price seed record identifier");
    std::uint64_t const updatedId = PriceSeedParser::parseIdentifier(result);
    simulator::log::debug(
        "updated price seed identifier was decoded - `{}'",
        updatedId);

    return selectSingle(updatedId, _trh);
}

auto PriceSeedDao::updateAll(
    PriceSeed::Patch const& _patch,
    std::optional<Predicate> const& _pred,
    Transaction::Handler _trh
) -> std::vector<PriceSeed>
{
    using Query = PriceSeedQuery::Update;

    ValueSanitizer sanitizer{mConnection};
    std::string const query = [&] {
        if (_pred.has_value()) {
            return Query::prepare(_patch, sanitizer)
                .by(*_pred, sanitizer)
                .returningId()
                .compose();
        }

        return Query::prepare(_patch, sanitizer).returningId().compose();
    }();

    simulator::log::debug("executing `{}'", query);
    pqxx::result const result = _trh.exec(query);
    simulator::log::debug("price seed update query was executed");

    simulator::log::debug("selecting updated price seeds records");
    std::vector<DataLayer::PriceSeed> updatedSeeds{};
    updatedSeeds.reserve(static_cast<std::size_t>(result.size()));
    for (pqxx::row const& row : result) {
        std::uint64_t const updatedId = PriceSeedParser::parseIdentifier(row);
        updatedSeeds.emplace_back(selectSingle(updatedId, _trh));
    }
    simulator::log::debug("{} updated records selected", updatedSeeds.size());
    return updatedSeeds;
}

auto PriceSeedDao::deleteSingle(
    Predicate const& _pred,
    Transaction::Handler _trh
) -> void
{
    using Query = PriceSeedQuery::Delete;
    ValueSanitizer sanitizer{mConnection};
    std::string const query = Query::prepare().by(_pred, sanitizer).compose();

    simulator::log::debug("executing `{}'", query);
    pqxx::result const deletedInfo = _trh.exec0(query);
    simulator::log::debug("price seed delete query executed");

    auto const numDeleted = deletedInfo.affected_rows();
    if (numDeleted != 1) {
        simulator::log::warn(
            "deletion of a single price seed resulted in "
            "deletion of {} price seed records, aborting transaction",
            numDeleted);
        throw CardinalityViolationError();
    }
}

auto PriceSeedDao::deleteAll(
    std::optional<Predicate> const& _pred,
    Transaction::Handler _trh
) -> void
{
    using Query = PriceSeedQuery::Delete;
    ValueSanitizer sanitizer{mConnection};
    std::string const query = [&] {
        if (_pred.has_value()) {
            return Query::prepare().by(*_pred, sanitizer).compose();
        }
        return Query::prepare().compose();
    }();

    simulator::log::debug("executing `{}'", query);
    pqxx::result const deletedInfo = _trh.exec0(query);

    auto const numDeleted = deletedInfo.affected_rows();
    simulator::log::debug(
        "price seed delete query executed, {} records deleted",
        numDeleted);
}

auto PriceSeedDao::decodePriceSeed(pqxx::row const& _row) -> PriceSeed
{
    std::uint64_t const parsedId = PriceSeedParser::parseIdentifier(_row);
    PriceSeed::Patch parsed = PriceSeedParser::parse(_row);
    return PriceSeed::create(std::move(parsed), parsedId);
}

} // namespace Simulator::DataLayer::Pqxx
