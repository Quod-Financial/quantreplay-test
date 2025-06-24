#ifndef SIMULATOR_GENERATOR_SRC_GENERATOR_IMPL_HPP_
#define SIMULATOR_GENERATOR_SRC_GENERATOR_IMPL_HPP_

#include <memory>
#include <unordered_map>
#include <vector>

#include "data_layer/api/database/context.hpp"
#include "ih/context/generation_manager.hpp"
#include "ih/context/instrument_context.hpp"
#include "ih/factory/executable_factory.hpp"
#include "ih/random/instrument_generator.hpp"
#include "ih/utils/executor.hpp"

namespace Simulator::Generator {

class GeneratorImpl final {
  public:
    GeneratorImpl() = delete;

    GeneratorImpl(
        DataLayer::Venue const& _targetVenue,
        DataLayer::Database::Context _databaseContext);

    GeneratorImpl(GeneratorImpl const&) = delete;
    auto operator=(GeneratorImpl const&) -> GeneratorImpl& = delete;

    GeneratorImpl(GeneratorImpl&&) = delete;
    auto operator=(GeneratorImpl&&) -> GeneratorImpl& = delete;

    ~GeneratorImpl() noexcept;

    auto status() -> bool;

    auto suspend() -> void;

    auto resume() -> void;

    auto start() -> void;

    auto terminate() noexcept -> void;

    auto process_reply(std::uint64_t instrument_id,
                       const GeneratedMessage& reply) -> void;

   private:
    auto initializeInstruments() -> void;

    auto initializeInstrument(DataLayer::Listing const& _listing) -> void;

    auto initializeRandomGenerationExecutors() -> void;

    auto initializeHistoricalExecutor() -> void;

    auto terminateGenerator() noexcept -> void;


    std::vector<std::unique_ptr<Executor>> mListingsRandomGenerators;
    std::vector<std::shared_ptr<OrderInstrumentContext>> mOrderListingsContexts;
    std::unordered_map<std::uint64_t, std::shared_ptr<OrderInstrumentContext>> mContextLookup;

    DataLayer::Database::Context mDatabaseContext;

    std::shared_ptr<GenerationManager> mGenerationManager;

    std::unique_ptr<Executor> mHistoricalReplier;
    std::unique_ptr<InstrumentRandomGeneratorFactory> mRndExecutorFactory;

    std::unique_ptr<InstrumentHistoricalReplierFactory> mHistExecutorFactory;

    bool m_wasTerminated{false};
};

} // namespace Simulator::Generator

#endif // SIMULATOR_GENERATOR_SRC_GENERATOR_IMPL_HPP_
