#include "ih/random/generators/event_generator.hpp"

#include <cassert>
#include <memory>

#include "ih/random/generators/value_generator.hpp"
#include "ih/random/values/event.hpp"

namespace Simulator::Generator::Random {

EventGeneratorImpl::EventGeneratorImpl(
    std::shared_ptr<Random::ValueGenerator> _pRandomIntGenerator
) noexcept
    :   m_pRandomIntGenerator { std::move(_pRandomIntGenerator) }
{
    assert(m_pRandomIntGenerator);
}


std::unique_ptr<EventGeneratorImpl> EventGeneratorImpl::create(
    std::shared_ptr<Random::ValueGenerator> _pRandomIntGenerator
)
{
    return std::make_unique<EventGeneratorImpl>(
        std::move(_pRandomIntGenerator)
    );
}


Event::RandomInteger EventGeneratorImpl::generateChoiceInteger()
{
    constexpr auto min = Event::minRandomInteger();
    constexpr auto max = Event::maxRandomInteger();
    static_assert(min <= max);

    return m_pRandomIntGenerator->generateUniformValue(min, max);
}

} // namespace Simulator::Generator
