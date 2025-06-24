#include <string>

#include <csv.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ih/historical/adapters/csv_reader.hpp"
#include "ih/historical/data/record.hpp"
#include "ih/historical/mapping/params.hpp"
#include "ih/historical/parsing/params.hpp"

namespace Simulator::Generator::Historical {
namespace {

using namespace ::testing;

class Generator_Historical_CsvReader : public testing::Test {
  public:
    static auto makeDefaultMapping(std::uint64_t text_header_row)
        -> MappingParams {
      return MappingParams{MappingParams::ColumnMappings{},
                           text_header_row > 0 ? DatasourceParams::CsvHasHeader
                                               : DatasourceParams::CsvNoHeader
      };
    }

    static auto makeParsingParams(
        std::uint64_t _headerRow = 0,
        std::uint64_t _dataRow = 1,
        char _delimiter = ','
    ) -> CsvParsingParams
    {
        std::string mockPath{"/a/path/to/nonexistent/csv-file.csv"};
        std::string mockName{"Mock datasource name"};

        GeneralDatasourceParams generalParams{};
        generalParams.set_name(mockName);
        generalParams.set_connection(mockPath);

        CsvParsingParams params{generalParams};
        params.set_header_row(_headerRow);
        params.set_data_row(_dataRow);
        params.set_delimiter(_delimiter);

        return params;
    }

    static auto parseCsv(std::string_view _content, CsvParsingParams _params)
        -> csv::CSVReader
    {
        csv::CSVFormat const csvFormat = CsvReader::makeFormat(_params);
        return csv::parse(_content, csvFormat);
    }
};

TEST_F(Generator_Historical_CsvReader, MakeFormat_WithHeader)
{
    CsvParsingParams const params = makeParsingParams(1, 2, ',');
    csv::CSVFormat const format = CsvReader::makeFormat(params);

    EXPECT_EQ(format.get_header(), 0);
    EXPECT_EQ(format.get_delim(), ',');
}

TEST_F(Generator_Historical_CsvReader, MakeFormat_WithoutHeader)
{
    CsvParsingParams const params = makeParsingParams(0, 1, ',');
    csv::CSVFormat const format = CsvReader::makeFormat(params);

    EXPECT_EQ(format.get_header(), -1);
    EXPECT_EQ(format.get_delim(), ',');
}

TEST_F(Generator_Historical_CsvReader, MakeFormat_WithCustomDelimiter)
{
    CsvParsingParams const params = makeParsingParams(0, 1, ';');
    csv::CSVFormat const format = CsvReader::makeFormat(params);

    EXPECT_EQ(format.get_header(), -1);
    EXPECT_EQ(format.get_delim(), ';');
}

TEST_F(Generator_Historical_CsvReader, ThrowsExceptionWhenCSVIsEmpty)
{
    constexpr std::string_view csvContent = ""; // NOLINT

    MappingParams const mappingParams = makeDefaultMapping(0);
    CsvParsingParams const parsingParams = makeParsingParams(0, 1, ',');
    csv::CSVReader csv = parseCsv(csvContent, parsingParams);

    ASSERT_THROW(Historical::CsvReader reader(parsingParams, mappingParams, csv), std::invalid_argument);
}

TEST_F(Generator_Historical_CsvReader, ReadCsv_WithHeader)
{
    // clang-format off
    constexpr std::string_view csvContent =
        "REC_TIME,MSG_TIME,INSTR,BID_PARTY,BID_PX,BID_QTY,ASK_QTY,ASK_PX,ASK_PARTY\n"
        "2023-01-20 12:00:32.345,2023-01-20 12:00:33.006,AUD/CAD,BP1,12,120,220,22,AP1\n"
        "2023-01-20 12:00:32.345,2023-01-20 12:00:33.006,AUD/CAD,BP2,32,320,420,42,AP2\n";
    // clang-format on

    MappingParams const mappingParams = makeDefaultMapping(1);
    CsvParsingParams const parsingParams = makeParsingParams(1, 2, ',');
    csv::CSVReader csv = parseCsv(csvContent, parsingParams);

    EXPECT_THAT(
        csv.get_col_names(),
        ElementsAre(
            "REC_TIME",
            "MSG_TIME",
            "INSTR",
            "BID_PARTY",
            "BID_PX",
            "BID_QTY",
            "ASK_QTY",
            "ASK_PX",
            "ASK_PARTY"
        )
    );

    MockFunction<void(Record)> record_visitor;
    EXPECT_CALL(record_visitor, Call).Times(2);

    Historical::CsvReader reader{parsingParams, mappingParams, csv};
    reader.accept(record_visitor.AsStdFunction());
}

TEST_F(Generator_Historical_CsvReader, ReadCsv_WithCommentedHeader)
{
    // clang-format off
    constexpr std::string_view csvContent =
        "This is an ugly comment in the beginning of the file,\n"
        "REC_TIME,MSG_TIME,INSTR,BID_PARTY,BID_PX,BID_QTY,ASK_QTY,ASK_PX,ASK_PARTY\n"
        "2023-01-20 12:00:32.345,2023-01-20 12:00:33.006,AUD/CAD,BP1,12,120,220,22,AP1\n"
        "2023-01-20 12:00:32.345,2023-01-20 12:00:33.006,AUD/CAD,BP2,32,320,420,42,AP2\n";
    // clang-format on

    MappingParams const mappingParams = makeDefaultMapping(2);
    CsvParsingParams const parsingParams = makeParsingParams(2, 3);
    csv::CSVReader csv = parseCsv(csvContent, parsingParams);

    EXPECT_THAT(
        csv.get_col_names(),
        ElementsAre(
            "REC_TIME",
            "MSG_TIME",
            "INSTR",
            "BID_PARTY",
            "BID_PX",
            "BID_QTY",
            "ASK_QTY",
            "ASK_PX",
            "ASK_PARTY"
        )
    );

    MockFunction<void(Record)> record_visitor;
    EXPECT_CALL(record_visitor, Call).Times(2);

    Historical::CsvReader reader{parsingParams, mappingParams, csv};
    reader.accept(record_visitor.AsStdFunction());
}

TEST_F(Generator_Historical_CsvReader, ReadCsv_WithMultiHeader)
{
    // clang-format off
    constexpr std::string_view csvContent =
        "rec_time,msg_time,instr,bid_party,bid_px,bid_qty,ask_qty,ask_px,ask_party,\n"
        "REC_TIME,MSG_TIME,INSTR,BID_PARTY,BID_PX,BID_QTY,ASK_QTY,ASK_PX,ASK_PARTY\n"
        "2023-01-20 12:00:32.345,2023-01-20 12:00:33.006,AUD/CAD,BP1,12,120,220,22,AP1\n"
        "2023-01-20 12:00:32.345,2023-01-20 12:00:33.006,AUD/CAD,BP2,32,320,420,42,AP2\n";
    // clang-format on

    MappingParams const mappingParams = makeDefaultMapping(2);
    CsvParsingParams const parsingParams = makeParsingParams(2, 3);
    csv::CSVReader csv = parseCsv(csvContent, parsingParams);

    EXPECT_THAT(
        csv.get_col_names(),
        ElementsAre(
            "REC_TIME",
            "MSG_TIME",
            "INSTR",
            "BID_PARTY",
            "BID_PX",
            "BID_QTY",
            "ASK_QTY",
            "ASK_PX",
            "ASK_PARTY"
        )
    );

    MockFunction<void(Record)> record_visitor;
    EXPECT_CALL(record_visitor, Call).Times(2);

    Historical::CsvReader reader{parsingParams, mappingParams, csv};
    reader.accept(record_visitor.AsStdFunction());
}

TEST_F(Generator_Historical_CsvReader, ReadCsv_WithoutHeader)
{
    // clang-format off
    constexpr std::string_view csvContent =
        "2023-01-20 12:00:32.345,2023-01-20 12:00:33.006,AUD/CAD,BP1,12,120,220,22,AP1\r\n"
        "2023-01-20 12:00:32.345,2023-01-20 12:00:33.006,AUD/CAD,BP2,32,320,420,42,AP2\r\n"
        "2023-01-20 12:00:32.345,2023-01-20 12:00:33.006,AUD/CAD,BP3,52,520,620,62,AP3\r\n";
    // clang-format on

    MappingParams const mappingParams = makeDefaultMapping(0);
    CsvParsingParams const parsingParams = makeParsingParams(0, 1, ',');
    csv::CSVReader csv = parseCsv(csvContent, parsingParams);

    MockFunction<void(Record)> record_visitor;
    EXPECT_CALL(record_visitor, Call).Times(3);

    Historical::CsvReader reader{parsingParams, mappingParams, csv};
    reader.accept(record_visitor.AsStdFunction());
}

TEST_F(Generator_Historical_CsvReader, ReadCsv_FromSpecificRow)
{
    // clang-format off
    constexpr std::string_view csvContent =
        "REC_TIME,MSG_TIME,INSTR,BID_PARTY,BID_PX,BID_QTY,ASK_QTY,ASK_PX,ASK_PARTY\r\n"
        "2023-01-20 12:00:32.345,2023-01-20 12:00:33.006,AUD/CAD,BP1,12,120,220,22,AP1\r\n"
        "2023-01-20 12:00:32.345,2023-01-20 12:00:33.006,AUD/CAD,BP2,32,320,420,42,AP2\r\n"
        "2023-01-20 12:00:32.345,2023-01-20 12:00:33.006,AUD/CAD,BP3,52,520,620,62,AP3\r\n";
    // clang-format on

    MappingParams const mappingParams = makeDefaultMapping(1);
    CsvParsingParams const parsingParams = makeParsingParams(1, 3, ',');
    csv::CSVReader csv = parseCsv(csvContent, parsingParams);

    MockFunction<void(Record)> record_visitor;
    EXPECT_CALL(record_visitor, Call).Times(2);

    Historical::CsvReader reader{parsingParams, mappingParams, csv};
    reader.accept(record_visitor.AsStdFunction());
}

TEST_F(Generator_Historical_CsvReader, ReadCsv_FromNonexistentFile)
{
    EXPECT_THROW(
        CsvReader::create(makeParsingParams(), makeDefaultMapping(0)),
        std::runtime_error
    );
}

TEST_F(Generator_Historical_CsvReader, ParsesOneLevelIfHeaderIsNotPresent) {
  // clang-format off
  constexpr std::string_view csv_content =
      "2023-01-20 12:00:32.345,2023-01-20 12:00:33.006,AUD/CAD,BP1,12,120,220,22,AP1\r\n";
  // clang-format on

  const MappingParams mapping_params = makeDefaultMapping(0);
  const CsvParsingParams parsing_params = makeParsingParams(0, 1, ',');
  csv::CSVReader csv = parseCsv(csv_content, parsing_params);

  Historical::CsvReader reader{parsing_params, mapping_params, csv};

  MockFunction<void(std::uint64_t, Level const&)> level_visitor;
  EXPECT_CALL(level_visitor, Call).Times(1);

  reader.accept([&](const Record& record) {
    record.visit_levels(level_visitor.AsStdFunction());
  });
}

TEST_F(Generator_Historical_CsvReader, ParsesOneLevelIfHeaderIsPresent) {
  // clang-format off
  constexpr std::string_view csv_content =
      "REC_TIME,MSG_TIME,INSTR,BID_PARTY,BID_PX,BID_QTY,ASK_QTY,ASK_PX,ASK_PARTY\r\n"
      "2023-01-20 12:00:32.345,2023-01-20 12:00:33.006,AUD/CAD,BP1,12,120,220,22,AP1\r\n";
  // clang-format on

  const MappingParams mapping_params = makeDefaultMapping(1);
  const CsvParsingParams parsing_params = makeParsingParams(1, 2, ',');
  csv::CSVReader csv = parseCsv(csv_content, parsing_params);

  Historical::CsvReader reader{parsing_params, mapping_params, csv};

  MockFunction<void(std::uint64_t, Level const&)> level_visitor;
  EXPECT_CALL(level_visitor, Call).Times(1);

  reader.accept([&](const Record& record) {
    record.visit_levels(level_visitor.AsStdFunction());
  });
}

TEST_F(Generator_Historical_CsvReader,
       ParsesTwoLevelsIfHeaderIsNotPresentAndAllNeededColumnsArePresent) {
  // clang-format off
  constexpr std::string_view csv_content =
      "2023-01-20 12:00:32.345,2023-01-20 12:00:33.006,AUD/CAD,BP1,12,120,BP2,32,320,220,22,AP1,420,42,AP2\r\n";
  // clang-format on

  const MappingParams mapping_params = makeDefaultMapping(0);
  const CsvParsingParams parsing_params = makeParsingParams(0, 1, ',');
  csv::CSVReader csv = parseCsv(csv_content, parsing_params);

  Historical::CsvReader reader{parsing_params, mapping_params, csv};

  MockFunction<void(std::uint64_t, Level const&)> level_visitor;
  EXPECT_CALL(level_visitor, Call).Times(2);

  reader.accept([&](const Record& record) {
    record.visit_levels(level_visitor.AsStdFunction());
  });
}

TEST_F(Generator_Historical_CsvReader,
       ParsesTwoLevelsIfHeaderIsPresentAndAllNeededColumnsArePresent) {
  // clang-format off
  constexpr std::string_view csv_content =
      "REC_TIME,MSG_TIME,INSTR,BID_PARTY1,BID_PX1,BID_QTY1,BID_PARTY2,BID_PX2,BID_QTY2,ASK_QTY1,ASK_PX1,ASK_PARTY1,ASK_QTY2,ASK_PX2,ASK_PARTY2\r\n"
      "2023-01-20 12:00:32.345,2023-01-20 12:00:33.006,AUD/CAD,BP1,12,120,BP2,32,320,220,22,AP1,420,42,AP2\r\n";
  // clang-format on

  const MappingParams mapping_params = makeDefaultMapping(1);
  const CsvParsingParams parsing_params = makeParsingParams(1, 2, ',');
  csv::CSVReader csv = parseCsv(csv_content, parsing_params);

  Historical::CsvReader reader{parsing_params, mapping_params, csv};

  MockFunction<void(std::uint64_t, Level const&)> level_visitor;
  EXPECT_CALL(level_visitor, Call).Times(2);

  reader.accept([&](const Record& record) {
    record.visit_levels(level_visitor.AsStdFunction());
  });
}

TEST_F(Generator_Historical_CsvReader,
       ParsesTwoLevelsIfHeaderIsPresentAndThereAreMoreColumnsThanNeeded) {
  // clang-format off
  constexpr std::string_view csv_content =
      "REC_TIME,MSG_TIME,INSTR,BID_PARTY1,BID_PX1,BID_QTY1,BID_PARTY2,BID_PX2,BID_QTY2,ASK_QTY1,ASK_PX1,ASK_PARTY1,ASK_QTY2,ASK_PX2,ASK_PARTY2,Column1,Column2\r\n"
      "2023-01-20 12:00:32.345,2023-01-20 12:00:33.006,AUD/CAD,BP1,12,120,BP2,32,320,220,22,AP1,420,42,AP2,fizz,bazz\r\n";
  // clang-format on

  const MappingParams mapping_params = makeDefaultMapping(1);
  const CsvParsingParams parsing_params = makeParsingParams(1, 2, ',');
  csv::CSVReader csv = parseCsv(csv_content, parsing_params);

  Historical::CsvReader reader{parsing_params, mapping_params, csv};

  MockFunction<void(std::uint64_t, Level const&)> level_visitor;
  EXPECT_CALL(level_visitor, Call).Times(2);

  reader.accept([&](const Record& record) {
    record.visit_levels(level_visitor.AsStdFunction());
  });
}

}  // namespace
}  // namespace Simulator::Generator::Historical