#ifndef SIMULATOR_GENERATOR_SRC_REGISTRY_REGISTRY_UPDATER_HPP_
#define SIMULATOR_GENERATOR_SRC_REGISTRY_REGISTRY_UPDATER_HPP_

#include <functional>

#include "ih/adaptation/generated_message.hpp"
#include "ih/registry/generated_orders_registry.hpp"

namespace Simulator::Generator {

class OrderRegistryUpdater {
 public:
  explicit OrderRegistryUpdater(GeneratedOrdersRegistry& _registry) noexcept;

  static void update(GeneratedOrdersRegistry& _registry,
                     GeneratedMessage const& _message);

  void update(GeneratedMessage const& _message);

 private:
  void handleNewOrder(GeneratedMessage const& _message);

  void handleModification(GeneratedMessage const& _message);

  void handleCancellation(GeneratedMessage const& _message);

  void handleExecution(GeneratedMessage const& _message);

  GeneratedOrdersRegistry& registry() noexcept;

  [[nodiscard]]
  static bool containsRestingOrder(GeneratedMessage const& _message);

  static auto validate_new_order(const GeneratedMessage& message) -> void;

  static auto validate_modification(const GeneratedMessage& message) -> void;

  static auto validate_cancellation(const GeneratedMessage& message) -> void;

  static auto validate_execution(const GeneratedMessage& message) -> void;

  std::reference_wrapper<GeneratedOrdersRegistry> m_registryRef;
};

}  // namespace Simulator::Generator

#endif  // SIMULATOR_GENERATOR_SRC_REGISTRY_REGISTRY_UPDATER_HPP_
