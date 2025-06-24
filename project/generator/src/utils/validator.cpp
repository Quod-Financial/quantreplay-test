#include "ih/utils/validator.hpp"

#include <fmt/format.h>

#include "log/logging.hpp"

#include "ih/constants.hpp"

namespace Simulator::Generator {

bool Validator::isAcceptable(DataLayer::Listing const & _listing)
{
    auto const listingID = _listing.getListingId();

    auto const & symbol = _listing.getSymbol();
    if (!symbol || symbol->empty())
    {
        simulator::log::info(
          "listing (id: {}) can not be accepted for random orders "
          "generation: listing does not have a symbol specified",
          listingID);
        return false;
    }

    if (!_listing.getEnabledFlag().value_or(Constant::DefaultListingEnabledFlag))
    {
        simulator::log::info(
          "listing '{}' (id: {}) can not be accepted for random orders "
          "generation: listing is disabled",
          symbol,
          listingID);
        return false;
    }

    return true;
}

bool Validator::isAcceptableForRandomGeneration(
    DataLayer::Listing const& _listing)
{
    if (!isAcceptable(_listing))
    {
        return false;
    }

    auto const listingID = _listing.getListingId();
    auto const & symbol = _listing.getSymbol();

    if (!_listing.getRandomOrdersEnabledFlag().value_or(
            Constant::DefaultListingRandomOrdersEnabled))
    {
        simulator::log::info(
          "listing '{}' (id: {}) can not be accepted for random orders "
          "generation: random orders generation is disabled for the listing",
          symbol,
          listingID);
        return false;
    }

    if (_listing.getRandomOrdersRate().value_or(0) <= 0)
    {
        simulator::log::info(
          "listing '{}' (id: {}) can not be accepted for random orders "
          "generation: random orders rate value has to be greater "
          "than zero (0)",
          symbol,
          listingID,
          _listing.getRandomOrdersRate());
        return false;
    }

    simulator::log::debug(
      "listing '{}' (id: {}) was validated and can be accepted for "
      "random orders generation",
      symbol,
      listingID);

    return true;
}


bool Validator::isAcceptable(DataLayer::PriceSeed const & _priceSeed)
{
    auto const & symbol = _priceSeed.getSymbol();

    bool const hasBuyPx = _priceSeed.getBidPrice().has_value();
    bool const hasSellPx = _priceSeed.getOfferPrice().has_value();
    bool const hasMidPx = _priceSeed.getMidPrice().has_value();

    bool const isValid = (hasBuyPx || hasMidPx) && (hasSellPx || hasMidPx);

    if (isValid)
    {
        simulator::log::debug(
          "price seed for '{}' (id: {}) was validated "
          "and can be accepted for random orders generation",
          symbol,
          _priceSeed.getPriceSeedId());

        return true;
    }

    if (!hasBuyPx && !hasMidPx)
    {
        simulator::log::warn(
          "price seed '{}' (id: {}) can not be used: "
          "initial price for buy side is not present. Check that mid price "
          "or bid price are specified for this price seed entry",
          _priceSeed.getSymbol(),
          _priceSeed.getPriceSeedId());
    }
    if (!hasSellPx && !hasMidPx)
    {
        simulator::log::warn(
          "price seed '{}' (id: {}) can not be used: "
          "initial price for sell side is not present. Check that mid price "
          "or offer price are specified for this price seed entry",
          _priceSeed.getSymbol(),
          _priceSeed.getPriceSeedId());
    }

    return false;
}

} // namespace Simulator::Generator
