#include "ih/pqxx/dao/listing_dao.hpp"

#include <pqxx/connection>

#include <utility>

#include "ih/pqxx/database/connector.hpp"
#include "ih/pqxx/database/transaction.hpp"
#include "ih/pqxx/database/value_sanitizer.hpp"
#include "ih/pqxx/queries/listing_queries.hpp"
#include "ih/pqxx/result/listing_parser.hpp"
#include "log/logging.hpp"

namespace Simulator::DataLayer::Pqxx {

ListingDao::ListingDao(pqxx::connection _pqxxConnection) noexcept :
    mConnection(std::move(_pqxxConnection))
{}

auto ListingDao::setupWith(Pqxx::Context const& _context) -> ListingDao
{
    return ListingDao{Pqxx::Connector::connect(_context)};
}

auto ListingDao::execute(InsertCommand& _command) -> void
{
    Transaction transaction{mConnection};
    Listing::Patch const& snapshot = _command.getInitialPatch();

    Listing inserted = insert(snapshot, transaction.handler());

    transaction.commit();
    _command.setResult(std::move(inserted));
}

auto ListingDao::execute(SelectOneCommand& _command) -> void
{
    Transaction transaction{mConnection};
    Predicate const& predicate = _command.getPredicate();

    Listing selected = selectSingle(predicate, transaction.handler());

    transaction.commit();
    _command.setResult(std::move(selected));
}

auto ListingDao::execute(SelectAllCommand& _command) -> void
{
    Transaction transaction{mConnection};

    std::vector<Listing> selected =
        selectAll(_command.getPredicate(), transaction.handler());

    transaction.commit();
    _command.setResult(std::move(selected));
}

auto ListingDao::execute(ListingDao::UpdateOneCommand& _command) -> void
{
    Transaction transaction{mConnection};
    Listing::Patch const& patch = _command.getPatch();
    Predicate const& predicate = _command.getPredicate();

    Listing updated = update(patch, predicate, transaction.handler());

    transaction.commit();
    _command.setResult(std::move(updated));
}

auto ListingDao::insert(
    Listing::Patch const& _snapshot,
    Transaction::Handler _trh
) -> Listing
{
    using Query = ListingQuery::Insert;

    ValueSanitizer sanitizer{mConnection};
    std::string const query =
        Query::prepare(_snapshot, sanitizer).returningId().compose();

    simulator::log::debug("executing `{}'", query);
    pqxx::row const result = [&] {
        try {
            return _trh.exec1(query);
        } catch (std::exception const& _ex) {
            simulator::log::warn(
                "listing insertion failed, error: `{}'",
                _ex.what());
            throw;
        }
    }();
    simulator::log::debug("listing insertion query executed");

    simulator::log::debug("decoding an inserted listing record identifier");
    std::uint64_t const insertedId = ListingParser::parseIdentifier(result);
    simulator::log::debug(
        "inserted listing identifier was decoded - `{}'", insertedId);

    return selectSingle(insertedId, _trh);
}

auto ListingDao::selectSingle(Predicate const& _pred, Transaction::Handler _trh)
    -> Listing
{
    using Query = ListingQuery::Select;
    ValueSanitizer sanitizer{mConnection};
    std::string const query = Query::prepare().by(_pred, sanitizer).compose();

    simulator::log::debug("executing `{}'", query);
    pqxx::row const selected = _trh.exec1(query);
    simulator::log::debug("listing selection query executed");

    Listing selectedListing = decodeListing(selected);
    return selectedListing;
}

auto ListingDao::selectSingle(
    std::uint64_t _listingId,
    Transaction::Handler _trh
) -> Listing
{
    using Query = ListingQuery::Select;
    ValueSanitizer sanitizer{mConnection};
    std::string const query =
        Query::prepare().byListingId(_listingId, sanitizer).compose();

    simulator::log::debug("executing `{}'", query);
    pqxx::row const selected = _trh.exec1(query);
    simulator::log::debug("listing selection query executed");

    Listing selectedListing = decodeListing(selected);
    simulator::log::info(
        "selected listing record with `{}' identifier", _listingId);
    return selectedListing;
}

auto ListingDao::selectAll(
    std::optional<Predicate> const& _pred,
    Transaction::Handler _trh
) -> std::vector<Listing>
{
    using Query = ListingQuery::Select;
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
        "listing selection query executed, {} records retrieved",
        selected.size());

    std::vector<Listing> selectedListings{};
    selectedListings.reserve(static_cast<std::size_t>(selected.size()));
    for (pqxx::row const& row : selected) {
        selectedListings.emplace_back(decodeListing(row));
    }

    return selectedListings;
}

auto ListingDao::update(
    Listing::Patch const& _patch,
    Predicate const& _pred,
    Transaction::Handler _trh
) -> Listing
{
    using Query = ListingQuery::Update;

    ValueSanitizer sanitizer{mConnection};
    std::string const query = Query::prepare(_patch, sanitizer)
                                  .by(_pred, sanitizer)
                                  .returningId()
                                  .compose();

    simulator::log::debug("executing `{}'", query);
    pqxx::row const result = _trh.exec1(query);
    simulator::log::debug("listing update query was executed");

    simulator::log::debug("decoding an updated listing record identifier");
    std::uint64_t const updatedId = ListingParser::parseIdentifier(result);
    simulator::log::debug(
        "updated listing identifier was decoded - `{}'", updatedId);

    return selectSingle(updatedId, _trh);
}

auto ListingDao::decodeListing(pqxx::row const& _row) -> Listing
{
    std::uint64_t const parsedId = ListingParser::parseIdentifier(_row);
    Listing::Patch parsed = ListingParser::parse(_row);
    return Listing::create(std::move(parsed), parsedId);
}

} // namespace Simulator::DataLayer::Pqxx
