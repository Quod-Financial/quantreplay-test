#include "ih/random/generators/counterparty_generator.hpp"

#include <utility>

#include "ih/random/generators/value_generator.hpp"

namespace Simulator::Generator::Random {

CounterpartyGeneratorImpl::CounterpartyGeneratorImpl(
        std::uint32_t _partiesCount
    ,   std::shared_ptr<Random::ValueGenerator> _pRandomIntGenerator
) noexcept
    :   m_pRandomIntGenerator { std::move(_pRandomIntGenerator) }
    ,   m_maxPartyNumber { _partiesCount }
{
    assert(m_pRandomIntGenerator);
}


std::unique_ptr<CounterpartyGeneratorImpl> CounterpartyGeneratorImpl::create(
        std::uint32_t _partiesCount
    ,   std::shared_ptr<Random::ValueGenerator> _pRandomIntGenerator
)
{
    return std::make_unique<CounterpartyGeneratorImpl>(
            _partiesCount
        ,   std::move(_pRandomIntGenerator)
    );
}


std::uint32_t CounterpartyGeneratorImpl::generateCounterpartyNumber()
{
    constexpr std::uint32_t minCounterpartyNumber = 1;
    return m_pRandomIntGenerator->generateUniformValue(
            minCounterpartyNumber
        ,   m_maxPartyNumber
    );
}

} // namespace Simulator::Generator::Random
