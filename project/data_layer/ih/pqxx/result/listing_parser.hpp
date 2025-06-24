#ifndef SIMULATOR_DATA_LAYER_IH_PQXX_RESULT_LISTING_PARSER_HPP_
#define SIMULATOR_DATA_LAYER_IH_PQXX_RESULT_LISTING_PARSER_HPP_

#include <pqxx/row>

#include "api/exceptions/exceptions.hpp"
#include "api/inspectors/listing.hpp"
#include "api/models/listing.hpp"
#include "ih/pqxx/result/detail/basic_row_parser.hpp"

namespace Simulator::DataLayer::Pqxx {

class ListingParser {
  public:
    explicit ListingParser(pqxx::row const& _databaseRow) noexcept :
        mRowParser(_databaseRow)
    {}

    auto parseInto(Listing::Patch& _destinationPatch) -> void
    {
        ListingPatchWriter<decltype(mRowParser)> writer{mRowParser};
        writer.write(_destinationPatch);
    }

    static auto parse(pqxx::row const& _databaseRow) -> Listing::Patch
    {
        Listing::Patch parsed{};
        ListingParser parser{_databaseRow};
        parser.parseInto(parsed);
        return parsed;
    }

    static auto parseIdentifier(pqxx::row const& _databaseRow) -> std::uint64_t
    {
        Detail::BasicRowParser rowParser{_databaseRow};

        std::uint64_t listingId{};
        if (rowParser(Listing::Attribute::ListingId, listingId)) {
            return listingId;
        }

        throw DataDecodingError(
            "failed to decode a listing record identifier "
            "from the database row"
        );
    }

  private:
    Detail::BasicRowParser mRowParser;
};

} // namespace Simulator::DataLayer::Pqxx

#endif // SIMULATOR_DATA_LAYER_IH_PQXX_RESULT_LISTING_PARSER_HPP_
