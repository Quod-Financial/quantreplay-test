#include "ih/random/algorithm/order_generation_algorithm.hpp"

#include <memory>

#include <fmt/format.h>

#include "core/tools/numeric.hpp"

#include "ih/constants.hpp"
#include "ih/context/order_generation_context.hpp"
#include "ih/random/algorithm/utils/attributes_setter.hpp"
#include "ih/random/algorithm/utils/max_mktdepth_selector.hpp"
#include "ih/random/algorithm/utils/price_params_selector.hpp"
#include "ih/random/generators/counterparty_generator.hpp"
#include "ih/random/generators/event_generator.hpp"
#include "ih/random/generators/price_generator.hpp"
#include "ih/random/generators/quantity_generator.hpp"
#include "ih/random/generators/resting_order_action_generator.hpp"
#include "ih/random/generators/value_generator.hpp"
#include "ih/random/generators/value_generator_impl.hpp"
#include "ih/random/values/event.hpp"
#include "ih/random/values/resting_order_action.hpp"
#include "ih/random/utils.hpp"
#include "ih/registry/registry_updater.hpp"
#include "ih/tracing/tracing.hpp"

namespace Simulator::Generator::Random {

OrderGenerationAlgorithm::OrderGenerationAlgorithm(
        std::shared_ptr<OrderGenerationContext> _pAlgorithmContext
    ,   std::unique_ptr<EventGenerator> _pEventGenerator
    ,   std::unique_ptr<CounterpartyGenerator> _pCounterpartyGenerator
    ,   std::unique_ptr<RestingOrderActionGenerator> _pRestingActionGenerator
    ,   std::unique_ptr<PriceGenerator> _pPriceGenerator
    ,   std::unique_ptr<QuantityGenerator> _pQtyGenerator
) noexcept
    :   m_pContext{ std::move(_pAlgorithmContext) }
    ,   m_pEventGenerator{ std::move(_pEventGenerator) }
    ,   m_pCounterpartyGenerator{ std::move(_pCounterpartyGenerator) }
    ,   m_pRestingActionGenerator { std::move(_pRestingActionGenerator) }
    ,   m_pPriceGenerator { std::move(_pPriceGenerator) }
    ,   m_pQtyGenerator { std::move(_pQtyGenerator) }
    ,   quantity_params_selector_{ takeContext().getInstrument() }
{
    assert(m_pContext);
    assert(m_pEventGenerator);
    assert(m_pCounterpartyGenerator);
    assert(m_pRestingActionGenerator);
    assert(m_pPriceGenerator);
    assert(m_pQtyGenerator);
}


std::unique_ptr<OrderGenerationAlgorithm> OrderGenerationAlgorithm::create(
    std::shared_ptr<OrderGenerationContext> _pAlgorithmContext
)
{
    assert(_pAlgorithmContext);
    auto const & targetVenue = _pAlgorithmContext->getVenue();

    auto pValueGenerator = ValueGeneratorImpl::create();
    auto pEventGenerator = EventGeneratorImpl::create(pValueGenerator);
    auto pPriceGenerator = PriceGeneratorImpl::create(pValueGenerator);
    auto pQtyGenerator = QuantityGeneratorImpl::create(pValueGenerator);
    auto pRestingActionGenerator = RestingOrderActionGeneratorImpl::create(
        pValueGenerator
    );
    auto pCounterpartyGenerator = CounterpartyGeneratorImpl::create(
        targetVenue.getRandomPartiesCount().value_or(
            Constant::DefaultVenueRandomPartiesCount),
        pValueGenerator);

    return create(
            std::move(_pAlgorithmContext)
        ,   std::move(pEventGenerator)
        ,   std::move(pCounterpartyGenerator)
        ,   std::move(pRestingActionGenerator)
        ,   std::move(pPriceGenerator)
        ,   std::move(pQtyGenerator)
    );
}


std::unique_ptr<OrderGenerationAlgorithm> OrderGenerationAlgorithm::create(
        std::shared_ptr<OrderGenerationContext> _pAlgorithmContext
    ,   std::unique_ptr<EventGenerator> _pEventGenerator
    ,   std::unique_ptr<CounterpartyGenerator> _pCounterpartyGenerator
    ,   std::unique_ptr<RestingOrderActionGenerator> _pRestingActionGenerator
    ,   std::unique_ptr<PriceGenerator> _pPriceGenerator
    ,   std::unique_ptr<QuantityGenerator> _pQtyGenerator
)
{
    using Pointer = std::unique_ptr<OrderGenerationAlgorithm>;
    return Pointer { new OrderGenerationAlgorithm {
            std::move(_pAlgorithmContext)
        ,   std::move(_pEventGenerator)
        ,   std::move(_pCounterpartyGenerator)
        ,   std::move(_pRestingActionGenerator)
        ,   std::move(_pPriceGenerator)
        ,   std::move(_pQtyGenerator)
    } };
}


bool OrderGenerationAlgorithm::generate(GeneratedMessage &_targetMessage)
{
    auto algorithm = [this, &_targetMessage](auto & _tracer) -> bool
    {
        return launchOn(_targetMessage, _tracer);
    };

    return Trace::trace(algorithm);
}


template<typename GenerationTracer>
bool OrderGenerationAlgorithm::launchOn(
        GeneratedMessage & _msg
    ,   GenerationTracer & _tracer
)
{
    Event event = takeEventGenerator().generateEvent(_tracer);
    if (event.isNoop())
    {
        return false;
    }

    generateCounterparty(_msg, _tracer);
    bool publish = false;
    MarketState currentState = takeContext().getCurrentMarketState();

    if (event.isAggressiveOrderEvent())
    {
        publish = fillAggressiveOrder(_msg, currentState, event, _tracer);
    }
    else
    {
        assert(event.isRestingOrderEvent());

        publish = fillRestingOrder(_msg, currentState, event, _tracer);
        if (publish)
        {
            OrderRegistryUpdater::update(takeContext().takeRegistry(), _msg);
        }
    }

    return publish;
}


template<typename GenerationTracer>
bool OrderGenerationAlgorithm::fillAggressiveOrder(
        GeneratedMessage & _msg
    ,   MarketState const & _mktState
    ,   Event const & _event
    ,   GenerationTracer & _tracer
)
{
    assert(_event.isAggressiveOrderEvent());
    assert(_event.isBuyEvent() || _event.isSellEvent());

    if (isOppositeSideEmpty(_event, _mktState, _tracer))
    {
        // Do not publish aggressive order in case an opposite side is empty
        return false;
    }

    assignGeneratedClOrderID(_msg, _tracer);

    AttributesSetter::set(_msg, _event.targetSide(), _tracer);

    AttributesSetter::set(_msg, MessageType::NewOrderSingle, _tracer);
    AttributesSetter::set(_msg, Constant::AggressiveOrderType, _tracer);
    AttributesSetter::set(_msg, Constant::AggressiveTimeInForce, _tracer);

    generatePrice(_msg, _mktState, _event, _tracer);
    generateQty(_msg, _event, _tracer);

    return true;
}


template<typename GenerationTracer>
bool OrderGenerationAlgorithm::fillRestingOrder(
        GeneratedMessage & _msg
    ,   MarketState const & _mktState
    ,   Event const & _event
    ,   GenerationTracer & _tracer
)
{
    assert(_event.isRestingOrderEvent());
    assert(_event.isBuyEvent() || _event.isSellEvent());

    assert(_msg.party_id.has_value());
    auto const & ownerId = *_msg.party_id;
    auto const & generatedOrdersRegistry = takeContext().takeRegistry();
    bool publish = true;

    auto const optPlacedOrder =
        generatedOrdersRegistry.findByOwner(ownerId.value());
    if (optPlacedOrder.has_value())
    {
        auto const & placedOrder = *optPlacedOrder;
        assert(placedOrder.getOwnerID() == ownerId);
        updateActiveRestingOrder(_msg, _mktState, _event, placedOrder, _tracer);
        assert(publish); // Updates on placed orders should always be published
    }
    else
    {
        publish = prepareNewRestingOrder(_msg, _mktState, _event, _tracer);
    }

    if (publish)
    {
        AttributesSetter::set(_msg, Constant::RestingOrderType, _tracer);
        AttributesSetter::set(_msg, Constant::RestingTimeInForce, _tracer);
    }

    return publish;
}


template<typename GenerationTracer>
void OrderGenerationAlgorithm::updateActiveRestingOrder(
        GeneratedMessage & _msg
    ,   MarketState const& _mktState
    ,   Event const & _event
    ,   GeneratedOrderData const & _existingOrder
    ,   GenerationTracer & _tracer
)
{
    using namespace Trace;

    auto const & ownerID = _existingOrder.getOwnerID();
    auto const & orderID = _existingOrder.getOrderID();

    auto step = makeStep(_tracer,
        "preparing action for the active resting order"
    );

    traceInput(step, makeValue("counterpartyId", ownerID.value()));
    traceInput(step, makeValue("orderID", orderID.value()));
    traceInput(step, makeValue("orderSide",
        fmt::format("{}", _existingOrder.getOrderSide())
    ));
    traceInput(step, makeValue("orderPrice",
        fmt::format("{}", _existingOrder.getOrderPx())
    ));
    traceInput(step, makeValue("orderQty",
        fmt::format("{}", _existingOrder.getOrderQty())
    ));

    _msg.client_order_id = _existingOrder.getOrderID();
    _msg.orig_client_order_id =
        simulator::OrigClientOrderId(_existingOrder.getOrderID().value());
    _msg.party_id = _existingOrder.getOwnerID();
    AttributesSetter::set(_msg, _existingOrder.getOrderSide());

    auto messageType = MessageType::OrderCancelReplaceRequest;
    RestingOrderAction const action = generateRestingOrderAction(_tracer);

    if (action.isCancellation())
    {
        messageType = MessageType::OrderCancelRequest;
    }
    else
    {
        _msg.order_price = _existingOrder.getOrderPx();
        _msg.quantity = _existingOrder.getOrderQty();

        if (action.isQuantityModification())
        {
            generateQty(_msg, _event, _tracer);
        }
        else
        {
            assert(action.isPriceModification());
            generatePrice(_msg, _mktState, _event, _tracer);
        }
    }

    AttributesSetter::set(_msg, messageType, _tracer);

    traceOutput(step,
                makeValue("messageType", fmt::format("{}", _msg.message_type)));
    if (!action.isCancellation())
    {
      traceOutput(step,
                  makeValue("price", fmt::format("{}", _msg.order_price)));
      traceOutput(step,
                  makeValue("quantity", fmt::format("{}", _msg.quantity)));
    }
    traceStep(std::move(step), _tracer);
}


template<typename GenerationTracer>
bool OrderGenerationAlgorithm::prepareNewRestingOrder(
        GeneratedMessage & _msg
    ,   MarketState const & _mktState
    ,   Event _event
    ,   GenerationTracer & _tracer
)
{
    assert(_event.isRestingOrderEvent());
    assert(_event.isBuyEvent() || _event.isSellEvent());

    if (!checkMktDepth(_event, _mktState, _tracer))
    {
        // We don't want to send a new order
        // when max configured mktdepth levels number is reached
        return false;
    }

    assignGeneratedClOrderID(_msg, _tracer);
    AttributesSetter::set(_msg, MessageType::NewOrderSingle, _tracer);
    AttributesSetter::set(_msg, _event.targetSide(), _tracer);

    generatePrice(_msg, _mktState, _event, _tracer);
    generateQty(_msg, _event, _tracer);

    return true;
}


template<typename GenerationTracer>
void OrderGenerationAlgorithm::generateCounterparty(
        GeneratedMessage & _msg
    ,   GenerationTracer & _tracer
)
{
    _msg.party_id = takePartyGenerator().generateCounterparty(_tracer);
}


template<typename GenerationTracer>
RestingOrderAction
OrderGenerationAlgorithm::generateRestingOrderAction(GenerationTracer & _tracer)
{
    return takeRestingOrderActionGenerator().generateAction(_tracer);
}


template<typename GenerationTracer>
void OrderGenerationAlgorithm::generatePrice(
        GeneratedMessage & _msg
    ,   MarketState const & _mktState
    ,   Event const & _event
    ,   GenerationTracer & _tracer
)
{
    auto & context = takeContext();
    auto const pxParams = PriceParamsSelector::select(
            context.getInstrument()
        ,   _tracer
    );

    _msg.order_price = std::make_optional<simulator::OrderPrice>(takePriceGenerator().generatePrice(
            pxParams
        ,   _mktState
        ,   context.getPriceSeed()
        ,   _event
        ,   _tracer
    ));
}


template<typename GenerationTracer>
void OrderGenerationAlgorithm::generateQty(
        GeneratedMessage& msg
    ,   Event event
    ,   GenerationTracer& tracer
)
{
    const auto params = quantity_params_selector_.select(
            msg.order_price->value()
        ,   event
        ,   tracer
    );
    msg.quantity = std::make_optional<simulator::Quantity>(
        takeQtyGenerator().generateQty(params, tracer));
}


template<typename GenerationTracer>
void OrderGenerationAlgorithm::assignGeneratedClOrderID(
        GeneratedMessage & _msg
    ,   GenerationTracer & _tracer
)
{
    using namespace Trace;
    auto step = makeStep(_tracer, "generating ClOrdID");

    std::string clOrdID = takeContext().getSyntheticIdentifier();
    traceOutput(step, makeValue("clOrdID", clOrdID));

    _msg.client_order_id = simulator::ClientOrderId{std::move(clOrdID)};
    traceStep(std::move(step), _tracer);
}


template<typename GenerationTracer>
bool OrderGenerationAlgorithm::checkMktDepth(
        Event const & _event
    ,   MarketState const & _mktState
    ,   GenerationTracer & _tracer
)
{
    using namespace Trace;
    assert(_event.isBuyEvent() || _event.isSellEvent());

    auto step = makeStep(_tracer, "checking current market depth state");
    auto & context = takeContext();

    auto const optMaxMktDepth = MaxMktDepthSelector::select(
            context.getInstrument()
        ,   context.getVenue().getRandomPartiesCount().value_or(Constant::DefaultVenueRandomPartiesCount)
        ,   _tracer
    );

    bool continueGeneration = false;
    if (optMaxMktDepth.has_value())
    {
        auto const maxMktDepth = *optMaxMktDepth;
        const auto currMktDepth = _event.targetSide() == simulator::Side::Option::Buy
            ?   _mktState.bidDepthLevels.value_or(0)
            :   _mktState.offerDepthLevels.value_or(0);

        traceInput(step, makeValue("maximalMarketDepth", maxMktDepth));
        traceInput(step, makeValue("currentMarketDepth", currMktDepth));

        continueGeneration = currMktDepth < maxMktDepth;
        traceOutput(step, makeValue("continueGeneration", continueGeneration));
    }
    else
    {
        traceInput(step, makeValue(
                "maximalMarketDepth"
             ,  std::string_view { "none" }
        ));

        continueGeneration = true;
        traceOutput(step, makeCommentedValue(
                "continueGeneration"
            ,   continueGeneration
            ,   "the instrument does not have a maximal depth levels configured"
        ));
    }

    traceStep(std::move(step), _tracer);
    return continueGeneration;
}


template<typename GenerationTracer>
bool OrderGenerationAlgorithm::isOppositeSideEmpty(
        Event const & _event
    ,   const MarketState& _mktState
    ,   GenerationTracer & _tracer
)
{
    using namespace Trace;
    constexpr double NullPrice = 0.0;

    assert(_event.isBuyEvent() || _event.isSellEvent());

    auto step = makeStep(_tracer, "checking if opposite side has prices");

    auto const side = _event.targetSide();
    auto const opSide = Utils::opposite(side);

    traceInput(step, makeValue("currentSide", fmt::format("{}", side)));
    traceInput(step, makeValue("oppositeSide", fmt::format("{}", opSide)));

    auto const oppositePx = Utils::select_price(_mktState, opSide);
    traceInput(step,
               makeValue("oppositeBestPx", oppositePx.value_or(NullPrice)));

    bool const isOppositeEmpty = !oppositePx.has_value() ||
                                 simulator::core::equal(*oppositePx, NullPrice);
    traceOutput(step, makeValue("isOppositeSideEmpty", isOppositeEmpty));

    traceStep(std::move(step), _tracer);
    return isOppositeEmpty;
}


OrderGenerationContext & OrderGenerationAlgorithm::takeContext() noexcept
{
    return *m_pContext;
}


EventGenerator & OrderGenerationAlgorithm::takeEventGenerator() noexcept
{
    return *m_pEventGenerator;
}


CounterpartyGenerator & OrderGenerationAlgorithm::takePartyGenerator() noexcept
{
    return *m_pCounterpartyGenerator;
}


RestingOrderActionGenerator &
OrderGenerationAlgorithm::takeRestingOrderActionGenerator() noexcept
{
    return *m_pRestingActionGenerator;
}


PriceGenerator & OrderGenerationAlgorithm::takePriceGenerator() noexcept
{
    return *m_pPriceGenerator;
}


QuantityGenerator & OrderGenerationAlgorithm::takeQtyGenerator() noexcept
{
    return *m_pQtyGenerator;
}

} // namespace Simulator::Generator::Random
