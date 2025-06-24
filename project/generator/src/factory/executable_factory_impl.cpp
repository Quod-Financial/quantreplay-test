#include "ih/factory/executable_factory_impl.hpp"

#include <cassert>

#include "ih/context/order_generation_context_impl.hpp"
#include "ih/context/order_market_data_provider.hpp"
#include "ih/historical/replier.hpp"
#include "ih/random/algorithm/order_generation_algorithm.hpp"
#include "ih/random/instrument_generator.hpp"
#include "ih/utils/validator.hpp"

namespace Simulator::Generator {

auto InstrumentRandomGeneratorFactoryImpl::create()
    -> std::unique_ptr<InstrumentRandomGeneratorFactory>
{
    return std::make_unique<InstrumentRandomGeneratorFactoryImpl>();
}

auto InstrumentRandomGeneratorFactoryImpl::createOrdersExecutable(
    std::shared_ptr<OrderInstrumentContext> _pInstrumentContext,
    DataLayer::PriceSeed const& _priceSeed
) -> std::unique_ptr<Executable>
{
    assert(_pInstrumentContext);

    auto const& instrument = _pInstrumentContext->getInstrument();

    if (!Validator::isAcceptableForRandomGeneration(instrument) ||
        !Validator::isAcceptable(_priceSeed))
    {
        return nullptr;
    }

    auto pGenerationContext = OrderGenerationContextImpl::create(
        _pInstrumentContext,
        _priceSeed
    );

    return std::make_unique<Random::OrderGenerator>(
        std::move(_pInstrumentContext),
        Random::OrderGenerationAlgorithm::create(pGenerationContext)
    );
}

auto HistoricalReplierFactoryImpl::createOrdersExecutable(
    Datasources const& _datasources,
    InstrumentsContexts const& _contexts
) -> std::unique_ptr<Executable>
{
    return std::make_unique<Historical::Replier>(
        _datasources,
        _contexts
    );
}

} // namespace Simulator::Generator
