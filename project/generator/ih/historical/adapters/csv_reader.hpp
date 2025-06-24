#ifndef SIMULATOR_PROJECT_GENERATOR_SRC_HISTORICAL_ADAPTERS_CSV_READER_HPP_
#define SIMULATOR_PROJECT_GENERATOR_SRC_HISTORICAL_ADAPTERS_CSV_READER_HPP_

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include <csv.hpp>

#include "ih/historical/adapters/data_access_adapter.hpp"
#include "ih/historical/data/record.hpp"
#include "ih/historical/mapping/params.hpp"
#include "ih/historical/parsing/params.hpp"

namespace Simulator::Generator::Historical {

class CsvReader final : public DataAccessAdapter {
  public:
    CsvReader() = delete;

    CsvReader(
        Historical::CsvParsingParams _params,
        MappingParams _mappingParams,
        csv::CSVReader& _reader
    );

    static auto create(
        Historical::CsvParsingParams _parsingPrams,
        Historical::MappingParams _mappingParams
    ) -> std::unique_ptr<CsvReader>;

    static auto makeFormat(
        Historical::CsvParsingParams const& _parsingPrams
    ) -> csv::CSVFormat;

  private:
    [[nodiscard]]
    auto hasNextRecord() const noexcept -> bool override;

    auto parseNextRecord(Historical::Record::Builder& _builder) -> void override;

    auto make_depth_config(const csv::CSVReader& _reader) const -> Mapping::DepthConfig;

    auto initMappingParams(csv::CSVReader const& _reader, Mapping::DepthConfig depth_config) -> void;

    auto loadCsvData(csv::CSVReader& _reader) -> void;

    auto columns_number(const csv::CSVReader& reader) const -> std::uint32_t;

    CsvParsingParams mParsingParams;
    MappingParams mMappingParams;
    std::uint32_t depth_{0};

    std::vector<std::pair<std::uint64_t, csv::CSVRow>> mExtractedRows;

    std::size_t mNextRowIdx{0};
};

} // namespace Simulator::Generator::Historical

#endif // SIMULATOR_PROJECT_GENERATOR_SRC_HISTORICAL_ADAPTERS_CSV_READER_HPP_
