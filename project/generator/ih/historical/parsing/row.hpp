#ifndef SIMULATOR_GENERATOR_IH_HISTORICAL_PARSING_ROW_HPP_
#define SIMULATOR_GENERATOR_IH_HISTORICAL_PARSING_ROW_HPP_

#include <fmt/format.h>

#include <csv.hpp>
#include <pqxx/row>
#include <stdexcept>
#include <string_view>
#include <vector>

namespace Simulator::Generator::Historical {

class Row {
 public:
  static auto from(const csv::CSVRow& csv_row) -> Row {
    Row row;
    for (const csv::CSVField& field : csv_row) {
      std::string_view const value = field.get_sv();
      row.emplace_next(value);
    }
    return row;
  }

  static auto from(const pqxx::row& pq_row) -> Row {
    Row row;
    for (const pqxx::field& field : pq_row) {
      auto const value = field.as<std::string_view>(std::string_view{});
      row.emplace_next(value);
    }
    return row;
  }

  Row() = default;
  explicit Row(std::initializer_list<std::string_view> cells) : cells_{cells} {}

  [[nodiscard]]
  auto columns() const noexcept -> std::size_t {
    return cells_.size();
  }

  [[nodiscard]]
  auto operator[](std::size_t index) const -> std::string_view {
    if (index < columns()) {
      return cells_[index];
    }

    throw std::out_of_range(
        fmt::format("column index {} is out of historical row column count {}",
                    index,
                    columns()));
  }

  void emplace_next(std::string_view _cell) { cells_.emplace_back(_cell); }

 private:
  std::vector<std::string_view> cells_;
};

}  // namespace Simulator::Generator::Historical

#endif  // SIMULATOR_GENERATOR_IH_HISTORICAL_PARSING_ROW_HPP_
