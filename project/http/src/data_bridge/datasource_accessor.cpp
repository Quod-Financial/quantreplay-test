#include "ih/data_bridge/datasource_accessor.hpp"

#include "data_layer/api/data_access_layer.hpp"
#include "log/logging.hpp"

namespace Simulator::Http::DataBridge {

DataLayerDatasourceAccessor::DataLayerDatasourceAccessor(
    DbContext _context) noexcept
    : context_(std::move(_context)) {}

auto DataLayerDatasourceAccessor::selectSingle(std::uint64_t _id) const noexcept
    -> tl::expected<DataLayer::Datasource, FailureInfo> {
  using Column = DataLayer::Datasource::Attribute;
  using DataLayer::DatasourceCmp;

  try {
    auto pred = DatasourceCmp::eq(Column::DatasourceId, _id);
    return DataLayer::selectOneDatasource(context_, std::move(pred));
  } catch (DataLayer::ConnectionFailure const&) {
    return tl::unexpected{FailureInfo{Failure::DatabaseConnectionError, {}}};
  } catch (DataLayer::CardinalityViolationError const&) {
    return tl::unexpected{FailureInfo{Failure::ResponseCardinalityError, {}}};
  } catch (DataLayer::DataDecodingError const&) {
    return tl::unexpected{FailureInfo{Failure::ResponseDecodingError, {}}};
  } catch (std::exception const& _ex) {
    simulator::log::warn(
        "data access layer raised unexpected exception while selecting "
        "single datasource by `{}' identifier: {}",
        _id,
        _ex.what());
  } catch (...) {
    simulator::log::err(
        "Unknown error is raised by data access layer while selecting "
        "single datasource by `{}' identifier",
        _id);
  }

  return tl::unexpected{FailureInfo{Failure::UnknownError, {}}};
}

auto DataLayerDatasourceAccessor::selectAll() const noexcept
    -> tl::expected<std::vector<DataLayer::Datasource>, FailureInfo> {
  try {
    return DataLayer::selectAllDatasources(context_);
  } catch (DataLayer::ConnectionFailure const&) {
    return tl::unexpected{FailureInfo{Failure::DatabaseConnectionError, {}}};
  } catch (DataLayer::DataDecodingError const&) {
    return tl::unexpected{FailureInfo{Failure::ResponseDecodingError, {}}};
  } catch (std::exception const& _ex) {
    simulator::log::warn(
        "data access layer raised unexpected exception while selecting all "
        "datasources: {}",
        _ex.what());
  } catch (...) {
    simulator::log::err(
        "Unknown error is raised by data access layer while selecting all "
        "datasources");
  }

  return tl::unexpected{FailureInfo{Failure::UnknownError, {}}};
}

auto DataLayerDatasourceAccessor::add(DataLayer::Datasource::Patch _initial)
    const noexcept -> tl::expected<void, FailureInfo> {
  try {
    DataLayer::insertDatasource(context_, std::move(_initial));
    return {};
  } catch (DataLayer::ConnectionFailure const&) {
    return tl::unexpected{FailureInfo{Failure::DatabaseConnectionError, {}}};
  } catch (DataLayer::CardinalityViolationError const&) {
    return tl::unexpected{FailureInfo{Failure::ResponseCardinalityError, {}}};
  } catch (const DataLayer::MalformedPatch& ex) {
    return tl::unexpected{FailureInfo{Failure::MalformedInput, ex.what()}};
  } catch (DataLayer::DataIntegrityError const&) {
    return tl::unexpected{
        FailureInfo{Failure::DataIntegrityViolationError, {}}};
  } catch (DataLayer::DataDecodingError const&) {
    return tl::unexpected{FailureInfo{Failure::ResponseDecodingError, {}}};
  } catch (std::exception const& _ex) {
    simulator::log::warn(
        "data access layer raised unexpected exception while adding a new "
        "datasource: {}",
        _ex.what());
  } catch (...) {
    simulator::log::err(
        "unknown error is raised by data access layer while adding a new "
        "datasource");
  }

  return tl::unexpected{FailureInfo{Failure::UnknownError, {}}};
}

auto DataLayerDatasourceAccessor::update(DataLayer::Datasource::Patch _update,
                                         std::uint64_t _id) const noexcept
    -> tl::expected<void, FailureInfo> {
  using Column = DataLayer::Datasource::Attribute;
  using DataLayer::DatasourceCmp;

  try {
    auto pred = DatasourceCmp::eq(Column::DatasourceId, _id);
    auto venue = DataLayer::updateOneDatasource(
        context_, std::move(_update), std::move(pred));
    return {};
  } catch (DataLayer::ConnectionFailure const&) {
    return tl::unexpected{FailureInfo{Failure::DatabaseConnectionError, {}}};
  } catch (DataLayer::CardinalityViolationError const&) {
    return tl::unexpected{FailureInfo{Failure::ResponseCardinalityError, {}}};
  } catch (const DataLayer::MalformedPatch& ex) {
    return tl::unexpected{FailureInfo{Failure::MalformedInput, ex.what()}};
  } catch (DataLayer::DataIntegrityError const&) {
    return tl::unexpected{
        FailureInfo{Failure::DataIntegrityViolationError, {}}};
  } catch (DataLayer::DataDecodingError const&) {
    return tl::unexpected{FailureInfo{Failure::ResponseDecodingError, {}}};
  } catch (std::exception const& _ex) {
    simulator::log::warn(
        "data access layer raised unexpected exception while updating "
        "the datasource with `{}' identifier: {}",
        _id,
        _ex.what());
  } catch (...) {
    simulator::log::err(
        "unknown error is raised by data access layer while updating "
        "the datasource with `{}' identifier",
        _id);
  }

  return tl::unexpected{FailureInfo{Failure::UnknownError, {}}};
}

}  // namespace Simulator::Http::DataBridge
