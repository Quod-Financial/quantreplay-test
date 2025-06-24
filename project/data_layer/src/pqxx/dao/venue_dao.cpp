#include "ih/pqxx/dao/venue_dao.hpp"

#include <pqxx/row>

#include <algorithm>
#include <utility>

#include "api/inspectors/venue.hpp"
#include "ih/pqxx/database/connector.hpp"
#include "ih/pqxx/database/transaction.hpp"
#include "ih/pqxx/database/value_sanitizer.hpp"
#include "ih/pqxx/queries/venue_queries.hpp"
#include "ih/pqxx/result/venue_parser.hpp"
#include "log/logging.hpp"

namespace Simulator::DataLayer::Pqxx {

VenueDao::VenueDao(pqxx::connection _pqxxConnection) noexcept :
    mConnection(std::move(_pqxxConnection))
{}

auto VenueDao::setupWith(Pqxx::Context const& _context) -> VenueDao
{
    return VenueDao{Pqxx::Connector::connect(_context)};
}

auto VenueDao::execute(InsertCommand& _command) -> void
{
    Transaction transaction{mConnection};
    Venue::Patch const& snapshot = _command.getInitialPatch();

    Venue inserted = insert(snapshot, transaction.handler());

    transaction.commit();
    _command.setResult(std::move(inserted));
}

auto VenueDao::execute(SelectOneCommand& _command) -> void
{
    Transaction transaction{mConnection};
    Predicate const& predicate = _command.getPredicate();

    Venue selected = selectSingle(predicate, transaction.handler());

    transaction.commit();
    _command.setResult(std::move(selected));
}

auto VenueDao::execute(SelectAllCommand& _command) -> void
{
    Transaction transaction{mConnection};

    std::vector<Venue> selected =
        selectAll(_command.getPredicate(), transaction.handler());

    transaction.commit();
    _command.setResult(std::move(selected));
}

auto VenueDao::execute(UpdateOneCommand& _command) -> void
{
    Transaction transaction{mConnection};
    Venue::Patch const& patch = _command.getPatch();
    Predicate const& predicate = _command.getPredicate();

    Venue updated = update(patch, predicate, transaction.handler());

    transaction.commit();
    _command.setResult(std::move(updated));
}

auto VenueDao::insert(Venue::Patch const& _snapshot, Transaction::Handler _trh)
    -> Venue
{
    using Query = VenueQuery::Insert;

    ValueSanitizer sanitizer{mConnection};
    std::string const query =
        Query::prepare(_snapshot, sanitizer).returningId().compose();

    simulator::log::debug("executing `{}'", query);
    pqxx::row const result = [&] {
        try {
            return _trh.exec1(query);
        } catch (std::exception const& _ex) {
          simulator::log::warn(
              "venue insertion failed, error: `{}'", _ex.what());
            throw;
        }
    }();
    simulator::log::debug("venue insertion query executed");

    simulator::log::debug("decoding an inserted venue record identifier");
    std::string const insertedId = [&] {
        try {
            return result.at(0).as<std::string>();
        } catch (...) {
            simulator::log::warn(
              "failed to decode an inserted venue record id");
            throw DataDecodingError(
                "unable to decode an identifier of inserted venue record"
            );
        }
    }();
    simulator::log::debug(
        "inserted venue identifier was decoded - `{}'", insertedId);

    if (const auto& phases = _snapshot.getMarketPhases()) {
      insertPhases(insertedId, *phases, _trh);
    }

    return selectSingle(insertedId, _trh);
}

auto VenueDao::selectSingle(Predicate const& _pred, Transaction::Handler _trh)
    -> Venue
{
    using Query = VenueQuery::Select;
    ValueSanitizer sanitizer{mConnection};
    std::string const query = Query::prepare().by(_pred, sanitizer).compose();

    simulator::log::debug("executing `{}'", query);
    pqxx::row const selected = _trh.exec1(query);
    simulator::log::debug("venue selection query executed");

    Venue selectedVenue = decodeVenue(selected, _trh);
    return selectedVenue;
}

auto VenueDao::selectSingle(
    std::string const& _venueId,
    Transaction::Handler _trh
) -> Venue
{
    using Query = VenueQuery::Select;
    ValueSanitizer sanitizer{mConnection};
    std::string const query =
        Query::prepare().byVenueId(_venueId, sanitizer).compose();

    simulator::log::debug("executing `{}'", query);
    pqxx::row const selected = _trh.exec1(query);
    simulator::log::debug("venue selection query executed");

    Venue selectedVenue = decodeVenue(selected, _trh);
    simulator::log::info("selected a venue with `{}' identifier", _venueId);
    return selectedVenue;
}

auto VenueDao::selectAll(
    std::optional<Predicate> const& _pred,
    Transaction::Handler _trh
) -> std::vector<Venue>
{
    using Query = VenueQuery::Select;
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
        "venue selection query executed, {} records retrieved",
        selected.size());

    std::vector<Venue> selectedVenues{};
    selectedVenues.reserve(static_cast<std::size_t>(selected.size()));
    for (pqxx::row const& row : selected) {
        selectedVenues.emplace_back(decodeVenue(row, _trh));
    }

    return selectedVenues;
}

auto VenueDao::update(
    Venue::Patch const& _patch,
    Predicate const& _pred,
    Transaction::Handler _trh
) -> Venue
{
    using Query = VenueQuery::Update;

    ValueSanitizer sanitizer{mConnection};
    std::string const query = Query::prepare(_patch, sanitizer)
                                  .by(_pred, sanitizer)
                                  .returningId()
                                  .compose();

    simulator::log::debug("executing `{}'", query);
    pqxx::row const updatedId = _trh.exec1(query);
    simulator::log::debug("venue update query was executed");

    simulator::log::debug("Decoding an updated venue record identifier");
    std::string const updatedVenueId = [&updatedId] {
        try {
            return updatedId.at(0).as<std::string>();
        } catch (...) {
            throw DataDecodingError(
                "unable to decode an identifier of updated venue record"
            );
        }
    }();
    simulator::log::debug(
        "decoded updated venue identifier - `{}'", updatedVenueId);

    if (auto const& patchedPhases = _patch.getMarketPhases()) {
        dropPhases(updatedVenueId, _trh);
        insertPhases(updatedVenueId, *patchedPhases, _trh);
    }

    return selectSingle(updatedVenueId, _trh);
}

auto VenueDao::decodeVenue(
    pqxx::row const& _venueRow,
    Transaction::Handler _trh
) const -> Venue
{
    Venue::Patch selected = VenueParser::parse(_venueRow);
    std::optional<std::string> const& venueId = selected.getVenueId();
    if (venueId.has_value()) {
        auto phases = selectPhases(*venueId, _trh);
        for (auto& phase : phases) {
            selected.withMarketPhase(std::move(phase));
        }
    }

    return Venue::create(selected);
}

auto VenueDao::insertPhases(
    std::vector<MarketPhase> const& _phases,
    Transaction::Handler _trh
) const -> void
{
    using Query = MarketPhaseQuery::Insert;

    ValueSanitizer sanitizer{mConnection};
    simulator::log::debug("inserting {} market phase records", _phases.size());

    for (auto const& phase : _phases) {
        std::string const query = Query::prepare(phase, sanitizer).compose();
        simulator::log::debug("executing `{}", query);
        try {
            _trh.exec0(query);
            simulator::log::debug("market phase insertion query executed");
        } catch (std::exception const& _ex) {
            simulator::log::warn(
                "failed to inset a market phase, error occurred: `{}'",
                _ex.what()
            );
        }
    }
}

auto VenueDao::insertPhases(
    std::string const& _venueId,
    std::vector<MarketPhase::Patch> _patches,
    Transaction::Handler _trh
) const -> void
{
    std::vector<MarketPhase> phases{};
    phases.reserve(_patches.size());
    std::transform(
        std::make_move_iterator(_patches.begin()),
        std::make_move_iterator(_patches.end()),
        std::back_inserter(phases),
        [&_venueId](MarketPhase::Patch&& _patch) {
            return MarketPhase::create(std::move(_patch), _venueId);
        }
    );

    insertPhases(phases, _trh);
}

auto VenueDao::selectPhases(
    std::string const& _venueId,
    Transaction::Handler _trh
) const -> std::vector<MarketPhase::Patch>
{
    using Query = MarketPhaseQuery::Select;

    ValueSanitizer sanitizer{mConnection};
    std::string const query =
        Query::prepare().byVenueId(_venueId, sanitizer).compose();

    simulator::log::debug("executing `{}'", query);
    pqxx::result const selectedRows = _trh.exec(query);
    simulator::log::debug(
        "{} market phase records selected with `{}' venue identifier",
        selectedRows.size(),
        _venueId
    );

    std::vector<MarketPhase::Patch> selectedPatches{};
    selectedPatches.reserve(static_cast<std::size_t>(selectedRows.size()));

    for (pqxx::row const& phaseRow : selectedRows) {
        try {
            selectedPatches.emplace_back(MarketPhaseParser::parse(phaseRow));
        } catch (std::exception const& _ex) {
            simulator::log::warn(
                "failed to decode a market phase, error occurred: `{}'",
                _ex.what()
            );
        }
    }

    simulator::log::debug(
        "{} market phase records decoded", selectedPatches.size());
    return selectedPatches;
}

auto VenueDao::dropPhases(
    std::string const& _venueId,
    Transaction::Handler _trh
) const -> void
{
    using Query = MarketPhaseQuery::Delete;

    ValueSanitizer sanitizer{mConnection};
    std::string const query =
        Query::prepare().byVenueId(_venueId, sanitizer).compose();

    simulator::log::debug("executing `{}'", query);
    pqxx::result const result = _trh.exec0(query);
    simulator::log::debug(
        "{} market phase records deleted with `{}' venue identifier",
        result.affected_rows(),
        _venueId
    );
}

} // namespace Simulator::DataLayer::Pqxx