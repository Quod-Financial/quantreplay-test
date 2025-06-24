#ifndef SIMULATOR_GENERATOR_IH_HISTORICAL_MAPPING_MAPPING_HPP_
#define SIMULATOR_GENERATOR_IH_HISTORICAL_MAPPING_MAPPING_HPP_

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <cstdint>
#include <map>
#include <optional>

#include "core/common/std_formatter.hpp"
#include "data_layer/api/converters/column_mapping.hpp"

namespace Simulator::Generator::Historical {

enum class Column : std::uint8_t {
  ReceivedTimestamp,
  MessageTimestamp,
  Instrument,
  BidParty,
  BidQuantity,
  BidPrice,
  OfferPrice,
  OfferQuantity,
  OfferParty
};

namespace Mapping {

class SourceColumn {
 public:
  SourceColumn() = delete;

  constexpr explicit SourceColumn(std::uint32_t column_index)
      : column_index_{column_index} {}

  [[nodiscard]]
  auto index() const noexcept -> std::uint32_t;

  [[nodiscard]]
  auto number() const noexcept -> std::uint32_t;

 private:
  std::uint32_t column_index_{0};
};

class Specification {
 public:
  void associate(DataLayer::Converter::ColumnFrom target_column,
                 SourceColumn source_column);

  [[nodiscard]]
  auto resolve_by(const DataLayer::Converter::ColumnFrom& target_column) const
      -> std::optional<SourceColumn>;

  friend auto operator<<(std::ostream& os, const Specification& spec)
      -> std::ostream&;

 private:
  using TargetAssociation =
      std::map<DataLayer::Converter::ColumnFrom, SourceColumn>;

  TargetAssociation by_target_;
};

}  // namespace Mapping

}  // namespace Simulator::Generator::Historical

template <>
struct fmt::formatter<Simulator::Generator::Historical::Column>
    : formatter<std::string_view> {
  using formattable = Simulator::Generator::Historical::Column;
  auto format(const formattable& value, format_context& context) const
      -> decltype(context.out());
};

template <>
struct fmt::formatter<Simulator::Generator::Historical::Mapping::SourceColumn>
    : formatter<std::string_view> {
  using formattable = Simulator::Generator::Historical::Mapping::SourceColumn;
  auto format(const formattable& value, format_context& context) const
      -> decltype(context.out());
};

template <>
struct fmt::formatter<Simulator::Generator::Historical::Mapping::Specification>
    : ostream_formatter {};

#endif  // SIMULATOR_GENERATOR_IH_HISTORICAL_MAPPING_MAPPING_HPP_
