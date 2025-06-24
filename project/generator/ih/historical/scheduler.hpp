#ifndef SIMULATOR_GENERATOR_IH_HISTORICAL_SCHEDULER_HPP_
#define SIMULATOR_GENERATOR_IH_HISTORICAL_SCHEDULER_HPP_

#include <chrono>
#include <deque>
#include <functional>
#include <memory>
#include <vector>

#include "ih/historical/data/provider.hpp"
#include "ih/historical/data/record.hpp"

namespace Simulator::Generator::Historical {

class Scheduler {
  public:
    using ActionProcessor = std::function<void(Historical::Action)>;

    Scheduler() = default;

    Scheduler(Scheduler const&) = default;
    auto operator=(Scheduler const&) -> Scheduler& = default;

    Scheduler(Scheduler&&) = default;
    auto operator=(Scheduler&&) -> Scheduler& = default;

    virtual ~Scheduler() = default;

    [[nodiscard]]
    virtual auto finished() const noexcept -> bool = 0;

    virtual auto initialize() noexcept -> void = 0;

    virtual auto processNextAction(ActionProcessor const& _processor)
        -> void = 0;

    [[nodiscard]]
    virtual auto nextActionTimeout() const -> std::chrono::microseconds = 0;
};

class ActionsScheduler final : public Scheduler {
  public:
    ActionsScheduler() = delete;

    explicit ActionsScheduler(std::unique_ptr<DataProvider> _provider);

    [[nodiscard]]
    auto finished() const noexcept -> bool override;

    auto initialize() noexcept -> void override;

    auto processNextAction(ActionProcessor const& _processor) -> void override;

    [[nodiscard]]
    auto nextActionTimeout() const -> std::chrono::microseconds override;

  private:
    [[nodiscard]]
    auto hasDataProvider() const noexcept -> bool;

    [[nodiscard]]
    auto canPullAction() const noexcept -> bool;

    [[nodiscard]]
    auto hasPendingActions() const noexcept -> bool;

    auto pull() noexcept -> void;

    auto pullNextAction() noexcept -> void;

    std::deque<Historical::Action> mPendingActions;
    std::unique_ptr<Historical::DataProvider> mDataProvider;
};

} // namespace Simulator::Generator::Historical

#endif // SIMULATOR_GENERATOR_IH_HISTORICAL_SCHEDULER_HPP_
