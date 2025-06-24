#ifndef SIMULATOR_DATA_LAYER_IH_PQXX_RESULT_DATASOURCE_PARSER_HPP_
#define SIMULATOR_DATA_LAYER_IH_PQXX_RESULT_DATASOURCE_PARSER_HPP_

#include <pqxx/row>

#include "api/exceptions/exceptions.hpp"
#include "api/inspectors/column_mapping.hpp"
#include "api/inspectors/datasource.hpp"
#include "api/models/column_mapping.hpp"
#include "api/models/datasource.hpp"
#include "ih/pqxx/result/detail/basic_row_parser.hpp"

namespace Simulator::DataLayer::Pqxx {

class ColumnMappingParser {
  public:
    explicit ColumnMappingParser(pqxx::row const& _databaseRow) noexcept :
        mRowParser(_databaseRow)
    {}

    auto parseInto(ColumnMapping::Patch& _destinationPatch) -> void
    {
        ColumnMappingPatchWriter<decltype(mRowParser)> writer{mRowParser};
        writer.write(_destinationPatch);
    }

    static auto parse(pqxx::row const& _databaseRow) -> ColumnMapping::Patch
    {
        ColumnMapping::Patch parsed{};
        ColumnMappingParser parser{_databaseRow};
        parser.parseInto(parsed);
        return parsed;
    }


  private:
    Detail::BasicRowParser mRowParser;
};

class DatasourceParser {
  public:
    explicit DatasourceParser(pqxx::row const& _databaseRow) noexcept :
        mRowParser(_databaseRow)
    {}

    auto parseInto(Datasource::Patch& _destinationPatch) -> void
    {
        DatasourcePatchWriter<decltype(mRowParser)> writer{mRowParser};
        writer.write(_destinationPatch);
    }

    static auto parse(pqxx::row const& _databaseRow) -> Datasource::Patch
    {
        Datasource::Patch parsed{};
        DatasourceParser parser{_databaseRow};
        parser.parseInto(parsed);
        return parsed;
    }

    static auto parseIdentifier(pqxx::row const& _databaseRow) -> std::uint64_t
    {
        Detail::BasicRowParser rowParser{_databaseRow};

        std::uint64_t datasourceId{};
        if (rowParser(Datasource::Attribute::DatasourceId, datasourceId)) {
            return datasourceId;
        }

        throw DataDecodingError(
            "failed to decode a datasource record identifier "
            " from the database row"
        );
    }

  private:
    Detail::BasicRowParser mRowParser;
};

} // namespace Simulator::DataLayer::Pqxx

#endif // SIMULATOR_DATA_LAYER_IH_PQXX_RESULT_DATASOURCE_PARSER_HPP_
