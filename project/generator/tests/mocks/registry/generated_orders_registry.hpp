#ifndef SIMULATOR_GENERATOR_TESTS_MOCKS_GENERATED_ORDERS_REGISTRY_HPP_
#define SIMULATOR_GENERATOR_TESTS_MOCKS_GENERATED_ORDERS_REGISTRY_HPP_

#include <optional>
#include <string_view>
#include <vector>

#include <gmock/gmock.h>

#include "ih/registry/generated_order_data.hpp"
#include "ih/registry/generated_orders_registry.hpp"

namespace Simulator::Generator::Mock {

class GeneratedOrdersRegistry
    :   public Generator::GeneratedOrdersRegistry
{
public:

    MOCK_METHOD(std::optional<GeneratedOrderData>, findByOwner, (std::string_view), (const, override));

    MOCK_METHOD(std::optional<GeneratedOrderData>, findByIdentifier, (std::string_view), (const, override));

    MOCK_METHOD(bool, add, (OrderData &&), (override));

    MOCK_METHOD(bool, updateByOwner, (std::string_view, OrderData::Patch &&), (override));

    MOCK_METHOD(bool, updateByIdentifier, (std::string_view, OrderData::Patch &&), (override));

    MOCK_METHOD(bool, removeByOwner, (std::string_view), (override));

    MOCK_METHOD(bool, removeByIdentifier, (std::string_view), (override));

    MOCK_METHOD(void, forEach, (Visitor const &), (const, override));

    MOCK_METHOD(std::vector<OrderData>, selectBy, (Predicate const &), (const, override));
};

} // Simulator::Generator::Mock

#endif // SIMULATOR_GENERATOR_TESTS_MOCKS_GENERATED_ORDERS_REGISTRY_HPP_
