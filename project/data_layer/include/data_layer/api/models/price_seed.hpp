#ifndef SIMULATOR_DATA_LAYER_INCLUDE_DATA_LAYER_API_MODELS_PRICE_SEED_HPP_
#define SIMULATOR_DATA_LAYER_INCLUDE_DATA_LAYER_API_MODELS_PRICE_SEED_HPP_

#include <cstdint>
#include <optional>
#include <string>

#include "data_layer/api/predicate/definitions.hpp"

namespace Simulator::DataLayer {

class PriceSeed {
  public:
    class Patch;

    using Predicate = Predicate::Expression<PriceSeed>;

    enum class Attribute {
        PriceSeedId,
        Symbol,
        SecurityType,
        PriceCurrency,
        SecurityId,
        SecurityIdSource,
        InstrumentSymbol,
        MidPrice,
        BidPrice,
        OfferPrice,
        LastUpdate
    };

    [[nodiscard]]
    static auto create(PriceSeed::Patch _snapshot, std::uint64_t _id) noexcept
        -> PriceSeed;

    [[nodiscard]]
    auto getPriceSeedId() const noexcept -> std::uint64_t;

    [[nodiscard]]
    auto getSymbol() const noexcept -> std::optional<std::string> const&;

    [[nodiscard]]
    auto getSecurityType() const noexcept -> std::optional<std::string> const&;

    [[nodiscard]]
    auto getPriceCurrency() const noexcept -> std::optional<std::string> const&;

    [[nodiscard]]
    auto getSecurityId() const noexcept -> std::optional<std::string> const&;

    [[nodiscard]]
    auto getSecurityIdSource() const noexcept
        -> std::optional<std::string> const&;

    [[nodiscard]]
    auto getInstrumentSymbol() const noexcept
        -> std::optional<std::string> const&;

    [[nodiscard]]
    auto getMidPrice() const noexcept -> std::optional<double>;

    [[nodiscard]]
    auto getBidPrice() const noexcept -> std::optional<double>;

    [[nodiscard]]
    auto getOfferPrice() const noexcept -> std::optional<double>;

    [[nodiscard]]
    auto getLastUpdate() const noexcept -> std::optional<std::string> const&;

  private:
    PriceSeed() = default;

    std::optional<std::string> mSymbol;
    std::optional<std::string> mSecurityType;
    std::optional<std::string> mPriceCurrency;
    std::optional<std::string> mSecurityId;
    std::optional<std::string> mSecurityIdSource;
    std::optional<std::string> mInstrumentSymbol;
    std::optional<std::string> mLastUpdate;

    std::optional<double> mMidPrice;
    std::optional<double> mBidPrice;
    std::optional<double> mOfferPrice;

    std::uint64_t mPriceSeedId{0};
};

class PriceSeed::Patch {
    friend class PriceSeed;

  public:
    using Attribute = PriceSeed::Attribute;

    [[nodiscard]]
    auto getSymbol() const noexcept -> std::optional<std::string> const&;
    auto withSymbol(std::string _symbol) noexcept -> Patch&;

    [[nodiscard]]
    auto getSecurityType() const noexcept -> std::optional<std::string> const&;
    auto withSecurityType(std::string _type) noexcept -> Patch&;

    [[nodiscard]]
    auto getPriceCurrency() const noexcept -> std::optional<std::string> const&;
    auto withPriceCurrency(std::string _currency) noexcept -> Patch&;

    [[nodiscard]]
    auto getSecurityId() const noexcept -> std::optional<std::string> const&;
    auto withSecurityId(std::string _id) noexcept -> Patch&;

    [[nodiscard]]
    auto getSecurityIdSource() const noexcept
        -> std::optional<std::string> const&;
    auto withSecurityIdSource(std::string _source) noexcept -> Patch&;

    [[nodiscard]]
    auto getInstrumentSymbol() const noexcept
        -> std::optional<std::string> const&;
    auto withInstrumentSymbol(std::string _symbol) noexcept -> Patch&;

    [[nodiscard]]
    auto getMidPrice() const noexcept -> std::optional<double>;
    auto withMidPrice(double _price) noexcept -> Patch&;

    [[nodiscard]]
    auto getBidPrice() const noexcept -> std::optional<double>;
    auto withBidPrice(double _price) noexcept -> Patch&;

    [[nodiscard]]
    auto getOfferPrice() const noexcept -> std::optional<double>;
    auto withOfferPrice(double _price) noexcept -> Patch&;

    [[nodiscard]]
    auto getLastUpdate() const noexcept -> std::optional<std::string> const&;
    auto withLastUpdate(std::string _updateTimestamp) noexcept -> Patch&;

  private:
    std::optional<std::string> mNewSymbol;
    std::optional<std::string> mNewSecurityType;
    std::optional<std::string> mNewPriceCurrency;
    std::optional<std::string> mNewSecurityId;
    std::optional<std::string> mNewSecurityIdSource;
    std::optional<std::string> mNewInstrumentSymbol;
    std::optional<std::string> mNewLastUpdate;

    std::optional<double> mNewMidPrice;
    std::optional<double> mNewBidPrice;
    std::optional<double> mNewOfferPrice;
};

} // namespace Simulator::DataLayer

#endif // SIMULATOR_DATA_LAYER_INCLUDE_DATA_LAYER_API_MODELS_PRICE_SEED_HPP_
