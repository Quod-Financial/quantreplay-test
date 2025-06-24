#ifndef SIMULATOR_DATA_LAYER_INCLUDE_DATA_LAYER_API_MODELS_VENUE_HPP_
#define SIMULATOR_DATA_LAYER_INCLUDE_DATA_LAYER_API_MODELS_VENUE_HPP_

#include <optional>
#include <string>
#include <vector>

#include "data_layer/api/models/market_phase.hpp"
#include "data_layer/api/predicate/definitions.hpp"

namespace Simulator::DataLayer {

class Venue {
  public:
    class Patch;

    using Predicate = Predicate::Expression<Venue>;

    enum class Attribute {
        VenueId,
        Name,
        EngineType,
        SupportTifIoc,
        SupportTifFok,
        SupportTifDay,
        IncludeOwnOrders,
        RestPort,
        OrderOnStartup,
        RandomPartiesCount,
        TnsEnabled,
        TnsQtyEnabled,
        TnsSideEnabled,
        TnsPartiesEnabled,
        Timezone,
        CancelOnDisconnect,
        PersistenceEnabled,
        PersistenceFilePath,
    };

    enum class EngineType { Matching, Quoting };

    [[nodiscard]]
    static auto create(Patch _snapshot) -> Venue;

    [[nodiscard]]
    auto getVenueId() const noexcept -> std::string const&;

    [[nodiscard]]
    auto getName() const noexcept -> std::optional<std::string> const&;

    [[nodiscard]]
    auto getEngineType() const noexcept -> std::optional<EngineType>;

    [[nodiscard]]
    auto getSupportTifIocFlag() const noexcept -> std::optional<bool>;

    [[nodiscard]]
    auto getSupportTifFokFlag() const noexcept -> std::optional<bool>;

    [[nodiscard]]
    auto getSupportTifDayFlag() const noexcept -> std::optional<bool>;

    [[nodiscard]]
    auto getIncludeOwnOrdersFlag() const noexcept -> std::optional<bool>;

    [[nodiscard]]
    auto getRestPort() const noexcept -> std::optional<std::uint16_t>;

    [[nodiscard]]
    auto getOrderOnStartupFlag() const noexcept -> std::optional<bool>;

    [[nodiscard]]
    auto getRandomPartiesCount() const noexcept -> std::optional<std::uint32_t>;

    [[nodiscard]]
    auto getTnsEnabledFlag() const noexcept -> std::optional<bool>;

    [[nodiscard]]
    auto getTnsQtyEnabledFlag() const noexcept -> std::optional<bool>;

    [[nodiscard]]
    auto getTnsSideEnabledFlag() const noexcept -> std::optional<bool>;

    [[nodiscard]]
    auto getTnsPartiesEnabledFlag() const noexcept -> std::optional<bool>;

    [[nodiscard]]
    auto getTimezone() const noexcept -> std::optional<std::string> const&;

    [[nodiscard]]
    auto getCancelOnDisconnectFlag() const noexcept -> std::optional<bool>;

    [[nodiscard]]
    auto getPersistenceEnabledFlag() const noexcept -> std::optional<bool>;

    [[nodiscard]]
    auto getPersistenceFilePath() const noexcept
        -> std::optional<std::string> const&;

    [[nodiscard]]
    auto getMarketPhases() const noexcept -> std::vector<MarketPhase> const&;

  private:
    Venue() = default;

    std::optional<std::string> mName;
    std::optional<std::string> mTimezone;
    std::optional<std::string> mPersistenceFilePath;

    std::string mVenueId;

    std::vector<DataLayer::MarketPhase> mMarketPhases;

    std::optional<std::uint32_t> mRandomPartiesCount;

    std::optional<EngineType> mEngineType;

    std::optional<std::uint16_t> mRestPort;

    std::optional<bool> mSupportTifIocFlag;
    std::optional<bool> mSupportTifFokFlag;
    std::optional<bool> mSupportTifDayFlag;
    std::optional<bool> mIncludeOwnOrdersFlag;
    std::optional<bool> mOrderOnStartupFlag;
    std::optional<bool> mTnsEnabledFlag;
    std::optional<bool> mTnsQtyEnabledFlag;
    std::optional<bool> mTnsSideEnabledFlag;
    std::optional<bool> mTnsPartiesEnabledFlag;
    std::optional<bool> mCancelOnDisconnectFlag;
    std::optional<bool> mPersistenceEnabledFlag;
};

class Venue::Patch {
    friend class Venue;

  public:
    using Attribute = Venue::Attribute;

    [[nodiscard]]
    auto getVenueId() const noexcept -> std::optional<std::string> const&;
    auto withVenueId(std::string _venueId) noexcept -> Patch&;

    [[nodiscard]]
    auto getName() const noexcept -> std::optional<std::string> const&;
    auto withName(std::string _name) noexcept -> Patch&;

    [[nodiscard]]
    auto getEngineType() const noexcept -> std::optional<EngineType>;
    auto withEngineType(EngineType _engineType) noexcept -> Patch&;

    [[nodiscard]]
    auto getSupportTifIocFlag() const noexcept -> std::optional<bool>;
    auto withSupportTifIocFlag(bool _flag) noexcept -> Patch&;

    [[nodiscard]]
    auto getSupportTifFokFlag() const noexcept -> std::optional<bool>;
    auto withSupportTifFokFlag(bool _flag) noexcept -> Patch&;

    [[nodiscard]]
    auto getSupportTifDayFlag() const noexcept -> std::optional<bool>;
    auto withSupportTifDayFlag(bool _flag) noexcept -> Patch&;

    [[nodiscard]]
    auto getIncludeOwnOrdersFlag() const noexcept -> std::optional<bool>;
    auto withIncludeOwnOrdersFlag(bool _flag) noexcept -> Patch&;

    [[nodiscard]]
    auto getRestPort() const noexcept -> std::optional<std::uint16_t>;
    auto withRestPort(std::uint32_t _port) noexcept -> Patch&;

    [[nodiscard]]
    auto getOrdersOnStartupFlag() const noexcept -> std::optional<bool>;
    auto withOrdersOnStartupFlag(bool _flag) noexcept -> Patch&;

    [[nodiscard]]
    auto getRandomPartiesCount() const noexcept -> std::optional<std::uint32_t>;
    auto withRandomPartiesCount(std::uint32_t _count) noexcept -> Patch&;

    [[nodiscard]]
    auto getTnsEnabledFlag() const noexcept -> std::optional<bool>;
    auto withTnsEnabledFlag(bool _flag) noexcept -> Patch&;

    [[nodiscard]]
    auto getTnsQtyEnabledFlag() const noexcept -> std::optional<bool>;
    auto withTnsQtyEnabledFlag(bool _flag) noexcept -> Patch&;

    [[nodiscard]]
    auto getTnsSideEnabledFlag() const noexcept -> std::optional<bool>;
    auto withTnsSideEnabledFlag(bool _flag) noexcept -> Patch&;

    [[nodiscard]]
    auto getTnsPartiesEnabledFlag() const noexcept -> std::optional<bool>;
    auto withTnsPartiesEnabledFlag(bool _flag) noexcept -> Patch&;

    [[nodiscard]]
    auto getTimezone() const noexcept -> std::optional<std::string> const&;
    auto withTimezone(std::string _timezone) noexcept -> Patch&;

    [[nodiscard]]
    auto getCancelOnDisconnectFlag() const noexcept -> std::optional<bool>;
    auto withCancelOnDisconnectFlag(bool _flag) noexcept -> Patch&;

    [[nodiscard]]
    auto getPersistenceEnabledFlag() const noexcept -> std::optional<bool>;
    auto withPersistenceEnabledFlag(bool _flag) noexcept -> Patch&;

    [[nodiscard]]
    auto getPersistenceFilePath() const noexcept
        -> std::optional<std::string> const&;
    auto withPersistenceFilePath(std::string _path) noexcept -> Patch&;

    [[nodiscard]]
    auto getMarketPhases() const noexcept
        -> std::optional<std::vector<MarketPhase::Patch>> const&;
    auto withMarketPhase(MarketPhase::Patch _phaseSnapshot) -> Patch&;
    auto withoutMarketPhases() noexcept -> Patch&;

  private:
    std::optional<std::string> mPatchedVenueId;
    std::optional<std::string> mPatchedName;
    std::optional<std::string> mPatchedTimezone;
    std::optional<std::string> mPatchedPersistenceFilePath;

    std::optional<std::vector<MarketPhase::Patch>> mPatchedMarketPhases;

    std::optional<std::uint32_t> mPatchedRandomPartiesCount;

    std::optional<EngineType> mPatchedEngineType;

    std::optional<std::uint16_t> mPatchedRestPort;

    std::optional<bool> mPatchedSupportTifIocFlag;
    std::optional<bool> mPatchedSupportTifFokFlag;
    std::optional<bool> mPatchedSupportTifDayFlag;
    std::optional<bool> mPatchedIncludeOwnOrdersFlag;
    std::optional<bool> mPatchedOrderOnStartupFlag;
    std::optional<bool> mPatchedTnsEnabledFlag;
    std::optional<bool> mPatchedTnsQtyEnabledFlag;
    std::optional<bool> mPatchedTnsSideEnabledFlag;
    std::optional<bool> mPatchedTnsPartiesEnabledFlag;
    std::optional<bool> mPatchedCancelOnDisconnectFlag;
    std::optional<bool> mPatchedPersistenceEnabledFlag;
};

} // namespace Simulator::DataLayer

#endif // SIMULATOR_DATA_LAYER_INCLUDE_DATA_LAYER_API_MODELS_VENUE_HPP_
