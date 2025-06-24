#include "ih/marshalling/json/listing.hpp"

#include <rapidjson/document.h>

#include "data_layer/api/inspectors/listing.hpp"

#include "ih/marshalling/json/detail/keys.hpp"
#include "ih/marshalling/json/detail/marshaller.hpp"
#include "ih/marshalling/json/detail/unmarshaller.hpp"
#include "ih/marshalling/json/detail/utils.hpp"

namespace Simulator::Http::Json {

auto ListingMarshaller::marshall(DataLayer::Listing const& _listing)
    -> std::string
{
    rapidjson::Document listingDoc{};
    marshall(_listing, listingDoc);
    return Json::encode(listingDoc);
}

auto ListingMarshaller::marshall(
    std::vector<DataLayer::Listing> const& _listings
) -> std::string
{
    rapidjson::Document root{};
    root.SetObject();
    auto& allocator = root.GetAllocator();

    rapidjson::Document listingsList{std::addressof(allocator)};
    listingsList.SetObject().SetArray();
    for (auto const& listing : _listings) {
        rapidjson::Document listingDoc{std::addressof(allocator)};
        marshall(listing, listingDoc);
        listingsList.PushBack(listingDoc, allocator);
    }

    constexpr auto key = ListingKey::Listings;
    root.AddMember(Json::makeKey(key), listingsList, allocator);

    return Json::encode(root);
}

auto ListingMarshaller::marshall(
    DataLayer::Listing const& _listing,
    rapidjson::Document& _dest
) -> void
{
    using DataLayer::ListingReader;

    Marshaller marshaller{_dest};
    ListingReader<decltype(marshaller)> reader{marshaller};
    reader.read(_listing);
}

auto ListingUnmarshaller::unmarshall(
    std::string_view _json,
    DataLayer::Listing::Patch& _dest
) -> void
{
    using DataLayer::ListingPatchWriter;

    rapidjson::Document document{};
    document.Parse(_json.data());
    if (!document.IsObject()) {
        throw std::runtime_error{"failed to parse Listing JSON object"};
    }

    Unmarshaller unmarshaller{document};
    ListingPatchWriter<decltype(unmarshaller)> writer{unmarshaller};
    writer.write(_dest);
}

} // namespace Simulator::Http::Json