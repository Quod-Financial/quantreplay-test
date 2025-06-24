#include "api/models/listing.hpp"

#include "ih/common/exceptions.hpp"

namespace Simulator::DataLayer {

// NOLINTNEXTLINE: No way to split it, it's an atomic model creation operation
auto Listing::create(Listing::Patch _snapshot, std::uint64_t _id) -> Listing
{
    if (!_snapshot.mNewVenueId.has_value()) {
        throw RequiredAttributeMissing("Listing", "VenueID");
    }

    Listing listing{};
    listing.mListingId = _id;

#define SIM_ASSIGN_FIELD(LISTING_FIELD, SNAPSHOT_FIELD)                        \
    if (_snapshot.SNAPSHOT_FIELD.has_value()) {                                \
        listing.LISTING_FIELD = std::move(*(_snapshot.SNAPSHOT_FIELD));        \
    }                                                                          \
    void(0)

    SIM_ASSIGN_FIELD(mSymbol, mNewSymbol);
    SIM_ASSIGN_FIELD(mVenueId, mNewVenueId);
    SIM_ASSIGN_FIELD(mSecurityType, mNewSecurityType);
    SIM_ASSIGN_FIELD(mPriceCurrency, mNewPriceCurrency);
    SIM_ASSIGN_FIELD(mFxBaseCurrency, mNewFxBaseCurrency);
    SIM_ASSIGN_FIELD(mInstrSymbol, mNewInstrSymbol);
    SIM_ASSIGN_FIELD(mSecurityExchange, mNewSecurityExchange);
    SIM_ASSIGN_FIELD(mPartyId, mNewPartyId);
    SIM_ASSIGN_FIELD(mPartyRole, mNewPartyRole);
    SIM_ASSIGN_FIELD(mCusipId, mNewCusipId);
    SIM_ASSIGN_FIELD(mSedolId, mNewSedolId);
    SIM_ASSIGN_FIELD(mIsinId, mNewIsinId);
    SIM_ASSIGN_FIELD(mRicId, mNewRicId);
    SIM_ASSIGN_FIELD(mExchangeSymbolId, mNewExchangeSymbolId);
    SIM_ASSIGN_FIELD(mBloombergSymbolId, mNewBloombergSymbolId);

    SIM_ASSIGN_FIELD(mQtyMinimum, mNewQtyMinimum);
    SIM_ASSIGN_FIELD(mQtyMaximum, mNewQtyMaximum);
    SIM_ASSIGN_FIELD(mQtyMultiple, mNewQtyMultiple);
    SIM_ASSIGN_FIELD(mPriceTickSize, mNewPriceTickSize);
    SIM_ASSIGN_FIELD(mRandomQtyMaximum, mNewRandomQtyMaximum);
    SIM_ASSIGN_FIELD(mRandomOrdersSpread, mNewRandomOrdersSpread);
    SIM_ASSIGN_FIELD(mRandomQtyMinimum, mNewRandomQtyMinimum);
    SIM_ASSIGN_FIELD(mRandomAmtMinimum, mNewRandomAmtMinimum);
    SIM_ASSIGN_FIELD(mRandomAmtMaximum, mNewRandomAmtMaximum);
    SIM_ASSIGN_FIELD(mRandomAggressiveQtyMinimum,
                     mNewRandomAggressiveQtyMinimum);
    SIM_ASSIGN_FIELD(mRandomAggressiveQtyMaximum,
                     mNewRandomAggressiveQtyMaximum);
    SIM_ASSIGN_FIELD(mRandomAggressiveAmtMinimum,
                     mNewRandomAggressiveAmtMinimum);
    SIM_ASSIGN_FIELD(mRandomAggressiveAmtMaximum,
                     mNewRandomAggressiveAmtMaximum);

    SIM_ASSIGN_FIELD(mRandomDepthLevels, mNewRandomDepthLevels);
    SIM_ASSIGN_FIELD(mRandomOrdersRate, mNewRandomOrdersRate);
    SIM_ASSIGN_FIELD(mRandomTickRange, mNewRandomTickRange);

    SIM_ASSIGN_FIELD(mEnabledFlag, mNewEnabledFlag);
    SIM_ASSIGN_FIELD(mRandomOrdersEnabledFlag, mNewRandomOrdersEnabledFlag);

#undef SIM_ASSIGN_FIELD
    return listing;
}

auto Listing::getListingId() const noexcept -> std::uint64_t
{
    return mListingId;
}

auto Listing::getSymbol() const noexcept -> std::optional<std::string> const&
{
    return mSymbol;
}

auto Listing::getVenueId() const noexcept -> std::string const&
{
    return mVenueId;
}

auto Listing::getSecurityType() const noexcept
    -> std::optional<std::string> const&
{
    return mSecurityType;
}

auto Listing::getPriceCurrency() const noexcept
    -> std::optional<std::string> const&
{
    return mPriceCurrency;
}

auto Listing::getFxBaseCurrency() const noexcept
    -> std::optional<std::string> const&
{
    return mFxBaseCurrency;
}

auto Listing::getInstrSymbol() const noexcept
    -> std::optional<std::string> const&
{
    return mInstrSymbol;
}

auto Listing::getQtyMinimum() const noexcept -> std::optional<double>
{
    return mQtyMinimum;
}

auto Listing::getQtyMaximum() const noexcept -> std::optional<double>
{
    return mQtyMaximum;
}

auto Listing::getQtyMultiple() const noexcept -> std::optional<double>
{
    return mQtyMultiple;
}

auto Listing::getPriceTickSize() const noexcept -> std::optional<double>
{
    return mPriceTickSize;
}

auto Listing::getEnabledFlag() const noexcept -> std::optional<bool>
{
    return mEnabledFlag;
}

auto Listing::getRandomQtyMaximum() const noexcept -> std::optional<double>
{
    return mRandomQtyMaximum;
}

auto Listing::getRandomDepthLevels() const noexcept
    -> std::optional<std::uint32_t>
{
    return mRandomDepthLevels;
}

auto Listing::getRandomOrdersSpread() const noexcept -> std::optional<double>
{
    return mRandomOrdersSpread;
}

auto Listing::getRandomOrdersRate() const noexcept
    -> std::optional<std::uint32_t>
{
    return mRandomOrdersRate;
}

auto Listing::getRandomTickRange() const noexcept
    -> std::optional<std::uint32_t>
{
    return mRandomTickRange;
}

auto Listing::getSecurityExchange() const noexcept
    -> std::optional<std::string> const&
{
    return mSecurityExchange;
}

auto Listing::getPartyId() const noexcept -> std::optional<std::string> const&
{
    return mPartyId;
}

auto Listing::getPartyRole() const noexcept -> std::optional<std::string> const&
{
    return mPartyRole;
}

auto Listing::getCusipId() const noexcept -> std::optional<std::string> const&
{
    return mCusipId;
}

auto Listing::getSedolId() const noexcept -> std::optional<std::string> const&
{
    return mSedolId;
}

auto Listing::getIsinId() const noexcept -> std::optional<std::string> const&
{
    return mIsinId;
}

auto Listing::getRicId() const noexcept -> std::optional<std::string> const&
{
    return mRicId;
}

auto Listing::getExchangeSymbolId() const noexcept
    -> std::optional<std::string> const&
{
    return mExchangeSymbolId;
}

auto Listing::getBloombergSymbolId() const noexcept
    -> std::optional<std::string> const&
{
    return mBloombergSymbolId;
}

auto Listing::getRandomQtyMinimum() const noexcept -> std::optional<double>
{
    return mRandomQtyMinimum;
}

auto Listing::getRandomAmtMinimum() const noexcept -> std::optional<double>
{
    return mRandomAmtMinimum;
}

auto Listing::getRandomAmtMaximum() const noexcept -> std::optional<double>
{
    return mRandomAmtMaximum;
}

auto Listing::getRandomOrdersEnabledFlag() const noexcept -> std::optional<bool>
{
    return mRandomOrdersEnabledFlag;
}

auto Listing::getRandomAggressiveQtyMinimum() const noexcept
    -> std::optional<double>
{
    return mRandomAggressiveQtyMinimum;
}

auto Listing::getRandomAggressiveQtyMaximum() const noexcept
    -> std::optional<double>
{
    return mRandomAggressiveQtyMaximum;
}

auto Listing::getRandomAggressiveAmtMinimum() const noexcept
    -> std::optional<double>
{
    return mRandomAggressiveAmtMinimum;
}

auto Listing::getRandomAggressiveAmtMaximum() const noexcept
    -> std::optional<double>
{
    return mRandomAggressiveAmtMaximum;
}

auto Listing::Patch::getSymbol() const noexcept
    -> std::optional<std::string> const&
{
    return mNewSymbol;
}

auto Listing::Patch::withSymbol(std::string _symbol) noexcept -> Patch&
{
    mNewSymbol = std::move(_symbol);
    return *this;
}

auto Listing::Patch::getVenueId() const noexcept
    -> std::optional<std::string> const&
{
    return mNewVenueId;
}

auto Listing::Patch::withVenueId(std::string _id) noexcept -> Patch&
{
    mNewVenueId = std::move(_id);
    return *this;
}

auto Listing::Patch::getSecurityType() const noexcept
    -> std::optional<std::string> const&
{
    return mNewSecurityType;
}

auto Listing::Patch::withSecurityType(std::string _type) noexcept -> Patch&
{
    mNewSecurityType = std::move(_type);
    return *this;
}

auto Listing::Patch::getPriceCurrency() const noexcept
    -> std::optional<std::string> const&
{
    return mNewPriceCurrency;
}

auto Listing::Patch::withPriceCurrency(std::string _currency) noexcept -> Patch&
{
    mNewPriceCurrency = std::move(_currency);
    return *this;
}

auto Listing::Patch::getFxBaseCurrency() const noexcept
    -> std::optional<std::string> const&
{
    return mNewFxBaseCurrency;
}

auto Listing::Patch::withFxBaseCurrency(std::string _currency) noexcept
    -> Patch&
{
    mNewFxBaseCurrency = std::move(_currency);
    return *this;
}

auto Listing::Patch::getInstrSymbol() const noexcept
    -> std::optional<std::string> const&
{
    return mNewInstrSymbol;
}

auto Listing::Patch::withInstrSymbol(std::string _symbol) noexcept -> Patch&
{
    mNewInstrSymbol = std::move(_symbol);
    return *this;
}

auto Listing::Patch::getQtyMinimum() const noexcept -> std::optional<double>
{
    return mNewQtyMinimum;
}

auto Listing::Patch::withQtyMinimum(double _qty) noexcept -> Patch&
{
    mNewQtyMinimum = _qty;
    return *this;
}

auto Listing::Patch::getQtyMaximum() const noexcept -> std::optional<double>
{
    return mNewQtyMaximum;
}

auto Listing::Patch::withQtyMaximum(double _qty) noexcept -> Patch&
{
    mNewQtyMaximum = _qty;
    return *this;
}

auto Listing::Patch::getQtyMultiple() const noexcept -> std::optional<double>
{
    return mNewQtyMultiple;
}

auto Listing::Patch::withQtyMultiple(double _multiple) noexcept -> Patch&
{
    mNewQtyMultiple = _multiple;
    return *this;
}

auto Listing::Patch::getPriceTickSize() const noexcept -> std::optional<double>
{
    return mNewPriceTickSize;
}

auto Listing::Patch::withPriceTickSize(double _size) noexcept -> Patch&
{
    mNewPriceTickSize = _size;
    return *this;
}

auto Listing::Patch::getEnabledFlag() const noexcept -> std::optional<bool>
{
    return mNewEnabledFlag;
}

auto Listing::Patch::withEnabledFlag(bool _flag) noexcept -> Patch&
{
    mNewEnabledFlag = _flag;
    return *this;
}

auto Listing::Patch::getRandomQtyMaximum() const noexcept
    -> std::optional<double>
{
    return mNewRandomQtyMaximum;
}

auto Listing::Patch::withRandomQtyMaximum(double _qty) noexcept -> Patch&
{
    mNewRandomQtyMaximum = _qty;
    return *this;
}

auto Listing::Patch::getRandomDepthLevels() const noexcept
    -> std::optional<std::uint32_t>
{
    return mNewRandomDepthLevels;
}

auto Listing::Patch::withRandomDepthLevels(std::uint32_t _numLevels) noexcept
    -> Patch&
{
    mNewRandomDepthLevels = _numLevels;
    return *this;
}

auto Listing::Patch::getRandomOrdersSpread() const noexcept
    -> std::optional<double>
{
    return mNewRandomOrdersSpread;
}

auto Listing::Patch::withRandomOrdersSpread(double _spread) noexcept -> Patch&
{
    mNewRandomOrdersSpread = _spread;
    return *this;
}

auto Listing::Patch::getRandomOrdersRate() const noexcept
    -> std::optional<std::uint32_t>
{
    return mNewRandomOrdersRate;
}

auto Listing::Patch::withRandomOrdersRate(std::uint32_t _rate) noexcept
    -> Patch&
{
    mNewRandomOrdersRate = _rate;
    return *this;
}

auto Listing::Patch::getRandomTickRange() const noexcept
    -> std::optional<std::uint32_t>
{
    return mNewRandomTickRange;
}

auto Listing::Patch::withRandomTickRange(std::uint32_t _range) noexcept
    -> Patch&
{
    mNewRandomTickRange = _range;
    return *this;
}

auto Listing::Patch::getSecurityExchange() const noexcept
    -> std::optional<std::string> const&
{
    return mNewSecurityExchange;
}

auto Listing::Patch::withSecurityExchange(std::string _exchange) noexcept
    -> Patch&
{
    mNewSecurityExchange = std::move(_exchange);
    return *this;
}

auto Listing::Patch::getPartyId() const noexcept
    -> std::optional<std::string> const&
{
    return mNewPartyId;
}

auto Listing::Patch::withPartyId(std::string _id) noexcept -> Patch&
{
    mNewPartyId = std::move(_id);
    return *this;
}

auto Listing::Patch::getPartyRole() const noexcept
    -> std::optional<std::string> const&
{
    return mNewPartyRole;
}

auto Listing::Patch::withPartyRole(std::string _role) noexcept -> Patch&
{
    mNewPartyRole = std::move(_role);
    return *this;
}

auto Listing::Patch::getCusipId() const noexcept
    -> std::optional<std::string> const&
{
    return mNewCusipId;
}

auto Listing::Patch::withCusipId(std::string _id) noexcept -> Patch&
{
    mNewCusipId = std::move(_id);
    return *this;
}

auto Listing::Patch::getSedolId() const noexcept
    -> std::optional<std::string> const&
{
    return mNewSedolId;
}

auto Listing::Patch::withSedolId(std::string _id) noexcept -> Patch&
{
    mNewSedolId = std::move(_id);
    return *this;
}

auto Listing::Patch::getIsinId() const noexcept
    -> std::optional<std::string> const&
{
    return mNewIsinId;
}

auto Listing::Patch::withIsinId(std::string _id) noexcept -> Patch&
{
    mNewIsinId = std::move(_id);
    return *this;
}

auto Listing::Patch::getRicId() const noexcept
    -> std::optional<std::string> const&
{
    return mNewRicId;
}

auto Listing::Patch::withRicId(std::string _id) noexcept -> Patch&
{
    mNewRicId = std::move(_id);
    return *this;
}

auto Listing::Patch::getExchangeSymbolId() const noexcept
    -> std::optional<std::string> const&
{
    return mNewExchangeSymbolId;
}

auto Listing::Patch::withExchangeSymbolId(std::string _id) noexcept -> Patch&
{
    mNewExchangeSymbolId = std::move(_id);
    return *this;
}

auto Listing::Patch::getBloombergSymbolId() const noexcept
    -> std::optional<std::string> const&
{
    return mNewBloombergSymbolId;
}

auto Listing::Patch::withBloombergSymbolId(std::string _id) noexcept -> Patch&
{
    mNewBloombergSymbolId = std::move(_id);
    return *this;
}

auto Listing::Patch::getRandomQtyMinimum() const noexcept
    -> std::optional<double>
{
    return mNewRandomQtyMinimum;
}

auto Listing::Patch::withRandomQtyMinimum(double _qty) noexcept -> Patch&
{
    mNewRandomQtyMinimum = _qty;
    return *this;
}

auto Listing::Patch::getRandomAmtMinimum() const noexcept
    -> std::optional<double>
{
    return mNewRandomAmtMinimum;
}

auto Listing::Patch::withRandomAmtMinimum(double _amount) noexcept -> Patch&
{
    mNewRandomAmtMinimum = _amount;
    return *this;
}

auto Listing::Patch::getRandomAmtMaximum() const noexcept
    -> std::optional<double>
{
    return mNewRandomAmtMaximum;
}

auto Listing::Patch::withRandomAmtMaximum(double _amount) noexcept -> Patch&
{
    mNewRandomAmtMaximum = _amount;
    return *this;
}

auto Listing::Patch::getRandomOrdersEnabledFlag() const noexcept
    -> std::optional<bool>
{
    return mNewRandomOrdersEnabledFlag;
}

auto Listing::Patch::withRandomOrdersEnabledFlag(bool _flag) noexcept -> Patch&
{
    mNewRandomOrdersEnabledFlag = _flag;
    return *this;
}

auto Listing::Patch::getRandomAggressiveQtyMinimum() const noexcept
    -> std::optional<double> {
  return mNewRandomAggressiveQtyMinimum;
}

auto Listing::Patch::withRandomAggressiveQtyMinimum(double qty) noexcept
    -> Patch& {
  mNewRandomAggressiveQtyMinimum = qty;
  return *this;
}

auto Listing::Patch::getRandomAggressiveQtyMaximum() const noexcept
    -> std::optional<double> {
  return mNewRandomAggressiveQtyMaximum;
}

auto Listing::Patch::withRandomAggressiveQtyMaximum(double qty) noexcept
    -> Patch& {
  mNewRandomAggressiveQtyMaximum = qty;
  return *this;
}

auto Listing::Patch::getRandomAggressiveAmtMinimum() const noexcept
    -> std::optional<double> {
  return mNewRandomAggressiveAmtMinimum;
}

auto Listing::Patch::withRandomAggressiveAmtMinimum(double amount) noexcept
    -> Patch& {
  mNewRandomAggressiveAmtMinimum = amount;
  return *this;
}

auto Listing::Patch::getRandomAggressiveAmtMaximum() const noexcept
    -> std::optional<double> {
  return mNewRandomAggressiveAmtMaximum;
}

auto Listing::Patch::withRandomAggressiveAmtMaximum(double amount) noexcept
    -> Patch& {
  mNewRandomAggressiveAmtMaximum = amount;
  return *this;
}

} // namespace Simulator::DataLayer
