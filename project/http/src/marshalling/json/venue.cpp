#include "ih/marshalling/json/venue.hpp"

#include <fmt/format.h>
#include <rapidjson/document.h>

#include "data_layer/api/inspectors/market_phase.hpp"
#include "data_layer/api/inspectors/venue.hpp"

#include "ih/marshalling/json/detail/keys.hpp"
#include "ih/marshalling/json/detail/marshaller.hpp"
#include "ih/marshalling/json/detail/unmarshaller.hpp"
#include "ih/marshalling/json/detail/utils.hpp"

namespace Simulator::Http::Json {

auto VenueMarshaller::marshall(DataLayer::Venue const& _venue) -> std::string
{
    rapidjson::Document venueDoc{};
    marshall(_venue, venueDoc);
    return Json::encode(venueDoc);
}

auto VenueMarshaller::marshall(std::vector<DataLayer::Venue> const& _venues)
    -> std::string
{
    rapidjson::Document root{};
    root.SetObject();
    auto& allocator = root.GetAllocator();

    rapidjson::Document venuesList{std::addressof(allocator)};
    venuesList.SetObject().SetArray();
    for (auto const& venue : _venues) {
        rapidjson::Document venueDoc{std::addressof(allocator)};
        marshall(venue, venueDoc);
        venuesList.PushBack(venueDoc, allocator);
    }

    constexpr auto key = VenueKey::Venues;
    root.AddMember(Json::makeKey(key), venuesList, allocator);

    return Json::encode(root);
}

auto VenueMarshaller::marshall(
    DataLayer::Venue const& _venue,
    rapidjson::Document& _dest
) -> void
{
    using DataLayer::VenueReader;

    Json::Marshaller marshaller{_dest};
    VenueReader<decltype(marshaller)> reader{marshaller};
    reader.read(_venue);
    marshall(_venue.getMarketPhases(), _dest);
}

auto VenueMarshaller::marshall(
    std::vector<DataLayer::MarketPhase> const& _phases,
    rapidjson::Document& _parent
) -> void
{
    using DataLayer::MarketPhaseReader;

    auto& allocator = _parent.GetAllocator();
    rapidjson::Document phasesList{std::addressof(allocator)};
    phasesList.SetObject().SetArray();

    for (auto const& phase : _phases) {
        rapidjson::Document phaseDoc{std::addressof(allocator)};
        Json::Marshaller marshaller{phaseDoc};
        MarketPhaseReader<decltype(marshaller)> reader{marshaller};
        reader.read(phase);

        phasesList.PushBack(phaseDoc, allocator);
    }

    constexpr auto key = VenueKey::MarketPhases;
    _parent.AddMember(Json::makeKey(key), phasesList, allocator);
}

auto VenueUnmarshaller::unmarshall(
    std::string_view _json,
    DataLayer::Venue::Patch& _dest
) -> void
{
    using DataLayer::VenuePatchWriter;

    rapidjson::Document document{};
    document.Parse(_json.data());
    if (!document.IsObject()) {
        throw std::runtime_error{"failed to parse Venue JSON object"};
    }

    Unmarshaller unmarshaller{document};
    VenuePatchWriter<decltype(unmarshaller)> writer{unmarshaller};
    writer.write(_dest);

    unmarshallPhases(document, _dest);
}

auto VenueUnmarshaller::unmarshallPhases(
    rapidjson::Document const& _venueDoc,
    DataLayer::Venue::Patch& _dest
) -> void
{
    using DataLayer::MarketPhasePatchWriter;
    constexpr auto listKey = VenueKey::MarketPhases;

    if (!_venueDoc.HasMember(listKey.data())) {
        return;
    }

    auto const& columnMappingsList = _venueDoc[listKey.data()];
    if (!columnMappingsList.IsArray()) {
        throw std::runtime_error{fmt::format(
            "can not parse `{}' in Venue JSON, which is not a JSON array",
            listKey
        )};
    }

    auto const array = columnMappingsList.GetArray();
    if (array.Empty()) {
        _dest.withoutMarketPhases();
        return;
    }

    for (auto const& object : array) {
        if (!object.IsObject()) {
            throw std::runtime_error{fmt::format(
                "can not parse a JSON object in `{}' JSON array",
                listKey
            )};
        }

        DataLayer::MarketPhase::Patch phase;
        Unmarshaller unmarshaller{object};
        MarketPhasePatchWriter<decltype(unmarshaller)> writer{unmarshaller};
        writer.write(phase);

        _dest.withMarketPhase(std::move(phase));
    }
}

} // namespace Simulator::Http::Json
