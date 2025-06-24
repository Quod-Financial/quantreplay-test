#include "ih/random/generators/price_generator.hpp"

#include <cassert>
#include <cmath>
#include <optional>
#include <utility>

#include "core/tools/numeric.hpp"

#include "ih/constants.hpp"
#include "ih/random/generators/value_generator.hpp"
#include "ih/random/values/event.hpp"
#include "ih/random/values/price_generation_params.hpp"
#include "ih/random/utils.hpp"

namespace Simulator::Generator::Random {
namespace {

double divideLogs(double _base, double _val)
{
  return std::log(_val) / std::log(_base);
}

double getGeometricSeriesSum(std::uint32_t _count,
                             double _koef,
                             double _firstValue)
{
  return (_firstValue * (1 - std::pow(_koef, _count))) / (1 - _koef);
}

} // namespace
} // namespace Simulator::Generator::Random


namespace Simulator::Generator::Random {

PriceGeneratorImpl::PriceGeneratorImpl(
    std::shared_ptr<Random::ValueGenerator> _pRandomIntGenerator
) noexcept
    :   m_pRandomIntGenerator { std::move(_pRandomIntGenerator) }
{
    assert(m_pRandomIntGenerator);
}


std::unique_ptr<PriceGeneratorImpl> PriceGeneratorImpl::create(
    std::shared_ptr<Random::ValueGenerator> _pRandomIntGenerator
)
{
    return std::make_unique<PriceGeneratorImpl>(std::move(_pRandomIntGenerator));
}


std::pair<double, PriceGenerator::GenerationDetails>
PriceGeneratorImpl::generatePx(
        PriceGenerationParams const & _params
    ,   Generator::MarketState const & _actualPrices
    ,   DataLayer::PriceSeed const & _configuredPrices
    ,   Random::Event _event
)
{
    assert(_event.isBuyEvent() || _event.isSellEvent());
    assert(_event.isRestingOrderEvent() || _event.isAggressiveOrderEvent());

    GenerationDetails details {};

    auto const optBasePrice = resolveBasePrice(_actualPrices, _params, _event);
    if (!optBasePrice.has_value())
    {
        double const configuredPx = resolveConfiguredPrice(
                _configuredPrices
            ,   _event
        );
        details.wasConfiguredPxUsed = true;
        return std::make_pair(configuredPx, details);
    }

    auto const basePx = *optBasePrice;
    details.basePx = basePx;

    // Add a random price increment to a base price for
    // aggressive-buy and resting-sell orders
    //
    // Subtract a random price increment from a base price for
    // aggressive-sell and resting-buy orders
    bool const incrementPx = _event.isBuyEvent() ^ _event.isRestingOrderEvent();
    details.wasBasePxIncrementAdded = incrementPx;

    double const tick = generateTick(_params, details);
    details.basePxIncrement = tick;

    if (tick >= basePx)
    {
        return std::make_pair(tick, details);
    }

    double const price = incrementPx
         ?   basePx + tick
         :   basePx - tick;

    return std::make_pair(price, details);
}


double PriceGeneratorImpl::generateTick(
        PriceGenerationParams const & _params
    ,   GenerationDetails & _details
)
{
    constexpr int randomMin = 0;
    auto const tickRange = _params.getPriceTickRange();

    auto const geometricSum = static_cast<int>(getGeometricSeriesSum(
            tickRange
        ,   Constant::PriceGenerationCoefficient
        ,   Constant::PriceGenerationCoefficient
    ));
    _details.geometricSum = geometricSum;

    int const randValue = m_pRandomIntGenerator->generateUniformValue(
            randomMin
        ,   geometricSum - 1
    );
    _details.randomNumber = randValue;

    constexpr auto logBase = Constant::PriceGenerationCoefficient;
    auto const logNum = (randValue * (logBase - 1)) / logBase + 1;
    double const pxDeviation = std::ceil(divideLogs(logBase, logNum));

    double const randomTick = tickRange - pxDeviation;
    double const scaledRandomTick = randomTick * _params.getPriceTickSize();

    _details.randomTick = scaledRandomTick;
    return scaledRandomTick;
}


std::optional<double> PriceGeneratorImpl::resolveBasePrice(
        Generator::MarketState const & _actualPrices
    ,   PriceGenerationParams const & _generationParams
    ,   Random::Event _event
)
{
    assert(_event.isAggressiveOrderEvent() || _event.isRestingOrderEvent());
    assert(_event.isBuyEvent() || _event.isSellEvent());
    auto const priceSide = _event.targetSide();

    // Try resolve a base price as the best price for an opposite side
    auto resolvedPrice = Utils::select_price(_actualPrices, Utils::opposite(priceSide));
    if (!resolvedPrice || simulator::core::equal(*resolvedPrice, 0.))
    {
        // In case an opposite side is empty (does not have the best price)
        // try to resolve base price as the best price for a current side
        // and return it immediately.
        resolvedPrice = Utils::select_price(_actualPrices, priceSide);
        return simulator::core::equal(*resolvedPrice, 0.)
           ?   std::nullopt
           :   resolvedPrice;
    }

    if (_event.isRestingOrderEvent())
    {
        // Subtract spread from the resolved price if generating a buy price OR
        // add spread to the resolved price if generating a sell price
        resolvedPrice = _event.isBuyEvent()
            ?   *resolvedPrice - _generationParams.getPriceSpread()
            :   *resolvedPrice + _generationParams.getPriceSpread();
    }

    return resolvedPrice;
}


double PriceGeneratorImpl::resolveConfiguredPrice(
        DataLayer::PriceSeed const & _configuredPx
    ,   Random::Event _event
)
{
    assert(_event.isBuyEvent() || _event.isSellEvent());
    auto const priceSide = _event.targetSide();

    return priceSide == simulator::Side::Option::Buy
        ?   *_configuredPx.getBidPrice()
        :   *_configuredPx.getOfferPrice();
}

} // namespace Simulator::Generator::Random
