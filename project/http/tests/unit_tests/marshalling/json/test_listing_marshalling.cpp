#include <initializer_list>
#include <string>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "data_layer/api/models/listing.hpp"

#include "ih/marshalling/json/listing.hpp"

namespace Simulator::Http {
namespace {

using namespace testing;

// NOLINTBEGIN(*magic-numbers*)

class Http_Json_ListingMarshaller : public ::testing::Test {
  public:
    using Listing = DataLayer::Listing;
    using Patch = Listing::Patch;

    static auto makeDefaultPatch() -> Patch
    {
        return Patch{}.withVenueId("dummy");
    }

    static auto marshall(Listing const& _listing) -> std::string
    {
        return Json::ListingMarshaller::marshall(_listing);
    }

    static auto marshall(std::initializer_list<Listing> _listings)
        -> std::string
    {
        return Json::ListingMarshaller::marshall(_listings);
    }
};


TEST_F(Http_Json_ListingMarshaller, Marshall_ListingID)
{
    auto const patch = makeDefaultPatch();
    auto const listing = Listing::create(patch, 42);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":42,)"
        R"("venueId":"dummy")"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(listing), expectedJson);
}

TEST_F(Http_Json_ListingMarshaller, Marshall_Symbol)
{
    auto const patch = makeDefaultPatch().withSymbol("AAPL");
    auto const listing = Listing::create(patch, 42);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":42,)"
        R"("venueId":"dummy",)"
        R"("symbol":"AAPL")"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(listing), expectedJson);
}

TEST_F(Http_Json_ListingMarshaller, Marshall_VenueID)
{
    auto const patch = makeDefaultPatch().withVenueId("NASDAQ");
    auto const listing = Listing::create(patch, 42);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":42,)"
        R"("venueId":"NASDAQ")"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(listing), expectedJson);
}

TEST_F(Http_Json_ListingMarshaller, Marshall_SecurityType)
{
    auto const patch = makeDefaultPatch().withSecurityType("Equity");
    auto const listing = Listing::create(patch, 42);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":42,)"
        R"("venueId":"dummy",)"
        R"("securityType":"Equity")"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(listing), expectedJson);
}

TEST_F(Http_Json_ListingMarshaller, Marshall_PriceCurrency)
{
    auto const patch = makeDefaultPatch().withPriceCurrency("USD");
    auto const listing = Listing::create(patch, 42);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":42,)"
        R"("venueId":"dummy",)"
        R"("priceCurrency":"USD")"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(listing), expectedJson);
}

TEST_F(Http_Json_ListingMarshaller, Marshall_FxBaseCurrency)
{
    auto const patch = makeDefaultPatch().withFxBaseCurrency("USD");
    auto const listing = Listing::create(patch, 42);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":42,)"
        R"("venueId":"dummy",)"
        R"("fxBaseCurrency":"USD")"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(listing), expectedJson);
}

TEST_F(Http_Json_ListingMarshaller, Marshall_InstrSymbol)
{
    auto const patch = makeDefaultPatch().withInstrSymbol("AAPL");
    auto const listing = Listing::create(patch, 42);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":42,)"
        R"("venueId":"dummy",)"
        R"("instrSymbol":"AAPL")"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(listing), expectedJson);
}

TEST_F(Http_Json_ListingMarshaller, Marshall_QtyMinimum)
{
    auto const patch = makeDefaultPatch().withQtyMinimum(42.42);
    auto const listing = Listing::create(patch, 42);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":42,)"
        R"("venueId":"dummy",)"
        R"("qtyMinimum":42.42)"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(listing), expectedJson);
}

TEST_F(Http_Json_ListingMarshaller, Marshall_QtyMaximum)
{
    auto const patch = makeDefaultPatch().withQtyMaximum(42.42);
    auto const listing = Listing::create(patch, 42);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":42,)"
        R"("venueId":"dummy",)"
        R"("qtyMaximum":42.42)"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(listing), expectedJson);
}

TEST_F(Http_Json_ListingMarshaller, Marshall_QtyMultiple)
{
    auto const patch = makeDefaultPatch().withQtyMultiple(42.42);
    auto const listing = Listing::create(patch, 42);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":42,)"
        R"("venueId":"dummy",)"
        R"("qtyMultiple":42.42)"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(listing), expectedJson);
}

TEST_F(Http_Json_ListingMarshaller, Marshall_PriceTickSize)
{
    auto const patch = makeDefaultPatch().withPriceTickSize(42.42);
    auto const listing = Listing::create(patch, 42);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":42,)"
        R"("venueId":"dummy",)"
        R"("priceTickSize":42.42)"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(listing), expectedJson);
}

TEST_F(Http_Json_ListingMarshaller, Marshall_Enabled)
{
    auto const patch = makeDefaultPatch().withEnabledFlag(false);
    auto const listing = Listing::create(patch, 42);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":42,)"
        R"("venueId":"dummy",)"
        R"("enabled":false)"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(listing), expectedJson);
}

TEST_F(Http_Json_ListingMarshaller, Marshall_RandomQtyMaximum)
{
    auto const patch = makeDefaultPatch().withRandomQtyMaximum(42.42);
    auto const listing = Listing::create(patch, 42);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":42,)"
        R"("venueId":"dummy",)"
        R"("randomQtyMaximum":42.42)"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(listing), expectedJson);
}

TEST_F(Http_Json_ListingMarshaller, Marshall_RandomDepthLevels)
{
    auto const patch = makeDefaultPatch().withRandomDepthLevels(42);
    auto const listing = Listing::create(patch, 42);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":42,)"
        R"("venueId":"dummy",)"
        R"("randomDepthLevels":42)"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(listing), expectedJson);
}

TEST_F(Http_Json_ListingMarshaller, Marshall_RandomOrdersSpread)
{
    auto const patch = makeDefaultPatch().withRandomOrdersSpread(42.42);
    auto const listing = Listing::create(patch, 42);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":42,)"
        R"("venueId":"dummy",)"
        R"("randomOrdersSpread":42.42)"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(listing), expectedJson);
}

TEST_F(Http_Json_ListingMarshaller, Marshall_RandomOrdersRate)
{
    auto const patch = makeDefaultPatch().withRandomOrdersRate(42);
    auto const listing = Listing::create(patch, 42);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":42,)"
        R"("venueId":"dummy",)"
        R"("randomOrdersRate":42)"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(listing), expectedJson);
}

TEST_F(Http_Json_ListingMarshaller, Marshall_RandomTickRange)
{
    auto const patch = makeDefaultPatch().withRandomTickRange(42);
    auto const listing = Listing::create(patch, 42);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":42,)"
        R"("venueId":"dummy",)"
        R"("randomTickRange":42)"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(listing), expectedJson);
}

TEST_F(Http_Json_ListingMarshaller, Marshall_SecurityExchange)
{
    auto const patch = makeDefaultPatch().withSecurityExchange("NASDAQ");
    auto const listing = Listing::create(patch, 42);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":42,)"
        R"("venueId":"dummy",)"
        R"("securityExchange":"NASDAQ")"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(listing), expectedJson);
}

TEST_F(Http_Json_ListingMarshaller, Marshall_PartyID)
{
    auto const patch = makeDefaultPatch().withPartyId("Party");
    auto const listing = Listing::create(patch, 42);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":42,)"
        R"("venueId":"dummy",)"
        R"("partyId":"Party")"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(listing), expectedJson);
}

TEST_F(Http_Json_ListingMarshaller, Marshall_PartyRole)
{
    auto const patch = makeDefaultPatch().withPartyRole("PartyRole");
    auto const listing = Listing::create(patch, 42);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":42,)"
        R"("venueId":"dummy",)"
        R"("partyRole":"PartyRole")"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(listing), expectedJson);
}

TEST_F(Http_Json_ListingMarshaller, Marshall_CusipID)
{
    auto const patch = makeDefaultPatch().withCusipId("Cusip");
    auto const listing = Listing::create(patch, 42);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":42,)"
        R"("venueId":"dummy",)"
        R"("cusipId":"Cusip")"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(listing), expectedJson);
}

TEST_F(Http_Json_ListingMarshaller, Marshall_SedolID)
{
    auto const patch = makeDefaultPatch().withSedolId("Sedol");
    auto const listing = Listing::create(patch, 42);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":42,)"
        R"("venueId":"dummy",)"
        R"("sedolId":"Sedol")"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(listing), expectedJson);
}

TEST_F(Http_Json_ListingMarshaller, Marshall_IsinID)
{
    auto const patch = makeDefaultPatch().withIsinId("Isin");
    auto const listing = Listing::create(patch, 42);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":42,)"
        R"("venueId":"dummy",)"
        R"("isinId":"Isin")"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(listing), expectedJson);
}

TEST_F(Http_Json_ListingMarshaller, Marshall_RicID)
{
    auto const patch = makeDefaultPatch().withRicId("Ric");
    auto const listing = Listing::create(patch, 42);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":42,)"
        R"("venueId":"dummy",)"
        R"("ricId":"Ric")"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(listing), expectedJson);
}

TEST_F(Http_Json_ListingMarshaller, Marshall_ExchangeSymbolID)
{
    auto const patch = makeDefaultPatch().withExchangeSymbolId("EXC");
    auto const listing = Listing::create(patch, 42);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":42,)"
        R"("venueId":"dummy",)"
        R"("exchangeSymbolId":"EXC")"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(listing), expectedJson);
}

TEST_F(Http_Json_ListingMarshaller, Marshall_BloombergSymbolID)
{
    auto const patch = makeDefaultPatch().withBloombergSymbolId("BBG");
    auto const listing = Listing::create(patch, 42);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":42,)"
        R"("venueId":"dummy",)"
        R"("bloombergSymbolId":"BBG")"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(listing), expectedJson);
}

TEST_F(Http_Json_ListingMarshaller, Marshall_RandomQtyMinimum)
{
    auto const patch = makeDefaultPatch().withRandomQtyMinimum(42.42);
    auto const listing = Listing::create(patch, 42);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":42,)"
        R"("venueId":"dummy",)"
        R"("randomQtyMinimum":42.42)"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(listing), expectedJson);
}

TEST_F(Http_Json_ListingMarshaller, Marshall_RandomAmtMinimum)
{
    auto const patch = makeDefaultPatch().withRandomAmtMinimum(42.42);
    auto const listing = Listing::create(patch, 42);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":42,)"
        R"("venueId":"dummy",)"
        R"("randomAmtMinimum":42.42)"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(listing), expectedJson);
}

TEST_F(Http_Json_ListingMarshaller, Marshall_RandomAmtMaximum)
{
    auto const patch = makeDefaultPatch().withRandomAmtMaximum(42.42);
    auto const listing = Listing::create(patch, 42);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":42,)"
        R"("venueId":"dummy",)"
        R"("randomAmtMaximum":42.42)"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(listing), expectedJson);
}

TEST_F(Http_Json_ListingMarshaller, Marshall_RandomOrdersEnabled)
{
    auto const patch = makeDefaultPatch().withRandomOrdersEnabledFlag(true);
    auto const listing = Listing::create(patch, 42);

    // clang-format off
    std::string const expectedJson{"{"
        R"("id":42,)"
        R"("venueId":"dummy",)"
        R"("randomOrdersEnabled":true)"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(listing), expectedJson);
}

TEST_F(Http_Json_ListingMarshaller, Marshall_RandomAggressiveQtyMinimum)
{
    const auto patch = makeDefaultPatch().withRandomAggressiveQtyMinimum(42.42);
    const auto listing = Listing::create(patch, 42);

    // clang-format off
    const std::string expectedJson{"{"
        R"("id":42,)"
        R"("venueId":"dummy",)"
        R"("randomAggQtyMinimum":42.42)"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(listing), expectedJson);
}

TEST_F(Http_Json_ListingMarshaller, Marshall_RandomAggressiveQtyMaximum)
{
    const auto patch = makeDefaultPatch().withRandomAggressiveQtyMaximum(42.42);
    const auto listing = Listing::create(patch, 42);

    // clang-format off
    const std::string expectedJson{"{"
        R"("id":42,)"
        R"("venueId":"dummy",)"
        R"("randomAggQtyMaximum":42.42)"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(listing), expectedJson);
}

TEST_F(Http_Json_ListingMarshaller, Marshall_RandomAggressiveAmtMinimum)
{
    const auto patch = makeDefaultPatch().withRandomAggressiveAmtMinimum(42.42);
    const auto listing = Listing::create(patch, 42);

    // clang-format off
    const std::string expectedJson{"{"
        R"("id":42,)"
        R"("venueId":"dummy",)"
        R"("randomAggAmtMinimum":42.42)"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(listing), expectedJson);
}

TEST_F(Http_Json_ListingMarshaller, Marshall_RandomAggressiveAmtMaximum)
{
    const auto patch = makeDefaultPatch().withRandomAggressiveAmtMaximum(42.42);
    const auto listing = Listing::create(patch, 42);

    // clang-format off
    const std::string expectedJson{"{"
        R"("id":42,)"
        R"("venueId":"dummy",)"
        R"("randomAggAmtMaximum":42.42)"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall(listing), expectedJson);
}

TEST_F(Http_Json_ListingMarshaller, Marshall_ListingsList)
{
    auto const listing1 = Listing ::create(makeDefaultPatch(), 1);
    auto const listing2 = Listing ::create(makeDefaultPatch(), 2);

    // clang-format off
    std::string const expectedJson{"{"
        R"("listings":[)"
            "{"
                R"("id":1,)"
                R"("venueId":"dummy")"
            "},"
            "{"
                R"("id":2,)"
                R"("venueId":"dummy")"
            "}"
        "]"
    "}"};
    // clang-format on

    EXPECT_EQ(marshall({listing1, listing2}), expectedJson);
}


TEST(Http_Json_ListingUnmarshaller, Unmarshall_Symbol)
{
    constexpr std::string_view json{R"({"symbol":"AAPL"})"};

    DataLayer::Listing::Patch patch{};
    Json::ListingUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getSymbol(), Optional(Eq("AAPL")));
}

TEST(Http_Json_ListingUnmarshaller, Unmarshall_VenueID)
{
    constexpr std::string_view json{R"({"venueId":"NASDAQ"})"};

    DataLayer::Listing::Patch patch{};
    Json::ListingUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getVenueId(), Optional(Eq("NASDAQ")));
}

TEST(Http_Json_ListingUnmarshaller, Unmarshall_SecurityType)
{
    constexpr std::string_view json{R"({"securityType":"Equity"})"};

    DataLayer::Listing::Patch patch{};
    Json::ListingUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getSecurityType(), Optional(Eq("Equity")));
}

TEST(Http_Json_ListingUnmarshaller, Unmarshall_PriceCurrency)
{
    constexpr std::string_view json{R"({"priceCurrency":"USD"})"};

    DataLayer::Listing::Patch patch{};
    Json::ListingUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getPriceCurrency(), Optional(Eq("USD")));
}

TEST(Http_Json_ListingUnmarshaller, Unmarshall_FxBaseCurrency)
{
    constexpr std::string_view json{R"({"fxBaseCurrency":"USD"})"};

    DataLayer::Listing::Patch patch{};
    Json::ListingUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getFxBaseCurrency(), Optional(Eq("USD")));
}

TEST(Http_Json_ListingUnmarshaller, Unmarshall_InstrSymbol)
{
    constexpr std::string_view json{R"({"instrSymbol":"AAPL"})"};

    DataLayer::Listing::Patch patch{};
    Json::ListingUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getInstrSymbol(), Optional(Eq("AAPL")));
}

TEST(Http_Json_ListingUnmarshaller, Unmarshall_QtyMinimum)
{
    constexpr std::string_view json{R"({"qtyMinimum":42.42})"};

    DataLayer::Listing::Patch patch{};
    Json::ListingUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getQtyMinimum(), Optional(DoubleEq(42.42)));
}

TEST(Http_Json_ListingUnmarshaller, Unmarshall_QtyMaximum)
{
    constexpr std::string_view json{R"({"qtyMaximum":42.42})"};

    DataLayer::Listing::Patch patch{};
    Json::ListingUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getQtyMaximum(), Optional(DoubleEq(42.42)));
}

TEST(Http_Json_ListingUnmarshaller, Unmarshall_QtyMultiple)
{
    constexpr std::string_view json{R"({"qtyMultiple":42.42})"};

    DataLayer::Listing::Patch patch{};
    Json::ListingUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getQtyMultiple(), Optional(DoubleEq(42.42)));
}

TEST(Http_Json_ListingUnmarshaller, Unmarshall_PriceTickSize)
{
    constexpr std::string_view json{R"({"priceTickSize":42.42})"};

    DataLayer::Listing::Patch patch{};
    Json::ListingUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getPriceTickSize(), Optional(DoubleEq(42.42)));
}

TEST(Http_Json_ListingUnmarshaller, Unmarshall_Enabled)
{
    constexpr std::string_view json{R"({"enabled":true})"};

    DataLayer::Listing::Patch patch{};
    Json::ListingUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getEnabledFlag(), Optional(Eq(true)));
}

TEST(Http_Json_ListingUnmarshaller, Unmarshall_RandomQtyMaximum)
{
    constexpr std::string_view json{R"({"randomQtyMaximum":42.42})"};

    DataLayer::Listing::Patch patch{};
    Json::ListingUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getRandomQtyMaximum(), Optional(DoubleEq(42.42)));
}

TEST(Http_Json_ListingUnmarshaller, Unmarshall_RandomDepthLevels)
{
    constexpr std::string_view json{R"({"randomDepthLevels":42})"};

    DataLayer::Listing::Patch patch{};
    Json::ListingUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getRandomDepthLevels(), Optional(Eq(42)));
}

TEST(Http_Json_ListingUnmarshaller, Unmarshall_RandomOrdersSpread)
{
    constexpr std::string_view json{R"({"randomOrdersSpread":42.42})"};

    DataLayer::Listing::Patch patch{};
    Json::ListingUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getRandomOrdersSpread(), Optional(DoubleEq(42.42)));
}

TEST(Http_Json_ListingUnmarshaller, Unmarshall_RandomOrdersRate)
{
    constexpr std::string_view json{R"({"randomOrdersRate":42})"};

    DataLayer::Listing::Patch patch{};
    Json::ListingUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getRandomOrdersRate(), Optional(Eq(42)));
}

TEST(Http_Json_ListingUnmarshaller, Unmarshall_RandomTickRange)
{
    constexpr std::string_view json{R"({"randomTickRange":42})"};

    DataLayer::Listing::Patch patch{};
    Json::ListingUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getRandomTickRange(), Optional(Eq(42)));
}

TEST(Http_Json_ListingUnmarshaller, Unmarshall_SecurityExchange)
{
    constexpr std::string_view json{R"({"securityExchange":"NASDAQ"})"};

    DataLayer::Listing::Patch patch{};
    Json::ListingUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getSecurityExchange(), Optional(Eq("NASDAQ")));
}

TEST(Http_Json_ListingUnmarshaller, Unmarshall_PartyID)
{
    constexpr std::string_view json{R"({"partyId":"PartyID"})"};

    DataLayer::Listing::Patch patch{};
    Json::ListingUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getPartyId(), Optional(Eq("PartyID")));
}

TEST(Http_Json_ListingUnmarshaller, Unmarshall_PartyRole)
{
    constexpr std::string_view json{R"({"partyRole":"PartyRole"})"};

    DataLayer::Listing::Patch patch{};
    Json::ListingUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getPartyRole(), Optional(Eq("PartyRole")));
}

TEST(Http_Json_ListingUnmarshaller, Unmarshall_CusipID)
{
    constexpr std::string_view json{R"({"cusipId":"Cusip"})"};

    DataLayer::Listing::Patch patch{};
    Json::ListingUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getCusipId(), Optional(Eq("Cusip")));
}

TEST(Http_Json_ListingUnmarshaller, Unmarshall_SedolID)
{
    constexpr std::string_view json{R"({"sedolId":"Sedol"})"};

    DataLayer::Listing::Patch patch{};
    Json::ListingUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getSedolId(), Optional(Eq("Sedol")));
}

TEST(Http_Json_ListingUnmarshaller, Unmarshall_IsinID)
{
    constexpr std::string_view json{R"({"isinId":"Isin"})"};

    DataLayer::Listing::Patch patch{};
    Json::ListingUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getIsinId(), Optional(Eq("Isin")));
}

TEST(Http_Json_ListingUnmarshaller, Unmarshall_RicID)
{
    constexpr std::string_view json{R"({"ricId":"Ric"})"};

    DataLayer::Listing::Patch patch{};
    Json::ListingUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getRicId(), Optional(Eq("Ric")));
}

TEST(Http_Json_ListingUnmarshaller, Unmarshall_ExchangeSymbolID)
{
    constexpr std::string_view json{R"({"exchangeSymbolId":"EXC"})"};

    DataLayer::Listing::Patch patch{};
    Json::ListingUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getExchangeSymbolId(), Optional(Eq("EXC")));
}

TEST(Http_Json_ListingUnmarshaller, Unmarshall_BloombergSymbolID)
{
    constexpr std::string_view json{R"({"bloombergSymbolId":"BBG"})"};

    DataLayer::Listing::Patch patch{};
    Json::ListingUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getBloombergSymbolId(), Optional(Eq("BBG")));
}

TEST(Http_Json_ListingUnmarshaller, Unmarshall_RandomQtyMinimum)
{
    constexpr std::string_view json{R"({"randomQtyMinimum":42.42})"};

    DataLayer::Listing::Patch patch{};
    Json::ListingUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getRandomQtyMinimum(), Optional(DoubleEq(42.42)));
}

TEST(Http_Json_ListingUnmarshaller, Unmarshall_RandomAmtMinimum)
{
    constexpr std::string_view json{R"({"randomAmtMinimum":42.42})"};

    DataLayer::Listing::Patch patch{};
    Json::ListingUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getRandomAmtMinimum(), Optional(DoubleEq(42.42)));
}

TEST(Http_Json_ListingUnmarshaller, Unmarshall_RandomAmtMaximum)
{
    constexpr std::string_view json{R"({"randomAmtMaximum":42.42})"};

    DataLayer::Listing::Patch patch{};
    Json::ListingUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getRandomAmtMaximum(), Optional(DoubleEq(42.42)));
}

TEST(Http_Json_ListingUnmarshaller, Unmarshall_RandomOrdersEnabled)
{
    constexpr std::string_view json{R"({"randomOrdersEnabled":true})"};

    DataLayer::Listing::Patch patch{};
    Json::ListingUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getRandomOrdersEnabledFlag(), Optional(Eq(true)));
}

TEST(Http_Json_ListingUnmarshaller, Unmarshall_RandomAggressiveQtyMinimum)
{
    constexpr std::string_view json{R"({"randomAggQtyMinimum":42.42})"};

    DataLayer::Listing::Patch patch;
    Json::ListingUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getRandomAggressiveQtyMinimum(), Optional(DoubleEq(42.42)));
}

TEST(Http_Json_ListingUnmarshaller, Unmarshall_RandomAggressiveQtyMaximum)
{
    constexpr std::string_view json{R"({"randomAggQtyMaximum":42.42})"};

    DataLayer::Listing::Patch patch;
    Json::ListingUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getRandomAggressiveQtyMaximum(), Optional(DoubleEq(42.42)));
}

TEST(Http_Json_ListingUnmarshaller, Unmarshall_RandomAggressiveAmtMinimum)
{
    constexpr std::string_view json{R"({"randomAggAmtMinimum":42.42})"};

    DataLayer::Listing::Patch patch;
    Json::ListingUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getRandomAggressiveAmtMinimum(), Optional(DoubleEq(42.42)));
}

TEST(Http_Json_ListingUnmarshaller, Unmarshall_RandomAggressiveAmtMaximum)
{
    constexpr std::string_view json{R"({"randomAggAmtMaximum":42.42})"};

    DataLayer::Listing::Patch patch;
    Json::ListingUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getRandomAggressiveAmtMaximum(), Optional(DoubleEq(42.42)));
}

// NOLINTEND(*magic-numbers*)

}  // namespace
}  // namespace Simulator::Http