#ifndef SIMULATOR_HTTP_IH_MARSHALLING_JSON_VENUE_HPP_
#define SIMULATOR_HTTP_IH_MARSHALLING_JSON_VENUE_HPP_

#include <string>
#include <string_view>
#include <vector>

#include <rapidjson/document.h>

#include "data_layer/api/models/market_phase.hpp"
#include "data_layer/api/models/venue.hpp"

namespace Simulator::Http::Json {

class VenueMarshaller {
  public:
    static auto marshall(DataLayer::Venue const& _venue) -> std::string;

    static auto marshall(std::vector<DataLayer::Venue> const& _venues)
        -> std::string;

  private:
    static auto marshall(
        DataLayer::Venue const& _venue,
        rapidjson::Document& _dest
    ) -> void;

    static auto marshall(
        std::vector<DataLayer::MarketPhase> const& _phases,
        rapidjson::Document& _parent
    ) -> void;
};


class VenueUnmarshaller {
  public:
    static auto unmarshall(
        std::string_view _json,
        DataLayer::Venue::Patch& _dest
    ) -> void;

  private:
    static auto unmarshallPhases(
        rapidjson::Document const& _venueDoc,
        DataLayer::Venue::Patch& _dest
    ) -> void;
};


} // namespace Simulator::Http::Json

#endif // SIMULATOR_HTTP_IH_MARSHALLING_JSON_VENUE_HPP_
