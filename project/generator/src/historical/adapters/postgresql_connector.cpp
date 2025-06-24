#include "ih/historical/adapters/postgresql_connector.hpp"

#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include <fmt/format.h>

#include <pqxx/connection>
#include <pqxx/result>
#include <pqxx/row>
#include <pqxx/transaction>

#include "ih/historical/parsing/params.hpp"
#include "ih/historical/parsing/parsing.hpp"
#include "ih/historical/parsing/row.hpp"
#include "log/logging.hpp"

namespace Simulator::Generator::Historical {

PostgresConnector::PostgresConnector(
    Historical::DatabaseParsingParams _params,
    Historical::MappingParams _mappingParams,
    pqxx::connection _connection
) :
    mMappingParams{std::move(_mappingParams)},
    mParsingParams{std::move(_params)}
{
    loadData(_connection);

    auto depth_config = make_depth_config(_connection);
    depth_ = depth_config.datasource_depth;

    initMappingParams(std::move(depth_config));
    extractData();
}

auto PostgresConnector::create(
    Historical::DatabaseParsingParams _params,
    Historical::MappingParams _mappingParams
) -> std::unique_ptr<PostgresConnector>
{
    pqxx::connection connection{_params.datasource_connection()};
    return std::make_unique<PostgresConnector>(
        std::move(_params),
        std::move(_mappingParams),
        std::move(connection)
    );
}

auto PostgresConnector::hasNextRecord() const noexcept -> bool
{
    return mNextRowIdx < mExtractedRows.size();
}

auto PostgresConnector::parseNextRecord(Record::Builder& _builder) -> void
{
    assert(hasNextRecord());

    auto& [rowNumber, pqRow] = mExtractedRows[mNextRowIdx++];

    _builder.with_source_row(rowNumber)
        .with_source_name(mParsingParams.datasource_name())
        .with_source_connection(mParsingParams.datasource_connection());

    Historical::Row const row = Historical::Row::from(pqRow);
    Historical::parse(row, _builder, mMappingParams, depth_);
}

auto PostgresConnector::loadData(pqxx::connection& _databaseConnection) -> void
{
    pqxx::work transaction{_databaseConnection};
    mData = transaction.exec(
        fmt::format("SELECT * FROM {};", mParsingParams.table_name())
    );
}

auto PostgresConnector::make_depth_config(
    pqxx::connection& database_connection) const -> Mapping::DepthConfig {
  const auto data_depth =
      Mapping::depth_from_columns_number(columns_number(database_connection));
  const auto depth_to_parse = Mapping::depth_to_parse(
      data_depth, mParsingParams.datasource_max_depth_levels());
  return {.datasource_depth = data_depth, .depth_to_parse = depth_to_parse};
}

auto PostgresConnector::initMappingParams(Mapping::DepthConfig depth_config)
    -> void {
    std::int32_t const columnsCount = mData.columns();

    std::vector<std::string> columnNames{};
    columnNames.reserve(static_cast<std::uint32_t>(columnsCount));

    for (std::int32_t rowIdx = 0; rowIdx < columnsCount; ++rowIdx) {
        std::string_view const columnName = mData.column_name(rowIdx);
        columnNames.emplace_back(columnName);
    }

    mMappingParams.initialize(std::move(columnNames), std::move(depth_config));
}

auto PostgresConnector::extractData() -> void
{
    std::uint64_t rowNumber{1};
    for (pqxx::row row : mData) {
        mExtractedRows.emplace_back(rowNumber++, std::move(row));
    }
}

auto PostgresConnector::columns_number(
    pqxx::connection& database_connection) const -> std::uint32_t {
  pqxx::work transaction{database_connection};
  pqxx::result result = transaction.exec(
      fmt::format("SELECT * FROM {} LIMIT 1;", mParsingParams.table_name()));

  const auto number = static_cast<std::uint32_t>(result.columns());
  simulator::log::debug("Selected number of columns {}.", number);
  return number;
}

} // namespace Simulator::Generator::Historical
