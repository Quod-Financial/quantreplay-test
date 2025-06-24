#ifndef SIMULATOR_DATA_LAYER_IH_PQXX_DAO_LISTING_DAO_HPP_
#define SIMULATOR_DATA_LAYER_IH_PQXX_DAO_LISTING_DAO_HPP_

#include <pqxx/connection>
#include <pqxx/row>
#include <pqxx/transaction>

#include "api/models/listing.hpp"
#include "ih/common/command/commands.hpp"
#include "ih/pqxx/context.hpp"
#include "ih/pqxx/database/transaction.hpp"

namespace Simulator::DataLayer::Pqxx {

class ListingDao {
  public:
    using Predicate = Predicate::Expression<DataLayer::Listing>;

    using InsertCommand = Command::Insert<DataLayer::Listing>;
    using SelectOneCommand = Command::SelectOne<DataLayer::Listing>;
    using SelectAllCommand = Command::SelectAll<DataLayer::Listing>;
    using UpdateOneCommand = Command::UpdateOne<DataLayer::Listing>;

    ListingDao() = delete;
    ListingDao(ListingDao const&) = delete;
    auto operator=(ListingDao const&) -> ListingDao& = delete;

    ListingDao(ListingDao&&) noexcept = default;
    auto operator=(ListingDao&&) noexcept -> ListingDao& = default;
    ~ListingDao() = default;

    [[nodiscard]]
    static auto setupWith(Pqxx::Context const& _context) -> ListingDao;

    auto execute(InsertCommand& _command) -> void;

    auto execute(SelectOneCommand& _command) -> void;

    auto execute(SelectAllCommand& _command) -> void;

    auto execute(UpdateOneCommand& _command) -> void;

  private:
    explicit ListingDao(pqxx::connection _pqxxConnection) noexcept;

    [[nodiscard]]
    auto insert(Listing::Patch const& _snapshot, Transaction::Handler _trh)
        -> Listing;

    [[nodiscard]]
    auto selectSingle(Predicate const& _pred, Transaction::Handler _trh)
        -> Listing;

    [[nodiscard]]
    auto selectSingle(std::uint64_t _listingId, Transaction::Handler _trh)
        -> Listing;

    [[nodiscard]]
    auto selectAll(
        std::optional<Predicate> const& _pred,
        Transaction::Handler _trh
    ) -> std::vector<Listing>;

    [[nodiscard]]
    auto update(
        Listing::Patch const& _patch,
        Predicate const& _pred,
        Transaction::Handler _trh
    ) -> Listing;

    [[nodiscard]]
    static auto decodeListing(pqxx::row const& _row) -> Listing;


    pqxx::connection mConnection;
};

} // namespace Simulator::DataLayer::Pqxx

#endif // SIMULATOR_DATA_LAYER_IH_PQXX_DAO_LISTING_DAO_HPP_
