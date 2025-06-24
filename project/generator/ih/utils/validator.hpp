#ifndef SIMULATOR_GENERATOR_SRC_UTILS_VALIDATOR_HPP_
#define SIMULATOR_GENERATOR_SRC_UTILS_VALIDATOR_HPP_

#include "data_layer/api/models/listing.hpp"
#include "data_layer/api/models/price_seed.hpp"

namespace Simulator::Generator {

class Validator
{
public:

    static bool isAcceptable(DataLayer::Listing const & _listing);

    static bool isAcceptableForRandomGeneration(
        DataLayer::Listing const & _listing
    );

    static bool isAcceptable(DataLayer::PriceSeed const & _priceSeed);
};

} // namespace Simulator::GeneratorImpl

#endif // SIMULATOR_GENERATOR_SRC_UTILS_VALIDATOR_HPP_
