#include "ih/historical/scheduler.hpp"

#include <cassert>
#include <chrono>
#include <exception>
#include <memory>
#include <utility>

#include <fmt/chrono.h>

#include "ih/historical/data/provider.hpp"
#include "ih/historical/data/record.hpp"
#include "log/logging.hpp"

namespace Simulator::Generator::Historical {

ActionsScheduler::ActionsScheduler(std::unique_ptr<DataProvider> _pProvider) :
    mDataProvider{std::move(_pProvider)}
{
    simulator::log::info(
        "historical records scheduler initialized successfully");
}

auto ActionsScheduler::finished() const noexcept -> bool
{
    return !hasPendingActions() && !canPullAction();
}

auto ActionsScheduler::initialize() noexcept -> void
{
    if (hasDataProvider()) {
        simulator::log::debug(
            "scheduler is asked to explicitly set a time offset "
            "for historical data provider");

        assert(mDataProvider);
        mDataProvider->initializeTimeOffset();
    }
    if (hasPendingActions()) {
        auto const newActionsTime = std::chrono::system_clock::now();
        simulator::log::debug(
            "scheduler is resetting a base time to `{}' "
            "for previously cached actions",
            newActionsTime);

        for (auto& pendingAction : mPendingActions) {
            auto updAction =
                Action::update_time(std::move(pendingAction), newActionsTime);
            pendingAction = std::move(updAction);
        }
    }
}

auto ActionsScheduler::processNextAction(ActionProcessor const& _processor)
    -> void
{
    if (finished()) {
        return;
    }

    pull();

    if (hasPendingActions()) {
        assert(!mPendingActions.empty());
        auto nextAction = std::move(mPendingActions.front());
        mPendingActions.pop_front();
        _processor(std::move(nextAction));
    }

    pull();
}

auto ActionsScheduler::nextActionTimeout() const -> std::chrono::microseconds
{
    using std::chrono::system_clock;
    using std::chrono::microseconds;
    using std::chrono::duration_cast;
    using std::chrono::time_point_cast;

    constexpr microseconds immediately{0};

    if (finished() || mPendingActions.empty()) {
        return immediately;
    }

    auto const actionTime = mPendingActions.front().action_time();
    auto const now = time_point_cast<Historical::Duration>(system_clock::now());
    return now < actionTime ? duration_cast<microseconds>(actionTime - now)
                            : immediately;
}

auto ActionsScheduler::hasDataProvider() const noexcept -> bool
{
    return mDataProvider != nullptr;
}

auto ActionsScheduler::canPullAction() const noexcept -> bool
{
    return hasDataProvider() && !mDataProvider->isEmpty();
}

auto ActionsScheduler::hasPendingActions() const noexcept -> bool
{
    return !mPendingActions.empty();
}

auto ActionsScheduler::pull() noexcept -> void
{
    while (!hasPendingActions() && canPullAction()) {
        pullNextAction();
    }
}

auto ActionsScheduler::pullNextAction() noexcept -> void
{
    if (!canPullAction()) {
        return;
    }

    if (!mDataProvider->hasTimeOffset()) {
        mDataProvider->initializeTimeOffset();
    }

    try {
        mDataProvider->pullAction([this](auto _action) {
            mPendingActions.emplace_back(std::move(_action));
        });
    } catch (std::exception const& _ex) {
        simulator::log::warn(
            "an error occurred while fetching a record from "
            "a data provider: {}",
            _ex.what());
    }
}

} // namespace Simulator::Generator::Historical
