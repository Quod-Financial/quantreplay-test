#ifndef SIMULATOR_GENERATOR_TESTS_MOCKS_PRICE_GENERATOR_HPP_
#define SIMULATOR_GENERATOR_TESTS_MOCKS_PRICE_GENERATOR_HPP_

#include <gmock/gmock.h>
#include <utility>

#include "data_layer/api/models/price_seed.hpp"

#include "ih/random/generators/price_generator.hpp"
#include "ih/random/values/event.hpp"

namespace Simulator::Generator::Mock {

class PriceGenerator
    :   public Generator::Random::PriceGenerator
{
public:

    using GenerationOutput = std::pair<double, GenerationDetails>;

    static auto makeOutput(double _price) -> GenerationOutput
    {
        return std::make_pair(_price, GenerationDetails{});
    }

    MOCK_METHOD(
        GenerationOutput, generatePx,
        (Random::PriceGenerationParams const &,
         Generator::MarketState const &,
         DataLayer::PriceSeed const &,
         Random::Event),
        (override)
    );
};

} // namespace Simulator::Generator::Mock

#endif // SIMULATOR_GENERATOR_TESTS_MOCKS_PRICE_GENERATOR_HPP_
