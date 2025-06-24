#ifndef SIMULATOR_GENERATOR_SRC_INSTRUMENT_GENERATOR_HPP_
#define SIMULATOR_GENERATOR_SRC_INSTRUMENT_GENERATOR_HPP_

#include <chrono>
#include <memory>

#include "ih/adaptation/generated_message.hpp"
#include "ih/context/instrument_context.hpp"
#include "ih/random/algorithm/generation_algorithm.hpp"
#include "ih/utils/executable.hpp"

namespace Simulator::Generator::Random {

class OrderGenerator
    :   public Executable
{
public:

    OrderGenerator() = delete;

    OrderGenerator(
            std::shared_ptr<OrderInstrumentContext> _pInstrumentContext
        ,   std::unique_ptr<GenerationAlgorithm> _pRandomGenerationAlgorithm
    );

    // An empty implementation.
    // InstrumentGenerator has nothing to prepare before starting generation
    void prepare() noexcept override;

    void execute() override;

    bool finished() const noexcept override;

    [[nodiscard]]
    std::chrono::microseconds nextExecTimeout() const override;

private:

    void initExecutionRate();


    GeneratedMessage m_generatedMessage;

    std::shared_ptr<OrderInstrumentContext> m_pInstrumentContext;

    std::unique_ptr<GenerationAlgorithm> m_pGenerationAlgorithm;

    std::chrono::microseconds m_executionRate { 0 };
};

} // namespace Simulator::Generator::Random

#endif // SIMULATOR_GENERATOR_SRC_INSTRUMENT_GENERATOR_HPP_
