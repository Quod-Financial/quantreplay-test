#ifndef SIMULATOR_GENERATOR_IH_FACTORY_EXECUTABLE_FACTORY_IMPL_HPP_
#define SIMULATOR_GENERATOR_IH_FACTORY_EXECUTABLE_FACTORY_IMPL_HPP_

#include <memory>

#include "data_layer/api/models/price_seed.hpp"

#include "ih/context/instrument_context.hpp"
#include "ih/factory/executable_factory.hpp"
#include "ih/utils/executable.hpp"

namespace Simulator::Generator {

class InstrumentRandomGeneratorFactoryImpl final
    : public InstrumentRandomGeneratorFactory {
  public:
    InstrumentRandomGeneratorFactoryImpl() = default;

    [[nodiscard]]
    static auto create() -> std::unique_ptr<InstrumentRandomGeneratorFactory>;

    [[nodiscard]]
    auto createOrdersExecutable(
        std::shared_ptr<OrderInstrumentContext> _pInstrumentContext,
        DataLayer::PriceSeed const& _priceSeed
    ) -> std::unique_ptr<Executable> override;
};

class HistoricalReplierFactoryImpl final
    : public InstrumentHistoricalReplierFactory {
  public:
    HistoricalReplierFactoryImpl() = default;

    [[nodiscard]]
    auto createOrdersExecutable(
        Datasources const& _datasources,
        InstrumentsContexts const& _contexts
    ) -> std::unique_ptr<Executable> override;
};

} // namespace Simulator::Generator

#endif // SIMULATOR_GENERATOR_IH_FACTORY_EXECUTABLE_FACTORY_IMPL_HPP_
