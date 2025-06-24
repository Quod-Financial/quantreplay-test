#include "ih/historical/replier.hpp"

#include <exception>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

#include "ih/historical/data/provider.hpp"
#include "ih/historical/data/record.hpp"
#include "ih/historical/processor.hpp"
#include "ih/historical/scheduler.hpp"
#include "log/logging.hpp"

namespace Simulator::Generator::Historical {

Replier::Replier(
    Datasources const& _datasources,
    InstrumentsContexts const& _instrumentsContexts)
{
    if (_instrumentsContexts.empty()) {
        throw std::invalid_argument{
            "can not initialize historical replier without instruments "
            "contexts"};
    }

    m_pHistoricalProcessor = std::make_unique<ActionProcessor>(
        _instrumentsContexts
    );

    // We'll redesign the approach of datasources initialization when
    // a reading from multiple datasources will be required.
    // It may require parallel initialization of data_providers and
    // data_adapters.
    auto pProvider = createProvider(_datasources);

    m_pActionsScheduler =
        std::make_unique<Historical::ActionsScheduler>(std::move(pProvider));

    simulator::log::info("historical data replier initialized successfully");
}


auto Replier::prepare() noexcept -> void { m_pActionsScheduler->initialize(); }


auto Replier::execute() -> void
{
    m_pActionsScheduler->processNextAction([this](Historical::Action _action) {
        simulator::log::debug("historical replier is applying historical {}",
                              _action);
    m_pHistoricalProcessor->process(std::move(_action));
    });
}


auto Replier::finished() const noexcept -> bool
{
    return m_pActionsScheduler->finished();
}


auto Replier::nextExecTimeout() const -> std::chrono::microseconds
{
    return m_pActionsScheduler->nextActionTimeout();
}


auto Replier::createProvider(Datasources const& _datasources)
    -> std::unique_ptr<DataProvider>
{
    if (_datasources.empty()) {
        simulator::log::info(
            "no data sources configured, skipping historical replying "
            "configuration");
        return nullptr;
    }

    auto const& datasource = _datasources.front();
    auto pProviderFactory = std::make_unique<DataProvidersFactoryImpl>();
    std::unique_ptr<DataProvider> pProvider{nullptr};

    try {
        pProvider = pProviderFactory->createProvider(datasource);

        simulator::log::info(
            "created a data provider for the `{}' datasource (id: {})",
            datasource.name(),
            datasource.datasource_id());
    } catch (std::exception const& _ex) {
        simulator::log::warn(
            "Failed to initialize a data provider for `{}' datasource "
            "(id: {}): {}",
            datasource.name(),
            datasource.datasource_id(),
            _ex.what());
    }

    return pProvider;
}

} // namespace Simulator::Generator::Historical
