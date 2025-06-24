#ifndef SIMULATOR_HTTP_TESTS_MOCKS_VENUE_ACCESSOR_HPP_
#define SIMULATOR_HTTP_TESTS_MOCKS_VENUE_ACCESSOR_HPP_

#include <gmock/gmock.h>

#include <tl/expected.hpp>

#include "data_layer/api/models/venue.hpp"
#include "ih/data_bridge/operation_failure.hpp"
#include "ih/data_bridge/venue_accessor.hpp"

namespace Simulator::Http::Mock {

class VenueAccessor : public DataBridge::VenueAccessor {
  template <typename T>
  using Result = tl::expected<T, DataBridge::Failure>;
  using Venue = DataLayer::Venue;
  using Patch = Venue::Patch;

 public:
  using VenueResult = Result<Venue>;
  using VenuesResult = Result<std::vector<Venue>>;
  using EmptyResult = Result<void>;

  MOCK_METHOD(VenueResult,
              selectSingle,
              (std::string const&),
              (const, noexcept, override));

  MOCK_METHOD(VenuesResult, selectAll, (), (const, noexcept, override));

  MOCK_METHOD(EmptyResult, add, (Patch), (const, noexcept, override));

  MOCK_METHOD(EmptyResult,
              update,
              (Patch, std::string const&),
              (const, noexcept, override));
};

}  // namespace Simulator::Http::Mock

#endif  // SIMULATOR_HTTP_TESTS_MOCKS_VENUE_ACCESSOR_HPP_
