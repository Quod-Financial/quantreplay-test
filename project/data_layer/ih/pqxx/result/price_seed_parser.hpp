#ifndef SIMULATOR_DATA_LAYER_IH_PQXX_RESULT_PRICE_SEED_PARSER_HPP_
#define SIMULATOR_DATA_LAYER_IH_PQXX_RESULT_PRICE_SEED_PARSER_HPP_

#include <pqxx/row>

#include "api/exceptions/exceptions.hpp"
#include "api/inspectors/price_seed.hpp"
#include "api/models/price_seed.hpp"
#include "ih/pqxx/result/detail/basic_row_parser.hpp"

namespace Simulator::DataLayer::Pqxx {

class PriceSeedParser {
  public:
    explicit PriceSeedParser(pqxx::row const& _databaseRow) noexcept :
        mRowParser(_databaseRow)
    {}

    auto parseInto(PriceSeed::Patch& _destinationPatch) -> void
    {
        PriceSeedPatchWriter<decltype(mRowParser)> writer{mRowParser};
        writer.write(_destinationPatch);
    }

    static auto parse(pqxx::row const& _databaseRow) -> PriceSeed::Patch
    {
        PriceSeed::Patch parsed{};
        PriceSeedParser parser{_databaseRow};
        parser.parseInto(parsed);
        return parsed;
    }

    static auto parseIdentifier(pqxx::row const& _databaseRow) -> std::uint64_t
    {
      // Warning:
      // GCC reports false-positive warning
      // that rowParser is used before being initialized
      // in release builds
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"

        Detail::BasicRowParser rowParser{_databaseRow};

        std::uint64_t priceSeedId{};
        if (rowParser(PriceSeed::Attribute::PriceSeedId, priceSeedId)) {
            return priceSeedId;
        }

        throw DataDecodingError(
            "failed to decode a price seed record identifier "
            " from the database row"
        );

#pragma GCC diagnostic pop
    }

  private:
    Detail::BasicRowParser mRowParser;
};

} // namespace Simulator::DataLayer::Pqxx

#endif // SIMULATOR_DATA_LAYER_IH_PQXX_RESULT_PRICE_SEED_PARSER_HPP_
