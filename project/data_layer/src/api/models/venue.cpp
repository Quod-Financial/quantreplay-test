#include "api/models/venue.hpp"

#include <algorithm>
#include <optional>
#include <stdexcept>
#include <vector>

#include "ih/common/exceptions.hpp"

namespace Simulator::DataLayer {

auto Venue::create(Patch _snapshot) -> Venue
{
    if (!_snapshot.mPatchedVenueId.has_value()) {
        throw RequiredAttributeMissing("Venue", "VenueID");
    }

    Venue venue{};

#define SIM_ASSIGN_FIELD(VENUE_FIELD, SNAPSHOT_FIELD)                          \
    if (_snapshot.SNAPSHOT_FIELD.has_value()) {                                \
        venue.VENUE_FIELD = std::move(*(_snapshot.SNAPSHOT_FIELD));            \
    }                                                                          \
    void(0)

    SIM_ASSIGN_FIELD(mVenueId, mPatchedVenueId);
    SIM_ASSIGN_FIELD(mName, mPatchedName);
    SIM_ASSIGN_FIELD(mTimezone, mPatchedTimezone);
    SIM_ASSIGN_FIELD(mPersistenceFilePath, mPatchedPersistenceFilePath);
    SIM_ASSIGN_FIELD(mRandomPartiesCount, mPatchedRandomPartiesCount);
    SIM_ASSIGN_FIELD(mEngineType, mPatchedEngineType);
    SIM_ASSIGN_FIELD(mRestPort, mPatchedRestPort);
    SIM_ASSIGN_FIELD(mSupportTifIocFlag, mPatchedSupportTifIocFlag);
    SIM_ASSIGN_FIELD(mSupportTifFokFlag, mPatchedSupportTifFokFlag);
    SIM_ASSIGN_FIELD(mSupportTifDayFlag, mPatchedSupportTifDayFlag);
    SIM_ASSIGN_FIELD(mIncludeOwnOrdersFlag, mPatchedIncludeOwnOrdersFlag);
    SIM_ASSIGN_FIELD(mOrderOnStartupFlag, mPatchedOrderOnStartupFlag);
    SIM_ASSIGN_FIELD(mTnsEnabledFlag, mPatchedTnsEnabledFlag);
    SIM_ASSIGN_FIELD(mTnsQtyEnabledFlag, mPatchedTnsQtyEnabledFlag);
    SIM_ASSIGN_FIELD(mTnsSideEnabledFlag, mPatchedTnsSideEnabledFlag);
    SIM_ASSIGN_FIELD(mTnsPartiesEnabledFlag, mPatchedTnsPartiesEnabledFlag);
    SIM_ASSIGN_FIELD(mCancelOnDisconnectFlag, mPatchedCancelOnDisconnectFlag);
    SIM_ASSIGN_FIELD(mPersistenceEnabledFlag, mPatchedPersistenceEnabledFlag);

#undef SIM_ASSIGN_FIELD

    if (auto& phases = _snapshot.mPatchedMarketPhases) {
        venue.mMarketPhases.reserve(phases->size());
        std::transform(
            std::make_move_iterator(phases->begin()),
            std::make_move_iterator(phases->end()),
            std::back_inserter(venue.mMarketPhases),
            [&venue](MarketPhase::Patch&& _phaseSnapshot) -> MarketPhase {
                return MarketPhase::create(
                    std::move(_phaseSnapshot),
                    venue.mVenueId
                );
            }
        );
    }

    return venue;
}

auto Venue::getVenueId() const noexcept -> std::string const&
{
    return mVenueId;
}

auto Venue::getName() const noexcept -> std::optional<std::string> const&
{
    return mName;
}

auto Venue::getEngineType() const noexcept -> std::optional<EngineType>
{
    return mEngineType;
}

auto Venue::getSupportTifIocFlag() const noexcept -> std::optional<bool>
{
    return mSupportTifIocFlag;
}

auto Venue::getSupportTifFokFlag() const noexcept -> std::optional<bool>
{
    return mSupportTifFokFlag;
}

auto Venue::getSupportTifDayFlag() const noexcept -> std::optional<bool>
{
    return mSupportTifDayFlag;
}

auto Venue::getIncludeOwnOrdersFlag() const noexcept -> std::optional<bool>
{
    return mIncludeOwnOrdersFlag;
}

auto Venue::getRestPort() const noexcept -> std::optional<std::uint16_t>
{
    return mRestPort;
}

auto Venue::getOrderOnStartupFlag() const noexcept -> std::optional<bool>
{
    return mOrderOnStartupFlag;
}

auto Venue::getRandomPartiesCount() const noexcept
    -> std::optional<std::uint32_t>
{
    return mRandomPartiesCount;
}

auto Venue::getTnsEnabledFlag() const noexcept -> std::optional<bool>
{
    return mTnsEnabledFlag;
}

auto Venue::getTnsQtyEnabledFlag() const noexcept -> std::optional<bool>
{
    return mTnsQtyEnabledFlag;
}

auto Venue::getTnsSideEnabledFlag() const noexcept -> std::optional<bool>
{
    return mTnsSideEnabledFlag;
}

auto Venue::getTnsPartiesEnabledFlag() const noexcept -> std::optional<bool>
{
    return mTnsPartiesEnabledFlag;
}

auto Venue::getTimezone() const noexcept -> std::optional<std::string> const&
{
    return mTimezone;
}

auto Venue::getCancelOnDisconnectFlag() const noexcept -> std::optional<bool>
{
    return mCancelOnDisconnectFlag;
}

auto Venue::getPersistenceEnabledFlag() const noexcept -> std::optional<bool>
{
    return mPersistenceEnabledFlag;
}

auto Venue::getPersistenceFilePath() const noexcept
    -> std::optional<std::string> const&
{
    return mPersistenceFilePath;
}

auto Venue::getMarketPhases() const noexcept -> std::vector<MarketPhase> const&
{
    return mMarketPhases;
}

auto Venue::Patch::getVenueId() const noexcept
    -> std::optional<std::string> const&
{
    return mPatchedVenueId;
}

auto Venue::Patch::withVenueId(std::string _venueId) noexcept -> Patch&
{
    mPatchedVenueId = std::move(_venueId);
    return *this;
}

auto Venue::Patch::getName() const noexcept -> std::optional<std::string> const&
{
    return mPatchedName;
}

auto Venue::Patch::withName(std::string _name) noexcept -> Patch&
{
    mPatchedName = std::move(_name);
    return *this;
}

auto Venue::Patch::getEngineType() const noexcept -> std::optional<EngineType>
{
    return mPatchedEngineType;
}

auto Venue::Patch::withEngineType(EngineType _engineType) noexcept -> Patch&
{
    mPatchedEngineType = _engineType;
    return *this;
}

auto Venue::Patch::getSupportTifIocFlag() const noexcept -> std::optional<bool>
{
    return mPatchedSupportTifIocFlag;
}

auto Venue::Patch::withSupportTifIocFlag(bool _flag) noexcept -> Patch&
{
    mPatchedSupportTifIocFlag = _flag;
    return *this;
}

auto Venue::Patch::getSupportTifFokFlag() const noexcept -> std::optional<bool>
{
    return mPatchedSupportTifFokFlag;
}

auto Venue::Patch::withSupportTifFokFlag(bool _flag) noexcept -> Patch&
{
    mPatchedSupportTifFokFlag = _flag;
    return *this;
}

auto Venue::Patch::getSupportTifDayFlag() const noexcept -> std::optional<bool>
{
    return mPatchedSupportTifDayFlag;
}

auto Venue::Patch::withSupportTifDayFlag(bool _flag) noexcept -> Patch&
{
    mPatchedSupportTifDayFlag = _flag;
    return *this;
}

auto Venue::Patch::getIncludeOwnOrdersFlag() const noexcept
    -> std::optional<bool>
{
    return mPatchedIncludeOwnOrdersFlag;
}

auto Venue::Patch::withIncludeOwnOrdersFlag(bool _flag) noexcept -> Patch&
{
    mPatchedIncludeOwnOrdersFlag = _flag;
    return *this;
}

auto Venue::Patch::getRestPort() const noexcept -> std::optional<std::uint16_t>
{
    return mPatchedRestPort;
}

auto Venue::Patch::withRestPort(std::uint32_t _port) noexcept -> Patch&
{
    mPatchedRestPort = _port;
    return *this;
}

auto Venue::Patch::getOrdersOnStartupFlag() const noexcept
    -> std::optional<bool>
{
    return mPatchedOrderOnStartupFlag;
}

auto Venue::Patch::withOrdersOnStartupFlag(bool _flag) noexcept -> Patch&
{
    mPatchedOrderOnStartupFlag = _flag;
    return *this;
}

auto Venue::Patch::getRandomPartiesCount() const noexcept
    -> std::optional<std::uint32_t>
{
    return mPatchedRandomPartiesCount;
}

auto Venue::Patch::withRandomPartiesCount(std::uint32_t _count) noexcept
    -> Patch&
{
    mPatchedRandomPartiesCount = _count;
    return *this;
}

auto Venue::Patch::getTnsEnabledFlag() const noexcept -> std::optional<bool>
{
    return mPatchedTnsEnabledFlag;
}

auto Venue::Patch::withTnsEnabledFlag(bool _flag) noexcept -> Patch&
{
    mPatchedTnsEnabledFlag = _flag;
    return *this;
}

auto Venue::Patch::getTnsQtyEnabledFlag() const noexcept -> std::optional<bool>
{
    return mPatchedTnsQtyEnabledFlag;
}

auto Venue::Patch::withTnsQtyEnabledFlag(bool _flag) noexcept -> Patch&
{
    mPatchedTnsQtyEnabledFlag = _flag;
    return *this;
}

auto Venue::Patch::getTnsSideEnabledFlag() const noexcept -> std::optional<bool>
{
    return mPatchedTnsSideEnabledFlag;
}

auto Venue::Patch::withTnsSideEnabledFlag(bool _flag) noexcept -> Patch&
{
    mPatchedTnsSideEnabledFlag = _flag;
    return *this;
}

auto Venue::Patch::getTnsPartiesEnabledFlag() const noexcept
    -> std::optional<bool>
{
    return mPatchedTnsPartiesEnabledFlag;
}

auto Venue::Patch::withTnsPartiesEnabledFlag(bool _flag) noexcept -> Patch&
{
    mPatchedTnsPartiesEnabledFlag = _flag;
    return *this;
}

auto Venue::Patch::getTimezone() const noexcept
    -> std::optional<std::string> const&
{
    return mPatchedTimezone;
}

auto Venue::Patch::withTimezone(std::string _timezone) noexcept -> Patch&
{
    mPatchedTimezone = std::move(_timezone);
    return *this;
}

auto Venue::Patch::getCancelOnDisconnectFlag() const noexcept
    -> std::optional<bool>
{
    return mPatchedCancelOnDisconnectFlag;
}

auto Venue::Patch::withCancelOnDisconnectFlag(bool _flag) noexcept -> Patch&
{
    mPatchedCancelOnDisconnectFlag = _flag;
    return *this;
}

auto Venue::Patch::getPersistenceEnabledFlag() const noexcept
    -> std::optional<bool>
{
    return mPatchedPersistenceEnabledFlag;
}

auto Venue::Patch::withPersistenceEnabledFlag(bool _flag) noexcept -> Patch&
{
    mPatchedPersistenceEnabledFlag = _flag;
    return *this;
}

auto Venue::Patch::getPersistenceFilePath() const noexcept
    -> std::optional<std::string> const&
{
    return mPatchedPersistenceFilePath;
}

auto Venue::Patch::withPersistenceFilePath(std::string _path) noexcept -> Patch&
{
    mPatchedPersistenceFilePath = std::move(_path);
    return *this;
}

auto Venue::Patch::getMarketPhases() const noexcept
    -> std::optional<std::vector<MarketPhase::Patch>> const&
{
    return mPatchedMarketPhases;
}

auto Venue::Patch::withMarketPhase(MarketPhase::Patch _phaseSnapshot) -> Patch&
{
    if (!mPatchedMarketPhases.has_value()) {
        mPatchedMarketPhases = std::vector<MarketPhase::Patch>{};
    }

    mPatchedMarketPhases->emplace_back(std::move(_phaseSnapshot));
    return *this;
}

auto Venue::Patch::withoutMarketPhases() noexcept -> Patch&
{
    mPatchedMarketPhases = decltype(mPatchedMarketPhases)::value_type{};
    return *this;
}

} // namespace Simulator::DataLayer