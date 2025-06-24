#include "ih/historical/adapters/csv_reader.hpp"

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include <csv.hpp>

#include "ih/historical/data/record.hpp"
#include "ih/historical/mapping/params.hpp"
#include "ih/historical/parsing/parsing.hpp"
#include "ih/historical/parsing/row.hpp"
#include "log/logging.hpp"

namespace Simulator::Generator::Historical {

CsvReader::CsvReader(
    Historical::CsvParsingParams _parsingParams,
    MappingParams _mappingParams,
    csv::CSVReader& _reader
) :
    mParsingParams(std::move(_parsingParams)),
    mMappingParams{std::move(_mappingParams)}
{
    loadCsvData(_reader);

    auto depth_config = make_depth_config(_reader);
    depth_ = depth_config.depth_to_parse;
    initMappingParams(_reader, std::move(depth_config));
}

auto CsvReader::create(
    Historical::CsvParsingParams _parsingPrams,
    Historical::MappingParams _mappingParams
) -> std::unique_ptr<CsvReader>
{
    csv::CSVFormat format = makeFormat(_parsingPrams);
    csv::CSVReader csvReader{
        _parsingPrams.datasource_connection(),
        std::move(format)};

    return std::make_unique<CsvReader>(
        std::move(_parsingPrams),
        std::move(_mappingParams),
        csvReader
    );
}

auto CsvReader::makeFormat(Historical::CsvParsingParams const& _parsingPrams)
    -> csv::CSVFormat
{
    csv::CSVFormat format{};
    format.variable_columns(csv::VariableColumnPolicy::KEEP)
        .delimiter(_parsingPrams.delimiter());

    if (_parsingPrams.has_header_row()) {
        auto const headerNum = _parsingPrams.header_row();
        auto const headerIdx = static_cast<std::int32_t>(headerNum - 1);
        format.header_row(headerIdx);
    } else {
        format.no_header();
    }

    return format;
}

auto CsvReader::hasNextRecord() const noexcept -> bool
{
    return mNextRowIdx < mExtractedRows.size();
}

auto CsvReader::parseNextRecord(Historical::Record::Builder& _builder) -> void
{
    assert(hasNextRecord());

    auto [rowNumber, csvRow] = mExtractedRows[mNextRowIdx++];

    _builder.with_source_row(rowNumber)
        .with_source_name(mParsingParams.datasource_name())
        .with_source_connection(mParsingParams.datasource_connection());

    Historical::Row const row = Historical::Row::from(csvRow);
    Historical::parse(row, _builder, mMappingParams, depth_);
}

auto CsvReader::make_depth_config(const csv::CSVReader& _reader) const
    -> Mapping::DepthConfig {
  const auto data_depth =
      Mapping::depth_from_columns_number(columns_number(_reader));
  const auto depth_to_parse = Mapping::depth_to_parse(
      data_depth, mParsingParams.datasource_max_depth_levels());
  return {.datasource_depth = data_depth, .depth_to_parse = depth_to_parse};
}

auto CsvReader::initMappingParams(csv::CSVReader const& _reader, Mapping::DepthConfig depth_config) -> void
{
    if (mParsingParams.has_header_row()) {
        std::vector<std::string> columnsNames = _reader.get_col_names();
        mMappingParams.initialize(std::move(columnsNames), std::move(depth_config));
    } else {
        mMappingParams.initialize(std::move(depth_config));
    }
}

auto CsvReader::loadCsvData(csv::CSVReader& _reader) -> void
{
    // The csv parsing library trims all rows before header row and a header row
    // itself. Thus, sentinel represents a 'global' row number with respect to
    // possibly-trimmed rows. Sentinel itself is initialized as index (with 0),
    // but is interpreted as a global row number inside a reading loop.

    std::uint64_t const dataRowNumber = mParsingParams.data_row();
    std::uint64_t rowSentinel = [&]() -> std::uint64_t {
        if (mParsingParams.has_header_row()) {
            return mParsingParams.header_row();
        }
        return 0;
    }();

    std::for_each(
        std::begin(_reader),
        std::end(_reader),
        [&](csv::CSVRow& _row) {
            if (++rowSentinel >= dataRowNumber) {
                mExtractedRows.emplace_back(rowSentinel, std::move(_row));
            }
        }
    );
}

auto CsvReader::columns_number(const csv::CSVReader& reader) const
    -> std::uint32_t {
  if (mParsingParams.has_header_row()) {
    const auto number =
        static_cast<std::uint32_t>(reader.get_col_names().size());
    simulator::log::debug("Got a number of columns {} from the header row.",
                          number);
    return number;
  }

  if (!mExtractedRows.empty()) {
    const auto number =
        static_cast<std::uint32_t>(mExtractedRows.front().second.size());
    simulator::log::debug("Got a number of columns {} from the first data row.",
                          number);
    return number;
  }

  simulator::log::debug(
      "Header and data rows are absent. Number of columns is 0.");
  return 0;
}

} // namespace Simulator::Generator::Historical
