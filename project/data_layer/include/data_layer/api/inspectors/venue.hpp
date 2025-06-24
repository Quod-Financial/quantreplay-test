#ifndef SIMULATOR_DATA_LAYER_API_COMMON_INSPECTORS_VENUE_HPP_
#define SIMULATOR_DATA_LAYER_API_COMMON_INSPECTORS_VENUE_HPP_

#include <cstdint>
#include <string>
#include <type_traits>
#include <utility>

#include "data_layer/api/models/venue.hpp"

namespace Simulator::DataLayer {

template<typename Marshaller>
class VenueReader final {
    using Attribute = Venue::Attribute;

    template<typename T>
    constexpr inline static bool can_marshall_v = std::is_invocable_v<
        Marshaller,
        Attribute,
        std::add_const_t<std::add_lvalue_reference_t<T>>>;

  public:
    explicit VenueReader(Marshaller& _marshaller) noexcept :
        mMarshaller(_marshaller)
    {}

    auto read(Venue const& _venue) -> void;

  private:
    std::reference_wrapper<Marshaller> mMarshaller;
};


template<typename Marshaller>
class VenuePatchReader final {
    using Attribute = Venue::Attribute;

    template<typename T>
    constexpr inline static bool can_marshall_v = std::is_invocable_v<
        Marshaller,
        Attribute,
        std::add_const_t<std::add_lvalue_reference_t<T>>>;

  public:
    explicit VenuePatchReader(Marshaller& _marshaller) noexcept :
        mMarshaller(_marshaller)
    {}

    auto read(Venue::Patch const& _patch) -> void;

  private:
    std::reference_wrapper<Marshaller> mMarshaller;
};


template<typename Unmarshaller>
class VenuePatchWriter final {
    using Attribute = Venue::Attribute;

    template<typename T>
    constexpr inline static bool can_unmarshall_v = std::is_invocable_r_v<
        bool,
        Unmarshaller,
        Attribute,
        std::add_lvalue_reference_t<T>>;

  public:
    explicit VenuePatchWriter(Unmarshaller& _unmarshaller) noexcept :
        mUnmarshaller(_unmarshaller)
    {}

    auto write(Venue::Patch& _patch) -> void;

  private:
    std::reference_wrapper<Unmarshaller> mUnmarshaller;
};


template<typename Marshaller>
auto VenueReader<Marshaller>::read(Venue const& _venue) -> void
{
    auto const& venueId = _venue.getVenueId();
    static_assert(can_marshall_v<decltype(venueId)>);
    mMarshaller(Attribute::VenueId, venueId);

    if (auto const& value = _venue.getName()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::Name, *value);
    }

    if (auto const& value = _venue.getEngineType()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::EngineType, *value);
    }

    if (auto const& value = _venue.getSupportTifIocFlag()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::SupportTifIoc, *value);
    }

    if (auto const& value = _venue.getSupportTifFokFlag()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::SupportTifFok, *value);
    }

    if (auto const& value = _venue.getSupportTifDayFlag()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::SupportTifDay, *value);
    }

    if (auto const& value = _venue.getIncludeOwnOrdersFlag()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::IncludeOwnOrders, *value);
    }

    if (auto const& value = _venue.getRestPort()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::RestPort, *value);
    }

    if (auto const& value = _venue.getOrderOnStartupFlag()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::OrderOnStartup, *value);
    }

    if (auto const& value = _venue.getRandomPartiesCount()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::RandomPartiesCount, *value);
    }

    if (auto const& value = _venue.getTnsEnabledFlag()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::TnsEnabled, *value);
    }

    if (auto const& value = _venue.getTnsQtyEnabledFlag()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::TnsQtyEnabled, *value);
    }

    if (auto const& value = _venue.getTnsSideEnabledFlag()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::TnsSideEnabled, *value);
    }

    if (auto const& value = _venue.getTnsPartiesEnabledFlag()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::TnsPartiesEnabled, *value);
    }

    if (auto const& value = _venue.getTimezone()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::Timezone, *value);
    }

    if (auto const& value = _venue.getCancelOnDisconnectFlag()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::CancelOnDisconnect, *value);
    }

    if (auto const& value = _venue.getPersistenceEnabledFlag()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::PersistenceEnabled, *value);
    }

    if (auto const& value = _venue.getPersistenceFilePath()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::PersistenceFilePath, *value);
    }
}


template<typename Marshaller>
inline auto VenuePatchReader<Marshaller>::read(Venue::Patch const& _patch)
    -> void
{
    if (auto const& value = _patch.getVenueId()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::VenueId, *value);
    }

    if (auto const& value = _patch.getName()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::Name, *value);
    }

    if (auto const& value = _patch.getEngineType()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::EngineType, *value);
    }

    if (auto const& value = _patch.getSupportTifIocFlag()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::SupportTifIoc, *value);
    }

    if (auto const& value = _patch.getSupportTifFokFlag()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::SupportTifFok, *value);
    }

    if (auto const& value = _patch.getSupportTifDayFlag()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::SupportTifDay, *value);
    }

    if (auto const& value = _patch.getIncludeOwnOrdersFlag()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::IncludeOwnOrders, *value);
    }

    if (auto const& value = _patch.getRestPort()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::RestPort, *value);
    }

    if (auto const& value = _patch.getOrdersOnStartupFlag()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::OrderOnStartup, *value);
    }

    if (auto const& value = _patch.getRandomPartiesCount()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::RandomPartiesCount, *value);
    }

    if (auto const& value = _patch.getTnsEnabledFlag()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::TnsEnabled, *value);
    }

    if (auto const& value = _patch.getTnsQtyEnabledFlag()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::TnsQtyEnabled, *value);
    }

    if (auto const& value = _patch.getTnsSideEnabledFlag()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::TnsSideEnabled, *value);
    }

    if (auto const& value = _patch.getTnsPartiesEnabledFlag()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::TnsPartiesEnabled, *value);
    }

    if (auto const& value = _patch.getTimezone()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::Timezone, *value);
    }

    if (auto const& value = _patch.getCancelOnDisconnectFlag()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::CancelOnDisconnect, *value);
    }

    if (auto const& value = _patch.getPersistenceEnabledFlag()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::PersistenceEnabled, *value);
    }

    if (auto const& value = _patch.getPersistenceFilePath()) {
        static_assert(can_marshall_v<decltype(*value)>);
        mMarshaller(Attribute::PersistenceFilePath, *value);
    }
}


template<typename Unmarshaller>
inline auto VenuePatchWriter<Unmarshaller>::write(Venue::Patch& _patch) -> void
{
    std::string venueId{};
    static_assert(can_unmarshall_v<decltype(venueId)>);
    if (mUnmarshaller(Attribute::VenueId, venueId)) {
        _patch.withVenueId(std::move(venueId));
    }

    std::string name{};
    static_assert(can_unmarshall_v<decltype(name)>);
    if (mUnmarshaller(Attribute::Name, name)) {
        _patch.withName(std::move(name));
    }

    Venue::EngineType engineType{};
    static_assert(can_unmarshall_v<decltype(engineType)>);
    if (mUnmarshaller(Attribute::EngineType, engineType)) {
        _patch.withEngineType(engineType);
    }

    bool supportTifIoc{};
    static_assert(can_unmarshall_v<decltype(supportTifIoc)>);
    if (mUnmarshaller(Attribute::SupportTifIoc, supportTifIoc)) {
        _patch.withSupportTifIocFlag(supportTifIoc);
    }

    bool supportTifFok{};
    static_assert(can_unmarshall_v<decltype(supportTifFok)>);
    if (mUnmarshaller(Attribute::SupportTifFok, supportTifFok)) {
        _patch.withSupportTifFokFlag(supportTifFok);
    }

    bool supportTifDay{};
    static_assert(can_unmarshall_v<decltype(supportTifDay)>);
    if (mUnmarshaller(Attribute::SupportTifDay, supportTifDay)) {
        _patch.withSupportTifDayFlag(supportTifDay);
    }

    bool includeOwnOrders{};
    static_assert(can_unmarshall_v<decltype(includeOwnOrders)>);
    if (mUnmarshaller(Attribute::IncludeOwnOrders, includeOwnOrders)) {
        _patch.withIncludeOwnOrdersFlag(includeOwnOrders);
    }

    std::uint16_t restPort{};
    static_assert(can_unmarshall_v<decltype(restPort)>);
    if (mUnmarshaller(Attribute::RestPort, restPort)) {
        _patch.withRestPort(restPort);
    }

    bool ordersOnStartup{};
    static_assert(can_unmarshall_v<decltype(ordersOnStartup)>);
    if (mUnmarshaller(Attribute::OrderOnStartup, ordersOnStartup)) {
        _patch.withOrdersOnStartupFlag(ordersOnStartup);
    }

    std::uint32_t randomPartiesCount{};
    static_assert(can_unmarshall_v<decltype(randomPartiesCount)>);
    if (mUnmarshaller(Attribute::RandomPartiesCount, randomPartiesCount)) {
        _patch.withRandomPartiesCount(randomPartiesCount);
    }

    bool tnsEnabled{};
    static_assert(can_unmarshall_v<decltype(tnsEnabled)>);
    if (mUnmarshaller(Attribute::TnsEnabled, tnsEnabled)) {
        _patch.withTnsEnabledFlag(tnsEnabled);
    }

    bool tnsQtyEnabled{};
    static_assert(can_unmarshall_v<decltype(tnsQtyEnabled)>);
    if (mUnmarshaller(Attribute::TnsQtyEnabled, tnsQtyEnabled)) {
        _patch.withTnsQtyEnabledFlag(tnsQtyEnabled);
    }

    bool tnsSideEnabled{};
    static_assert(can_unmarshall_v<decltype(tnsSideEnabled)>);
    if (mUnmarshaller(Attribute::TnsSideEnabled, tnsSideEnabled)) {
        _patch.withTnsSideEnabledFlag(tnsSideEnabled);
    }

    bool tnsPartiesEnabled{};
    static_assert(can_unmarshall_v<decltype(tnsPartiesEnabled)>);
    if (mUnmarshaller(Attribute::TnsPartiesEnabled, tnsPartiesEnabled)) {
        _patch.withTnsPartiesEnabledFlag(tnsPartiesEnabled);
    }

    std::string timezone{};
    static_assert(can_unmarshall_v<decltype(timezone)>);
    if (mUnmarshaller(Attribute::Timezone, timezone)) {
        _patch.withTimezone(std::move(timezone));
    }

    bool cancelOnDisconnect{};
    static_assert(can_unmarshall_v<decltype(cancelOnDisconnect)>);
    if (mUnmarshaller(Attribute::CancelOnDisconnect, cancelOnDisconnect)) {
        _patch.withCancelOnDisconnectFlag(cancelOnDisconnect);
    }

    bool persistenceEnabled{};
    static_assert(can_unmarshall_v<decltype(persistenceEnabled)>);
    if (mUnmarshaller(Attribute::PersistenceEnabled, persistenceEnabled)) {
        _patch.withPersistenceEnabledFlag(persistenceEnabled);
    }

    std::string persistenceFilePath{};
    static_assert(can_unmarshall_v<decltype(persistenceFilePath)>);
    if (mUnmarshaller(Attribute::PersistenceFilePath, persistenceFilePath)) {
        _patch.withPersistenceFilePath(std::move(persistenceFilePath));
    }
}

} // namespace Simulator::DataLayer

#endif // SIMULATOR_DATA_LAYER_API_COMMON_INSPECTORS_VENUE_HPP_
