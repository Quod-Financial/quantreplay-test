#include "api/models/column_mapping.hpp"

#include <string>
#include <utility>

namespace Simulator::DataLayer {

auto ColumnMapping::create(ColumnMapping::Patch snapshot,
                           std::uint64_t datasource_id) noexcept
    -> ColumnMapping {
  ColumnMapping columnMapping;
  columnMapping.column_from_ = std::move(snapshot.column_from_);
  columnMapping.column_to_ = std::move(snapshot.column_to_);
  columnMapping.datasource_id_ = datasource_id;
  return columnMapping;
}

auto ColumnMapping::column_from() const noexcept -> const std::string& {
  return column_from_;
}

auto ColumnMapping::column_to() const noexcept -> const std::string& {
  return column_to_;
}

auto ColumnMapping::datasource_id() const noexcept -> std::uint64_t {
  return datasource_id_;
}

auto ColumnMapping::Patch::column_from() const noexcept -> const std::string& {
  return column_from_;
}

auto ColumnMapping::Patch::with_column_from(std::string column) noexcept
    -> Patch& {
  column_from_ = std::move(column);
  return *this;
}

auto ColumnMapping::Patch::column_to() const noexcept -> const std::string& {
  return column_to_;
}

auto ColumnMapping::Patch::with_column_to(std::string column) noexcept
    -> Patch& {
  column_to_ = std::move(column);
  return *this;
}

}  // namespace Simulator::DataLayer
