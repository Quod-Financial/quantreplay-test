#ifndef SIMULATOR_DATA_LAYER_INCLUDE_DATA_LAYER_API_MODELS_LISTING_HPP_
#define SIMULATOR_DATA_LAYER_INCLUDE_DATA_LAYER_API_MODELS_LISTING_HPP_

#include <cstdint>
#include <optional>
#include <string>

#include "data_layer/api/predicate/definitions.hpp"

namespace Simulator::DataLayer {

class Listing {
  public:
    class Patch;

    using Predicate = Predicate::Expression<Listing>;

    enum class Attribute {
        ListingId,
        Symbol,
        VenueId,
        SecurityType,
        PriceCurrency,
        FxBaseCurrency,
        InstrSymbol,
        QtyMinimum,
        QtyMaximum,
        QtyMultiple,
        PriceTickSize,
        Enabled,
        RandomQtyMaximum,
        RandomDepthLevels,
        RandomOrdersSpread,
        RandomOrdersRate,
        RandomTickRange,
        SecurityExchange,
        PartyId,
        PartyRole,
        CusipId,
        SedolId,
        IsinId,
        RicId,
        ExchangeSymbolId,
        BloombergSymbolId,
        RandomQtyMinimum,
        RandomAmtMinimum,
        RandomAmtMaximum,
        RandomOrdersEnabled,
        RandomAggressiveQtyMinimum,
        RandomAggressiveQtyMaximum,
        RandomAggressiveAmtMinimum,
        RandomAggressiveAmtMaximum
    };

    [[nodiscard]]
    static auto create(Listing::Patch _snapshot, std::uint64_t _id) -> Listing;

    [[nodiscard]]
    auto getListingId() const noexcept -> std::uint64_t;

    [[nodiscard]]
    auto getSymbol() const noexcept -> std::optional<std::string> const&;

    [[nodiscard]]
    auto getVenueId() const noexcept -> std::string const&;

    [[nodiscard]]
    auto getSecurityType() const noexcept -> std::optional<std::string> const&;

    [[nodiscard]]
    auto getPriceCurrency() const noexcept -> std::optional<std::string> const&;

    [[nodiscard]]
    auto getFxBaseCurrency() const noexcept
        -> std::optional<std::string> const&;

    [[nodiscard]]
    auto getInstrSymbol() const noexcept -> std::optional<std::string> const&;

    [[nodiscard]]
    auto getQtyMinimum() const noexcept -> std::optional<double>;

    [[nodiscard]]
    auto getQtyMaximum() const noexcept -> std::optional<double>;

    [[nodiscard]]
    auto getQtyMultiple() const noexcept -> std::optional<double>;

    [[nodiscard]]
    auto getPriceTickSize() const noexcept -> std::optional<double>;

    [[nodiscard]]
    auto getEnabledFlag() const noexcept -> std::optional<bool>;

    [[nodiscard]]
    auto getRandomQtyMaximum() const noexcept -> std::optional<double>;

    [[nodiscard]]
    auto getRandomDepthLevels() const noexcept -> std::optional<std::uint32_t>;

    [[nodiscard]]
    auto getRandomOrdersSpread() const noexcept -> std::optional<double>;

    [[nodiscard]]
    auto getRandomOrdersRate() const noexcept -> std::optional<std::uint32_t>;

    [[nodiscard]]
    auto getRandomTickRange() const noexcept -> std::optional<std::uint32_t>;

    [[nodiscard]]
    auto getSecurityExchange() const noexcept
        -> std::optional<std::string> const&;

    [[nodiscard]]
    auto getPartyId() const noexcept -> std::optional<std::string> const&;

    [[nodiscard]]
    auto getPartyRole() const noexcept -> std::optional<std::string> const&;

    [[nodiscard]]
    auto getCusipId() const noexcept -> std::optional<std::string> const&;

    [[nodiscard]]
    auto getSedolId() const noexcept -> std::optional<std::string> const&;

    [[nodiscard]]
    auto getIsinId() const noexcept -> std::optional<std::string> const&;

    [[nodiscard]]
    auto getRicId() const noexcept -> std::optional<std::string> const&;

    [[nodiscard]]
    auto getExchangeSymbolId() const noexcept
        -> std::optional<std::string> const&;

    [[nodiscard]]
    auto getBloombergSymbolId() const noexcept
        -> std::optional<std::string> const&;

    [[nodiscard]]
    auto getRandomQtyMinimum() const noexcept -> std::optional<double>;

    [[nodiscard]]
    auto getRandomAmtMinimum() const noexcept -> std::optional<double>;

    [[nodiscard]]
    auto getRandomAmtMaximum() const noexcept -> std::optional<double>;

    [[nodiscard]]
    auto getRandomOrdersEnabledFlag() const noexcept -> std::optional<bool>;

    [[nodiscard]]
    auto getRandomAggressiveQtyMinimum() const noexcept
        -> std::optional<double>;

    [[nodiscard]]
    auto getRandomAggressiveQtyMaximum() const noexcept
        -> std::optional<double>;

    [[nodiscard]]
    auto getRandomAggressiveAmtMinimum() const noexcept
        -> std::optional<double>;

    [[nodiscard]]
    auto getRandomAggressiveAmtMaximum() const noexcept
        -> std::optional<double>;

  private:
    Listing() = default;

    std::optional<std::string> mSymbol;
    std::optional<std::string> mSecurityType;
    std::optional<std::string> mPriceCurrency;
    std::optional<std::string> mFxBaseCurrency;
    std::optional<std::string> mInstrSymbol;
    std::optional<std::string> mSecurityExchange;
    std::optional<std::string> mPartyId;
    std::optional<std::string> mPartyRole;
    std::optional<std::string> mCusipId;
    std::optional<std::string> mSedolId;
    std::optional<std::string> mIsinId;
    std::optional<std::string> mRicId;
    std::optional<std::string> mExchangeSymbolId;
    std::optional<std::string> mBloombergSymbolId;

    std::string mVenueId;

    std::optional<double> mQtyMinimum;
    std::optional<double> mQtyMaximum;
    std::optional<double> mQtyMultiple;
    std::optional<double> mPriceTickSize;
    std::optional<double> mRandomQtyMaximum;
    std::optional<double> mRandomOrdersSpread;
    std::optional<double> mRandomQtyMinimum;
    std::optional<double> mRandomAmtMinimum;
    std::optional<double> mRandomAmtMaximum;
    std::optional<double> mRandomAggressiveQtyMinimum;
    std::optional<double> mRandomAggressiveQtyMaximum;
    std::optional<double> mRandomAggressiveAmtMinimum;
    std::optional<double> mRandomAggressiveAmtMaximum;

    std::optional<std::uint32_t> mRandomDepthLevels;
    std::optional<std::uint32_t> mRandomOrdersRate;
    std::optional<std::uint32_t> mRandomTickRange;

    std::uint64_t mListingId{0};

    std::optional<bool> mEnabledFlag;
    std::optional<bool> mRandomOrdersEnabledFlag;
};

class Listing::Patch {
    friend class Listing;

  public:
    using Attribute = Listing::Attribute;

    [[nodiscard]]
    auto getSymbol() const noexcept -> std::optional<std::string> const&;
    auto withSymbol(std::string _symbol) noexcept -> Patch&;

    [[nodiscard]]
    auto getVenueId() const noexcept -> std::optional<std::string> const&;
    auto withVenueId(std::string _id) noexcept -> Patch&;

    [[nodiscard]]
    auto getSecurityType() const noexcept -> std::optional<std::string> const&;
    auto withSecurityType(std::string _type) noexcept -> Patch&;

    [[nodiscard]]
    auto getPriceCurrency() const noexcept -> std::optional<std::string> const&;
    auto withPriceCurrency(std::string _currency) noexcept -> Patch&;

    [[nodiscard]]
    auto getFxBaseCurrency() const noexcept
        -> std::optional<std::string> const&;
    auto withFxBaseCurrency(std::string _currency) noexcept -> Patch&;

    [[nodiscard]]
    auto getInstrSymbol() const noexcept -> std::optional<std::string> const&;
    auto withInstrSymbol(std::string _symbol) noexcept -> Patch&;

    [[nodiscard]]
    auto getQtyMinimum() const noexcept -> std::optional<double>;
    auto withQtyMinimum(double _qty) noexcept -> Patch&;

    [[nodiscard]]
    auto getQtyMaximum() const noexcept -> std::optional<double>;
    auto withQtyMaximum(double _qty) noexcept -> Patch&;

    [[nodiscard]]
    auto getQtyMultiple() const noexcept -> std::optional<double>;
    auto withQtyMultiple(double _multiple) noexcept -> Patch&;

    [[nodiscard]]
    auto getPriceTickSize() const noexcept -> std::optional<double>;
    auto withPriceTickSize(double _size) noexcept -> Patch&;

    [[nodiscard]]
    auto getEnabledFlag() const noexcept -> std::optional<bool>;
    auto withEnabledFlag(bool _flag) noexcept -> Patch&;

    [[nodiscard]]
    auto getRandomQtyMaximum() const noexcept -> std::optional<double>;
    auto withRandomQtyMaximum(double _qty) noexcept -> Patch&;

    [[nodiscard]]
    auto getRandomDepthLevels() const noexcept -> std::optional<std::uint32_t>;
    auto withRandomDepthLevels(std::uint32_t _numLevels) noexcept -> Patch&;

    [[nodiscard]]
    auto getRandomOrdersSpread() const noexcept -> std::optional<double>;
    auto withRandomOrdersSpread(double _spread) noexcept -> Patch&;

    [[nodiscard]]
    auto getRandomOrdersRate() const noexcept -> std::optional<std::uint32_t>;
    auto withRandomOrdersRate(std::uint32_t _rate) noexcept -> Patch&;

    [[nodiscard]]
    auto getRandomTickRange() const noexcept -> std::optional<std::uint32_t>;
    auto withRandomTickRange(std::uint32_t _range) noexcept -> Patch&;

    [[nodiscard]]
    auto getSecurityExchange() const noexcept
        -> std::optional<std::string> const&;
    auto withSecurityExchange(std::string _exchange) noexcept -> Patch&;

    [[nodiscard]]
    auto getPartyId() const noexcept -> std::optional<std::string> const&;
    auto withPartyId(std::string _id) noexcept -> Patch&;

    [[nodiscard]]
    auto getPartyRole() const noexcept -> std::optional<std::string> const&;
    auto withPartyRole(std::string _role) noexcept -> Patch&;

    [[nodiscard]]
    auto getCusipId() const noexcept -> std::optional<std::string> const&;
    auto withCusipId(std::string _id) noexcept -> Patch&;

    [[nodiscard]]
    auto getSedolId() const noexcept -> std::optional<std::string> const&;
    auto withSedolId(std::string _id) noexcept -> Patch&;

    [[nodiscard]]
    auto getIsinId() const noexcept -> std::optional<std::string> const&;
    auto withIsinId(std::string _id) noexcept -> Patch&;

    [[nodiscard]]
    auto getRicId() const noexcept -> std::optional<std::string> const&;
    auto withRicId(std::string _id) noexcept -> Patch&;

    [[nodiscard]]
    auto getExchangeSymbolId() const noexcept
        -> std::optional<std::string> const&;
    auto withExchangeSymbolId(std::string _id) noexcept -> Patch&;

    [[nodiscard]]
    auto getBloombergSymbolId() const noexcept
        -> std::optional<std::string> const&;
    auto withBloombergSymbolId(std::string _id) noexcept -> Patch&;

    [[nodiscard]]
    auto getRandomQtyMinimum() const noexcept -> std::optional<double>;
    auto withRandomQtyMinimum(double _qty) noexcept -> Patch&;

    [[nodiscard]]
    auto getRandomAmtMinimum() const noexcept -> std::optional<double>;
    auto withRandomAmtMinimum(double _amount) noexcept -> Patch&;

    [[nodiscard]]
    auto getRandomAmtMaximum() const noexcept -> std::optional<double>;
    auto withRandomAmtMaximum(double _amount) noexcept -> Patch&;

    [[nodiscard]]
    auto getRandomOrdersEnabledFlag() const noexcept -> std::optional<bool>;
    auto withRandomOrdersEnabledFlag(bool _flag) noexcept -> Patch&;

    [[nodiscard]]
    auto getRandomAggressiveQtyMinimum() const noexcept -> std::optional<double>;
    auto withRandomAggressiveQtyMinimum(double qty) noexcept -> Patch&;

    [[nodiscard]]
    auto getRandomAggressiveQtyMaximum() const noexcept -> std::optional<double>;
    auto withRandomAggressiveQtyMaximum(double qty) noexcept -> Patch&;

    [[nodiscard]]
    auto getRandomAggressiveAmtMinimum() const noexcept -> std::optional<double>;
    auto withRandomAggressiveAmtMinimum(double amount) noexcept -> Patch&;

    [[nodiscard]]
    auto getRandomAggressiveAmtMaximum() const noexcept -> std::optional<double>;
    auto withRandomAggressiveAmtMaximum(double amount) noexcept -> Patch&;


  private:
    std::optional<std::string> mNewSymbol;
    std::optional<std::string> mNewVenueId;
    std::optional<std::string> mNewSecurityType;
    std::optional<std::string> mNewPriceCurrency;
    std::optional<std::string> mNewFxBaseCurrency;
    std::optional<std::string> mNewInstrSymbol;
    std::optional<std::string> mNewSecurityExchange;
    std::optional<std::string> mNewPartyId;
    std::optional<std::string> mNewPartyRole;
    std::optional<std::string> mNewCusipId;
    std::optional<std::string> mNewSedolId;
    std::optional<std::string> mNewIsinId;
    std::optional<std::string> mNewRicId;
    std::optional<std::string> mNewExchangeSymbolId;
    std::optional<std::string> mNewBloombergSymbolId;

    std::optional<double> mNewQtyMinimum;
    std::optional<double> mNewQtyMaximum;
    std::optional<double> mNewQtyMultiple;
    std::optional<double> mNewPriceTickSize;
    std::optional<double> mNewRandomQtyMaximum;
    std::optional<double> mNewRandomOrdersSpread;
    std::optional<double> mNewRandomQtyMinimum;
    std::optional<double> mNewRandomAmtMinimum;
    std::optional<double> mNewRandomAmtMaximum;
    std::optional<double> mNewRandomAggressiveQtyMinimum;
    std::optional<double> mNewRandomAggressiveQtyMaximum;
    std::optional<double> mNewRandomAggressiveAmtMinimum;
    std::optional<double> mNewRandomAggressiveAmtMaximum;

    std::optional<std::uint32_t> mNewRandomDepthLevels;
    std::optional<std::uint32_t> mNewRandomOrdersRate;
    std::optional<std::uint32_t> mNewRandomTickRange;

    std::optional<bool> mNewEnabledFlag;
    std::optional<bool> mNewRandomOrdersEnabledFlag;
};

} // namespace Simulator::DataLayer

#endif // SIMULATOR_DATA_LAYER_INCLUDE_DATA_LAYER_API_MODELS_LISTING_HPP_
