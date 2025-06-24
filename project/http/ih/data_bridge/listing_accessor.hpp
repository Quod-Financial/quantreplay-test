#ifndef SIMULATOR_HTTP_IH_DATA_BRIDGE_LISTING_ACCESSOR_HPP_
#define SIMULATOR_HTTP_IH_DATA_BRIDGE_LISTING_ACCESSOR_HPP_

#include <cstdint>
#include <tl/expected.hpp>
#include <vector>

#include "data_layer/api/database/context.hpp"
#include "data_layer/api/models/listing.hpp"
#include "ih/data_bridge/operation_failure.hpp"

namespace Simulator::Http::DataBridge {

class ListingAccessor {
 public:
  ListingAccessor() = default;
  ListingAccessor(ListingAccessor const&) = default;
  ListingAccessor(ListingAccessor&&) noexcept = default;

  auto operator=(ListingAccessor const&) -> ListingAccessor& = default;
  auto operator=(ListingAccessor&&) noexcept -> ListingAccessor& = default;

  virtual ~ListingAccessor() = default;

  [[nodiscard]]
  virtual auto selectSingle(std::uint64_t _listingId) const noexcept
      -> tl::expected<DataLayer::Listing, Failure> = 0;

  [[nodiscard]]
  virtual auto selectSingle(std::string const& _symbol) const noexcept
      -> tl::expected<DataLayer::Listing, Failure> = 0;

  [[nodiscard]]
  virtual auto selectAll() const noexcept
      -> tl::expected<std::vector<DataLayer::Listing>, Failure> = 0;

  [[nodiscard]]
  virtual auto add(DataLayer::Listing::Patch _snapshot) const noexcept
      -> tl::expected<void, Failure> = 0;

  [[nodiscard]]
  virtual auto update(DataLayer::Listing::Patch _update,
                      std::uint64_t _listingId) const noexcept
      -> tl::expected<void, Failure> = 0;

  [[nodiscard]]
  virtual auto update(DataLayer::Listing::Patch _update,
                      std::string const& _symbol) const noexcept
      -> tl::expected<void, Failure> = 0;
};

class DataLayerListingAccessor final : public ListingAccessor {
 public:
  using DbContext = DataLayer::Database::Context;

  DataLayerListingAccessor() = delete;

  explicit DataLayerListingAccessor(DbContext _context) noexcept;

  [[nodiscard]]
  auto selectSingle(std::uint64_t _listingId) const noexcept
      -> tl::expected<DataLayer::Listing, Failure> override;

  [[nodiscard]]
  auto selectSingle(std::string const& _symbol) const noexcept
      -> tl::expected<DataLayer::Listing, Failure> override;

  [[nodiscard]]
  auto selectAll() const noexcept
      -> tl::expected<std::vector<DataLayer::Listing>, Failure> override;

  [[nodiscard]]
  auto add(DataLayer::Listing::Patch _snapshot) const noexcept
      -> tl::expected<void, Failure> override;

  [[nodiscard]]
  auto update(DataLayer::Listing::Patch _update,
              std::uint64_t _listingId) const noexcept
      -> tl::expected<void, Failure> override;

  [[nodiscard]]
  auto update(DataLayer::Listing::Patch _update,
              std::string const& _symbol) const noexcept
      -> tl::expected<void, Failure> override;

 private:
  DataLayer::Database::Context context_;
};

}  // namespace Simulator::Http::DataBridge

#endif  // SIMULATOR_HTTP_IH_DATA_BRIDGE_LISTING_ACCESSOR_HPP_
