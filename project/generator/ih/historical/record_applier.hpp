#ifndef SIMULATOR_GENERATOR_IH_HISTORICAL_RECORD_APPLIER_HPP_
#define SIMULATOR_GENERATOR_IH_HISTORICAL_RECORD_APPLIER_HPP_

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "ih/adaptation/generated_message.hpp"
#include "ih/context/instrument_context.hpp"
#include "ih/historical/data/record.hpp"
#include "ih/registry/generated_orders_registry.hpp"

namespace Simulator::Generator::Historical {

class RecordApplier {
 public:
  struct Order;
  class RecordChecker;

  using ContextPointer = std::shared_ptr<OrderInstrumentContext>;

  RecordApplier() = delete;

  static auto apply(Historical::Record record, ContextPointer context) noexcept
      -> std::vector<GeneratedMessage>;

 private:
  explicit RecordApplier(ContextPointer context) noexcept;

  auto process(Historical::Record record) -> void;

  auto process(const Historical::Level& level, std::uint64_t level_idx) -> bool;

  auto place_bid(const Historical::Level& level) -> bool;

  auto place_offer(const Historical::Level& level) -> bool;

  auto place(RecordApplier::Order order) -> void;

  auto cancel(const GeneratedOrdersRegistry::Predicate& cancel_criteria)
      -> void;

  auto cancel_bid_part() -> void;

  auto cancel_offer_part() -> void;

  auto cancel_other_parties(const Historical::Record& record) -> void;

  auto next_party_id() -> std::string;

  std::vector<GeneratedMessage> request_messages_;

  ContextPointer context_;

  std::uint64_t party_id_counter_{0};
};

struct RecordApplier::Order {
  Order() = delete;

  Order(double _price,
        simulator::Side _side,
        double _quantity,
        std::string _counterparty_id) noexcept;

  std::string counterparty_id;
  double price;
  double quantity;
  simulator::Side side;
};

class RecordApplier::RecordChecker {
 public:
  static auto is_processable(const Historical::Level& level) noexcept -> bool;

  static auto has_bid_part(const Historical::Level& level) noexcept -> bool;

  static auto has_offer_part(const Historical::Level& level) noexcept -> bool;
};

}  // namespace Simulator::Generator::Historical

#endif  // SIMULATOR_GENERATOR_IH_HISTORICAL_RECORD_APPLIER_HPP_
