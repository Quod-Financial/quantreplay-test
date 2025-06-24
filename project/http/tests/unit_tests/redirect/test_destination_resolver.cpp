#include <string>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <tl/expected.hpp>

#include "data_layer/api/models/venue.hpp"

#include "ih/redirect/resolver.hpp"
#include "mocks/venue_accessor.hpp"
#include "ih/redirect/destination_resolver.hpp"

using namespace Simulator;
using namespace Simulator::Http;
using namespace Simulator::Http::Redirect;

using ::testing::DoAll;
using ::testing::Eq;
using ::testing::Return;
using ::testing::SetArgReferee;

class Http_Redirect_DestinationResolver : public testing::Test {
  public:
    auto venueAccessor() -> Mock::VenueAccessor& { return mVenueAccessor; }

    auto resolve(std::string const& _venueID)
        -> DestinationResolver::ResolvingResult
    {
      return makeResolver().resolveByVenueID(_venueID);
    }

    auto resolveHostnameAsVenueId(bool value) -> void {
      mResolveHostnameAsVenueId = value;
    }

    static auto makeVenue(std::string const& _venueID)
        -> DataLayer::Venue
    {
        DataLayer::Venue::Patch patch{};
        patch.withVenueId(_venueID);
        return DataLayer::Venue::create(std::move(patch));
    }

    static auto makeVenue(std::string const& _venueID, std::uint16_t _port)
        -> DataLayer::Venue
    {
        DataLayer::Venue::Patch patch{};
        patch.withVenueId(_venueID).withRestPort(_port);
        return DataLayer::Venue::create(std::move(patch));
    }

  private:
    auto makeResolver() -> Redirect::DestinationResolver {
      return Redirect::DestinationResolver(
          mVenueAccessor,
          mResolveHostnameAsVenueId);
    }

    Mock::VenueAccessor mVenueAccessor;
    bool mResolveHostnameAsVenueId = false;
};


TEST_F(Http_Redirect_DestinationResolver, Resolve_ByVenueID_NonexistentVenue)
{
    std::string const venueID = "XLSE";
    Mock::VenueAccessor::VenueResult const accessorReply{
        tl::unexpected(DataBridge::Failure::ResponseCardinalityError)};

    EXPECT_CALL(venueAccessor(), selectSingle(Eq(venueID)))
        .Times(1)
        .WillOnce(Return(accessorReply));

    auto [destination, status] = resolve(venueID);

    EXPECT_EQ(status, Resolver::Status::NonexistentInstance);
    EXPECT_FALSE(destination.has_value());
}

TEST_F(Http_Redirect_DestinationResolver, Resolve_ByVenueID_RestPortAbsent)
{
    std::string const venueID = "XLSE";
    DataLayer::Venue const venue = makeVenue(venueID);
    ASSERT_FALSE(venue.getRestPort().has_value());

    Mock::VenueAccessor::VenueResult const accessorReply{venue};
    EXPECT_CALL(venueAccessor(), selectSingle(Eq(venueID)))
        .Times(1)
        .WillOnce(Return(accessorReply));

    auto [destination, status] = resolve(venueID);

    EXPECT_EQ(status, Resolver::Status::ResolvingFailed);
    EXPECT_FALSE(destination.has_value());
}

TEST_F(Http_Redirect_DestinationResolver, Resolve_ByVenueID_RestPortPresent)
{
    std::string const venueID = "XLSE";
    std::string const venueHost = "localhost";
    std::uint16_t const venuePort = 9001;
    DataLayer::Venue const venue = makeVenue(venueID, venuePort);
    ASSERT_TRUE(venue.getRestPort().has_value());

    Mock::VenueAccessor::VenueResult const accessorReply{venue};
    EXPECT_CALL(venueAccessor(), selectSingle(Eq(venueID)))
       .Times(1)
       .WillOnce(Return(accessorReply));

    auto [destination, status] = resolve(venueID);

    EXPECT_EQ(status, Resolver::Status::Success);

    // NOLINTBEGIN bugprone-unchecked-optional-access
    ASSERT_TRUE(destination.has_value());
    EXPECT_EQ(destination->getHost(), venueHost);
    EXPECT_EQ(destination->getPort(), venuePort);
    // NOLINTEND
}

TEST_F(Http_Redirect_DestinationResolver,
       Resolve_ByVenueID_ReolveHostnameAsVenueId) {
    resolveHostnameAsVenueId(true);
    DataLayer::Venue const venue = makeVenue("XLSE", 9000);

    Mock::VenueAccessor::VenueResult const accessorReply{venue};
    EXPECT_CALL(venueAccessor(), selectSingle(Eq("XLSE")))
       .Times(1)
       .WillOnce(Return(accessorReply));

    auto [destination, status] = resolve("XLSE");

    EXPECT_EQ(status, Resolver::Status::Success);

    ASSERT_TRUE(destination.has_value());
    ASSERT_EQ(destination->getHost(), "XLSE");
    ASSERT_EQ(destination->getPort(), 9000);
}
