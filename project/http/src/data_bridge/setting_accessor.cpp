#include "ih/data_bridge/setting_accessor.hpp"

#include "data_layer/api/data_access_layer.hpp"
#include "log/logging.hpp"

namespace Simulator::Http::DataBridge {

DataLayerSettingAccessor::DataLayerSettingAccessor(DbContext _context) noexcept
    : context_(std::move(_context)) {}

auto DataLayerSettingAccessor::selectSingle(std::string const& _key)
    const noexcept -> tl::expected<DataLayer::Setting, Failure> {
  using Column = DataLayer::Setting::Attribute;
  using DataLayer::SettingCmp;

  try {
    auto pred = SettingCmp::eq(Column::Key, _key);
    return DataLayer::selectOneSetting(context_, std::move(pred));
  } catch (DataLayer::ConnectionFailure const&) {
    return tl::unexpected{Failure::DatabaseConnectionError};
  } catch (DataLayer::CardinalityViolationError const&) {
    return tl::unexpected{Failure::ResponseCardinalityError};
  } catch (DataLayer::DataDecodingError const&) {
    return tl::unexpected{Failure::ResponseDecodingError};
  } catch (std::exception const& _ex) {
    simulator::log::warn(
        "data access layer raised unexpected exception while selecting "
        "single setting by `{}' key: {}",
        _key,
        _ex.what());
  } catch (...) {
    simulator::log::err(
        "unknown error is raised by data access layer while selecting "
        "single setting by `{}' key",
        _key);
  }

  return tl::unexpected{Failure::UnknownError};
}

auto DataLayerSettingAccessor::selectAll() const noexcept
    -> tl::expected<std::vector<DataLayer::Setting>, Failure> {
  try {
    return DataLayer::selectAllSettings(context_);
  } catch (DataLayer::ConnectionFailure const&) {
    return tl::unexpected{Failure::DatabaseConnectionError};
  } catch (DataLayer::DataDecodingError const&) {
    return tl::unexpected{Failure::ResponseDecodingError};
  } catch (std::exception const& _ex) {
    simulator::log::warn(
        "data access layer raised unexpected exception while selecting all "
        "settings: {}",
        _ex.what());
  } catch (...) {
    simulator::log::err(
        "unknown error is raised by data access layer while selecting all "
        "settings");
  }

  return tl::unexpected{Failure::UnknownError};
}

auto DataLayerSettingAccessor::add(DataLayer::Setting::Patch _snapshot)
    const noexcept -> tl::expected<void, Failure> {
  try {
    DataLayer::insertSetting(context_, std::move(_snapshot));
    return {};
  } catch (DataLayer::ConnectionFailure const&) {
    return tl::unexpected{Failure::DatabaseConnectionError};
  } catch (DataLayer::CardinalityViolationError const&) {
    return tl::unexpected{Failure::ResponseCardinalityError};
  } catch (DataLayer::MalformedPatch const&) {
    return tl::unexpected{Failure::MalformedInput};
  } catch (DataLayer::DataIntegrityError const&) {
    return tl::unexpected{Failure::DataIntegrityViolationError};
  } catch (DataLayer::DataDecodingError const&) {
    return tl::unexpected{Failure::ResponseDecodingError};
  } catch (std::exception const& _ex) {
    simulator::log::warn(
        "data access layer raised unexpected exception while adding a new "
        "setting: {}",
        _ex.what());
  } catch (...) {
    simulator::log::err(
        "unknown error is raised by data access layer while adding a new "
        "setting");
  }

  return tl::unexpected{Failure::UnknownError};
}

auto DataLayerSettingAccessor::update(DataLayer::Setting::Patch _update,
                                      std::string const& _key) const noexcept
    -> tl::expected<void, Failure> {
  using Column = DataLayer::Setting::Attribute;
  using DataLayer::SettingCmp;

  try {
    auto pred = SettingCmp::eq(Column::Key, _key);
    auto venue = DataLayer::updateOneSetting(
        context_, std::move(_update), std::move(pred));

    return {};
  } catch (DataLayer::ConnectionFailure const&) {
    return tl::unexpected{Failure::DatabaseConnectionError};
  } catch (DataLayer::CardinalityViolationError const&) {
    return tl::unexpected{Failure::ResponseCardinalityError};
  } catch (DataLayer::MalformedPatch const&) {
    return tl::unexpected{Failure::MalformedInput};
  } catch (DataLayer::DataIntegrityError const&) {
    return tl::unexpected{Failure::DataIntegrityViolationError};
  } catch (DataLayer::DataDecodingError const&) {
    return tl::unexpected{Failure::ResponseDecodingError};
  } catch (std::exception const& _ex) {
    simulator::log::warn(
        "data access layer raised unexpected exception while updating "
        "the setting with `{}' key: {}",
        _key,
        _ex.what());
  } catch (...) {
    simulator::log::err(
        "unknown error is raised by data access layer while updating "
        "the setting with `{}' key",
        _key);
  }

  return tl::unexpected{Failure::UnknownError};
}

}  // namespace Simulator::Http::DataBridge
