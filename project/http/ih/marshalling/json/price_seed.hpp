#ifndef SIMULATOR_HTTP_IH_MARSHALLING_JSON_PRICE_SEED_HPP_
#define SIMULATOR_HTTP_IH_MARSHALLING_JSON_PRICE_SEED_HPP_

#include <string>
#include <string_view>
#include <vector>

#include <rapidjson/document.h>

#include "data_layer/api/models/price_seed.hpp"

namespace Simulator::Http::Json {

// TODO: add tests
class PriceSeedMarshaller {
  public:
    static auto marshall(DataLayer::PriceSeed const& _seed) -> std::string;

    static auto marshall(std::vector<DataLayer::PriceSeed> const& _seeds)
        -> std::string;

  private:
    static auto marshall(
        DataLayer::PriceSeed const& _seed,
        rapidjson::Document& _dest
    ) -> void;
};


// TODO: add tests
class PriceSeedUnmarshaller {
  public:
    static auto unmarshall(
        std::string_view _json,
        DataLayer::PriceSeed::Patch& _dest
    ) -> void;
};

} // namespace Simulator::Http::Json

#endif // SIMULATOR_HTTP_IH_MARSHALLING_JSON_PRICE_SEED_HPP_
