#include "ih/random/generators/quantity_generator.hpp"

#include <memory>
#include <utility>

#include "core/tools/numeric.hpp"

#include "ih/constants.hpp"
#include "ih/random/generators/value_generator.hpp"

namespace Simulator::Generator::Random {

QuantityGeneratorImpl::QuantityGeneratorImpl(
    std::shared_ptr<Random::ValueGenerator> _pRandomIntGenerator
) noexcept
    :   m_pRandomIntGenerator { std::move(_pRandomIntGenerator) }
{}


std::unique_ptr<QuantityGeneratorImpl> QuantityGeneratorImpl::create(
    std::shared_ptr<Random::ValueGenerator> _pRandomIntGenerator
)
{
    return std::make_unique<QuantityGeneratorImpl>(
        std::move(_pRandomIntGenerator)
    );
}


QuantityGenerator::GenerationOutput
QuantityGeneratorImpl::generateRandomQty(
    QuantityGenerationParams const & _params
)
{
    constexpr int randomMin = 0;

    double multiplier = _params.getMultiplier();
    if (simulator::core::equal(multiplier, 0.)) {
      multiplier = Constant::DefaultListingQtyMultiple;
    }
    auto const minQty = _params.getMinimalQuantity() / multiplier;
    auto const maxQty = _params.getMaximalQuantity() / multiplier;

    assert(minQty >= 0);
    assert(maxQty >= 0);
    assert(minQty <= maxQty);
    auto const randomMax = static_cast<int>(maxQty - minQty);
    int const randomNumber = m_pRandomIntGenerator->generateUniformValue(
            randomMin
        ,   randomMax
    );

    GenerationOutput output {};
    output.randomNumber = randomNumber;

    double const randomQty = (randomNumber + minQty) * multiplier;
    if (!simulator::core::equal(randomQty, 0.))
    {
        output.generatedQuantity = randomQty;
    }
    else
    {
        // randomNumber and minQty are equal to zero in case randomQty is zero.
        // We use multiplier in this case as it cannot not be zero by definition.
        output.generatedQuantity = multiplier;
    }

    return output;
}

} // namespace Simulator::Generator::Random
