#include <optional>
#include <string>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "api/models/price_seed.hpp"

using ::testing::DoubleEq;
using ::testing::Eq;
using ::testing::Optional;

using namespace Simulator::DataLayer;


TEST(DataLayer_Model_PriceSeed, Patch_Set_Symbol)
{
    PriceSeed::Patch patch{};
    EXPECT_FALSE(patch.getSymbol().has_value());

    patch.withSymbol("AAPL");
    EXPECT_THAT(patch.getSymbol(), Optional(Eq("AAPL")));
}

TEST(DataLayer_Model_PriceSeed, Patch_Set_SecurityType)
{
    PriceSeed::Patch patch{};
    EXPECT_FALSE(patch.getSecurityType().has_value());

    patch.withSecurityType("CS");
    EXPECT_THAT(patch.getSecurityType(), Optional(Eq("CS")));
}

TEST(DataLayer_Model_PriceSeed, Patch_Set_PriceCurrency)
{
    PriceSeed::Patch patch{};
    EXPECT_FALSE(patch.getPriceCurrency().has_value());

    patch.withPriceCurrency("USD");
    EXPECT_THAT(patch.getPriceCurrency(), Optional(Eq("USD")));
}

TEST(DataLayer_Model_PriceSeed, Patch_Set_SecurityID)
{
    PriceSeed::Patch patch{};
    EXPECT_FALSE(patch.getSecurityId().has_value());

    patch.withSecurityId("US0378331005");
    EXPECT_THAT(patch.getSecurityId(), Optional(Eq("US0378331005")));
}

TEST(DataLayer_Model_PriceSeed, Patch_Set_SecurityIDSource)
{
    PriceSeed::Patch patch{};
    EXPECT_FALSE(patch.getSecurityIdSource().has_value());

    patch.withSecurityIdSource("ISIN");
    EXPECT_THAT(patch.getSecurityIdSource(), Optional(Eq("ISIN")));
}

TEST(DataLayer_Model_PriceSeed, Patch_Set_InstrumentSymbol)
{
    PriceSeed::Patch patch{};
    EXPECT_FALSE(patch.getInstrumentSymbol().has_value());

    patch.withInstrumentSymbol("AAPL");
    EXPECT_THAT(patch.getInstrumentSymbol(), Optional(Eq("AAPL")));
}

TEST(DataLayer_Model_PriceSeed, Patch_Set_MidPrice)
{
    PriceSeed::Patch patch{};
    EXPECT_FALSE(patch.getMidPrice().has_value());

    patch.withMidPrice(42.42); // NOLINT: test value
    EXPECT_THAT(patch.getMidPrice(), Optional(DoubleEq(42.42)));
}

TEST(DataLayer_Model_PriceSeed, Patch_Set_BidPrice)
{
    PriceSeed::Patch patch{};
    EXPECT_FALSE(patch.getBidPrice().has_value());

    patch.withBidPrice(42.42); // NOLINT: test value
    EXPECT_THAT(patch.getBidPrice(), Optional(DoubleEq(42.42)));
}

TEST(DataLayer_Model_PriceSeed, Patch_Set_OfferPrice)
{
    PriceSeed::Patch patch{};
    EXPECT_FALSE(patch.getOfferPrice().has_value());

    patch.withOfferPrice(42.42); // NOLINT: test value
    EXPECT_THAT(patch.getOfferPrice(), Optional(DoubleEq(42.42)));
}

TEST(DataLayer_Model_PriceSeed, Patch_Set_LastUpdate)
{
    PriceSeed::Patch patch{};
    EXPECT_FALSE(patch.getLastUpdate().has_value());

    patch.withLastUpdate("2023-08-25 12:03:45");
    EXPECT_THAT(patch.getLastUpdate(), Optional(Eq("2023-08-25 12:03:45")));
}

TEST(DataLayer_Model_PriceSeed, Get_PriceSeedID)
{
    PriceSeed::Patch const patch{};

    PriceSeed seed = PriceSeed::create(patch, 42);
    EXPECT_EQ(seed.getPriceSeedId(), 42);
}

TEST(DataLayer_Model_PriceSeed, Get_Symbol_Missing)
{
    PriceSeed::Patch const patch{};

    PriceSeed seed = PriceSeed::create(patch, 42);
    EXPECT_EQ(seed.getSymbol(), std::nullopt);
}

TEST(DataLayer_Model_PriceSeed, Get_Symbol_Specified)
{
    PriceSeed::Patch patch{};
    patch.withSymbol("AAPL");

    PriceSeed seed = PriceSeed::create(patch, 42);
    EXPECT_THAT(seed.getSymbol(), Optional(Eq("AAPL")));
}

TEST(DataLayer_Model_PriceSeed, Get_SecurityType_Missing)
{
    PriceSeed::Patch const patch{};

    PriceSeed seed = PriceSeed::create(patch, 42);
    EXPECT_EQ(seed.getSecurityType(), std::nullopt);
}

TEST(DataLayer_Model_PriceSeed, Get_SecurityType_Specified)
{
    PriceSeed::Patch patch{};
    patch.withSecurityType("CS");

    PriceSeed seed = PriceSeed::create(patch, 42);
    EXPECT_THAT(seed.getSecurityType(), Optional(Eq("CS")));
}

TEST(DataLayer_Model_PriceSeed, Get_PriceCurrency_Missing)
{
    PriceSeed::Patch const patch{};

    PriceSeed seed = PriceSeed::create(patch, 42);
    EXPECT_EQ(seed.getPriceCurrency(), std::nullopt);
}

TEST(DataLayer_Model_PriceSeed, Get_PriceCurrency_Specified)
{
    PriceSeed::Patch patch{};
    patch.withPriceCurrency("USD");

    PriceSeed seed = PriceSeed::create(patch, 42);
    EXPECT_THAT(seed.getPriceCurrency(), Optional(Eq("USD")));
}

TEST(DataLayer_Model_PriceSeed, Get_SecurityID_Missing)
{
    PriceSeed::Patch const patch{};

    PriceSeed seed = PriceSeed::create(patch, 42);
    EXPECT_EQ(seed.getSecurityId(), std::nullopt);
}

TEST(DataLayer_Model_PriceSeed, Get_SecurityID_Specified)
{
    PriceSeed::Patch patch{};
    patch.withSecurityId("US0378331005");

    PriceSeed seed = PriceSeed::create(patch, 42);
    EXPECT_THAT(seed.getSecurityId(), Optional(Eq("US0378331005")));
}

TEST(DataLayer_Model_PriceSeed, Get_SecurityIDSource_Missing)
{
    PriceSeed::Patch const patch{};

    PriceSeed seed = PriceSeed::create(patch, 42);
    EXPECT_EQ(seed.getSecurityIdSource(), std::nullopt);
}

TEST(DataLayer_Model_PriceSeed, Get_SecurityIDSource_Specified)
{
    PriceSeed::Patch patch{};
    patch.withSecurityIdSource("ISIN");

    PriceSeed seed = PriceSeed::create(patch, 42);
    EXPECT_THAT(seed.getSecurityIdSource(), Optional(Eq("ISIN")));
}

TEST(DataLayer_Model_PriceSeed, Get_MidPrice_Missing)
{
    PriceSeed::Patch const patch{};

    PriceSeed seed = PriceSeed::create(patch, 42);
    EXPECT_EQ(seed.getMidPrice(), std::nullopt);
}

TEST(DataLayer_Model_PriceSeed, Get_MidPrice_Specified)
{
    PriceSeed::Patch patch{};
    patch.withMidPrice(42.42);

    PriceSeed seed = PriceSeed::create(patch, 42);
    EXPECT_THAT(seed.getMidPrice(), Optional(DoubleEq(42.42)));
}

TEST(DataLayer_Model_PriceSeed, Get_BidPrice_Missing)
{
    PriceSeed::Patch const patch{};

    PriceSeed seed = PriceSeed::create(patch, 42);
    EXPECT_EQ(seed.getBidPrice(), std::nullopt);
}

TEST(DataLayer_Model_PriceSeed, Get_BidPrice_Specified)
{
    PriceSeed::Patch patch{};
    patch.withBidPrice(42.42);

    PriceSeed seed = PriceSeed::create(patch, 42);
    EXPECT_THAT(seed.getBidPrice(), Optional(DoubleEq(42.42)));
}

TEST(DataLayer_Model_PriceSeed, Get_OfferPrice_Missing)
{
    PriceSeed::Patch const patch{};

    PriceSeed seed = PriceSeed::create(patch, 42);
    EXPECT_EQ(seed.getOfferPrice(), std::nullopt);
}

TEST(DataLayer_Model_PriceSeed, Get_OfferPrice_Specified)
{
    PriceSeed::Patch patch{};
    patch.withOfferPrice(42.42);

    PriceSeed seed = PriceSeed::create(patch, 42);
    EXPECT_THAT(seed.getOfferPrice(), Optional(DoubleEq(42.42)));
}

TEST(DataLayer_Model_PriceSeed, Get_LastUpdate_Missing)
{
    PriceSeed::Patch const patch{};

    PriceSeed seed = PriceSeed::create(patch, 42);
    EXPECT_EQ(seed.getLastUpdate(), std::nullopt);
}

TEST(DataLayer_Model_PriceSeed, Get_LastUpdate_Specified)
{
    PriceSeed::Patch patch{};
    patch.withLastUpdate("2023-08-25 12:03:45");

    PriceSeed seed = PriceSeed::create(patch, 42);
    EXPECT_THAT(seed.getLastUpdate(), Optional(Eq("2023-08-25 12:03:45")));
}