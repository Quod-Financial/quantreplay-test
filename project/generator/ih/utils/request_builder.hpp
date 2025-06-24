#ifndef SIMULATOR_GENERATOR_SRC_UTILS_MESSAGE_BUILDER_HPP_
#define SIMULATOR_GENERATOR_SRC_UTILS_MESSAGE_BUILDER_HPP_

#include <optional>

#include "ih/adaptation/generated_message.hpp"

namespace Simulator::Generator {

class RequestBuilder
{
public:

    RequestBuilder & makeNewOrderRequest() noexcept;

    RequestBuilder & makeModificationRequest() noexcept;

    RequestBuilder & makeCancelRequest() noexcept;


    RequestBuilder & withClOrdID(simulator::ClientOrderId _id) noexcept;

    RequestBuilder & withOrigClOrdID(simulator::OrigClientOrderId _id) noexcept;

    RequestBuilder & withCounterparty(simulator::PartyId _cp) noexcept;


    RequestBuilder & withAggressiveAttributes() noexcept;

    RequestBuilder & withRestingAttributes() noexcept;


    RequestBuilder & withPrice(simulator::OrderPrice _price) noexcept;

    RequestBuilder & withQuantity(simulator::Quantity _qty) noexcept;

    RequestBuilder & withSide(simulator::Side _side) noexcept;


    static GeneratedMessage construct(RequestBuilder && _builder);

private:

    static void validate(RequestBuilder const & _builder);

    std::optional<simulator::ClientOrderId> m_targetClOrdID;
    std::optional<simulator::OrigClientOrderId> m_targetOrigClOrdID;
    std::optional<simulator::PartyId> m_targetCpID;

    std::optional<simulator::OrderPrice> m_targetPrice;
    std::optional<simulator::Quantity> m_targetQty;

    std::optional<MessageType> m_targetMsgType;
    std::optional<simulator::TimeInForce> m_targetTif;
    std::optional<simulator::OrderType> m_targetOrdType;
    std::optional<simulator::Side> m_targetSide;
};

} // namespace Simulator::Generator

#endif // SIMULATOR_GENERATOR_SRC_UTILS_MESSAGE_BUILDER_HPP_
