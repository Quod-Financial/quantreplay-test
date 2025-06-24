#include <initializer_list>
#include <string>
#include <string_view>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "data_layer/api/models/price_seed.hpp"

#include "ih/marshalling/json/price_seed.hpp"

using namespace Simulator;
using namespace Simulator::Http;

using ::testing::DoubleEq;
using ::testing::Eq;
using ::testing::Optional;

class Http_Json_PriceSeedMarshaller : public ::testing::Test {
  public:
    using PriceSeed = DataLayer::PriceSeed;
    using Patch = PriceSeed::Patch;

    static auto makeDefaultPatch() -> Patch
    {
        return Patch{};
    }

    static auto marshall(PriceSeed const& _seed) -> std::string
    {
        return Json::PriceSeedMarshaller::marshall(_seed);
    }

    static auto marshall(std::initializer_list<PriceSeed> _seeds)
        -> std::string
    {
        return Json::PriceSeedMarshaller::marshall(_seeds);
    }
};


TEST_F(Http_Json_PriceSeedMarshaller, Marshall_PriceSeedID)
{
    auto const patch = makeDefaultPatch();
    auto const seed = PriceSeed::create(patch, 42); // NOLINT

    constexpr std::string_view expectedJson = R"({"id":42})";

    EXPECT_EQ(marshall(seed), expectedJson);
}

TEST_F(Http_Json_PriceSeedMarshaller, Marshall_Symbol)
{
    auto const patch = makeDefaultPatch().withSymbol("AAPL");
    auto const seed = PriceSeed::create(patch, 42); // NOLINT

    // clang-format off
    constexpr std::string_view expectedJson = "{"
        R"("id":42,)"
        R"("symbol":"AAPL")"
    "}";
    // clang-format on

    EXPECT_EQ(marshall(seed), expectedJson);
}

TEST_F(Http_Json_PriceSeedMarshaller, Marshall_SecurityType)
{
    auto const patch = makeDefaultPatch().withSecurityType("Equity");
    auto const seed = PriceSeed::create(patch, 42); // NOLINT

    // clang-format off
    constexpr std::string_view expectedJson = "{"
        R"("id":42,)"
        R"("securityType":"Equity")"
    "}";
    // clang-format on

    EXPECT_EQ(marshall(seed), expectedJson);
}

TEST_F(Http_Json_PriceSeedMarshaller, Marshall_PriceCurrency)
{
    auto const patch = makeDefaultPatch().withPriceCurrency("USD");
    auto const seed = PriceSeed::create(patch, 42); // NOLINT

    // clang-format off
    constexpr std::string_view expectedJson = "{"
        R"("id":42,)"
        R"("priceCurrency":"USD")"
    "}";
    // clang-format on

    EXPECT_EQ(marshall(seed), expectedJson);
}

TEST_F(Http_Json_PriceSeedMarshaller, Marshall_SecurityID)
{
    auto const patch = makeDefaultPatch().withSecurityId("IsinNumber");
    auto const seed = PriceSeed::create(patch, 42); // NOLINT

    // clang-format off
    constexpr std::string_view expectedJson = "{"
        R"("id":42,)"
        R"("securityId":"IsinNumber")"
    "}";
    // clang-format on

    EXPECT_EQ(marshall(seed), expectedJson);
}

TEST_F(Http_Json_PriceSeedMarshaller, Marshall_SecurityIDSource)
{
    auto const patch = makeDefaultPatch().withSecurityIdSource("ISIN");
    auto const seed = PriceSeed::create(patch, 42); // NOLINT

    // clang-format off
    constexpr std::string_view expectedJson = "{"
        R"("id":42,)"
        R"("securityIdSource":"ISIN")"
    "}";
    // clang-format on

    EXPECT_EQ(marshall(seed), expectedJson);
}

TEST_F(Http_Json_PriceSeedMarshaller, Marshall_InstrumentSymbol)
{
    auto const patch = makeDefaultPatch().withInstrumentSymbol("AAPL");
    auto const seed = PriceSeed::create(patch, 42); // NOLINT

    // clang-format off
    constexpr std::string_view expectedJson = "{"
        R"("id":42,)"
        R"("instrumentSymbol":"AAPL")"
    "}";
    // clang-format on

    EXPECT_EQ(marshall(seed), expectedJson);
}

TEST_F(Http_Json_PriceSeedMarshaller, Marshall_MidPrice)
{
    auto const patch = makeDefaultPatch().withMidPrice(42.42); // NOLINT
    auto const seed = PriceSeed::create(patch, 42); // NOLINT

    // clang-format off
    constexpr std::string_view expectedJson = "{"
        R"("id":42,)"
        R"("midPrice":42.42)"
    "}";
    // clang-format on

    EXPECT_EQ(marshall(seed), expectedJson);
}

TEST_F(Http_Json_PriceSeedMarshaller, Marshall_BidPrice)
{
    auto const patch = makeDefaultPatch().withBidPrice(42.42); // NOLINT
    auto const seed = PriceSeed::create(patch, 42); // NOLINT

    // clang-format off
    constexpr std::string_view expectedJson = "{"
        R"("id":42,)"
        R"("bidPrice":42.42)"
    "}";
    // clang-format on

    EXPECT_EQ(marshall(seed), expectedJson);
}

TEST_F(Http_Json_PriceSeedMarshaller, Marshall_OfferPrice)
{
    auto const patch = makeDefaultPatch().withOfferPrice(42.42); // NOLINT
    auto const seed = PriceSeed::create(patch, 42); // NOLINT

    // clang-format off
    constexpr std::string_view expectedJson = "{"
        R"("id":42,)"
        R"("offerPrice":42.42)"
    "}";
    // clang-format on

    EXPECT_EQ(marshall(seed), expectedJson);
}

TEST_F(Http_Json_PriceSeedMarshaller, Marshall_LastUpdate)
{
    auto const patch = makeDefaultPatch().withLastUpdate("2023-09-01");
    auto const seed = PriceSeed::create(patch, 42); // NOLINT

    // clang-format off
    constexpr std::string_view expectedJson = "{"
        R"("id":42,)"
        R"("lastUpdate":"2023-09-01")"
    "}";
    // clang-format on

    EXPECT_EQ(marshall(seed), expectedJson);
}

TEST_F(Http_Json_PriceSeedMarshaller, Marshall_PriceSeedsList)
{
    auto const seed1 = PriceSeed ::create(makeDefaultPatch(), 1);
    auto const seed2 = PriceSeed ::create(makeDefaultPatch(), 2);

    // clang-format off
    constexpr std::string_view expectedJson = "{"
        R"("priceSeeds":[)"
            "{"
                R"("id":1)"
            "},"
            "{"
                R"("id":2)"
            "}"
        "]"
    "}";
    // clang-format on

    EXPECT_EQ(marshall({seed1, seed2}), expectedJson);
}


TEST(Http_Json_PriceSeedUnmarshaller, Unmarshall_Symbol)
{
    constexpr std::string_view json = R"({"symbol":"AAPL"})";

    DataLayer::PriceSeed::Patch patch{};
    Json::PriceSeedUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getSymbol(), Optional(Eq("AAPL")));
}

TEST(Http_Json_PriceSeedUnmarshaller, Unmarshall_SecurityType)
{
    constexpr std::string_view json = R"({"securityType":"Equity"})";

    DataLayer::PriceSeed::Patch patch{};
    Json::PriceSeedUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getSecurityType(), Optional(Eq("Equity")));
}

TEST(Http_Json_PriceSeedUnmarshaller, Unmarshall_PriceCurrency)
{
    constexpr std::string_view json = R"({"priceCurrency":"USD"})";

    DataLayer::PriceSeed::Patch patch{};
    Json::PriceSeedUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getPriceCurrency(), Optional(Eq("USD")));
}

TEST(Http_Json_PriceSeedUnmarshaller, Unmarshall_SecurityID)
{
    constexpr std::string_view json = R"({"securityId":"IsinNumber"})";

    DataLayer::PriceSeed::Patch patch{};
    Json::PriceSeedUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getSecurityId(), Optional(Eq("IsinNumber")));
}

TEST(Http_Json_PriceSeedUnmarshaller, Unmarshall_SecurityIDSource)
{
    constexpr std::string_view json = R"({"securityIdSource":"ISIN"})";

    DataLayer::PriceSeed::Patch patch{};
    Json::PriceSeedUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getSecurityIdSource(), Optional(Eq("ISIN")));
}

TEST(Http_Json_PriceSeedUnmarshaller, Unmarshall_InstrumentSymbol)
{
    constexpr std::string_view json = R"({"instrumentSymbol":"AAPL"})";

    DataLayer::PriceSeed::Patch patch{};
    Json::PriceSeedUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getInstrumentSymbol(), Optional(Eq("AAPL")));
}

TEST(Http_Json_PriceSeedUnmarshaller, Unmarshall_MidPrice)
{
    constexpr std::string_view json = R"({"midPrice":42.42})";

    DataLayer::PriceSeed::Patch patch{};
    Json::PriceSeedUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getMidPrice(), Optional(DoubleEq(42.42)));
}

TEST(Http_Json_PriceSeedUnmarshaller, Unmarshall_BidPrice)
{
    constexpr std::string_view json = R"({"bidPrice":42.42})";

    DataLayer::PriceSeed::Patch patch{};
    Json::PriceSeedUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getBidPrice(), Optional(DoubleEq(42.42)));
}

TEST(Http_Json_PriceSeedUnmarshaller, Unmarshall_OfferPrice)
{
    constexpr std::string_view json = R"({"offerPrice":42.42})";

    DataLayer::PriceSeed::Patch patch{};
    Json::PriceSeedUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getOfferPrice(), Optional(DoubleEq(42.42)));
}

TEST(Http_Json_PriceSeedUnmarshaller, Unmarshall_LastUpdate)
{
    constexpr std::string_view json = R"({"lastUpdate":"2023-09-01"})";

    DataLayer::PriceSeed::Patch patch{};
    Json::PriceSeedUnmarshaller::unmarshall(json, patch);

    EXPECT_THAT(patch.getLastUpdate(), Optional(Eq("2023-09-01")));
}