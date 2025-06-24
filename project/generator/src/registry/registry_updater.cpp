#include "ih/registry/registry_updater.hpp"

#include <cassert>
#include <stdexcept>
#include <utility>

#include "ih/constants.hpp"
#include "ih/registry/generated_order_data.hpp"
#include "log/logging.hpp"

namespace Simulator::Generator {

namespace {

auto validate_client_order_id(const GeneratedMessage& _message) -> void {
  if (!_message.client_order_id.has_value() ||
      _message.client_order_id->value().empty()) {
    throw std::invalid_argument{fmt::format(
        "registry updater expects a `{}' message to have non-empty ClOrdID",
        _message.message_type)};
  }
}

auto validate_party_id(const GeneratedMessage& _message) -> void {
  if (!_message.party_id.has_value() || _message.party_id->value().empty()) {
    throw std::invalid_argument{
        fmt::format("registry updater expects a `{}' message to have non-empty "
                    "1 counterparty (owner)",
                    _message.message_type)};
  }
}

auto validate_side(const GeneratedMessage& _message) -> void {
  if (!_message.side.has_value()) {
    throw std::invalid_argument{
        fmt::format("registry updater expects a `{}' message to have Side",
                    _message.message_type)};
  }
}

auto validate_order_status(const GeneratedMessage& _message) -> void {
  if (!_message.order_status.has_value()) {
    throw std::invalid_argument{fmt::format(
        "registry updater expects a `{}' message to have OrderStatus",
        _message.message_type)};
  }
}

} // namespace

OrderRegistryUpdater::OrderRegistryUpdater(
    GeneratedOrdersRegistry & _registry
) noexcept
    :   m_registryRef { _registry }
{}


void OrderRegistryUpdater::update(
        GeneratedOrdersRegistry & _registry
    ,   GeneratedMessage const & _message
)
{
    OrderRegistryUpdater updater { _registry };
    updater.update(_message);
}


void OrderRegistryUpdater::update(GeneratedMessage const & _message)
{
    // Reject is not handled in this implementation:
    // not clear when a matching engine sends reject messages
    switch (_message.message_type)
    {
        case MessageType::NewOrderSingle:
            handleNewOrder(_message);
            break;
        case MessageType::OrderCancelReplaceRequest:
            handleModification(_message);
            break;
        case MessageType::OrderCancelRequest:
            handleCancellation(_message);
            break;
        case MessageType::ExecutionReport:
            handleExecution(_message);
            break;
        default:
        {
            simulator::log::debug(
                "generated orders registry updater received a `{}' "
                "order message, which will not be processed "
                "to update the registry", _message.message_type);
            break;
        }
    }
}


void OrderRegistryUpdater::handleNewOrder(GeneratedMessage const & _message)
{
    assert(_message.message_type == MessageType::NewOrderSingle);

    if (!containsRestingOrder(_message))
    {
        return;
    }

    validate_new_order(_message);

    auto const & ownerID = _message.party_id;
    auto const & orderID = _message.client_order_id;
    auto const side = _message.side;

    GeneratedOrderData::Builder ordBuilder { *ownerID, *orderID, *side };
    ordBuilder.setPrice(_message.order_price.value_or(simulator::OrderPrice{0.}))
        .setQuantity(_message.quantity.value_or(simulator::Quantity{0.}));

    GeneratedOrderData order { std::move(ordBuilder) };
    bool const inserted = registry().add(std::move(order));
    if (!inserted)
    {
        simulator::log::warn(
            "generated orders registry updater failed to register a "
            "new generated order with `{0}' identifier for counterparty `{1}'",
            ownerID,
            ownerID);
    }
}


void OrderRegistryUpdater::handleModification(GeneratedMessage const & _message)
{
    assert(_message.message_type == MessageType::OrderCancelReplaceRequest);

    if (!containsRestingOrder(_message))
    {
        return;
    }

    validate_modification(_message);

    auto const & ownerID = _message.party_id->value();
    auto const & orderID = *_message.client_order_id;

    GeneratedOrderData::Patch update;
    update.setUpdatedID(orderID);
    if (const auto price = _message.order_price) {
      update.setUpdatedPrice(*price);
    }
    if ( const auto qty = _message.quantity) {
      update.setUpdatedQuantity(*qty);
    }

    bool const updated = registry().updateByOwner(ownerID, std::move(update));
    if (!updated)
    {
        simulator::log::warn(
            "generated orders registry updater failed to update an order "
            "with new ID `{0}' for `{1}' counterparty as no active order "
            "was found for that counterparty",
            orderID,
            ownerID);
    }
}


void OrderRegistryUpdater::handleCancellation(GeneratedMessage const & _message)
{
    assert(_message.message_type == MessageType::OrderCancelRequest);

    if (!containsRestingOrder(_message))
    {
        return;
    }

    validate_cancellation(_message);

    auto const & ownerID = _message.party_id->value();
    bool const removed = registry().removeByOwner(ownerID);
    if (!removed)
    {
        simulator::log::warn(
            "generated orders registry updater failed to remove an order "
            "for `{0}' counterparty as no active order was found "
            "for that counterparty",
            ownerID);
    }
}


void OrderRegistryUpdater::handleExecution(GeneratedMessage const & _message)
{
    assert(_message.message_type == MessageType::ExecutionReport);

    validate_execution(_message);
    auto const & orderID = _message.client_order_id->value();

    // We don't care about the registry response when handling execution
    // It's possible that we received a message for an order that is not stored
    // (f.e. aggressive order), no need to log unsuccessful operations
    switch (*_message.order_status)
    {
        case simulator::OrderStatus::Option::PartiallyFilled:
        {
            // Only an order qty can be updated on partial execution
            if (const auto quantity = _message.quantity) {
              GeneratedOrderData::Patch update;
              update.setUpdatedQuantity(*quantity);
              registry().updateByIdentifier(orderID, std::move(update));
            }
            break;
        }
        case simulator::OrderStatus::Option::Filled:
        case simulator::OrderStatus::Option::Cancelled:
        case simulator::OrderStatus::Option::Rejected:
        {
            registry().removeByIdentifier(orderID);
            break;
        }
        case simulator::OrderStatus::Option::New:
        case simulator::OrderStatus::Option::Modified:
            break;
    }
}


GeneratedOrdersRegistry & OrderRegistryUpdater::registry() noexcept
{
    return m_registryRef.get();
}


bool OrderRegistryUpdater::containsRestingOrder(GeneratedMessage const & _message)
{
    auto const ordType = _message.order_type;
    auto const tif = _message.time_in_force;

    assert(_message.message_type == MessageType::NewOrderSingle ||
           _message.message_type == MessageType::OrderCancelReplaceRequest ||
           _message.message_type == MessageType::OrderCancelRequest);

    bool const isRestingOrder =
        tif == Constant::RestingTimeInForce &&
        ordType == Constant::RestingOrderType;

    if (!isRestingOrder)
    {
        simulator::log::warn(
            "Generator's order registry updater ignores a `{}' message, "
            "message contains a non-resting order (order_type={}, time_in_force={})",
            _message.message_type,
            _message.order_type,
            _message.time_in_force);
    }

    return isRestingOrder;
}

auto OrderRegistryUpdater::validate_new_order(const GeneratedMessage& message)
    -> void {
  validate_client_order_id(message);
  validate_party_id(message);
  validate_side(message);
}

auto OrderRegistryUpdater::validate_modification(
    const GeneratedMessage& message) -> void {
  validate_client_order_id(message);
  validate_party_id(message);
}

auto OrderRegistryUpdater::validate_cancellation(
    const GeneratedMessage& message) -> void {
  validate_party_id(message);
}

auto OrderRegistryUpdater::validate_execution(const GeneratedMessage& message)
    -> void {
  validate_client_order_id(message);
  validate_order_status(message);
}

} // namespace Simulator::Generator
