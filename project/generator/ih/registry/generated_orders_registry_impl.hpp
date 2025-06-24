#ifndef SIMULATOR_GENERATOR_SRC_REGISTRY_GENERATED_ORDERS_REGISTRY_HPP_
#define SIMULATOR_GENERATOR_SRC_REGISTRY_GENERATED_ORDERS_REGISTRY_HPP_

#include <list>
#include <optional>
#include <shared_mutex>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "ih/registry/generated_order_data.hpp"
#include "ih/registry/generated_orders_registry.hpp"

namespace Simulator::Generator {

class GeneratedOrdersRegistryImpl
    :   public GeneratedOrdersRegistry
{
private:

    using Storage = std::list<OrderData>;

    // An orders registry must guarantee that keys in hash-tables pointing
    // to the stored object's string members.
    using HashTable = std::unordered_map<
            std::string_view  // A string key referenced to a stored object
        ,   Storage::iterator // An iterator to a stored object
    >;

public:

    std::optional<OrderData> findByOwner(
        std::string_view _ownerID
    ) const override;

    std::optional<OrderData> findByIdentifier(
        std::string_view _identifier
    ) const override;


    bool add(OrderData && _newOrderData) override;

    bool updateByOwner(
            std::string_view _ownerID
        ,   OrderData::Patch && _patch
    ) override;

    bool updateByIdentifier(
            std::string_view _identifier
        ,   OrderData::Patch && _patch
    ) override;

    bool removeByOwner(std::string_view _ownerID) override;

    bool removeByIdentifier(std::string_view _identifier) override;


    void forEach(Visitor const & _visitor) const override;

    std::vector<OrderData> selectBy(Predicate const & _predicate) const override;

private:

    [[nodiscard]]
    bool violatesUniqueConstraints(OrderData const & _orderData) const;

    void insert(OrderData && _orderData);

    void update(Storage::iterator _storedIt, OrderData::Patch && _patch);

    void remove(Storage::iterator _storedIt);

    [[nodiscard]]
    static std::optional<Storage::iterator> lookupIn(
            HashTable const & _hashtable
        ,   std::string_view _key
    );


    HashTable m_byOwnerAssoc;
    HashTable m_byIdentifierAssoc;

    mutable std::shared_mutex m_mutex;

    Storage m_storage;
};

} // namespace Simulator::Generator

#endif // SIMULATOR_GENERATOR_SRC_REGISTRY_GENERATED_ORDERS_REGISTRY_HPP_
