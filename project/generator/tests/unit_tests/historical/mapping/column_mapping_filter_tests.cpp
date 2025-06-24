#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ih/historical/mapping/column_mapping_filter.hpp"

namespace Simulator::Generator::Historical::Mapping {
namespace {

using namespace ::testing;

auto column_mapping(std::string column_from, std::string column_to)
    -> DataLayer::ColumnMapping {
  DataLayer::ColumnMapping::Patch patch;
  patch.with_column_from(std::move(column_from))
      .with_column_to(std::move(column_to));
  return DataLayer::ColumnMapping::create(std::move(patch), 0);
}

auto column_config(DataLayer::Converter::ColumnFrom::Column column_from,
                   DataLayer::Converter::ColumnFrom::Depth depth,
                   std::string column_to)
    -> DataLayer::Converter::ColumnConfig {
  return DataLayer::Converter::ColumnConfig{
      DataLayer::Converter::ColumnFrom::create(column_from, std::move(depth))
          .value(),
      std::move(column_to),
      0};
}

TEST(GeneratorHistoricalMappingColumnMappingFilter,
     ReturnsTheSameMappingWhenNoVariableDepth) {
  using ColumnFrom = DataLayer::Converter::ColumnFrom;
  const std::vector<DataLayer::ColumnMapping> mappings{
      column_mapping("ReceivedTimeStamp", "timestamp"),
      column_mapping("BidParty42", "bid_party")};

  const auto filtered = filter(mappings, 42);
  ASSERT_THAT(filtered,
              ElementsAre(column_config(ColumnFrom::Column::ReceivedTimestamp,
                                        ColumnFrom::NoDepth{},
                                        "timestamp"),
                          column_config(
                              ColumnFrom::Column::BidParty, 42u, "bid_party")));
}

TEST(GeneratorHistoricalMappingColumnMappingFilter, UnfoldsVariableDepth) {
  using ColumnFrom = DataLayer::Converter::ColumnFrom;

  const std::vector<DataLayer::ColumnMapping> mappings{
      column_mapping("BidParty#", "bid_party#")};

  const auto filtered = filter(mappings, 2);
  ASSERT_THAT(
      filtered,
      ElementsAre(
          column_config(ColumnFrom::Column::BidParty, 1u, "bid_party1"),
          column_config(ColumnFrom::Column::BidParty, 2u, "bid_party2")));
}

TEST(GeneratorHistoricalMappingColumnMappingFilter,
     IgnoresDepthMappingIfVariableDepthIsSet) {
  using ColumnFrom = DataLayer::Converter::ColumnFrom;

  const std::vector<DataLayer::ColumnMapping> mappings{
      column_mapping("BidParty#", "bid_party#"),
      column_mapping("BidParty1", "ignored1"),
      column_mapping("BidParty2", "ignored2"),
  };

  const auto filtered = filter(mappings, 2);
  ASSERT_THAT(
      filtered,
      ElementsAre(
          column_config(ColumnFrom::Column::BidParty, 1u, "bid_party1"),
          column_config(ColumnFrom::Column::BidParty, 2u, "bid_party2")));
}

}  // namespace
}  // namespace Simulator::Generator::Historical::Mapping
