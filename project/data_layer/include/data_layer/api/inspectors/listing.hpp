#ifndef SIMULATOR_PROJECT_DATA_LAYER_API_COMMON_INSPECTORS_LISTING_HPP_
#define SIMULATOR_PROJECT_DATA_LAYER_API_COMMON_INSPECTORS_LISTING_HPP_

#include <cstdint>
#include <functional>
#include <string>
#include <type_traits>
#include <utility>

#include "data_layer/api/models/listing.hpp"

namespace Simulator::DataLayer {

template<typename Marshaller>
class ListingReader final {
    using Attribute = Listing::Attribute;

    template<typename T>
    constexpr inline static bool can_marshall_v = std::is_invocable_v<
        Marshaller,
        Attribute,
        std::add_const_t<std::add_lvalue_reference_t<T>>>;

  public:
    explicit ListingReader(Marshaller& _marshaller) noexcept :
        mMarshaller(_marshaller)
    {}

    // TODO: Add unit tests
    auto read(Listing const& _listing) -> void;

  private:
    std::reference_wrapper<Marshaller> mMarshaller;
};


template<typename Marshaller>
class ListingPatchReader final {
    using Attribute = Listing::Attribute;

    template<typename T>
    constexpr inline static bool can_marshall_v = std::is_invocable_v<
        Marshaller,
        Attribute,
        std::add_const_t<std::add_lvalue_reference_t<T>>>;

  public:
    explicit ListingPatchReader(Marshaller& _marshaller) noexcept :
        mMarshaller(_marshaller)
    {}

    auto read(Listing::Patch const& _patch) -> void;

  private:
    std::reference_wrapper<Marshaller> mMarshaller;
};


template<typename Unmarshaller>
class ListingPatchWriter final {
    using Attribute = Listing::Attribute;

    template<typename T>
    constexpr inline static bool can_unmarshall_v = std::is_invocable_r_v<
        bool,
        Unmarshaller,
        Attribute,
        std::add_lvalue_reference_t<T>>;

  public:
    explicit ListingPatchWriter(Unmarshaller& _unmarshaller) noexcept :
        mUnmarshaller(_unmarshaller)
    {}

    auto write(Listing::Patch& _patch) -> void;

  private:
    std::reference_wrapper<Unmarshaller> mUnmarshaller;
};


template<typename Marshaller>
inline auto ListingReader<Marshaller>::read(Listing const& _listing) -> void
{
    std::uint64_t const listingId = _listing.getListingId();
    static_assert(can_marshall_v<decltype(listingId)>);
    mMarshaller(Attribute::ListingId, listingId);

    std::string const& venueId = _listing.getVenueId();
    static_assert(can_marshall_v<decltype(venueId)>);
    mMarshaller(Attribute::VenueId, venueId);

    if (auto const& value = _listing.getSymbol()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::Symbol, *value);
    }

    if (auto const& value = _listing.getSecurityType()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::SecurityType, *value);
    }

    if (auto const& value = _listing.getPriceCurrency()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::PriceCurrency, *value);
    }

    if (auto const& value = _listing.getFxBaseCurrency()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::FxBaseCurrency, *value);
    }

    if (auto const& value = _listing.getInstrSymbol()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::InstrSymbol, *value);
    }

    if (auto const& value = _listing.getQtyMinimum()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::QtyMinimum, *value);
    }

    if (auto const& value = _listing.getQtyMaximum()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::QtyMaximum, *value);
    }

    if (auto const& value = _listing.getQtyMultiple()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::QtyMultiple, *value);
    }

    if (auto const& value = _listing.getPriceTickSize()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::PriceTickSize, *value);
    }

    if (auto const& value = _listing.getEnabledFlag()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::Enabled, *value);
    }

    if (auto const& value = _listing.getRandomQtyMaximum()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::RandomQtyMaximum, *value);
    }

    if (auto const& value = _listing.getRandomDepthLevels()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::RandomDepthLevels, *value);
    }

    if (auto const& value = _listing.getRandomOrdersSpread()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::RandomOrdersSpread, *value);
    }

    if (auto const& value = _listing.getRandomOrdersRate()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::RandomOrdersRate, *value);
    }

    if (auto const& value = _listing.getRandomTickRange()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::RandomTickRange, *value);
    }

    if (auto const& value = _listing.getSecurityExchange()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::SecurityExchange, *value);
    }

    if (auto const& value = _listing.getPartyId()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::PartyId, *value);
    }

    if (auto const& value = _listing.getPartyRole()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::PartyRole, *value);
    }

    if (auto const& value = _listing.getCusipId()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::CusipId, *value);
    }

    if (auto const& value = _listing.getSedolId()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::SedolId, *value);
    }

    if (auto const& value = _listing.getIsinId()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::IsinId, *value);
    }

    if (auto const& value = _listing.getRicId()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::RicId, *value);
    }

    if (auto const& value = _listing.getExchangeSymbolId()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::ExchangeSymbolId, *value);
    }

    if (auto const& value = _listing.getBloombergSymbolId()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::BloombergSymbolId, *value);
    }

    if (auto const& value = _listing.getRandomQtyMinimum()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::RandomQtyMinimum, *value);
    }

    if (auto const& value = _listing.getRandomAmtMinimum()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::RandomAmtMinimum, *value);
    }

    if (auto const& value = _listing.getRandomAmtMaximum()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::RandomAmtMaximum, *value);
    }

    if (auto const& value = _listing.getRandomOrdersEnabledFlag()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::RandomOrdersEnabled, *value);
    }

    if (const auto& value = _listing.getRandomAggressiveQtyMinimum()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::RandomAggressiveQtyMinimum, *value);
    }

    if (const auto& value = _listing.getRandomAggressiveQtyMaximum()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::RandomAggressiveQtyMaximum, *value);
    }

    if (const auto& value = _listing.getRandomAggressiveAmtMinimum()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::RandomAggressiveAmtMinimum, *value);
    }

    if (const auto& value = _listing.getRandomAggressiveAmtMaximum()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::RandomAggressiveAmtMaximum, *value);
    }
}


template<typename Marshaller>
inline auto ListingPatchReader<Marshaller>::read(Listing::Patch const& _patch)
    -> void
{
    if (auto const& value = _patch.getSymbol()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::Symbol, *value);
    }

    if (auto const& value = _patch.getVenueId()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::VenueId, *value);
    }

    if (auto const& value = _patch.getSecurityType()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::SecurityType, *value);
    }

    if (auto const& value = _patch.getPriceCurrency()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::PriceCurrency, *value);
    }

    if (auto const& value = _patch.getFxBaseCurrency()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::FxBaseCurrency, *value);
    }

    if (auto const& value = _patch.getInstrSymbol()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::InstrSymbol, *value);
    }

    if (auto const& value = _patch.getQtyMinimum()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::QtyMinimum, *value);
    }

    if (auto const& value = _patch.getQtyMaximum()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::QtyMaximum, *value);
    }

    if (auto const& value = _patch.getQtyMultiple()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::QtyMultiple, *value);
    }

    if (auto const& value = _patch.getPriceTickSize()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::PriceTickSize, *value);
    }

    if (auto const& value = _patch.getEnabledFlag()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::Enabled, *value);
    }

    if (auto const& value = _patch.getRandomQtyMaximum()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::RandomQtyMaximum, *value);
    }

    if (auto const& value = _patch.getRandomDepthLevels()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::RandomDepthLevels, *value);
    }

    if (auto const& value = _patch.getRandomOrdersSpread()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::RandomOrdersSpread, *value);
    }

    if (auto const& value = _patch.getRandomOrdersRate()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::RandomOrdersRate, *value);
    }

    if (auto const& value = _patch.getRandomTickRange()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::RandomTickRange, *value);
    }

    if (auto const& value = _patch.getSecurityExchange()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::SecurityExchange, *value);
    }

    if (auto const& value = _patch.getPartyId()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::PartyId, *value);
    }

    if (auto const& value = _patch.getPartyRole()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::PartyRole, *value);
    }

    if (auto const& value = _patch.getCusipId()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::CusipId, *value);
    }

    if (auto const& value = _patch.getSedolId()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::SedolId, *value);
    }

    if (auto const& value = _patch.getIsinId()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::IsinId, *value);
    }

    if (auto const& value = _patch.getRicId()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::RicId, *value);
    }

    if (auto const& value = _patch.getExchangeSymbolId()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::ExchangeSymbolId, *value);
    }

    if (auto const& value = _patch.getBloombergSymbolId()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::BloombergSymbolId, *value);
    }

    if (auto const& value = _patch.getRandomQtyMinimum()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::RandomQtyMinimum, *value);
    }

    if (auto const& value = _patch.getRandomAmtMinimum()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::RandomAmtMinimum, *value);
    }

    if (auto const& value = _patch.getRandomAmtMaximum()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::RandomAmtMaximum, *value);
    }

    if (auto const& value = _patch.getRandomOrdersEnabledFlag()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::RandomOrdersEnabled, *value);
    }

    if (const auto& value = _patch.getRandomAggressiveQtyMinimum()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::RandomAggressiveQtyMinimum, *value);
    }

    if (const auto& value = _patch.getRandomAggressiveQtyMaximum()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::RandomAggressiveQtyMaximum, *value);
    }

    if (const auto& value = _patch.getRandomAggressiveAmtMinimum()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::RandomAggressiveAmtMinimum, *value);
    }

    if (const auto& value = _patch.getRandomAggressiveAmtMaximum()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::RandomAggressiveAmtMaximum, *value);
    }
}


template<typename Unmarshaller>
inline auto ListingPatchWriter<Unmarshaller>::write(Listing::Patch& _patch)
    -> void
{
    std::string symbol{};
    static_assert(can_unmarshall_v<decltype(symbol)>);
    if (mUnmarshaller(Attribute::Symbol, symbol)) {
        _patch.withSymbol(std::move(symbol));
    }

    std::string venueId{};
    static_assert(can_unmarshall_v<decltype(venueId)>);
    if (mUnmarshaller(Attribute::VenueId, venueId)) {
        _patch.withVenueId(std::move(venueId));
    }

    std::string securityType{};
    static_assert(can_unmarshall_v<decltype(securityType)>);
    if (mUnmarshaller(Attribute::SecurityType, securityType)) {
        _patch.withSecurityType(std::move(securityType));
    }

    std::string priceCurrency{};
    static_assert(can_unmarshall_v<decltype(priceCurrency)>);
    if (mUnmarshaller(Attribute::PriceCurrency, priceCurrency)) {
        _patch.withPriceCurrency(std::move(priceCurrency));
    }

    std::string fxBaseCurrency{};
    static_assert(can_unmarshall_v<decltype(fxBaseCurrency)>);
    if (mUnmarshaller(Attribute::FxBaseCurrency, fxBaseCurrency)) {
        _patch.withFxBaseCurrency(std::move(fxBaseCurrency));
    }

    std::string instrSymbol{};
    static_assert(can_unmarshall_v<decltype(instrSymbol)>);
    if (mUnmarshaller(Attribute::InstrSymbol, instrSymbol)) {
        _patch.withInstrSymbol(std::move(instrSymbol));
    }

    double qtyMinimum{};
    static_assert(can_unmarshall_v<decltype(qtyMinimum)>);
    if (mUnmarshaller(Attribute::QtyMinimum, qtyMinimum)) {
        _patch.withQtyMinimum(qtyMinimum);
    }

    double qtyMaximum{};
    static_assert(can_unmarshall_v<decltype(qtyMaximum)>);
    if (mUnmarshaller(Attribute::QtyMaximum, qtyMaximum)) {
        _patch.withQtyMaximum(qtyMaximum);
    }

    double qtyMultiple{};
    static_assert(can_unmarshall_v<decltype(qtyMultiple)>);
    if (mUnmarshaller(Attribute::QtyMultiple, qtyMultiple)) {
        _patch.withQtyMultiple(qtyMultiple);
    }

    double priceTickSize{};
    static_assert(can_unmarshall_v<decltype(priceTickSize)>);
    if (mUnmarshaller(Attribute::PriceTickSize, priceTickSize)) {
        _patch.withPriceTickSize(priceTickSize);
    }

    bool enabled{};
    static_assert(can_unmarshall_v<decltype(enabled)>);
    if (mUnmarshaller(Attribute::Enabled, enabled)) {
        _patch.withEnabledFlag(enabled);
    }

    double randomQtyMaximum{};
    static_assert(can_unmarshall_v<decltype(randomQtyMaximum)>);
    if (mUnmarshaller(Attribute::RandomQtyMaximum, randomQtyMaximum)) {
        _patch.withRandomQtyMaximum(randomQtyMaximum);
    }

    std::uint32_t randomDepthLevels{};
    static_assert(can_unmarshall_v<decltype(randomDepthLevels)>);
    if (mUnmarshaller(Attribute::RandomDepthLevels, randomDepthLevels)) {
        _patch.withRandomDepthLevels(randomDepthLevels);
    }

    double randomOrdersSpread{};
    static_assert(can_unmarshall_v<decltype(randomOrdersSpread)>);
    if (mUnmarshaller(Attribute::RandomOrdersSpread, randomOrdersSpread)) {
        _patch.withRandomOrdersSpread(randomOrdersSpread);
    }

    std::uint32_t randomOrdersRate{};
    static_assert(can_unmarshall_v<decltype(randomOrdersRate)>);
    if (mUnmarshaller(Attribute::RandomOrdersRate, randomOrdersRate)) {
        _patch.withRandomOrdersRate(randomOrdersRate);
    }

    std::uint32_t randomTickRange{};
    static_assert(can_unmarshall_v<decltype(randomTickRange)>);
    if (mUnmarshaller(Attribute::RandomTickRange, randomTickRange)) {
        _patch.withRandomTickRange(randomTickRange);
    }

    std::string securityExchange{};
    static_assert(can_unmarshall_v<decltype(securityExchange)>);
    if (mUnmarshaller(Attribute::SecurityExchange, securityExchange)) {
        _patch.withSecurityExchange(std::move(securityExchange));
    }

    std::string partyId{};
    static_assert(can_unmarshall_v<decltype(partyId)>);
    if (mUnmarshaller(Attribute::PartyId, partyId)) {
        _patch.withPartyId(std::move(partyId));
    }

    std::string partyRole{};
    static_assert(can_unmarshall_v<decltype(partyRole)>);
    if (mUnmarshaller(Attribute::PartyRole, partyRole)) {
        _patch.withPartyRole(std::move(partyRole));
    }

    std::string cusipId{};
    static_assert(can_unmarshall_v<decltype(cusipId)>);
    if (mUnmarshaller(Attribute::CusipId, cusipId)) {
        _patch.withCusipId(std::move(cusipId));
    }

    std::string sedolId{};
    static_assert(can_unmarshall_v<decltype(sedolId)>);
    if (mUnmarshaller(Attribute::SedolId, sedolId)) {
        _patch.withSedolId(std::move(sedolId));
    }

    std::string isinId{};
    static_assert(can_unmarshall_v<decltype(isinId)>);
    if (mUnmarshaller(Attribute::IsinId, isinId)) {
        _patch.withIsinId(std::move(isinId));
    }

    std::string ricId{};
    static_assert(can_unmarshall_v<decltype(ricId)>);
    if (mUnmarshaller(Attribute::RicId, ricId)) {
        _patch.withRicId(std::move(ricId));
    }

    std::string exchangeSymbolId{};
    static_assert(can_unmarshall_v<decltype(exchangeSymbolId)>);
    if (mUnmarshaller(Attribute::ExchangeSymbolId, exchangeSymbolId)) {
        _patch.withExchangeSymbolId(std::move(exchangeSymbolId));
    }

    std::string bloombergSymbolId{};
    static_assert(can_unmarshall_v<decltype(bloombergSymbolId)>);
    if (mUnmarshaller(Attribute::BloombergSymbolId, bloombergSymbolId)) {
        _patch.withBloombergSymbolId(std::move(bloombergSymbolId));
    }

    double randomQtyMinimum{};
    static_assert(can_unmarshall_v<decltype(randomQtyMinimum)>);
    if (mUnmarshaller(Attribute::RandomQtyMinimum, randomQtyMinimum)) {
        _patch.withRandomQtyMinimum(randomQtyMinimum);
    }

    double randomAmtMinimum{};
    static_assert(can_unmarshall_v<decltype(randomAmtMinimum)>);
    if (mUnmarshaller(Attribute::RandomAmtMinimum, randomAmtMinimum)) {
        _patch.withRandomAmtMinimum(randomAmtMinimum);
    }

    double randomAmtMaximum{};
    static_assert(can_unmarshall_v<decltype(randomAmtMaximum)>);
    if (mUnmarshaller(Attribute::RandomAmtMaximum, randomAmtMaximum)) {
        _patch.withRandomAmtMaximum(randomAmtMaximum);
    }

    bool randomOrdersEnabled{};
    static_assert(can_unmarshall_v<decltype(randomOrdersEnabled)>);
    if (mUnmarshaller(Attribute::RandomOrdersEnabled, randomOrdersEnabled)) {
        _patch.withRandomOrdersEnabledFlag(randomOrdersEnabled);
    }

    double randomAggressiveQtyMinimum{};
    static_assert(can_unmarshall_v<decltype(randomAggressiveQtyMinimum)>);
    if (mUnmarshaller(Attribute::RandomAggressiveQtyMinimum, randomAggressiveQtyMinimum)) {
        _patch.withRandomAggressiveQtyMinimum(randomAggressiveQtyMinimum);
    }

    double randomAggressiveQtyMaximum{};
    static_assert(can_unmarshall_v<decltype(randomAggressiveQtyMaximum)>);
    if (mUnmarshaller(Attribute::RandomAggressiveQtyMaximum, randomAggressiveQtyMaximum)) {
        _patch.withRandomAggressiveQtyMaximum(randomAggressiveQtyMaximum);
    }

    double randomAggressiveAmtMinimum{};
    static_assert(can_unmarshall_v<decltype(randomAggressiveAmtMinimum)>);
    if (mUnmarshaller(Attribute::RandomAggressiveAmtMinimum, randomAggressiveAmtMinimum)) {
        _patch.withRandomAggressiveAmtMinimum(randomAggressiveAmtMinimum);
    }

    double randomAggressiveAmtMaximum{};
    static_assert(can_unmarshall_v<decltype(randomAggressiveAmtMaximum)>);
    if (mUnmarshaller(Attribute::RandomAggressiveAmtMaximum, randomAggressiveAmtMaximum)) {
        _patch.withRandomAggressiveAmtMaximum(randomAggressiveAmtMaximum);
    }
}

} // namespace Simulator::DataLayer

#endif // SIMULATOR_PROJECT_DATA_LAYER_IH_COMMON_INSPECTORS_LISTING_HPP_
