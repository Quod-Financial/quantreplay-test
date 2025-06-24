#ifndef SIMULATOR_HTTP_IH_DATA_BRIDGE_VENUE_ACCESSOR_HPP_
#define SIMULATOR_HTTP_IH_DATA_BRIDGE_VENUE_ACCESSOR_HPP_

#include <tl/expected.hpp>
#include <vector>

#include "data_layer/api/database/context.hpp"
#include "data_layer/api/models/venue.hpp"
#include "ih/data_bridge/operation_failure.hpp"

namespace Simulator::Http::DataBridge {

class VenueAccessor {
 public:
  VenueAccessor() = default;
  VenueAccessor(VenueAccessor const&) = default;
  VenueAccessor(VenueAccessor&&) noexcept = default;

  auto operator=(VenueAccessor const&) -> VenueAccessor& = default;
  auto operator=(VenueAccessor&&) noexcept -> VenueAccessor& = default;

  virtual ~VenueAccessor() = default;

  [[nodiscard]]
  virtual auto selectSingle(std::string const& _venueId) const noexcept
      -> tl::expected<DataLayer::Venue, Failure> = 0;

  [[nodiscard]]
  virtual auto selectAll() const noexcept
      -> tl::expected<std::vector<DataLayer::Venue>, Failure> = 0;

  [[nodiscard]]
  virtual auto add(DataLayer::Venue::Patch _snapshot) const noexcept
      -> tl::expected<void, Failure> = 0;

  [[nodiscard]]
  virtual auto update(DataLayer::Venue::Patch _update,
                      std::string const& _venueId) const noexcept
      -> tl::expected<void, Failure> = 0;
};

class DataLayerVenueAccessor final : public VenueAccessor {
 public:
  using DbContext = DataLayer::Database::Context;

  DataLayerVenueAccessor() = delete;

  explicit DataLayerVenueAccessor(DbContext _context) noexcept;

  [[nodiscard]]
  auto selectSingle(std::string const& _venueId) const noexcept
      -> tl::expected<DataLayer::Venue, Failure> override;

  [[nodiscard]]
  auto selectAll() const noexcept
      -> tl::expected<std::vector<DataLayer::Venue>, Failure> override;

  [[nodiscard]]
  auto add(DataLayer::Venue::Patch _snapshot) const noexcept
      -> tl::expected<void, Failure> override;

  [[nodiscard]]
  auto update(DataLayer::Venue::Patch _update,
              std::string const& _venueId) const noexcept
      -> tl::expected<void, Failure> override;

 private:
  DataLayer::Database::Context context_;
};

}  // namespace Simulator::Http::DataBridge

#endif  // SIMULATOR_HTTP_IH_DATA_BRIDGE_VENUE_ACCESSOR_HPP_
