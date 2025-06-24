#ifndef SIMULATOR_GENERATOR_IH_REGISTRY_GENERATED_ORDER_DATA_HPP_
#define SIMULATOR_GENERATOR_IH_REGISTRY_GENERATED_ORDER_DATA_HPP_

#include <optional>
#include <string>

#include "core/domain/attributes.hpp"

namespace Simulator::Generator {

class GeneratedOrderData
{
public:

    class Patch
    {
        friend class GeneratedOrderData;

    public:

        Patch & setUpdatedID(simulator::ClientOrderId _updatedID)
        {
            if (_updatedID.value().empty())
            {
                throw std::invalid_argument {
                    "an updated order id can not be represented "
                    "by an empty string"
                };
            }

            m_optNewOrderID = std::move(_updatedID);
            return *this;
        }

        Patch & setUpdatedPrice(simulator::OrderPrice _updatedPx) noexcept
        {
            m_optNewPx = _updatedPx;
            return *this;
        }

        Patch & setUpdatedQuantity(simulator::Quantity _updatedQty) noexcept
        {
            m_optNewQty = _updatedQty;
            return *this;
        }

    private:

        std::optional<simulator::ClientOrderId> m_optNewOrderID;
        std::optional<simulator::OrderPrice> m_optNewPx;
        std::optional<simulator::Quantity> m_optNewQty;
    };


    class Builder
    {
        friend class GeneratedOrderData;

    public:

        Builder() = delete;

        Builder(
                simulator::PartyId _orderOwner
            ,   simulator::ClientOrderId _orderID
            ,   simulator::Side _orderSide
        )
            :   m_newOrderOwner { std::move(_orderOwner) }
            ,   m_newOrderID { std::move(_orderID) }
            ,   m_newOrderSide { _orderSide }
        {
            if (m_newOrderOwner.value().empty())
            {
                throw std::invalid_argument {
                    "an order owner id can not be represented "
                    "by an empty string"
                };
            }
            if (m_newOrderID.value().empty())
            {
                throw std::invalid_argument {
                    "an order id can not be represented "
                    "by an empty string"
                };
            }
        }

        Builder & setPrice(simulator::OrderPrice _orderPrice) noexcept
        {
            m_newOrderPrice = _orderPrice;
            return *this;
        }

        Builder & setQuantity(simulator::Quantity _orderQty) noexcept
        {
            m_newOrderQty = simulator::Quantity{_orderQty};
            return *this;
        }

    private:

        simulator::PartyId m_newOrderOwner;
        simulator::ClientOrderId m_newOrderID;

        simulator::OrderPrice m_newOrderPrice { 0.0 };
        simulator::Quantity m_newOrderQty { 0.0 };

        simulator::Side m_newOrderSide;
    };


    GeneratedOrderData() = delete;

    explicit GeneratedOrderData(Builder && _builder) noexcept
        :   m_orderID { std::move(_builder.m_newOrderID) }
        ,   m_ownerID { std::move(_builder.m_newOrderOwner) }
        ,   m_orderPx { _builder.m_newOrderPrice }
        ,   m_orderQty { _builder.m_newOrderQty }
        ,   m_orderSide { _builder.m_newOrderSide }
    {}


    void apply(Patch && _patch)
    {
        if (_patch.m_optNewOrderID.has_value() &&
            !_patch.m_optNewOrderID->value().empty())
        {
            if (!m_origOrderID.has_value())
            {
                m_origOrderID = simulator::OrigClientOrderId(m_orderID.value());
            }
            m_orderID = std::move(*_patch.m_optNewOrderID);
        }

        if (_patch.m_optNewPx.has_value())
        {
            m_orderPx = *_patch.m_optNewPx;
        }
        if (_patch.m_optNewQty.has_value())
        {
            m_orderQty = *_patch.m_optNewQty;
        }
    }


    [[nodiscard]]
    auto getOrderID() const noexcept -> const simulator::ClientOrderId&
    {
        return m_orderID;
    }

    [[nodiscard]]
    auto getOrigOrderID() const noexcept -> simulator::OrigClientOrderId {
      return m_origOrderID.has_value()
                 ? *m_origOrderID
                 : simulator::OrigClientOrderId{m_orderID.value()};
    }

    [[nodiscard]]
    auto getOwnerID() const noexcept -> const simulator::PartyId&
    {
        return m_ownerID;
    }

    [[nodiscard]]
    auto getOrderPx() const noexcept -> simulator::OrderPrice
    {
        return m_orderPx;
    }

    [[nodiscard]]
    auto getOrderQty() const noexcept -> simulator::Quantity
    {
        return m_orderQty;
    }

    [[nodiscard]]
    auto getOrderSide() const noexcept -> simulator::Side
    {
        return m_orderSide;
    }

private:

    simulator::ClientOrderId m_orderID;
    std::optional<simulator::OrigClientOrderId> m_origOrderID;
    simulator::PartyId m_ownerID;

    simulator::OrderPrice m_orderPx;
    simulator::Quantity m_orderQty;

    simulator::Side m_orderSide;
};

} // namespace Simulator::Generator

#endif // SIMULATOR_GENERATOR_IH_REGISTRY_GENERATED_ORDER_DATA_HPP_
