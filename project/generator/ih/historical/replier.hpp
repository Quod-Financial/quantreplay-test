#ifndef SIMULATOR_GENERATOR_SRC_HISTORICAL_REPLIER_HPP_
#define SIMULATOR_GENERATOR_SRC_HISTORICAL_REPLIER_HPP_

#include <chrono>
#include <memory>
#include <vector>

#include "data_layer/api/models/datasource.hpp"

#include "ih/context/instrument_context.hpp"
#include "ih/historical/data/provider.hpp"
#include "ih/historical/processor.hpp"
#include "ih/historical/scheduler.hpp"
#include "ih/utils/executable.hpp"

namespace Simulator::Generator::Historical {

class Replier : public Generator::Executable {
  public:
    using Datasources = std::vector<DataLayer::Datasource>;
    using InstrumentsContexts =
        std::vector<std::shared_ptr<OrderInstrumentContext>>;

    Replier() = delete;

    Replier(
        Datasources const& _datasources,
        InstrumentsContexts const& _instrumentsContexts
    );

    [[nodiscard]]
    auto finished() const noexcept -> bool override;

    void prepare() noexcept override;

    void execute() override;

    [[nodiscard]]
    auto nextExecTimeout() const -> std::chrono::microseconds override;

  private:
    static auto createProvider(Datasources const& _datasources)
        -> std::unique_ptr<DataProvider>;

    std::unique_ptr<Processor> m_pHistoricalProcessor;
    std::unique_ptr<Scheduler> m_pActionsScheduler;
};

} // namespace Simulator::Generator::Historical

#endif // SIMULATOR_GENERATOR_SRC_HISTORICAL_REPLIER_HPP_
