#include "api/models/price_seed.hpp"

#include <utility>

namespace Simulator::DataLayer {

auto PriceSeed::create(PriceSeed::Patch _snapshot, std::uint64_t _id) noexcept
    -> PriceSeed
{
    PriceSeed seed{};
    seed.mPriceSeedId = _id;

#define SIM_ASSIGN_FIELD(SEED_FIELD, SNAPSHOT_FIELD)                           \
    if (_snapshot.SNAPSHOT_FIELD.has_value()) {                                \
        seed.SEED_FIELD = std::move(*(_snapshot.SNAPSHOT_FIELD));              \
    }                                                                          \
    void(0)

    SIM_ASSIGN_FIELD(mSymbol, mNewSymbol);
    SIM_ASSIGN_FIELD(mSecurityType, mNewSecurityType);
    SIM_ASSIGN_FIELD(mPriceCurrency, mNewPriceCurrency);
    SIM_ASSIGN_FIELD(mSecurityId, mNewSecurityId);
    SIM_ASSIGN_FIELD(mSecurityIdSource, mNewSecurityIdSource);
    SIM_ASSIGN_FIELD(mInstrumentSymbol, mNewInstrumentSymbol);
    SIM_ASSIGN_FIELD(mLastUpdate, mNewLastUpdate);
    SIM_ASSIGN_FIELD(mMidPrice, mNewMidPrice);
    SIM_ASSIGN_FIELD(mBidPrice, mNewBidPrice);
    SIM_ASSIGN_FIELD(mOfferPrice, mNewOfferPrice);

#undef SIM_ASSIGN_FIELD

    return seed;
}

auto PriceSeed::getPriceSeedId() const noexcept -> std::uint64_t
{
    return mPriceSeedId;
}

auto PriceSeed::getSymbol() const noexcept -> std::optional<std::string> const&
{
    return mSymbol;
}

auto PriceSeed::getSecurityType() const noexcept
    -> std::optional<std::string> const&
{
    return mSecurityType;
}

auto PriceSeed::getPriceCurrency() const noexcept
    -> std::optional<std::string> const&
{
    return mPriceCurrency;
}

auto PriceSeed::getSecurityId() const noexcept
    -> std::optional<std::string> const&
{
    return mSecurityId;
}

auto PriceSeed::getSecurityIdSource() const noexcept
    -> std::optional<std::string> const&
{
    return mSecurityIdSource;
}

auto PriceSeed::getInstrumentSymbol() const noexcept
    -> std::optional<std::string> const&
{
    return mInstrumentSymbol;
}

auto PriceSeed::getMidPrice() const noexcept -> std::optional<double>
{
    return mMidPrice;
}

auto PriceSeed::getBidPrice() const noexcept -> std::optional<double>
{
    return mBidPrice;
}

auto PriceSeed::getOfferPrice() const noexcept -> std::optional<double>
{
    return mOfferPrice;
}

auto PriceSeed::getLastUpdate() const noexcept
    -> std::optional<std::string> const&
{
    return mLastUpdate;
}

auto PriceSeed::Patch::getSymbol() const noexcept
    -> std::optional<std::string> const&
{
    return mNewSymbol;
}

auto PriceSeed::Patch::withSymbol(std::string _symbol) noexcept -> Patch&
{
    mNewSymbol = std::move(_symbol);
    return *this;
}

auto PriceSeed::Patch::getSecurityType() const noexcept
    -> std::optional<std::string> const&
{
    return mNewSecurityType;
}

auto PriceSeed::Patch::withSecurityType(std::string _type) noexcept -> Patch&
{
    mNewSecurityType = std::move(_type);
    return *this;
}

auto PriceSeed::Patch::getPriceCurrency() const noexcept
    -> std::optional<std::string> const&
{
    return mNewPriceCurrency;
}

auto PriceSeed::Patch::withPriceCurrency(std::string _currency) noexcept
    -> Patch&
{
    mNewPriceCurrency = std::move(_currency);
    return *this;
}

auto PriceSeed::Patch::getSecurityId() const noexcept
    -> std::optional<std::string> const&
{
    return mNewSecurityId;
}

auto PriceSeed::Patch::withSecurityId(std::string _id) noexcept -> Patch&
{
    mNewSecurityId = std::move(_id);
    return *this;
}

auto PriceSeed::Patch::getSecurityIdSource() const noexcept
    -> std::optional<std::string> const&
{
    return mNewSecurityIdSource;
}

auto PriceSeed::Patch::withSecurityIdSource(std::string _source) noexcept
    -> Patch&
{
    mNewSecurityIdSource = std::move(_source);
    return *this;
}

auto PriceSeed::Patch::getInstrumentSymbol() const noexcept
    -> std::optional<std::string> const&
{
    return mNewInstrumentSymbol;
}

auto PriceSeed::Patch::withInstrumentSymbol(std::string _symbol) noexcept
    -> Patch&
{
    mNewInstrumentSymbol = std::move(_symbol);
    return *this;
}

auto PriceSeed::Patch::getMidPrice() const noexcept -> std::optional<double>
{
    return mNewMidPrice;
}

auto PriceSeed::Patch::withMidPrice(double _price) noexcept -> Patch&
{
    mNewMidPrice = _price;
    return *this;
}

auto PriceSeed::Patch::getBidPrice() const noexcept -> std::optional<double>
{
    return mNewBidPrice;
}

auto PriceSeed::Patch::withBidPrice(double _price) noexcept -> Patch&
{
    mNewBidPrice = _price;
    return *this;
}

auto PriceSeed::Patch::getOfferPrice() const noexcept -> std::optional<double>
{
    return mNewOfferPrice;
}

auto PriceSeed::Patch::withOfferPrice(double _price) noexcept -> Patch&
{
    mNewOfferPrice = _price;
    return *this;
}

auto PriceSeed::Patch::getLastUpdate() const noexcept
    -> std::optional<std::string> const&
{
    return mNewLastUpdate;
}

auto PriceSeed::Patch::withLastUpdate(std::string _updateTimestamp) noexcept
    -> Patch&
{
    mNewLastUpdate = std::move(_updateTimestamp);
    return *this;
}

} // namespace Simulator::DataLayer
