#include "ih/utils/request_builder.hpp"

#include <cassert>
#include <stdexcept>

#include "ih/constants.hpp"

namespace Simulator::Generator {

RequestBuilder & RequestBuilder::makeNewOrderRequest() noexcept
{
    m_targetMsgType = MessageType::NewOrderSingle;
    return *this;
}


RequestBuilder & RequestBuilder::makeModificationRequest() noexcept
{
    m_targetMsgType = MessageType::OrderCancelReplaceRequest;
    return *this;
}


RequestBuilder & RequestBuilder::makeCancelRequest() noexcept
{
    m_targetMsgType = MessageType::OrderCancelRequest;
    return *this;
}


RequestBuilder & RequestBuilder::withClOrdID(simulator::ClientOrderId _id) noexcept
{
    m_targetClOrdID = std::make_optional(std::move(_id));
    return *this;
}


RequestBuilder & RequestBuilder::withOrigClOrdID(simulator::OrigClientOrderId _id) noexcept
{
    m_targetOrigClOrdID = std::make_optional(std::move(_id));
    return *this;
}


RequestBuilder & RequestBuilder::withCounterparty(simulator::PartyId _id) noexcept
{
    m_targetCpID = std::make_optional(std::move(_id));
    return *this;
}


RequestBuilder & RequestBuilder::withAggressiveAttributes() noexcept
{
    m_targetOrdType = Generator::Constant::AggressiveOrderType;
    m_targetTif = Generator::Constant::AggressiveTimeInForce;
    return *this;
}


RequestBuilder & RequestBuilder::withRestingAttributes() noexcept
{
    m_targetOrdType = Generator::Constant::RestingOrderType;
    m_targetTif = Generator::Constant::RestingTimeInForce;
    return *this;
}


RequestBuilder & RequestBuilder::withPrice(simulator::OrderPrice _price) noexcept
{
    m_targetPrice = std::make_optional(_price);
    return *this;
}


RequestBuilder & RequestBuilder::withQuantity(simulator::Quantity _qty) noexcept
{
    m_targetQty = std::make_optional(_qty);
    return *this;
}


RequestBuilder &RequestBuilder::withSide(simulator::Side _side) noexcept
{
    m_targetSide = std::make_optional(_side);
    return *this;
}


GeneratedMessage RequestBuilder::construct(RequestBuilder && _builder)
{
    validate(_builder);

    GeneratedMessage message;

    assert(_builder.m_targetMsgType.has_value());
    message.message_type = *_builder.m_targetMsgType;

    assert(_builder.m_targetOrdType);
    message.order_type = _builder.m_targetOrdType;

    assert(_builder.m_targetTif);
    message.time_in_force = _builder.m_targetTif;

    assert(_builder.m_targetSide);
    message.side = _builder.m_targetSide;

    assert(_builder.m_targetClOrdID);
    message.client_order_id = std::move(_builder.m_targetClOrdID);

    message.orig_client_order_id = std::move(_builder.m_targetOrigClOrdID);
    message.party_id = std::move(_builder.m_targetCpID);
    message.order_price = _builder.m_targetPrice;
    message.quantity = _builder.m_targetQty;

    return message;
}


void RequestBuilder::validate(RequestBuilder const & _builder)
{
    if (!_builder.m_targetMsgType.has_value())
    {
        throw std::invalid_argument {
            "can not construct order request without a MessageType value"
        };
    }

    if (!_builder.m_targetOrdType.has_value())
    {
        throw std::invalid_argument {
            "can not construct order request without a OrderType value"
        };
    }

    if (!_builder.m_targetTif.has_value())
    {
        throw std::invalid_argument {
            "can not construct order request without a TimeInForce value"
        };
    }

    if (!_builder.m_targetSide.has_value())
    {
        throw std::invalid_argument {
            "can not construct order request without a Side value"
        };
    }

    if (!_builder.m_targetClOrdID.has_value())
    {
        throw std::invalid_argument {
            "can not construct order request without a ClOrdID value"
        };
    }
}

} // namespace Simulator::Generator