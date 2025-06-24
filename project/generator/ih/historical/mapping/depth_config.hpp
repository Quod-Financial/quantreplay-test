#ifndef SIMULATOR_GENERATOR_IH_HISTORICAL_MAPPING_DEPTH_CONFIG_HPP_
#define SIMULATOR_GENERATOR_IH_HISTORICAL_MAPPING_DEPTH_CONFIG_HPP_

#include <cstdint>

#include "ih/constants.hpp"
#include "log/logging.hpp"

namespace Simulator::Generator::Historical::Mapping {

struct DepthConfig {
  std::uint32_t datasource_depth;
  std::uint32_t depth_to_parse;
};

inline auto depth_from_columns_number(std::uint32_t columns_number) noexcept
    -> std::uint32_t {
  const auto number = static_cast<std::int64_t>(columns_number);

  // Columns Column::ReceivedTimestamp, Column::MessageTimestamp,
  // Column::Instrument do not have depth. They are 3 columns.

  // Column::BidParty, Column::BidQuantity, Column::BidPrice,
  // Column::OfferPrice, Column::OfferQuantity, Column::OfferParty represent a
  // level. They are 6 columns.
  const auto depth = (number - 3) / 6;
  return static_cast<std::uint32_t>(depth);
}

inline auto depth_to_parse(std::uint32_t data_depth,
                           std::uint32_t max_depth_to_parse) noexcept
    -> std::uint32_t {
  const auto& depth = max_depth_to_parse == Constant::Historical::AllDepthLevels
                          ? data_depth
                          : std::min(data_depth, max_depth_to_parse);
  simulator::log::debug(
      "Depth to parse {} has been chosen from values of data depth {} and "
      "maximum depth to parse {}.",
      depth,
      data_depth,
      max_depth_to_parse);
  return depth;
}

}  // namespace Simulator::Generator::Historical::Mapping

#endif  // SIMULATOR_GENERATOR_IH_HISTORICAL_MAPPING_DEPTH_CONFIG_HPP_
