#include "ih/registry/generated_orders_registry_impl.hpp"

#include <cassert>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <vector>

#include "ih/registry/generated_order_data.hpp"
#include "ih/registry/generated_orders_registry.hpp"

namespace Simulator::Generator {

std::optional<GeneratedOrdersRegistryImpl::OrderData>
GeneratedOrdersRegistryImpl::findByOwner(
    std::string_view _ownerID
) const
{
    std::shared_lock<decltype(m_mutex)> const lock { m_mutex };

    if (auto optStoredIt = lookupIn(m_byOwnerAssoc, _ownerID))
    {
        auto storedIt = *optStoredIt;
        return std::make_optional(*storedIt);
    }

    return std::nullopt;
}


std::optional<GeneratedOrdersRegistryImpl::OrderData>
GeneratedOrdersRegistryImpl::findByIdentifier(
    std::string_view _identifier
) const
{
    std::shared_lock<decltype(m_mutex)> const lock { m_mutex };

    if (auto optStoredIt = lookupIn(m_byIdentifierAssoc, _identifier))
    {
        auto storedIt = *optStoredIt;
        return std::make_optional(*storedIt);
    }

    return std::nullopt;
}


bool GeneratedOrdersRegistryImpl::add(OrderData && _newOrderData)
{
    std::unique_lock<decltype(m_mutex)> const lock { m_mutex };

    if (violatesUniqueConstraints(_newOrderData))
    {
        return false;
    }

    insert(std::move(_newOrderData));
    return true;
}


bool GeneratedOrdersRegistryImpl::updateByOwner(
        std::string_view _ownerID
    ,   OrderData::Patch && _patch
)
{
    std::unique_lock<decltype(m_mutex)> const lock { m_mutex };

    auto optStoredIt = lookupIn(m_byOwnerAssoc, _ownerID);
    if (!optStoredIt.has_value())
    {
        return false;
    }

    update(*optStoredIt, std::move(_patch));
    return true;
}


bool GeneratedOrdersRegistryImpl::updateByIdentifier(
    std::string_view _identifier
    ,   OrderData::Patch && _patch
)
{
    std::unique_lock<decltype(m_mutex)> const lock { m_mutex };

    auto optStoredIt = lookupIn(m_byIdentifierAssoc, _identifier);
    if (!optStoredIt.has_value())
    {
        return false;
    }

    update(*optStoredIt, std::move(_patch));
    return true;
}


bool GeneratedOrdersRegistryImpl::removeByOwner(std::string_view _ownerID)
{
    std::unique_lock<decltype(m_mutex)> const lock { m_mutex };

    auto optStoredIt = lookupIn(m_byOwnerAssoc, _ownerID);
    if (!optStoredIt.has_value())
    {
        return false;
    }

    remove(*optStoredIt);
    return true;
}


bool GeneratedOrdersRegistryImpl::removeByIdentifier(
    std::string_view _identifier
)
{
    std::unique_lock<decltype(m_mutex)> const lock { m_mutex };

    auto optStoredIt = lookupIn(m_byIdentifierAssoc, _identifier);
    if (!optStoredIt.has_value())
    {
        return false;
    }

    remove(*optStoredIt);
    return true;
}


void GeneratedOrdersRegistryImpl::forEach(Visitor const & _visitor) const
{
    std::shared_lock<decltype(m_mutex)> const lock { m_mutex };

    for (auto const & storedOrder : m_storage)
    {
        _visitor(storedOrder);
    }
}


std::vector<GeneratedOrdersRegistryImpl::OrderData>
GeneratedOrdersRegistryImpl::selectBy(Predicate const & _predicate) const
{
    std::vector<OrderData> selected {};

    {
        std::shared_lock<decltype(m_mutex)> const lock { m_mutex };

        for (auto const & storedOrder : m_storage)
        {
            if (_predicate(storedOrder))
            {
                selected.emplace_back(storedOrder);
            }
        }
    }

    return selected;
}


bool GeneratedOrdersRegistryImpl::violatesUniqueConstraints(
    OrderData const & _orderData
) const
{
    auto byOwnerIt = m_byOwnerAssoc.find(_orderData.getOwnerID().value());
    auto byOrderIDIt = m_byIdentifierAssoc.find(_orderData.getOrderID().value());

    const bool byOwnerIdxExists = byOwnerIt != std::end(m_byOwnerAssoc);
    const bool byIdIdxExists = byOrderIDIt != std::end(m_byIdentifierAssoc);
    return byOwnerIdxExists || byIdIdxExists;
}


void GeneratedOrdersRegistryImpl::insert(OrderData && _orderData)
{
    auto & inserted = m_storage.emplace_back(std::move(_orderData));
    assert(!m_storage.empty());
    auto insertedIt = std::prev(std::end(m_storage));

    // It's critical to ensure that we create a string_views keys
    // that are pointing to strings in inserted element
    std::string_view const ownerKey { inserted.getOwnerID().value() };
    auto ownerAssoc = std::make_pair(ownerKey, insertedIt);
    [[maybe_unused]]
    auto const ownerAssocRes = m_byOwnerAssoc.emplace(std::move(ownerAssoc));
    assert(ownerAssocRes.second);

    std::string_view const idKey { inserted.getOrderID().value() };
    auto idAssoc = std::make_pair(idKey, insertedIt);
    [[maybe_unused]]
    auto const idAssocRes = m_byIdentifierAssoc.emplace(std::move(idAssoc));
    assert(idAssocRes.second);
}


void GeneratedOrdersRegistryImpl::update(
        Storage::iterator _storedIt
    ,   OrderData::Patch && _patch
)
{
    assert(_storedIt != std::end(m_storage));

    // Remove existent by-owner association
    auto byOwnerAssocIt = m_byOwnerAssoc.find(_storedIt->getOwnerID().value());
    assert(byOwnerAssocIt != std::end(m_byOwnerAssoc));
    m_byOwnerAssoc.erase(byOwnerAssocIt);

    // Remove existent by-id association
    auto byIDAssocIt = m_byIdentifierAssoc.find(_storedIt->getOrderID().value());
    assert(byIDAssocIt != std::end(m_byIdentifierAssoc));
    m_byIdentifierAssoc.erase(byIDAssocIt);

    _storedIt->apply(std::move(_patch));

    // Re-associate updated order data with an owner id
    std::string_view const ownerKey { _storedIt->getOwnerID().value() };
    auto ownerAssoc = std::make_pair(ownerKey, _storedIt);
    [[maybe_unused]]
    auto byOwnerAssocRes = m_byOwnerAssoc.emplace(std::move(ownerAssoc));
    assert(byOwnerAssocRes.second);

    // Re-associate updated order data with an order id
    std::string_view const idKey { _storedIt->getOrderID().value() };
    auto idAssoc = std::make_pair(idKey, _storedIt);
    [[maybe_unused]]
    auto byIDAssocRes = m_byIdentifierAssoc.emplace(std::move(idAssoc));
    assert(byIDAssocRes.second);
}


void GeneratedOrdersRegistryImpl::remove(Storage::iterator _storedIt)
{
    assert(_storedIt != std::end(m_storage));

    // Remove association firstly by accessing valid _storedIt
    m_byIdentifierAssoc.erase(_storedIt->getOrderID().value());
    m_byOwnerAssoc.erase(_storedIt->getOwnerID().value());

    m_storage.erase(_storedIt);
}


std::optional<GeneratedOrdersRegistryImpl::Storage::iterator>
GeneratedOrdersRegistryImpl::lookupIn(
        GeneratedOrdersRegistryImpl::HashTable const & _hashtable
    ,   std::string_view _key
)
{
    auto targetIt = _hashtable.find(_key);
    return targetIt == std::end(_hashtable)
        ?   std::nullopt
        :   std::make_optional(targetIt->second);
}

} // namespace Simulator::Generator
