#include "ih/random/values/price_generation_params.hpp"

namespace Simulator::Generator::Random {

void PriceGenerationParams::Builder::setPriceTickRange(
    std::uint32_t _pxTickRange
) noexcept
{
    m_specifiedParams.m_priceTickRange = _pxTickRange;
}


void PriceGenerationParams::Builder::setPriceTickSize(
    double _pxTickSize
) noexcept
{
    m_specifiedParams.m_priceTickSize = _pxTickSize;
}


void PriceGenerationParams::Builder::setPriceSpread(double _pxSpread) noexcept
{
    m_specifiedParams.m_priceSpread = _pxSpread;
}


PriceGenerationParams::PriceGenerationParams(Builder _builder) noexcept
    :   m_params { _builder.m_specifiedParams }
{}


std::uint32_t PriceGenerationParams::getPriceTickRange() const noexcept
{
    return m_params.m_priceTickRange;
}


double PriceGenerationParams::getPriceTickSize() const noexcept
{
    return m_params.m_priceTickSize;
}


double PriceGenerationParams::getPriceSpread() const noexcept
{
    return m_params.m_priceSpread;
}

} // namespace Simulator::Generator::Random
