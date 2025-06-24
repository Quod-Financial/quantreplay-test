#ifndef SIMULATOR_GENERATOR_SRC_GENERATORS_COUNTERPARTY_GENERATOR_HPP_
#define SIMULATOR_GENERATOR_SRC_GENERATORS_COUNTERPARTY_GENERATOR_HPP_

#include <memory>

#include <fmt/format.h>

#include "ih/constants.hpp"
#include "ih/random/generators/value_generator.hpp"
#include "ih/tracing/null_tracer.hpp"
#include "ih/tracing/tracing.hpp"

namespace Simulator::Generator::Random {

class CounterpartyGenerator
{
public:

    virtual ~CounterpartyGenerator() = default;

    template<typename GenerationTracer = Trace::NullTracer>
    simulator::PartyId generateCounterparty(
        GenerationTracer && _tracer = GenerationTracer {}
    );

private:

    virtual unsigned int generateCounterpartyNumber() = 0;
};


template<typename GenerationTracer>
inline simulator::PartyId
CounterpartyGenerator::generateCounterparty(GenerationTracer && _tracer)
{
    using namespace Trace;
    auto step = makeStep(_tracer, "generating order counterparty");

    auto const cpNumber = generateCounterpartyNumber();
    traceInput(step, makeValue("randomCounterpartyNumber", cpNumber));

    std::string cpIdentifier = fmt::format(
            "{}{}"
        ,   Constant::CounterpartyIDPrefix
        ,   cpNumber
    );
    traceOutput(step, makeValue("partyId", cpIdentifier));

    traceStep(std::move(step), _tracer);
    return simulator::PartyId{cpIdentifier};
}

} // namespace Simulator::Generator::Random


namespace Simulator::Generator::Random {

class CounterpartyGeneratorImpl final
    :   public Random::CounterpartyGenerator
{
public:

    explicit CounterpartyGeneratorImpl(
            unsigned int _partiesCount
        ,   std::shared_ptr<Random::ValueGenerator> _pRandomIntGenerator
    ) noexcept;

    static std::unique_ptr<CounterpartyGeneratorImpl> create(
            unsigned int _partiesCount
        ,   std::shared_ptr<Random::ValueGenerator> _pRandomIntGenerator
    );

private:

    std::uint32_t generateCounterpartyNumber() override;


    std::shared_ptr<Random::ValueGenerator> m_pRandomIntGenerator;
    std::uint32_t m_maxPartyNumber { 0 };
};

} // namespace Simulator::Generator::Random

#endif // SIMULATOR_GENERATOR_SRC_GENERATORS_COUNTERPARTY_GENERATOR_HPP_
