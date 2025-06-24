#ifndef SIMULATOR_HTTP_IH_DATA_BRIDGE_PRICE_SEED_ACCESSOR_HPP_
#define SIMULATOR_HTTP_IH_DATA_BRIDGE_PRICE_SEED_ACCESSOR_HPP_

#include <cstdint>
#include <tl/expected.hpp>
#include <vector>

#include "data_layer/api/database/context.hpp"
#include "data_layer/api/models/price_seed.hpp"
#include "ih/data_bridge/operation_failure.hpp"

namespace Simulator::Http::DataBridge {

class PriceSeedAccessor {
 public:
  PriceSeedAccessor() = default;
  PriceSeedAccessor(PriceSeedAccessor const&) = default;
  PriceSeedAccessor(PriceSeedAccessor&&) noexcept = default;

  auto operator=(PriceSeedAccessor const&) -> PriceSeedAccessor& = default;
  auto operator=(PriceSeedAccessor&&) noexcept -> PriceSeedAccessor& = default;

  virtual ~PriceSeedAccessor() = default;

  [[nodiscard]]
  virtual auto selectSingle(std::uint64_t _seedId) const noexcept
      -> tl::expected<DataLayer::PriceSeed, Failure> = 0;

  [[nodiscard]]
  virtual auto selectAll() const noexcept
      -> tl::expected<std::vector<DataLayer::PriceSeed>, Failure> = 0;

  [[nodiscard]]
  virtual auto add(DataLayer::PriceSeed::Patch _snapshot) const noexcept
      -> tl::expected<void, Failure> = 0;

  [[nodiscard]]
  virtual auto update(DataLayer::PriceSeed::Patch _update,
                      std::uint64_t _seedId) const noexcept
      -> tl::expected<void, Failure> = 0;

  [[nodiscard]]
  virtual auto dropSingle(std::uint64_t _seedId) const noexcept
      -> tl::expected<void, Failure> = 0;

  [[nodiscard]]
  virtual auto sync(std::string _pqxxSourceConnection) const noexcept
      -> tl::expected<void, Failure> = 0;
};

class DataLayerPriceSeedAccessor final : public PriceSeedAccessor {
 public:
  using DbContext = DataLayer::Database::Context;

  DataLayerPriceSeedAccessor() = delete;

  explicit DataLayerPriceSeedAccessor(DbContext _context) noexcept;

  [[nodiscard]]
  auto selectSingle(std::uint64_t _seedId) const noexcept
      -> tl::expected<DataLayer::PriceSeed, Failure> override;

  [[nodiscard]]
  auto selectAll() const noexcept
      -> tl::expected<std::vector<DataLayer::PriceSeed>, Failure> override;

  [[nodiscard]]
  auto add(DataLayer::PriceSeed::Patch _snapshot) const noexcept
      -> tl::expected<void, Failure> override;

  [[nodiscard]]
  auto update(DataLayer::PriceSeed::Patch _update,
              std::uint64_t _seedId) const noexcept
      -> tl::expected<void, Failure> override;

  [[nodiscard]]
  auto dropSingle(std::uint64_t _seedId) const noexcept
      -> tl::expected<void, Failure> override;

  [[nodiscard]]
  auto sync(std::string _pqxxSourceConnection) const noexcept
      -> tl::expected<void, Failure> override;

 private:
  void syncSeeds(DataLayer::PriceSeed const& _extSeed) const;

  DataLayer::Database::Context context_;
};

}  // namespace Simulator::Http::DataBridge

#endif  // SIMULATOR_HTTP_IH_DATA_BRIDGE_PRICE_SEED_ACCESSOR_HPP_
