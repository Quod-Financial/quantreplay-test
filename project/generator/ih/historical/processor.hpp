#ifndef SIMULATOR_GENERATOR_IH_HISTORICAL_PROCESSOR_HPP_
#define SIMULATOR_GENERATOR_IH_HISTORICAL_PROCESSOR_HPP_

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "ih/context/instrument_context.hpp"
#include "ih/historical/data/record.hpp"
#include "ih/registry/generated_order_data.hpp"
#include "ih/registry/generated_orders_registry.hpp"

namespace Simulator::Generator::Historical {

class Processor {
  public:
    Processor() = default;

    Processor(Processor const&) = default;
    auto operator=(Processor const&) -> Processor& = default;

    Processor(Processor&&) = default;
    auto operator=(Processor&&) -> Processor& = default;

    virtual ~Processor() = default;

    virtual auto process(Historical::Action _action) -> void = 0;
};

class ActionProcessor : public Historical::Processor {
  public:
    using ContextPointer = std::shared_ptr<OrderInstrumentContext>;
    using Contexts = std::vector<ContextPointer>;

    class ContextsRegistry {
        using BySymbolRegistry = std::unordered_map<
            std::string_view,
            std::shared_ptr<OrderInstrumentContext>>;

      public:
        ContextsRegistry() = delete;
        explicit ContextsRegistry(Contexts const& _availableContexts) noexcept;

        auto resolveContext(Historical::Record const& _record) const
            -> ContextPointer;

      private:
        BySymbolRegistry mRegistry;
    };


    ActionProcessor() = delete;

    explicit ActionProcessor(
        Contexts const& _availableContexts
    );


    auto process(Historical::Action _action) -> void override;

  private:
    auto process(Historical::Record _record) const -> void;
    ContextsRegistry mCtxRegistry;
};

} // namespace Simulator::Generator::Historical

#endif // SIMULATOR_GENERATOR_IH_HISTORICAL_PROCESSOR_HPP_
