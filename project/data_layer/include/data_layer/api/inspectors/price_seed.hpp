#ifndef SIMULATOR_DATA_LAYER_API_COMMON_INSPECTORS_PRICE_SEED_HPP_
#define SIMULATOR_DATA_LAYER_API_COMMON_INSPECTORS_PRICE_SEED_HPP_

#include <functional>
#include <string>
#include <type_traits>
#include <utility>

#include "data_layer/api/models/price_seed.hpp"

namespace Simulator::DataLayer {

template<typename Marshaller>
class PriceSeedReader final {
    using Attribute = PriceSeed::Attribute;

    template<typename T>
    constexpr inline static bool can_marshall_v = std::is_invocable_v<
        Marshaller,
        Attribute,
        std::add_const_t<std::add_lvalue_reference_t<T>>>;

  public:
    explicit PriceSeedReader(Marshaller& _marshaller) noexcept :
        mMarshaller(_marshaller)
    {}

    // TODO: Add unit tests
    auto read(PriceSeed const& _priceSeed) -> void;

  private:
    std::reference_wrapper<Marshaller> mMarshaller;
};


template<typename Marshaller>
class PriceSeedPatchReader final {
    using Attribute = PriceSeed::Attribute;

    template<typename T>
    constexpr inline static bool can_marshall_v = std::is_invocable_v<
        Marshaller,
        Attribute,
        std::add_const_t<std::add_lvalue_reference_t<T>>>;

  public:
    explicit PriceSeedPatchReader(Marshaller& _marshaller) noexcept :
        mMarshaller(_marshaller)
    {}

    auto read(PriceSeed::Patch const& _patch) -> void;

  private:
    std::reference_wrapper<Marshaller> mMarshaller;
};


template<typename Unmarshaller>
class PriceSeedPatchWriter final {
    using Attribute = PriceSeed::Attribute;

    template<typename T>
    constexpr inline static bool can_unmarshall_v = std::is_invocable_r_v<
        bool,
        Unmarshaller,
        Attribute,
        std::add_lvalue_reference_t<T>>;

  public:
    explicit PriceSeedPatchWriter(Unmarshaller& _unmarshaller) noexcept :
        mUnmarshaller(_unmarshaller)
    {}

    auto write(PriceSeed::Patch& _patch) -> void;

  private:
    std::reference_wrapper<Unmarshaller> mUnmarshaller;
};

template<typename Marshaller>
inline auto PriceSeedReader<Marshaller>::read(PriceSeed const& _priceSeed)
    -> void
{
    std::uint64_t const priceSeedId = _priceSeed.getPriceSeedId();
    static_assert(can_marshall_v<decltype(priceSeedId)>);
    mMarshaller(Attribute::PriceSeedId, priceSeedId);

    if (auto const& value = _priceSeed.getSymbol()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::Symbol, *value);
    }

    if (auto const& value = _priceSeed.getSecurityType()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::SecurityType, *value);
    }

    if (auto const& value = _priceSeed.getPriceCurrency()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::PriceCurrency, *value);
    }

    if (auto const& value = _priceSeed.getSecurityId()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::SecurityId, *value);
    }

    if (auto const& value = _priceSeed.getSecurityIdSource()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::SecurityIdSource, *value);
    }

    if (auto const& value = _priceSeed.getInstrumentSymbol()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::InstrumentSymbol, *value);
    }

    if (auto const& value = _priceSeed.getMidPrice()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::MidPrice, *value);
    }

    if (auto const& value = _priceSeed.getBidPrice()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::BidPrice, *value);
    }

    if (auto const& value = _priceSeed.getOfferPrice()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::OfferPrice, *value);
    }

    if (auto const& value = _priceSeed.getLastUpdate()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::LastUpdate, *value);
    }
}


template<typename Marshaller>
auto PriceSeedPatchReader<Marshaller>::read(PriceSeed::Patch const& _patch)
    -> void
{
    if (auto const& value = _patch.getSymbol()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::Symbol, *value);
    }

    if (auto const& value = _patch.getSecurityType()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::SecurityType, *value);
    }

    if (auto const& value = _patch.getPriceCurrency()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::PriceCurrency, *value);
    }

    if (auto const& value = _patch.getSecurityId()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::SecurityId, *value);
    }

    if (auto const& value = _patch.getSecurityIdSource()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::SecurityIdSource, *value);
    }

    if (auto const& value = _patch.getInstrumentSymbol()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::InstrumentSymbol, *value);
    }

    if (auto const& value = _patch.getMidPrice()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::MidPrice, *value);
    }

    if (auto const& value = _patch.getBidPrice()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::BidPrice, *value);
    }

    if (auto const& value = _patch.getOfferPrice()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::OfferPrice, *value);
    }

    if (auto const& value = _patch.getLastUpdate()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::LastUpdate, *value);
    }
}


template<typename Unmarshaller>
auto PriceSeedPatchWriter<Unmarshaller>::write(PriceSeed::Patch& _patch) -> void
{
    std::string symbol{};
    static_assert(can_unmarshall_v<decltype(symbol)>);
    if (mUnmarshaller(Attribute::Symbol, symbol)) {
        _patch.withSymbol(std::move(symbol));
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

    std::string securityId{};
    static_assert(can_unmarshall_v<decltype(securityId)>);
    if (mUnmarshaller(Attribute::SecurityId, securityId)) {
        _patch.withSecurityId(std::move(securityId));
    }

    std::string securityIdSource{};
    static_assert(can_unmarshall_v<decltype(securityIdSource)>);
    if (mUnmarshaller(Attribute::SecurityIdSource, securityIdSource)) {
        _patch.withSecurityIdSource(std::move(securityIdSource));
    }

    std::string instrumentSymbol{};
    static_assert(can_unmarshall_v<decltype(instrumentSymbol)>);
    if (mUnmarshaller(Attribute::InstrumentSymbol, instrumentSymbol)) {
        _patch.withInstrumentSymbol(std::move(instrumentSymbol));
    }

    double midPrice{};
    static_assert(can_unmarshall_v<decltype(midPrice)>);
    if (mUnmarshaller(Attribute::MidPrice, midPrice)) {
        _patch.withMidPrice(midPrice);
    }

    double bidPrice{};
    static_assert(can_unmarshall_v<decltype(bidPrice)>);
    if (mUnmarshaller(Attribute::BidPrice, bidPrice)) {
        _patch.withBidPrice(bidPrice);
    }

    double offerPrice{};
    static_assert(can_unmarshall_v<decltype(offerPrice)>);
    if (mUnmarshaller(Attribute::OfferPrice, offerPrice)) {
        _patch.withOfferPrice(offerPrice);
    }

    std::string lastUpdateTime{};
    static_assert(can_unmarshall_v<decltype(lastUpdateTime)>);
    if (mUnmarshaller(Attribute::LastUpdate, lastUpdateTime)) {
        _patch.withLastUpdate(std::move(lastUpdateTime));
    }
}

} // namespace Simulator::DataLayer

#endif // SIMULATOR_DATA_LAYER_API_COMMON_INSPECTORS_PRICE_SEED_HPP_
