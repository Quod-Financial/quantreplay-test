#ifndef SIMULATOR_DATA_LAYER_API_COMMON_INSPECTORS_MARKET_PHASE_HPP_
#define SIMULATOR_DATA_LAYER_API_COMMON_INSPECTORS_MARKET_PHASE_HPP_

#include <type_traits>
#include <utility>

#include "data_layer/api/models/market_phase.hpp"

namespace Simulator::DataLayer {

template<typename Marshaller>
class MarketPhaseReader final {
    using Attribute = MarketPhase::Attribute;

    template<typename T>
    constexpr inline static bool can_marshall_v = std::is_invocable_v<
        Marshaller,
        Attribute,
        std::add_const_t<std::add_lvalue_reference_t<T>>>;

  public:
    explicit MarketPhaseReader(Marshaller& _marshaller) noexcept :
        mMarshaller(_marshaller)
    {}

    auto read(MarketPhase const& _phase) -> void;

  private:
    std::reference_wrapper<Marshaller> mMarshaller;
};


template<typename Unmarshaller>
class MarketPhasePatchWriter final {
    using Attribute = MarketPhase::Attribute;

    template<typename T>
    constexpr inline static bool can_unmarshall_v = std::is_invocable_r_v<
        bool,
        Unmarshaller,
        Attribute,
        std::add_lvalue_reference_t<T>>;

  public:
    explicit MarketPhasePatchWriter(Unmarshaller& _unmarshaller) noexcept :
        mUnmarshaller(_unmarshaller)
    {}

    auto write(MarketPhase::Patch& _patch) -> void;

  private:
    std::reference_wrapper<Unmarshaller> mUnmarshaller;
};


template<typename Marshaller>
inline auto MarketPhaseReader<Marshaller>::read(
    MarketPhase const& _phase
) -> void
{
    static_assert(can_marshall_v<decltype(_phase.phase())>);
    mMarshaller(Attribute::Phase, _phase.phase());

    static_assert(can_marshall_v<decltype(_phase.start_time())>);
    mMarshaller(Attribute::StartTime, _phase.start_time());

    static_assert(can_marshall_v<decltype(_phase.end_time())>);
    mMarshaller(Attribute::EndTime, _phase.end_time());

    static_assert(can_marshall_v<decltype(_phase.venue_id())>);
    mMarshaller(Attribute::VenueId, _phase.venue_id());

    if (auto&& optValue = _phase.allow_cancels()) {
        static_assert(can_marshall_v<decltype(*optValue)>);
        mMarshaller(Attribute::AllowCancels, *optValue);
    }

    if (auto&& optValue = _phase.end_time_range()) {
        static_assert(can_marshall_v<decltype(*optValue)>);
        mMarshaller(Attribute::EndTimeRange, *optValue);
    }
}


template<typename Unmarshaller>
inline auto MarketPhasePatchWriter<Unmarshaller>::write(
    MarketPhase::Patch& _patch
) -> void
{
    MarketPhase::Phase phase{};
    static_assert(can_unmarshall_v<decltype(phase)>);
    if (mUnmarshaller(Attribute::Phase, phase)) {
        _patch.with_phase(phase);
    }

    std::string startTime{};
    static_assert(can_unmarshall_v<decltype(startTime)>);
    if (mUnmarshaller(Attribute::StartTime, startTime)) {
        _patch.with_start_time(std::move(startTime));
    }

    std::string endTime{};
    static_assert(can_unmarshall_v<decltype(endTime)>);
    if (mUnmarshaller(Attribute::EndTime, endTime)) {
        _patch.with_end_time(std::move(endTime));
    }

    bool allowCancels = false;
    static_assert(can_unmarshall_v<decltype(allowCancels)>);
    if (mUnmarshaller(Attribute::AllowCancels, allowCancels)) {
        _patch.with_allow_cancels(allowCancels);
    }

    std::int32_t endTimeRange{};
    static_assert(can_unmarshall_v<decltype(endTimeRange)>);
    if (mUnmarshaller(Attribute::EndTimeRange, endTimeRange)) {
        _patch.with_end_time_range(endTimeRange);
    }
}

} // namespace Simulator::DataLayer

#endif // SIMULATOR_DATA_LAYER_API_COMMON_INSPECTORS_MARKET_PHASE_HPP_
