#include "ih/historical/record_applier.hpp"

#include <algorithm>
#include <cassert>
#include <exception>
#include <optional>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "ih/constants.hpp"
#include "ih/historical/data/record.hpp"
#include "ih/registry/generated_order_data.hpp"
#include "ih/registry/generated_orders_registry.hpp"
#include "ih/registry/registry_updater.hpp"
#include "ih/utils/request_builder.hpp"
#include "log/logging.hpp"

namespace Simulator::Generator::Historical {

auto RecordApplier::apply(Historical::Record record,
                          RecordApplier::ContextPointer context) noexcept
    -> std::vector<GeneratedMessage> {
  bool failed = false;
  std::vector<GeneratedMessage> replies;

  const std::uint64_t row_number = record.source_row();
  const std::optional<std::string> source_name = record.source_name();
  const std::optional<std::string> source_conn = record.source_connection();

  RecordApplier applier{std::move(context)};

  try {
    applier.process(std::move(record));
  } catch (const std::exception& ex) {
    simulator::log::err(
        "an error occurred while processing a historical record "
        "from row {} from the `{}' datasource (connection: `{}'): {}. "
        "Discarding all generated historical messages, "
        "internal generated orders registry may be corrupted",
        row_number,
        source_name.value_or("unknown"),
        source_conn.value_or("unknown"),
        ex.what());

    failed = true;
  }

  if (!failed) {
    replies = std::move(applier.request_messages_);

    const std::size_t num_messages = replies.size();
    simulator::log::debug(
        "{} messages generated based on historical record from row {} of "
        "`{}' datasource",
        num_messages,
        row_number,
        source_name.value_or("unknown"));
  }

  return replies;
}

RecordApplier::RecordApplier(RecordApplier::ContextPointer context) noexcept
    : context_{std::move(context)} {}

void RecordApplier::process(Historical::Record record) {
  if (record.has_levels()) {
    cancel_other_parties(record);

    const std::optional<std::string>& source_name = record.source_name();
    const std::uint64_t source_row = record.source_row();
    std::size_t levels_applied = 0;

    record.steal_levels([this, &levels_applied, &source_name, source_row](
                            std::uint64_t level_idx, Historical::Level level) {
      if (process(level, level_idx)) {
        ++levels_applied;
      } else {
        simulator::log::warn(
            "level at index {} has been skipped in a record from "
            "`{}' datasource at row {}: {}",
            level_idx,
            source_name.value_or("undefined"),
            source_row,
            level);
      }
    });

    simulator::log::debug(
        "{} level applied from historical {}", levels_applied, record);
  } else {
    cancel_bid_part();
    cancel_offer_part();

    simulator::log::debug(
        "created cancel messages for all bid and offer generated orders, "
        "no levels are present in historical {}",
        record);
  }
}

auto RecordApplier::process(const Historical::Level& level,
                            std::uint64_t level_idx) -> bool {
  if (!RecordChecker::is_processable(level)) {
    return false;
  }

  if (!place_bid(level)) {
    simulator::log::warn(
        "no bid data was found at the historical level at "
        "index {}; the bid part of the level has been ignored",
        level_idx);
  }

  if (!place_offer(level)) {
    simulator::log::warn(
        "no offer data was found at the historical level at "
        "index {}; the offer part of the level has been ignored",
        level_idx);
  }

  return true;
}

auto RecordApplier::place_bid(const Historical::Level& level) -> bool {
  if (!RecordChecker::has_bid_part(level)) {
    return false;
  }

  constexpr auto target_side = simulator::Side::Option::Buy;

  assert(level.bid_price().has_value());
  const double price = level.bid_price().value();

  assert(level.bid_quantity().has_value());
  const double quantity = level.bid_quantity().value();

  std::string party = level.bid_counterparty().has_value()
                          ? level.bid_counterparty().value()
                          : next_party_id();

  place(Order{price, target_side, quantity, std::move(party)});
  return true;
}

auto RecordApplier::place_offer(const Historical::Level& level) -> bool {
  if (!RecordChecker::has_offer_part(level)) {
    return false;
  }

  constexpr auto target_side = simulator::Side::Option::Sell;

  assert(level.offer_price().has_value());
  const double price = level.offer_price().value();

  assert(level.offer_quantity().has_value());
  const double quantity = level.offer_quantity().value();

  std::string party = level.offer_counterparty().has_value()
                          ? level.offer_counterparty().value()
                          : next_party_id();

  place(Order{price, target_side, quantity, std::move(party)});
  return true;
}

auto RecordApplier::place(RecordApplier::Order order) -> void {
  auto& context = *context_;
  auto& registry = context.takeRegistry();

  const std::optional<GeneratedOrderData> existing_order =
      registry.findByOwner(order.counterparty_id);

  RequestBuilder message_builder;
  message_builder.withRestingAttributes()
      .withPrice(simulator::OrderPrice{order.price})
      .withQuantity(simulator::Quantity{order.quantity})
      .withSide(order.side)
      .withCounterparty(simulator::PartyId{std::move(order.counterparty_id)});

  if (existing_order.has_value() &&
      existing_order->getOrderSide() == order.side) {
    message_builder.makeModificationRequest()
        .withClOrdID(existing_order->getOrderID())
        .withOrigClOrdID(existing_order->getOrigOrderID());
  } else {
    if (existing_order.has_value()) {
      const auto& target_ord_id = existing_order->getOrderID();
      cancel([&target_ord_id](const GeneratedOrderData& _placedOrder) {
        return _placedOrder.getOrderID() == target_ord_id;
      });
    }

    message_builder.makeNewOrderRequest().withClOrdID(
        simulator::ClientOrderId{context.getSyntheticIdentifier()});
  }

  auto order_message = RequestBuilder::construct(std::move(message_builder));
  OrderRegistryUpdater::update(registry, order_message);
  request_messages_.emplace_back(std::move(order_message));
}

auto RecordApplier::cancel(
    const GeneratedOrdersRegistry::Predicate& cancel_criteria) -> void {
  GeneratedOrdersRegistry& registry = context_->takeRegistry();

  const std::vector<GeneratedOrderData> orders =
      registry.selectBy(cancel_criteria);

  if (orders.empty()) {
    return;
  }

  std::vector<GeneratedMessage> cancel_requests;
  for (const GeneratedOrderData& order : orders) {
    RequestBuilder request_builder;
    request_builder.makeCancelRequest()
        .withRestingAttributes()
        .withClOrdID(order.getOrderID())
        .withOrigClOrdID(order.getOrigOrderID())
        .withSide(order.getOrderSide())
        .withPrice(order.getOrderPx())
        .withQuantity(order.getOrderQty())
        .withCounterparty(order.getOwnerID());

    auto cancel_request = RequestBuilder::construct(std::move(request_builder));
    cancel_requests.emplace_back(std::move(cancel_request));
  }

  for (const GeneratedMessage& cancel_request : cancel_requests) {
    OrderRegistryUpdater::update(registry, cancel_request);
  }

  std::copy(std::make_move_iterator(std::begin(cancel_requests)),
            std::make_move_iterator(std::end(cancel_requests)),
            std::back_inserter(request_messages_));
}

auto RecordApplier::cancel_bid_part() -> void {
  static const GeneratedOrdersRegistry::Predicate all_bid_order =
      [](const GeneratedOrderData& _order) {
        return _order.getOrderSide() == simulator::Side::Option::Buy;
      };

  cancel(all_bid_order);
}

auto RecordApplier::cancel_offer_part() -> void {
  static GeneratedOrdersRegistry::Predicate const all_offer_order =
      [](const GeneratedOrderData& _order) {
        return _order.getOrderSide() == simulator::Side::Option::Sell;
      };

  cancel(all_offer_order);
}

auto RecordApplier::cancel_other_parties(const Historical::Record& record)
    -> void {
  std::unordered_set<std::string> parties;
  record.visit_levels([&parties](std::uint64_t, const Level& level) {
    if (const auto party = level.bid_counterparty()) {
      parties.insert(*party);
    }
    if (const auto party = level.offer_counterparty()) {
      parties.insert(*party);
    }
  });

  cancel([&parties = std::as_const(parties)](const GeneratedOrderData& order) {
    return !parties.contains(order.getOwnerID().value());
  });
}

auto RecordApplier::next_party_id() -> std::string {
  return fmt::format(Constant::Historical::DefaultCounterpartyPattern,
                     ++party_id_counter_);
}

RecordApplier::Order::Order(double _price,
                            simulator::Side _side,
                            double _quantity,
                            std::string _counterparty_id) noexcept
    : counterparty_id{std::move(_counterparty_id)},
      price{_price},
      quantity{_quantity},
      side{_side} {}

auto RecordApplier::RecordChecker::is_processable(
    const Historical::Level& level) noexcept -> bool {
  const bool has_bid_px = level.bid_price().has_value();
  const bool has_bid_qty = level.bid_quantity().has_value();

  const bool has_offer_px = level.offer_price().has_value();
  const bool has_offer_qty = level.offer_quantity().has_value();

  const bool is_bid_valid = !static_cast<bool>(has_bid_px ^ has_bid_qty);
  const bool is_offer_valid = !static_cast<bool>(has_offer_px ^ has_offer_qty);

  return is_bid_valid && is_offer_valid;
}

auto RecordApplier::RecordChecker::has_bid_part(
    const Historical::Level& level) noexcept -> bool {
  if (!is_processable(level)) {
    return false;
  }

  const bool has_price = level.bid_price().has_value();
  const bool has_qty = level.bid_quantity().has_value();

  assert(!(has_price ^ has_qty));
  return has_price && has_qty;
}

auto RecordApplier::RecordChecker::has_offer_part(
    const Historical::Level& level) noexcept -> bool {
  if (!is_processable(level)) {
    return false;
  }

  const bool has_price = level.offer_price().has_value();
  const bool has_qty = level.offer_quantity().has_value();

  assert(!(has_price ^ has_qty));
  return has_price && has_qty;
}

}  // namespace Simulator::Generator::Historical
