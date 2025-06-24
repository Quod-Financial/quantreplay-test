#ifndef SIMULATOR_HTTP_IH_MARSHALLING_JSON_LISTING_HPP_
#define SIMULATOR_HTTP_IH_MARSHALLING_JSON_LISTING_HPP_

#include <string>
#include <string_view>
#include <vector>

#include <rapidjson/document.h>

#include "data_layer/api/models/listing.hpp"

namespace Simulator::Http::Json {

// TODO: add tests
class ListingMarshaller {
  public:
    static auto marshall(DataLayer::Listing const& _listing)
        -> std::string;

    static auto marshall(std::vector<DataLayer::Listing> const& _listings)
        -> std::string;

  private:
    static auto marshall(
        DataLayer::Listing const& _listing,
        rapidjson::Document& _dest
    ) -> void;
};


// TODO: add tests
class ListingUnmarshaller {
  public:
    static auto unmarshall(
        std::string_view _json,
        DataLayer::Listing::Patch& _dest
    ) -> void;
};

} // namespace Simulator::Http::Json

#endif // SIMULATOR_HTTP_IH_MARSHALLING_JSON_LISTING_HPP_
