#ifndef SIMULATOR_GENERATOR_IH_ADAPTATION_GENERATED_MESSAGE_HPP_
#define SIMULATOR_GENERATOR_IH_ADAPTATION_GENERATED_MESSAGE_HPP_

#include "core/domain/attributes.hpp"
#include "core/domain/instrument_descriptor.hpp"

namespace Simulator::Generator {

enum class MessageType : std::uint8_t {
  NewOrderSingle,
  OrderCancelReplaceRequest,
  OrderCancelRequest,
  ExecutionReport
};

struct GeneratedMessage {
  MessageType message_type;
  std::optional<simulator::OrderType> order_type;
  std::optional<simulator::TimeInForce> time_in_force;
  std::optional<simulator::Side> side;
  std::optional<simulator::OrderPrice> order_price;
  std::optional<simulator::Quantity> quantity;
  std::optional<simulator::ClientOrderId> client_order_id;
  std::optional<simulator::OrigClientOrderId> orig_client_order_id;
  std::optional<simulator::PartyId> party_id;
  std::optional<simulator::OrderStatus> order_status;
};

auto operator<<(std::ostream& os, MessageType& message_type) -> std::ostream&;

auto operator<<(std::ostream& os, const GeneratedMessage& message)
    -> std::ostream&;

}  // namespace Simulator::Generator

template <>
struct fmt::formatter<Simulator::Generator::MessageType>
    : fmt::formatter<std::string_view> {
  using MessageType = Simulator::Generator::MessageType;

  auto format(MessageType message_type, format_context& ctx) const
      -> decltype(ctx.out());
};

template <>
struct fmt::formatter<Simulator::Generator::GeneratedMessage>
    : fmt::formatter<std::string_view> {
  using formattable = Simulator::Generator::GeneratedMessage;

  auto format(formattable message, format_context& ctx) const
      -> decltype(ctx.out());
};

#endif  // SIMULATOR_GENERATOR_IH_ADAPTATION_GENERATED_MESSAGE_HPP_
