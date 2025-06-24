#ifndef SIMULATOR_GENERATOR_IH_REGISTRY_GENERATED_ORDERS_REGISTRY_HPP_
#define SIMULATOR_GENERATOR_IH_REGISTRY_GENERATED_ORDERS_REGISTRY_HPP_

#include <functional>
#include <optional>
#include <string_view>
#include <vector>

#include "ih/registry/generated_order_data.hpp"

namespace Simulator::Generator {

/**
 * An interface for a thread-safe container for per-instrument generated orders.
 * Associates each generated order with an identifier and owning counterparty.
 *
 * Each order must have a unique identifier and owner (PartyID),
 * otherwise implementation shall reject a request to save the order.
 */
class GeneratedOrdersRegistry
{
public:

    using OrderData = GeneratedOrderData;
    using Visitor = std::function<void(OrderData const &)>;
    using Predicate = std::function<bool(OrderData const &)>;


    virtual ~GeneratedOrdersRegistry() = default;

    /// Searches for an order associated with a specified owner id (PartyID)
    [[nodiscard]]
    virtual std::optional<OrderData> findByOwner(
        std::string_view _ownerID
    ) const = 0;

    /// Searches for an order associated with a specified OrderID
    [[nodiscard]]
    virtual std::optional<OrderData> findByIdentifier(
        std::string_view _identifier
    ) const = 0;


    virtual bool add(OrderData && _newOrderData) = 0;

    /// Updates an order associated with an owner id (PartyID).
    /// OrderID-to-order association shall be updated by the implementation.
    virtual bool updateByOwner(
            std::string_view _ownerID
        ,   OrderData::Patch && _patch
    ) = 0;

    /// Updates an order associated with an identifier.
    /// A last-known order identifier is expected by the container
    /// to resolve target order.
    /// OrderID-to-order association shall be updated by the implementation.
    virtual bool updateByIdentifier(
            std::string_view _identifier
        ,   OrderData::Patch && _patch
    ) = 0;

    virtual bool removeByOwner(std::string_view _ownerID) = 0;

    virtual bool removeByIdentifier(std::string_view _identifier) = 0;


    /// Applies provided `_visitor` to each stored order.
    /// Please note: a `_visitor` callback must not call any method
    /// on the same container object to avoid deadlock in the implementation.
    virtual void forEach(Visitor const & _visitor) const = 0;


    /// Returns all stored orders that satisfy a given `_predicate`.
    /// Please note: a `_predicate` callback must not call any method
    /// on the same container object to avoid deadlock in the implementation.
    [[nodiscard]]
    virtual std::vector<OrderData> selectBy(
        Predicate const & _predicate
    ) const = 0;
};

}// namespace Simulator::Generator

#endif // SIMULATOR_GENERATOR_IH_REGISTRY_GENERATED_ORDERS_REGISTRY_HPP_
