#ifndef SIMULATOR_GENERATOR_IH_FACTORY_EXECUTABLE_FACTORY_HPP_
#define SIMULATOR_GENERATOR_IH_FACTORY_EXECUTABLE_FACTORY_HPP_

#include <memory>
#include <vector>

#include "data_layer/api/models/datasource.hpp"
#include "data_layer/api/models/price_seed.hpp"

#include "ih/context/component_context.hpp"
#include "ih/context/instrument_context.hpp"
#include "ih/utils/executable.hpp"

namespace Simulator::Generator {

class InstrumentRandomGeneratorFactory {
  public:
    virtual ~InstrumentRandomGeneratorFactory() = default;

    [[nodiscard]]
    virtual auto createOrdersExecutable(
        std::shared_ptr<OrderInstrumentContext> _pInstrumentContext,
        DataLayer::PriceSeed const& _priceSeed
    ) -> std::unique_ptr<Executable> = 0;
};


class InstrumentHistoricalReplierFactory {
  public:
    using Datasources = std::vector<DataLayer::Datasource>;

    using InstrumentsContexts =
        std::vector<std::shared_ptr<OrderInstrumentContext>>;

    virtual ~InstrumentHistoricalReplierFactory() = default;

    [[nodiscard]]
    virtual auto createOrdersExecutable(
        Datasources const& _datasources,
        InstrumentsContexts const& _contexts
    ) -> std::unique_ptr<Executable> = 0;
};

} // namespace Simulator::Generator

#endif // SIMULATOR_GENERATOR_IH_FACTORY_EXECUTABLE_FACTORY_HPP_
