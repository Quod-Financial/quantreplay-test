#ifndef SIMULATOR_PROJECT_DATA_LAYER_IH_PQXX_RESULT_VENUE_PARSER_HPP_
#define SIMULATOR_PROJECT_DATA_LAYER_IH_PQXX_RESULT_VENUE_PARSER_HPP_

#include <pqxx/row>

#include "api/inspectors/market_phase.hpp"
#include "api/inspectors/venue.hpp"
#include "api/models/market_phase.hpp"
#include "api/models/venue.hpp"
#include "ih/pqxx/result/detail/basic_row_parser.hpp"

namespace Simulator::DataLayer::Pqxx {

class MarketPhaseParser {
  public:
    explicit MarketPhaseParser(pqxx::row const& _databaseRow) noexcept :
        mRowParser(_databaseRow)
    {}

    auto parseInto(MarketPhase::Patch& _destinationPatch) -> void
    {
        MarketPhasePatchWriter<decltype(mRowParser)> writer{mRowParser};
        writer.write(_destinationPatch);
    }

    static auto parse(pqxx::row const& _databaseRow) -> MarketPhase::Patch
    {
        MarketPhase::Patch parsed{};
        MarketPhaseParser parser{_databaseRow};
        parser.parseInto(parsed);
        return parsed;
    }

  private:
    Detail::BasicRowParser mRowParser;
};

class VenueParser {
  public:
    explicit VenueParser(pqxx::row const& _databaseRow) noexcept :
        mRowParser(_databaseRow)
    {}

    auto parseInto(Venue::Patch& _destinationPatch) -> void
    {
        VenuePatchWriter<decltype(mRowParser)> writer{mRowParser};
        writer.write(_destinationPatch);
    }

    static auto parse(pqxx::row const& _databaseRow) -> Venue::Patch
    {
        Venue::Patch parsed{};
        VenueParser parser{_databaseRow};
        parser.parseInto(parsed);
        return parsed;
    }

  private:
    Detail::BasicRowParser mRowParser;
};

} // namespace Simulator::DataLayer::Pqxx

#endif // SIMULATOR_PROJECT_DATA_LAYER_IH_PQXX_RESULT_VENUE_PARSER_HPP_
