#include "ih/data_bridge/price_seed_accessor.hpp"

#include "data_layer/api/data_access_layer.hpp"
#include "log/logging.hpp"

namespace Simulator::Http::DataBridge {

DataLayerPriceSeedAccessor::DataLayerPriceSeedAccessor(
    DataLayer::Database::Context _context) noexcept
    : context_(std::move(_context)) {}

auto DataLayerPriceSeedAccessor::selectSingle(std::uint64_t _seedId)
    const noexcept -> tl::expected<DataLayer::PriceSeed, Failure> {
  using Column = DataLayer::PriceSeed::Attribute;
  using DataLayer::PriceSeedCmp;

  try {
    auto pred = PriceSeedCmp::eq(Column::PriceSeedId, _seedId);
    return DataLayer::selectOnePriceSeed(context_, std::move(pred));
  } catch (DataLayer::ConnectionFailure const&) {
    return tl::unexpected{Failure::DatabaseConnectionError};
  } catch (DataLayer::CardinalityViolationError const&) {
    return tl::unexpected{Failure::ResponseCardinalityError};
  } catch (DataLayer::DataDecodingError const&) {
    return tl::unexpected{Failure::ResponseDecodingError};
  } catch (std::exception const& _ex) {
    simulator::log::warn(
        "data access layer raised unexpected exception while selecting "
        "single price seed by `{}' identifier: {}",
        _seedId,
        _ex.what());
  } catch (...) {
    simulator::log::err(
        "unknown error is raised by data access layer while selecting "
        "single price seed by `{}' identifier",
        _seedId);
  }

  return tl::unexpected{Failure::UnknownError};
}

auto DataLayerPriceSeedAccessor::selectAll() const noexcept
    -> tl::expected<std::vector<DataLayer::PriceSeed>, Failure> {
  try {
    return DataLayer::selectAllPriceSeeds(context_);
  } catch (DataLayer::ConnectionFailure const&) {
    return tl::unexpected{Failure::DatabaseConnectionError};
  } catch (DataLayer::DataDecodingError const&) {
    return tl::unexpected{Failure::ResponseDecodingError};
  } catch (std::exception const& _ex) {
    simulator::log::warn(
        "data access layer raised unexpected exception while selecting all "
        "price seeds: {}",
        _ex.what());
  } catch (...) {
    simulator::log::err(
        "unknown error is raised by data access layer while selecting all "
        "price seeds");
  }

  return tl::unexpected{Failure::UnknownError};
}

auto DataLayerPriceSeedAccessor::add(DataLayer::PriceSeed::Patch _snapshot)
    const noexcept -> tl::expected<void, Failure> {
  try {
    DataLayer::insertPriceSeed(context_, std::move(_snapshot));
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
        "price seed: {}",
        _ex.what());
  } catch (...) {
    simulator::log::err(
        "unknown error is raised by data access layer while adding a new "
        "price seed");
  }

  return tl::unexpected{Failure::UnknownError};
}

auto DataLayerPriceSeedAccessor::update(DataLayer::PriceSeed::Patch _update,
                                        std::uint64_t _seedId) const noexcept
    -> tl::expected<void, Failure> {
  using Column = DataLayer::PriceSeed::Attribute;
  using DataLayer::PriceSeedCmp;

  try {
    auto pred = PriceSeedCmp::eq(Column::PriceSeedId, _seedId);
    DataLayer::updateOnePriceSeed(
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
        "the price seed with `{}' identifier: {}",
        _seedId,
        _ex.what());
  } catch (...) {
    simulator::log::err(
        "Unknown error is raised by data access layer while updating "
        "the price seed with `{}' identifier",
        _seedId);
  }

  return tl::unexpected{Failure::UnknownError};
}

auto DataLayerPriceSeedAccessor::dropSingle(
    std::uint64_t _seedId) const noexcept -> tl::expected<void, Failure> {
  using Column = DataLayer::PriceSeed::Attribute;
  using DataLayer::PriceSeedCmp;

  try {
    auto pred = PriceSeedCmp::eq(Column::PriceSeedId, _seedId);
    DataLayer::deleteOnePriceSeed(context_, std::move(pred));
    return {};
  } catch (DataLayer::ConnectionFailure const&) {
    return tl::unexpected{Failure::DatabaseConnectionError};
  } catch (DataLayer::CardinalityViolationError const&) {
    return tl::unexpected{Failure::ResponseCardinalityError};
  } catch (std::exception const& _ex) {
    simulator::log::warn(
        "data access layer raised unexpected exception while deleting "
        "single price seed by `{}' identifier: {}",
        _seedId,
        _ex.what());
  } catch (...) {
    simulator::log::err(
        "unknown error is raised by data access layer while deleting "
        "single price seed by `{}' identifier",
        _seedId);
  }

  return tl::unexpected{Failure::UnknownError};
}

auto DataLayerPriceSeedAccessor::sync(std::string _pqxxSourceConnection)
    const noexcept -> tl::expected<void, Failure> {
  DataLayer::Database::Context const extDatabase =
      DataLayer::Database::createPqxxConnection(
          std::move(_pqxxSourceConnection));

  std::vector<DataLayer::PriceSeed> externalSeeds{};
  try {
    externalSeeds = DataLayer::selectAllPriceSeeds(extDatabase);
  } catch (std::exception const& _ex) {
    simulator::log::warn(
        "data access layer raised exception while loading price seeds from "
        "external price seed sync database: {}",
        _ex.what());
    return tl::unexpected{Failure::UnknownError};
  } catch (...) {
    simulator::log::err(
        "data access layer raised unknown exception while  "
        "loading price seeds from external price seed sync database");
    return tl::unexpected{Failure::UnknownError};
  }

  try {
    for (auto const& extPriceSeed : externalSeeds) {
      syncSeeds(extPriceSeed);
    }
  } catch (std::exception const& _ex) {
    simulator::log::warn(
        "data access layer raised exception while syncing price seeds "
        "with a downloaded external price seed: {}",
        _ex.what());
    return tl::unexpected{Failure::UnknownError};
  } catch (...) {
    simulator::log::err(
        "data access layer raised unknown exception while "
        "syncing price seeds  with a downloaded external price seed");
    return tl::unexpected{Failure::UnknownError};
  }

  return {};
}

void DataLayerPriceSeedAccessor::syncSeeds(
    DataLayer::PriceSeed const& _extSeed) const {
  std::optional<std::string> const& symbol = _extSeed.getSymbol();
  if (!symbol.has_value()) {
    simulator::log::info(
        "the price seed record without a symbol found in the external "
        "price seed table with an identifier `{}', record can not be synced",
        _extSeed.getPriceSeedId());
    return;
  }

  DataLayer::PriceSeed::Patch patch{};
  if (std::optional<double> const value = _extSeed.getMidPrice()) {
    patch.withMidPrice(*value);
  }
  if (std::optional<double> const value = _extSeed.getBidPrice()) {
    patch.withBidPrice(*value);
  }
  if (std::optional<double> const value = _extSeed.getOfferPrice()) {
    patch.withOfferPrice(*value);
  }
  if (auto const& seedUpdateTime = _extSeed.getLastUpdate()) {
    patch.withLastUpdate(*seedUpdateTime);
  }

  using DataLayer::PriceSeedCmp;
  using Column = DataLayer::PriceSeed::Attribute;
  auto pred = PriceSeedCmp::eq(Column::Symbol, *symbol);

  std::vector<DataLayer::PriceSeed> const updated =
      DataLayer::updateAllPriceSeeds(context_, patch, pred);
  if (!updated.empty()) {
    // Price seeds with such symbol are updated in the db, nothing to do...
    return;
  }

  simulator::log::info(
      "the price seed with `{}' symbol is not present in the local "
      "price seed table, but exists in the external one, inserting "
      "it into the local table",
      *symbol);

  patch.withSymbol(*symbol);
  if (auto const& value = _extSeed.getSecurityType()) {
    patch.withSecurityType(*value);
  }
  if (auto const& value = _extSeed.getPriceCurrency()) {
    patch.withPriceCurrency(*value);
  }
  if (auto const& value = _extSeed.getSecurityId()) {
    patch.withSecurityId(*value);
  }
  if (auto const& value = _extSeed.getSecurityIdSource()) {
    patch.withSecurityIdSource(*value);
  }
  if (auto const& value = _extSeed.getInstrumentSymbol()) {
    patch.withInstrumentSymbol(*value);
  }

  DataLayer::insertPriceSeed(context_, std::move(patch));
}

}  // namespace Simulator::Http::DataBridge
