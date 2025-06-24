#ifndef SIMULATOR_GENERATOR_SRC_ALGORITHM_ORDER_GENERATION_ALGORITHM_HPP_
#define SIMULATOR_GENERATOR_SRC_ALGORITHM_ORDER_GENERATION_ALGORITHM_HPP_

#include <memory>

#include "ih/adaptation/generated_message.hpp"
#include "ih/context/order_generation_context.hpp"
#include "ih/random/algorithm/generation_algorithm.hpp"
#include "ih/random/algorithm/utils/quantity_params_selector.hpp"
#include "ih/random/generators/counterparty_generator.hpp"
#include "ih/random/generators/event_generator.hpp"
#include "ih/random/generators/price_generator.hpp"
#include "ih/random/generators/quantity_generator.hpp"
#include "ih/random/generators/resting_order_action_generator.hpp"
#include "ih/random/values/event.hpp"
#include "ih/random/values/resting_order_action.hpp"
#include "ih/registry/generated_order_data.hpp"

namespace Simulator::Generator::Random {

class OrderGenerationAlgorithm final
    :   public Random::GenerationAlgorithm
{
private:

    OrderGenerationAlgorithm(
            std::shared_ptr<OrderGenerationContext> _pAlgorithmContext
        ,   std::unique_ptr<EventGenerator> _pEventGenerator
        ,   std::unique_ptr<CounterpartyGenerator> _pCounterpartyGenerator
        ,   std::unique_ptr<RestingOrderActionGenerator> _pRestingActionGenerator
        ,   std::unique_ptr<PriceGenerator> _pPriceGenerator
        ,   std::unique_ptr<QuantityGenerator> _pQtyGenerator
    ) noexcept;

public:

    static std::unique_ptr<OrderGenerationAlgorithm> create(
        std::shared_ptr<OrderGenerationContext> _pAlgorithmContext
    );

    static std::unique_ptr<OrderGenerationAlgorithm> create(
            std::shared_ptr<OrderGenerationContext> _pAlgorithmContext
        ,   std::unique_ptr<EventGenerator> _pEventGenerator
        ,   std::unique_ptr<CounterpartyGenerator> _pCounterpartyGenerator
        ,   std::unique_ptr<RestingOrderActionGenerator> _pRestingActionGenerator
        ,   std::unique_ptr<PriceGenerator> _pPriceGenerator
        ,   std::unique_ptr<QuantityGenerator> _pQtyGenerator
    );


    bool generate(GeneratedMessage & _targetMessage) override;

private:

    template<typename GenerationTracer>
    bool launchOn (GeneratedMessage & _msg, GenerationTracer & _tracer);


    template<typename GenerationTracer>
    bool fillAggressiveOrder(
            GeneratedMessage & _msg
        ,   MarketState const & _mktState
        ,   Event const & _event
        ,   GenerationTracer & _tracer
    );

    template<typename GenerationTracer>
    bool fillRestingOrder(
            GeneratedMessage & _msg
        ,   MarketState const & _mktState
        ,   Event const & _event
        ,   GenerationTracer & _tracer
    );

    template<typename GenerationTracer>
    void updateActiveRestingOrder(
            GeneratedMessage & _msg
        ,   MarketState const & _mktState
        ,   Event const & _randomEvent
        ,   GeneratedOrderData const & _existingOrder
        ,   GenerationTracer & _tracer
    );

    template<typename GenerationTracer>
    bool prepareNewRestingOrder(
            GeneratedMessage & _msg
        ,   MarketState const & _mktState
        ,   Event _event
        ,   GenerationTracer & _tracer
    );

    template<typename GenerationTracer>
    void generateCounterparty(
            GeneratedMessage & _msg
        ,   GenerationTracer & _tracer
    );

    template<typename GenerationTracer>
    RestingOrderAction generateRestingOrderAction(GenerationTracer & _tracer);

    template<typename GenerationTracer>
    void generatePrice(
            GeneratedMessage & _msg
        ,   MarketState const & _mktState
        ,   Event const & _event
        ,   GenerationTracer & _tracer
    );

    template<typename GenerationTracer>
    void generateQty(GeneratedMessage& msg,
                     Event event,
                     GenerationTracer& tracer);

    template<typename GenerationTracer>
    void assignGeneratedClOrderID(
            GeneratedMessage & _msg
        ,   GenerationTracer & _tracer
    );

    template<typename GenerationTracer>
    bool checkMktDepth(
          Event const & _event
      ,   const MarketState& _mktState
      ,   GenerationTracer & _tracer
    );

    template<typename GenerationTracer>
    bool isOppositeSideEmpty(
            Event const & _event
        ,   const MarketState& _mktState
        ,   GenerationTracer & _tracer
    );


    OrderGenerationContext & takeContext() noexcept;

    EventGenerator & takeEventGenerator() noexcept;

    CounterpartyGenerator & takePartyGenerator() noexcept;

    RestingOrderActionGenerator & takeRestingOrderActionGenerator() noexcept;

    PriceGenerator & takePriceGenerator() noexcept;

    QuantityGenerator & takeQtyGenerator() noexcept;


    std::shared_ptr<OrderGenerationContext> m_pContext;
    std::unique_ptr<EventGenerator> m_pEventGenerator;
    std::unique_ptr<CounterpartyGenerator> m_pCounterpartyGenerator;
    std::unique_ptr<RestingOrderActionGenerator> m_pRestingActionGenerator;
    std::unique_ptr<PriceGenerator> m_pPriceGenerator;
    std::unique_ptr<QuantityGenerator> m_pQtyGenerator;

    QuantityParamsSelector quantity_params_selector_;
};

} // namespace Simulator::Generator::Random

#endif // SIMULATOR_GENERATOR_SRC_ALGORITHM_ORDER_GENERATION_ALGORITHM_HPP_
