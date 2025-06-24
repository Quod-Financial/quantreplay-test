#ifndef SIMULATOR_GENERATOR_SRC_GENERATORS_PRICE_GENERATOR_HPP_
#define SIMULATOR_GENERATOR_SRC_GENERATORS_PRICE_GENERATOR_HPP_

#include <cassert>
#include <memory>
#include <optional>
#include <utility>

#include "data_layer/api/models/price_seed.hpp"

#include "ih/context/order_market_data_provider.hpp"
#include "ih/random/generators/value_generator.hpp"
#include "ih/random/values/event.hpp"
#include "ih/random/values/price_generation_params.hpp"
#include "ih/tracing/tracing.hpp"

namespace Simulator::Generator::Random {

class PriceGenerator
{
protected:

    struct GenerationDetails
    {
        double basePx { 0. };
        double basePxIncrement { 0. };
        double randomTick { 0. };

        int geometricSum { 0 };
        int randomNumber { 0 };

        bool wasBasePxIncrementAdded { false };
        bool wasConfiguredPxUsed { false };
    };

public:

    virtual ~PriceGenerator() = default;

    template<typename Tracer = Trace::NullTracer>
    double generatePrice(
            PriceGenerationParams const & _params
        ,   Generator::MarketState const & _actualPrices
        ,   DataLayer::PriceSeed const & _configuredPrices
        ,   Random::Event _event
        ,   Tracer && _tracer = Tracer {}
    );

private:

    virtual std::pair<double, GenerationDetails> generatePx(
            PriceGenerationParams const & _params
        ,   Generator::MarketState const & _actualPrices
        ,   DataLayer::PriceSeed const & _configuredPrices
        ,   Random::Event _event
    ) = 0;
};


template<typename Tracer>
double PriceGenerator::generatePrice(
        PriceGenerationParams const & _params
    ,   Generator::MarketState const & _actualPrices
    ,   DataLayer::PriceSeed const & _configuredPrices
    ,   Random::Event _event
    ,   Tracer && _tracer
)
{
    using namespace Trace;

    assert(_event.isBuyEvent() || _event.isSellEvent());
    assert(_event.isRestingOrderEvent() || _event.isAggressiveOrderEvent());

    auto step = makeStep(_tracer, "generating order price");
    traceInput(step, makeValue("priceTickRange", _params.getPriceTickRange()));
    traceInput(step, makeValue("priceTickSize", _params.getPriceTickSize()));
    traceInput(step, makeValue("priceSpread", _params.getPriceSpread()));
    traceInput(step,
        makeValue("bestBuyPrice", _actualPrices.bestBidPrice.value_or(0))
    );
    traceInput(step,
        makeValue("bestSellPrice", _actualPrices.bestOfferPrice.value_or(0))
    );

    const auto mid_price = _configuredPrices.getMidPrice().value_or(0);
    traceInput(step,
        makeValue("priceSeedBuy", _configuredPrices.getBidPrice().value_or(mid_price))
    );
    traceInput(step,
        makeValue("priceSeedSell", _configuredPrices.getOfferPrice().value_or(mid_price))
    );
    traceInput(step,
        makeValue("priceSeedMid", mid_price)
    );
    traceInput(step,
        makeValue("currentSide", fmt::format("{}", _event.targetSide()))
    );

    auto const [generatedPx, details] = generatePx(
            _params
        ,   _actualPrices
        ,   _configuredPrices
        ,   _event
    );

    if (details.wasConfiguredPxUsed)
    {
        traceOutput(step, makeCommentedValue(
                "generatedPrice"
            ,   generatedPx
            ,   "price was generated from seed configuration"
        ));
    }
    else
    {
        traceOutput(step, makeValue("randomValue", details.randomNumber));
        traceOutput(step, makeValue("geometricSum", details.geometricSum));
        traceOutput(step, makeValue("randomTick", details.randomTick));

        traceOutput(step, makeCommentedValue(
                "basePx"
            ,   details.basePx
            ,   "The current best price "
                "with price spread conditionally added/subtracted"
        ));

        traceOutput(step, makeCommentedValue(
                "basePxIncrement"
            ,   details.basePxIncrement
            ,   "Random price deviation added/subtracted to/from basePx"
        ));

        traceOutput(step, makeCommentedValue(
                "wasBasePxIncrementAdded"
            ,   details.wasBasePxIncrementAdded
            ,   "indicates if a basePxIncrement was added to "
                "or subtracted from basePx"
        ));

        traceOutput(step, makeCommentedValue(
                "generatedPrice"
            ,   generatedPx
            ,   "price was generated from a base price and random price tick"
        ));
    }

    traceStep(std::move(step), _tracer);
    return generatedPx;
}

} // namespace Simulator::Generator::Random


namespace Simulator::Generator::Random {

class PriceGeneratorImpl final
    :   public Random::PriceGenerator
{
public:

    explicit PriceGeneratorImpl(
        std::shared_ptr<Random::ValueGenerator> _pRandomIntGenerator
    ) noexcept;

    static std::unique_ptr<PriceGeneratorImpl> create(
        std::shared_ptr<Random::ValueGenerator> _pRandomIntGenerator
    );

private:

    std::pair<double, GenerationDetails> generatePx(
            PriceGenerationParams const & _params
        ,   Generator::MarketState const & _actualPrices
        ,   DataLayer::PriceSeed const & _configuredPrices
        ,   Random::Event _event
    ) override;

    double generateTick(
            PriceGenerationParams const & _params
        ,   GenerationDetails &_details);

    static std::optional<double> resolveBasePrice(
            Generator::MarketState const & _actualPrices
        ,   PriceGenerationParams const & _generationParams
        ,   Random::Event _event
    );

    static double resolveConfiguredPrice(
            DataLayer::PriceSeed const & _configuredPx
        ,   Random::Event _event
    );


    std::shared_ptr<Random::ValueGenerator> m_pRandomIntGenerator;
};

} // namespace Simulator::Generator::Random

#endif // SIMULATOR_GENERATOR_SRC_GENERATORS_PRICE_GENERATOR_HPP_
