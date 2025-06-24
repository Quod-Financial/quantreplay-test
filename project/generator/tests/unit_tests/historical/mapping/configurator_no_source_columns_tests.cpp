#include <gmock/gmock.h>

#include "ih/historical/mapping/configurator.hpp"
#include "ih/historical/mapping/specification.hpp"
#include "test_utils/historical_mapping_utils.hpp"

namespace Simulator::Generator::Historical::Mapping {
namespace {

using namespace ::testing;

struct GeneratorHistoricalMappingConfiguratorNoSourceColumnsCSV
    : public testing::Test {
  auto make_configurator(DepthConfig depth_config) -> Configurator {
    return Configurator{
        spec, {}, std::move(depth_config), DatasourceParams::CsvNoHeader};
  }

  Specification spec;
};

TEST_F(GeneratorHistoricalMappingConfiguratorNoSourceColumnsCSV,
       KeepsDefaultAssociationWhenColumnToIsNotANumber) {
  using ColumnFrom = DataLayer::Converter::ColumnFrom;

  const auto target = ColumnFrom::MessageTimestamp;
  constexpr std::uint64_t default_message_timestamp_source_idx = 1;

  const auto column_from = ColumnFrom::create("MessageTimeStamp").value();
  const std::string column_to{"Not234A321Number"};

  auto configurator = make_configurator({1, 1});
  configurator.configure(make_column_config(column_from, column_to));

  ASSERT_THAT(spec,
              ResolvesColumnFromAndIndexIs(
                  target, default_message_timestamp_source_idx));
}

TEST_F(GeneratorHistoricalMappingConfiguratorNoSourceColumnsCSV,
       KeepsDefaultAssociationWhenColumnToIsNegativeNumber) {
  using ColumnFrom = DataLayer::Converter::ColumnFrom;

  const auto target = ColumnFrom::MessageTimestamp;
  constexpr std::uint64_t default_message_timestamp_source_idx = 1;

  const auto column_from = ColumnFrom::create("MessageTimeStamp").value();
  const std::string column_to{"-1"};

  auto configurator = make_configurator({1, 1});
  configurator.configure(make_column_config(column_from, column_to));

  ASSERT_THAT(spec,
              ResolvesColumnFromAndIndexIs(
                  target, default_message_timestamp_source_idx));
}

TEST_F(GeneratorHistoricalMappingConfiguratorNoSourceColumnsCSV,
       KeepsDefaultAssociationWhenColumnToIsZero) {
  using ColumnFrom = DataLayer::Converter::ColumnFrom;

  const auto affected_target = ColumnFrom::MessageTimestamp;
  constexpr std::uint64_t default_message_timestamp_source_idx = 1;

  const auto column_from = ColumnFrom::create("MessageTimeStamp").value();
  const std::string column_to{"0"};

  auto configurator = make_configurator({1, 1});
  configurator.configure(make_column_config(column_from, column_to));

  // Zero column number is not valid as a 1-based columnTo column number
  ASSERT_THAT(spec,
              ResolvesColumnFromAndIndexIs(
                  affected_target, default_message_timestamp_source_idx));
}

TEST_F(GeneratorHistoricalMappingConfiguratorNoSourceColumnsCSV,
       KeepsDefaultAssociationWhenColumnToStartsWithNumber) {
  using ColumnFrom = DataLayer::Converter::ColumnFrom;

  const auto affected_target = ColumnFrom::MessageTimestamp;
  constexpr std::uint64_t default_message_timestamp_source_idx = 1;

  const auto column_from = ColumnFrom::create("MessageTimeStamp").value();
  const std::string column_to{"6T"};

  auto configurator = make_configurator({1, 1});
  configurator.configure(make_column_config(column_from, column_to));

  ASSERT_THAT(spec,
              ResolvesColumnFromAndIndexIs(
                  affected_target, default_message_timestamp_source_idx));
}

TEST_F(GeneratorHistoricalMappingConfiguratorNoSourceColumnsCSV,
       AppliesAssociationForReceivedTimestampWithoutDepthAndDepthToParseIsOne) {
  using ColumnFrom = DataLayer::Converter::ColumnFrom;

  const auto target = ColumnFrom::ReceivedTimestamp;

  const auto column_from = ColumnFrom::create("ReceivedTimeStamp").value();
  const std::string column_to{"6"};
  constexpr std::uint64_t new_source_idx = 5;

  auto configurator = make_configurator({1, 1});
  configurator.configure(make_column_config(column_from, column_to));

  ASSERT_THAT(spec, ResolvesColumnFromAndIndexIs(target, new_source_idx));
}

TEST_F(GeneratorHistoricalMappingConfiguratorNoSourceColumnsCSV,
       AppliesAssociationForMessageTimestampWithoutDepthAndDepthToParseIsOne) {
  using ColumnFrom = DataLayer::Converter::ColumnFrom;

  const auto target = ColumnFrom::MessageTimestamp;

  const auto column_from = ColumnFrom::create("MessageTimeStamp").value();
  const std::string column_to{"6"};
  constexpr std::uint64_t new_source_idx = 5;

  auto configurator = make_configurator({1, 1});
  configurator.configure(make_column_config(column_from, column_to));

  ASSERT_THAT(spec, ResolvesColumnFromAndIndexIs(target, new_source_idx));
}

TEST_F(GeneratorHistoricalMappingConfiguratorNoSourceColumnsCSV,
       AppliesAssociationForInstrumentWithoutDepthAndDepthToParseIsOne) {
  using ColumnFrom = DataLayer::Converter::ColumnFrom;

  const auto target = ColumnFrom::Instrument;

  const auto column_from = ColumnFrom::create("Instrument").value();
  const std::string column_to{"6"};
  constexpr std::uint64_t new_source_idx = 5;

  auto configurator = make_configurator({1, 1});
  configurator.configure(make_column_config(column_from, column_to));

  ASSERT_THAT(spec, ResolvesColumnFromAndIndexIs(target, new_source_idx));
}

TEST_F(GeneratorHistoricalMappingConfiguratorNoSourceColumnsCSV,
       AppliesAssociationForBidPartyWithDepthIsLessThanDepthToParse) {
  using ColumnFrom = DataLayer::Converter::ColumnFrom;

  const auto target =
      ColumnFrom::create(ColumnFrom::Column::BidParty, 1u).value();

  const auto column_from = ColumnFrom::create("BidParty1").value();
  const std::string column_to{"6"};
  constexpr std::uint64_t new_source_idx = 5;

  auto configurator = make_configurator({3, 2});
  configurator.configure(make_column_config(column_from, column_to));

  ASSERT_THAT(spec, ResolvesColumnFromAndIndexIs(target, new_source_idx));
}

TEST_F(GeneratorHistoricalMappingConfiguratorNoSourceColumnsCSV,
       AppliesAssociationForBidPartyWhenItsDepthIsEqualToDepthToParse) {
  using ColumnFrom = DataLayer::Converter::ColumnFrom;

  const auto target =
      ColumnFrom::create(ColumnFrom::Column::BidParty, 2u).value();

  const auto column_from = ColumnFrom::create("BidParty2").value();
  const std::string column_to{"6"};
  constexpr std::uint64_t new_source_idx = 5;

  auto configurator = make_configurator({3, 2});
  configurator.configure(make_column_config(column_from, column_to));

  ASSERT_THAT(spec, ResolvesColumnFromAndIndexIs(target, new_source_idx));
}

TEST_F(GeneratorHistoricalMappingConfiguratorNoSourceColumnsCSV,
       AppliesAssociationForBidQuantityWithDepthIsLessThanDepthToParse) {
  using ColumnFrom = DataLayer::Converter::ColumnFrom;

  const auto target =
      ColumnFrom::create(ColumnFrom::Column::BidQuantity, 1u).value();

  const auto column_from = ColumnFrom::create("BidQuantity1").value();
  const std::string column_to{"6"};
  constexpr std::uint64_t new_source_idx = 5;

  auto configurator = make_configurator({3, 2});
  configurator.configure(make_column_config(column_from, column_to));

  ASSERT_THAT(spec, ResolvesColumnFromAndIndexIs(target, new_source_idx));
}

TEST_F(GeneratorHistoricalMappingConfiguratorNoSourceColumnsCSV,
       AppliesAssociationForBidQuantityWhenItsDepthIsEqualToDepthToParse) {
  using ColumnFrom = DataLayer::Converter::ColumnFrom;

  const auto target =
      ColumnFrom::create(ColumnFrom::Column::BidQuantity, 2u).value();

  const auto column_from = ColumnFrom::create("BidQuantity2").value();
  const std::string column_to{"6"};
  constexpr std::uint64_t new_source_idx = 5;

  auto configurator = make_configurator({3, 2});
  configurator.configure(make_column_config(column_from, column_to));

  ASSERT_THAT(spec, ResolvesColumnFromAndIndexIs(target, new_source_idx));
}

TEST_F(GeneratorHistoricalMappingConfiguratorNoSourceColumnsCSV,
       AppliesAssociationForBidPriceWithDepthIsLessThanDepthToParse) {
  using ColumnFrom = DataLayer::Converter::ColumnFrom;

  const auto target =
      ColumnFrom::create(ColumnFrom::Column::BidPrice, 1u).value();

  const auto column_from = ColumnFrom::create("BidPrice1").value();
  const std::string column_to{"6"};
  constexpr std::uint64_t new_source_idx = 5;

  auto configurator = make_configurator({3, 2});
  configurator.configure(make_column_config(column_from, column_to));

  ASSERT_THAT(spec, ResolvesColumnFromAndIndexIs(target, new_source_idx));
}

TEST_F(GeneratorHistoricalMappingConfiguratorNoSourceColumnsCSV,
       AppliesAssociationForBidPriceWhenItsDepthIsEqualToDepthToParse) {
  using ColumnFrom = DataLayer::Converter::ColumnFrom;

  const auto target =
      ColumnFrom::create(ColumnFrom::Column::BidPrice, 2u).value();

  const auto column_from = ColumnFrom::create("BidPrice2").value();
  const std::string column_to{"6"};
  constexpr std::uint64_t new_source_idx = 5;

  auto configurator = make_configurator({3, 2});
  configurator.configure(make_column_config(column_from, column_to));

  ASSERT_THAT(spec, ResolvesColumnFromAndIndexIs(target, new_source_idx));
}

TEST_F(GeneratorHistoricalMappingConfiguratorNoSourceColumnsCSV,
       AppliesAssociationForOfferPriceWithDepthIsLessThanDepthToParse) {
  using ColumnFrom = DataLayer::Converter::ColumnFrom;

  const auto target =
      ColumnFrom::create(ColumnFrom::Column::OfferPrice, 1u).value();

  const auto column_from = ColumnFrom::create("AskPrice1").value();
  const std::string column_to{"6"};
  constexpr std::uint64_t new_source_idx = 5;

  auto configurator = make_configurator({3, 2});
  configurator.configure(make_column_config(column_from, column_to));

  ASSERT_THAT(spec, ResolvesColumnFromAndIndexIs(target, new_source_idx));
}

TEST_F(GeneratorHistoricalMappingConfiguratorNoSourceColumnsCSV,
       AppliesAssociationForOfferPriceWhenItsDepthIsEqualToDepthToParse) {
  using ColumnFrom = DataLayer::Converter::ColumnFrom;

  const auto target =
      ColumnFrom::create(ColumnFrom::Column::OfferPrice, 2u).value();

  const auto column_from = ColumnFrom::create("AskPrice2").value();
  const std::string column_to{"6"};
  constexpr std::uint64_t new_source_idx = 5;

  auto configurator = make_configurator({3, 2});
  configurator.configure(make_column_config(column_from, column_to));

  ASSERT_THAT(spec, ResolvesColumnFromAndIndexIs(target, new_source_idx));
}

TEST_F(GeneratorHistoricalMappingConfiguratorNoSourceColumnsCSV,
       AppliesAssociationForOfferQuantityWithDepthIsLessThanDepthToParse) {
  using ColumnFrom = DataLayer::Converter::ColumnFrom;

  const auto target =
      ColumnFrom::create(ColumnFrom::Column::OfferQuantity, 1u).value();

  const auto column_from = ColumnFrom::create("AskQuantity1").value();
  const std::string column_to{"6"};
  constexpr std::uint64_t new_source_idx = 5;

  auto configurator = make_configurator({3, 2});
  configurator.configure(make_column_config(column_from, column_to));

  ASSERT_THAT(spec, ResolvesColumnFromAndIndexIs(target, new_source_idx));
}

TEST_F(GeneratorHistoricalMappingConfiguratorNoSourceColumnsCSV,
       AppliesAssociationForOfferQuantityWhenItsDepthIsEqualToDepthToParse) {
  using ColumnFrom = DataLayer::Converter::ColumnFrom;

  const auto target =
      ColumnFrom::create(ColumnFrom::Column::OfferQuantity, 2u).value();

  const auto column_from = ColumnFrom::create("AskQuantity2").value();
  const std::string column_to{"6"};
  constexpr std::uint64_t new_source_idx = 5;

  auto configurator = make_configurator({3, 2});
  configurator.configure(make_column_config(column_from, column_to));

  ASSERT_THAT(spec, ResolvesColumnFromAndIndexIs(target, new_source_idx));
}

TEST_F(GeneratorHistoricalMappingConfiguratorNoSourceColumnsCSV,
       AppliesAssociationForOfferPartyWithDepthIsLessThanDepthToParse) {
  using ColumnFrom = DataLayer::Converter::ColumnFrom;

  const auto target =
      ColumnFrom::create(ColumnFrom::Column::OfferParty, 1u).value();

  const auto column_from = ColumnFrom::create("AskParty1").value();
  const std::string column_to{"6"};
  constexpr std::uint64_t new_source_idx = 5;

  auto configurator = make_configurator({3, 2});
  configurator.configure(make_column_config(column_from, column_to));

  ASSERT_THAT(spec, ResolvesColumnFromAndIndexIs(target, new_source_idx));
}

TEST_F(GeneratorHistoricalMappingConfiguratorNoSourceColumnsCSV,
       AppliesAssociationForOfferPartyWhenItsDepthIsEqualToDepthToParse) {
  using ColumnFrom = DataLayer::Converter::ColumnFrom;

  const auto target =
      ColumnFrom::create(ColumnFrom::Column::OfferParty, 2u).value();

  const auto column_from = ColumnFrom::create("AskParty2").value();
  const std::string column_to{"6"};
  constexpr std::uint64_t new_source_idx = 5;

  auto configurator = make_configurator({3, 2});
  configurator.configure(make_column_config(column_from, column_to));

  ASSERT_THAT(spec, ResolvesColumnFromAndIndexIs(target, new_source_idx));
}

}  // namespace
}  // namespace Simulator::Generator::Historical::Mapping
