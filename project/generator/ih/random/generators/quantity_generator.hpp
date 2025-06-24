#ifndef SIMULATOR_GENERATOR_SRC_GENERATORS_QUANTITY_GENERATOR_HPP_
#define SIMULATOR_GENERATOR_SRC_GENERATORS_QUANTITY_GENERATOR_HPP_

#include <memory>

#include "ih/random/generators/value_generator.hpp"
#include "ih/random/values/quantity_generation_params.hpp"
#include "ih/tracing/tracing.hpp"

namespace Simulator::Generator::Random {

class QuantityGenerator
{
protected:

    struct GenerationOutput
    {
        double generatedQuantity { 0. };
        int randomNumber { 0 };
    };

public:

    virtual ~QuantityGenerator() = default;

    template<typename Tracer = Trace::NullTracer>
    double generateQty(
            QuantityGenerationParams const & _params
        ,   Tracer && _tracer = Tracer {}
    );

private:

    virtual GenerationOutput generateRandomQty(
        QuantityGenerationParams const & _params
    ) = 0;
};


template<typename Tracer>
double QuantityGenerator::generateQty(
        QuantityGenerationParams const & _params
    ,   Tracer &&_tracer
)
{
    using namespace Trace;

    auto step = makeStep(_tracer, "generating random quantity");

    traceInput(step, makeValue("quantityMultiplier", _params.getMultiplier()));
    traceInput(step,makeValue("minimalQuantity", _params.getMinimalQuantity()));
    traceInput(step,makeValue("maximalQuantity", _params.getMaximalQuantity()));

    auto generationOut = generateRandomQty(_params);
    traceOutput(step, makeValue("randomValue", generationOut.randomNumber));
    traceOutput(step,makeValue("generatedQty", generationOut.generatedQuantity));

    traceStep(std::move(step), _tracer);
    return generationOut.generatedQuantity;
}

} // namespace Simulator::Generator::Random


namespace Simulator::Generator::Random {


class QuantityGeneratorImpl final
    :   public Random::QuantityGenerator
{
public:

    QuantityGeneratorImpl() = delete;

    explicit QuantityGeneratorImpl(
        std::shared_ptr<Random::ValueGenerator> _pRandomIntGenerator
    ) noexcept;

    static std::unique_ptr<QuantityGeneratorImpl> create(
        std::shared_ptr<Random::ValueGenerator> _pRandomIntGenerator
    );

private:

    GenerationOutput generateRandomQty(
        QuantityGenerationParams const & _params
    ) override;


    std::shared_ptr<Random::ValueGenerator> m_pRandomIntGenerator;
};

} // namespace Simulator::Generator::Random

#endif // SIMULATOR_GENERATOR_SRC_GENERATORS_QUANTITY_GENERATOR_HPP_
