#ifndef SIMULATOR_DATA_LAYER_IH_PQXX_DAO_VENUE_DAO_HPP_
#define SIMULATOR_DATA_LAYER_IH_PQXX_DAO_VENUE_DAO_HPP_

#include <pqxx/connection>
#include <pqxx/row>
#include <pqxx/transaction>

#include "api/models/market_phase.hpp"
#include "api/models/venue.hpp"
#include "ih/common/command/commands.hpp"
#include "ih/pqxx/context.hpp"
#include "ih/pqxx/database/transaction.hpp"

namespace Simulator::DataLayer::Pqxx {

class VenueDao {
  public:
    using Predicate = Predicate::Expression<DataLayer::Venue>;

    using InsertCommand = Command::Insert<DataLayer::Venue>;
    using SelectOneCommand = Command::SelectOne<DataLayer::Venue>;
    using SelectAllCommand = Command::SelectAll<DataLayer::Venue>;
    using UpdateOneCommand = Command::UpdateOne<DataLayer::Venue>;

    VenueDao() = delete;
    VenueDao(VenueDao const&) = delete;
    auto operator=(VenueDao const&) -> VenueDao& = delete;

    VenueDao(VenueDao&&) noexcept = default;
    auto operator=(VenueDao&&) noexcept -> VenueDao& = default;
    ~VenueDao() = default;

    [[nodiscard]]
    static auto setupWith(Pqxx::Context const& _context) -> VenueDao;

    auto execute(InsertCommand& _command) -> void;

    auto execute(SelectOneCommand& _command) -> void;

    auto execute(SelectAllCommand& _command) -> void;

    auto execute(UpdateOneCommand& _command) -> void;

  private:
    explicit VenueDao(pqxx::connection _pqxxConnection) noexcept;

    [[nodiscard]]
    auto insert(Venue::Patch const& _snapshot, Transaction::Handler _trh)
        -> Venue;

    [[nodiscard]]
    auto selectSingle(Predicate const& _pred, Transaction::Handler _trh)
        -> Venue;

    [[nodiscard]]
    auto selectSingle(std::string const& _venueId, Transaction::Handler _trh)
        -> Venue;

    [[nodiscard]]
    auto selectAll(
        std::optional<Predicate> const& _pred,
        Transaction::Handler _trh
    ) -> std::vector<Venue>;

    [[nodiscard]]
    auto update(
        Venue::Patch const& _patch,
        Predicate const& _pred,
        Transaction::Handler _trh
    ) -> Venue;


    [[nodiscard]]
    auto decodeVenue(
        pqxx::row const& _venueRow,
        Transaction::Handler _trh
    ) const -> Venue;

    auto insertPhases(
        std::string const& _venueId,
        std::vector<MarketPhase::Patch> _patches,
        Transaction::Handler _trh
    ) const -> void;

    auto insertPhases(
        std::vector<MarketPhase> const& _phases,
        Transaction::Handler _trh
    ) const -> void;

    [[nodiscard]]
    auto selectPhases(
        std::string const& _venueId,
        Transaction::Handler _trh
    ) const -> std::vector<MarketPhase::Patch>;

    auto dropPhases(
        std::string const& _venueId,
        Transaction::Handler _trh
    ) const -> void;


    pqxx::connection mConnection;
};

} // namespace Simulator::DataLayer::Pqxx


#endif // SIMULATOR_DATA_LAYER_IH_PQXX_DAO_VENUE_DAO_HPP_
