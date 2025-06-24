#include "ih/random/generators/resting_order_action_generator.hpp"

#include <cassert>
#include <utility>

#include "ih/random/generators/value_generator.hpp"
#include "ih/random/values/resting_order_action.hpp"

namespace Simulator::Generator::Random {

RestingOrderActionGeneratorImpl::RestingOrderActionGeneratorImpl(
    std::shared_ptr<Random::ValueGenerator> _pRandomIntGenerator
) noexcept
    :   m_pRandomIntGenerator { std::move(_pRandomIntGenerator) }
{
    assert(m_pRandomIntGenerator);
}


std::unique_ptr<RestingOrderActionGeneratorImpl>
RestingOrderActionGeneratorImpl::create(
    std::shared_ptr<Random::ValueGenerator> _pRandomIntGenerator
)
{
    return std::make_unique<RestingOrderActionGeneratorImpl>(
        std::move(_pRandomIntGenerator)
    );
}


RestingOrderAction::RandomInteger
RestingOrderActionGeneratorImpl::generateInteger()
{
    constexpr auto min = RestingOrderAction::minRandomInteger();
    constexpr auto max = RestingOrderAction::maxRandomInteger();
    static_assert(min <= max);

    return m_pRandomIntGenerator->generateUniformValue(min, max);
}

} // namespace Simulator::Generator::Random
