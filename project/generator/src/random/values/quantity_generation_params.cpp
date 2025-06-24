#include "ih/random/values/quantity_generation_params.hpp"

#include <stdexcept>

#include <fmt/format.h>

namespace Simulator::Generator::Random {

void QuantityGenerationParams::Builder::setQuantityMultiplier(
    double _multiplier
) noexcept
{
    m_specifiedParams.m_qtyMultiplier = _multiplier;
}


void QuantityGenerationParams::Builder::setMinimalQuantity(
    double _minQuantity
) noexcept
{
    m_specifiedParams.m_minQty = _minQuantity;
}


void QuantityGenerationParams::Builder::setMaximalQuantity(
    double _maxQuantity
) noexcept
{
    m_specifiedParams.m_maxQty = _maxQuantity;
}


QuantityGenerationParams::QuantityGenerationParams(Builder _builder)
    :   m_params { _builder.m_specifiedParams }
{
    validate(m_params);
}


double QuantityGenerationParams::getMultiplier() const noexcept
{
    return m_params.m_qtyMultiplier;
}


double QuantityGenerationParams::getMinimalQuantity() const noexcept
{
    return m_params.m_minQty;
}


double QuantityGenerationParams::getMaximalQuantity() const noexcept
{
    return m_params.m_maxQty;
}


void QuantityGenerationParams::validate(Params const & _qtyGenerationParams)
{
    constexpr double zero = 0.0;

    if (_qtyGenerationParams.m_minQty < zero)
    {
        throw std::logic_error { fmt::format(
            "Invalid minimal quantity ({}) is provided for "
            "QuantityGenerationParams: random quantity must be greater "
            "or equal to zero", _qtyGenerationParams.m_minQty
        ) };
    }

    if (_qtyGenerationParams.m_maxQty < zero)
    {
        throw std::logic_error { fmt::format(
            "Invalid maximal quantity ({}) is provided for "
            "QuantityGenerationParams: random quantity must be greater "
            "or equal to zero", _qtyGenerationParams.m_minQty
        ) };
    }

    if ( _qtyGenerationParams.m_maxQty < _qtyGenerationParams.m_minQty)
    {
        throw std::logic_error { fmt::format(
            "Invalid minimal ({}) and maximal ({}) random quantities "
            "are provided for QuantityGenerationParams: maximal random quantity "
            "must be greater or equal to minimal random quantity",
            _qtyGenerationParams.m_minQty, _qtyGenerationParams.m_maxQty
        ) };
    }
}

} // namespace Generator::Simulator::Random
