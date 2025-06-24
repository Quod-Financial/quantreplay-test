#ifndef SIMULATOR_GENERATOR_IH_ALGORITHM_GENERATION_ALGORITHM_HPP_
#define SIMULATOR_GENERATOR_IH_ALGORITHM_GENERATION_ALGORITHM_HPP_

#include "ih/adaptation/generated_message.hpp"

namespace Simulator::Generator::Random {

class GenerationAlgorithm
{
public:

    virtual ~GenerationAlgorithm() = default;

    // Fills the message with a generated data.
    // `_msg` is considered as finished and valid when `generate` returns `true`
    // `_msg` content has to be ignored in case `false` is returned
    virtual bool generate(GeneratedMessage & _msg) = 0;
};

} // namespace Simulator::Generator::Random

#endif // SIMULATOR_GENERATOR_IH_ALGORITHM_GENERATION_ALGORITHM_HPP_
