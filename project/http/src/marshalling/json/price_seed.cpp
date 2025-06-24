#include "ih/marshalling/json/price_seed.hpp"

#include <rapidjson/document.h>

#include "data_layer/api/inspectors/price_seed.hpp"

#include "ih/marshalling/json/detail/keys.hpp"
#include "ih/marshalling/json/detail/marshaller.hpp"
#include "ih/marshalling/json/detail/unmarshaller.hpp"
#include "ih/marshalling/json/detail/utils.hpp"

namespace Simulator::Http::Json {

auto PriceSeedMarshaller::marshall(DataLayer::PriceSeed const& _seed)
    -> std::string
{
    rapidjson::Document seedDoc{};
    marshall(_seed, seedDoc);
    return Json::encode(seedDoc);
}

auto PriceSeedMarshaller::marshall(
    std::vector<DataLayer::PriceSeed> const& _seeds
) -> std::string
{
    rapidjson::Document root{};
    root.SetObject();
    auto& allocator = root.GetAllocator();

    rapidjson::Document seedsList{std::addressof(allocator)};
    seedsList.SetObject().SetArray();
    for (auto const& seed : _seeds) {
        rapidjson::Document seedDoc{std::addressof(allocator)};
        marshall(seed, seedDoc);
        seedsList.PushBack(seedDoc, allocator);
    }

    constexpr auto key = PriceSeedKey::PriceSeeds;
    root.AddMember(Json::makeKey(key), seedsList, allocator);

    return Json::encode(root);
}

auto PriceSeedMarshaller::marshall(
    DataLayer::PriceSeed const& _seed,
    rapidjson::Document& _dest
) -> void
{
    using DataLayer::PriceSeedReader;

    Marshaller marshaller{_dest};
    PriceSeedReader<decltype(marshaller)> reader{marshaller};
    reader.read(_seed);
}

auto PriceSeedUnmarshaller::unmarshall(
    std::string_view _json,
    DataLayer::PriceSeed::Patch& _dest
) -> void
{
    using DataLayer::PriceSeedPatchWriter;

    rapidjson::Document document{};
    document.Parse(_json.data());
    if (!document.IsObject()) {
        throw std::runtime_error{"failed to parse PriceSeed JSON object"};
    }

    Unmarshaller unmarshaller{document};
    PriceSeedPatchWriter<decltype(unmarshaller)> writer{unmarshaller};
    writer.write(_dest);
}

} // namespace Simulator::Http::Json
