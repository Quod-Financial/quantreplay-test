#include "ih/adaptation/protocol_conversion.hpp"

#include <algorithm>
#include <optional>

#include "core/domain/attributes.hpp"
#include "core/domain/instrument_descriptor.hpp"

namespace Simulator::Generator {
namespace {

auto convert_instrument_currency(const DataLayer::Listing& listing,
                                 simulator::InstrumentDescriptor& descriptor)
    -> void {
  if (!descriptor.security_type.has_value()) {
    return;
  }

  switch (descriptor.security_type->value()) {
    case simulator::SecurityType::Option::CommonStock:
    case simulator::SecurityType::Option::Future:
    case simulator::SecurityType::Option::Option:
    case simulator::SecurityType::Option::MultiLeg:
    case simulator::SecurityType::Option::SyntheticMultiLeg:
    case simulator::SecurityType::Option::Warrant:
    case simulator::SecurityType::Option::MutualFund:
    case simulator::SecurityType::Option::CorporateBond:
    case simulator::SecurityType::Option::ConvertibleBond:
    case simulator::SecurityType::Option::RepurchaseAgreement:
    case simulator::SecurityType::Option::Index:
    case simulator::SecurityType::Option::ContractForDifference:
    case simulator::SecurityType::Option::Certificate: {
      if (const auto& price_currency = listing.getPriceCurrency()) {
        descriptor.currency = simulator::Currency{*price_currency};
      }
      break;
    }

    case simulator::SecurityType::Option::FxSpot:
    case simulator::SecurityType::Option::Forward:
    case simulator::SecurityType::Option::FxForward:
    case simulator::SecurityType::Option::FxNonDeliverableForward:
    case simulator::SecurityType::Option::FxSwap:
    case simulator::SecurityType::Option::FxNonDeliverableSwap: {
      if (const auto& fx_base_currency = listing.getFxBaseCurrency()) {
        descriptor.currency = simulator::Currency{*fx_base_currency};
      }
      break;
    }
  }
}

auto convert_instrument_party(const DataLayer::Listing& listing,
                              simulator::InstrumentDescriptor& descriptor)
    -> void {
  const auto& party_role = listing.getPartyRole();
  const auto& party_id = listing.getPartyId();
  if (!party_role || !party_id) {
    return;
  }

  using simulator::convert_party_role_from_string;
  const auto role = convert_party_role_from_string(*party_role);
  if (!role.has_value()) {
    return;
  }

  constexpr auto source = simulator::PartyIdSource::Option::Proprietary;
  auto id = simulator::PartyId{*party_id};
  descriptor.parties = {
      {simulator::PartyIdentifier{std::move(id), source}, *role}};
}

auto convert_instrument_security_identifier(
    const DataLayer::Listing& listing,
    simulator::InstrumentDescriptor& descriptor) -> void {
  if (const auto& symbol_id = listing.getExchangeSymbolId()) {
    descriptor.security_id = simulator::SecurityId{*symbol_id};
    descriptor.security_id_source =
        simulator::SecurityIdSource::Option::ExchangeSymbol;
  } else if (const auto& isin_id = listing.getIsinId()) {
    descriptor.security_id = simulator::SecurityId{*isin_id};
    descriptor.security_id_source = simulator::SecurityIdSource::Option::Isin;
  } else if (const auto& cusip_id = listing.getCusipId()) {
    descriptor.security_id = simulator::SecurityId{*cusip_id};
    descriptor.security_id_source = simulator::SecurityIdSource::Option::Cusip;
  } else if (const auto& sedol_id = listing.getSedolId()) {
    descriptor.security_id = simulator::SecurityId{*sedol_id};
    descriptor.security_id_source = simulator::SecurityIdSource::Option::Sedol;
  } else if (const auto& ric_id = listing.getRicId()) {
    descriptor.security_id = simulator::SecurityId{*ric_id};
    descriptor.security_id_source = simulator::SecurityIdSource::Option::Ric;
  } else if (const auto& bloomberg_id = listing.getBloombergSymbolId()) {
    descriptor.security_id = simulator::SecurityId{*bloomberg_id};
    descriptor.security_id_source =
        simulator::SecurityIdSource::Option::BloombergSymbol;
  }
}

[[nodiscard]] auto convert_order_price(
    std::optional<simulator::OrderPrice> price,
    std::optional<simulator::OrderType> order_type)
    -> std::optional<simulator::OrderPrice> {
  if (order_type.has_value() &&
      order_type->value() == simulator::OrderType::Option::Limit) {
    return price;
  }
  return std::nullopt;
}

[[nodiscard]] auto convert_order_quantity(
    std::optional<simulator::Quantity> quantity)
    -> std::optional<simulator::OrderQuantity> {
  if (quantity.has_value()) {
    return std::optional<simulator::OrderQuantity>{quantity->value()};
  }
  return std::nullopt;
}

[[nodiscard]] auto convert_order_quantity(
    std::optional<simulator::OrderQuantity> order_quantity)
    -> std::optional<simulator::Quantity> {
  if (order_quantity.has_value()) {
    return std::optional<simulator::Quantity>{order_quantity->value()};
  }
  return std::nullopt;
}

[[nodiscard]] auto convert_order_parties(const simulator::PartyId& party_id)
    -> std::vector<simulator::Party> {
  return {simulator::Party{
      simulator::PartyIdentifier{party_id,
                                 simulator::PartyIdSource::Option::Proprietary},
      simulator::PartyRole::Option::ExecutingFirm}};
}

template <typename T, typename U>
auto assign(T& destination, U source) -> void {
  if constexpr (std::same_as<T, std::string>) {
    destination = std::move(static_cast<std::string>(source));
  } else {
    destination = static_cast<T>(source);
  }
}

template <typename T, typename U>
auto assign(T& destination, std::optional<U> source) -> void {
  if (source.has_value()) {
    assign(destination, std::move(*source));
  }
}

auto get_total_quantity(std::optional<simulator::CumExecutedQuantity> executed,
                        std::optional<simulator::LeavesQuantity> leaves)
    -> simulator::Quantity {
  using namespace simulator;
  const double traded = executed.value_or(CumExecutedQuantity{0}).value();
  double active = leaves.value_or(LeavesQuantity{0}).value();
  return Quantity{traded + active};
}

}  // namespace

auto convert_to_instrument_descriptor(const DataLayer::Listing& listing)
    -> simulator::InstrumentDescriptor {
  simulator::InstrumentDescriptor descriptor;

  const auto& symbol = listing.getSymbol();
  if (symbol.has_value() && !symbol->empty()) {
    descriptor.symbol = simulator::Symbol{*symbol};
  }

  if (const auto& security_type = listing.getSecurityType()) {
    descriptor.security_type =
        simulator::convert_security_type_from_string(*security_type);
  }

  if (const auto& security_exchange = listing.getSecurityExchange()) {
    descriptor.security_exchange =
        simulator::SecurityExchange{*security_exchange};
  }

  convert_instrument_currency(listing, descriptor);
  convert_instrument_party(listing, descriptor);
  convert_instrument_security_identifier(listing, descriptor);

  return descriptor;
}

auto convert_to_order_placement_request(
    const GeneratedMessage& message,
    const simulator::InstrumentDescriptor& instrument)
    -> simulator::protocol::OrderPlacementRequest {
  simulator::protocol::OrderPlacementRequest request{
      simulator::protocol::Session{simulator::protocol::generator::Session{}}};

  request.order_type = message.order_type;
  request.time_in_force = message.time_in_force;
  request.side = message.side;
  request.order_price =
      convert_order_price(message.order_price, request.order_type);
  request.order_quantity = convert_order_quantity(message.quantity);
  request.client_order_id = message.client_order_id;
  if (const auto& party_id = message.party_id) {
    request.parties = convert_order_parties(*party_id);
  }
  request.instrument = instrument;

  return request;
}

auto convert_to_order_modification_request(
    const GeneratedMessage& message,
    const simulator::InstrumentDescriptor& instrument)
    -> simulator::protocol::OrderModificationRequest {
  simulator::protocol::OrderModificationRequest request{
      simulator::protocol::Session{simulator::protocol::generator::Session{}}};

  request.order_type = message.order_type;
  request.time_in_force = message.time_in_force;
  request.side = message.side;
  request.order_price =
      convert_order_price(message.order_price, request.order_type);
  request.order_quantity = convert_order_quantity(message.quantity);
  request.client_order_id = message.client_order_id;
  request.orig_client_order_id = message.orig_client_order_id;
  if (const auto& party_id = message.party_id) {
    request.parties = convert_order_parties(*party_id);
  }
  request.instrument = instrument;

  return request;
}

auto convert_to_order_cancellation_request(
    const GeneratedMessage& message,
    const simulator::InstrumentDescriptor& instrument)
    -> simulator::protocol::OrderCancellationRequest {
  simulator::protocol::OrderCancellationRequest request{
      simulator::protocol::Session{simulator::protocol::generator::Session{}}};

  request.side = message.side;
  request.client_order_id = message.client_order_id;
  request.orig_client_order_id = message.orig_client_order_id;
  request.instrument = instrument;

  return request;
}

auto convert_to_generated_message(
    simulator::protocol::OrderPlacementConfirmation const& confirmation)
    -> GeneratedMessage {
  GeneratedMessage message;
  message.message_type = MessageType::ExecutionReport;
  message.order_status = simulator::OrderStatus::Option::New;
  message.order_type = confirmation.order_type;
  message.side = confirmation.side;
  message.time_in_force = confirmation.time_in_force;
  message.order_price = confirmation.order_price;
  message.quantity = convert_order_quantity(confirmation.order_quantity);
  message.client_order_id = confirmation.client_order_id;
  return message;
}

auto convert_to_generated_message(
    simulator::protocol::OrderPlacementReject const& reject)
    -> GeneratedMessage {
  GeneratedMessage message;
  message.message_type = MessageType::ExecutionReport;
  message.order_status = simulator::OrderStatus::Option::Rejected;
  message.order_type = reject.order_type;
  message.side = reject.side;
  message.time_in_force = reject.time_in_force;
  message.order_price = reject.order_price;
  message.quantity = convert_order_quantity(reject.order_quantity);
  message.client_order_id = reject.client_order_id;
  // Text is not used in the generator. DO WE NEED IT?
  // assign(message.Text, reject.reject_text);

  return message;
}

[[nodiscard]]
auto convert_to_generated_message(
    simulator::protocol::OrderModificationConfirmation const& confirmation)
    -> GeneratedMessage {
  GeneratedMessage message;
  message.message_type = MessageType::ExecutionReport;
  message.order_status = simulator::OrderStatus::Option::Modified;
  message.order_type = confirmation.order_type;
  message.side = confirmation.side;
  message.time_in_force = confirmation.time_in_force;
  message.order_price = confirmation.order_price;
  message.quantity = get_total_quantity(confirmation.cum_executed_quantity,
                                        confirmation.leaving_quantity);
  message.client_order_id = confirmation.client_order_id;
  message.orig_client_order_id = confirmation.orig_client_order_id;
  return message;
}

auto convert_to_generated_message(
    simulator::protocol::OrderCancellationConfirmation const& confirmation)
    -> GeneratedMessage {
  GeneratedMessage message;
  message.message_type = MessageType::ExecutionReport;
  message.order_status = simulator::OrderStatus::Option::Cancelled;
  message.order_type = confirmation.order_type;
  message.side = confirmation.side;
  message.time_in_force = confirmation.time_in_force;
  message.order_price = confirmation.order_price;
  message.quantity = get_total_quantity(confirmation.cum_executed_quantity,
                                        confirmation.leaving_quantity);
  message.client_order_id = confirmation.client_order_id;
  message.orig_client_order_id = confirmation.orig_client_order_id;
  return message;
}

auto convert_to_generated_message(
    simulator::protocol::ExecutionReport const& report) -> GeneratedMessage {
  GeneratedMessage message;
  message.message_type = MessageType::ExecutionReport;
  message.order_status = report.order_status;
  message.order_type = report.order_type;
  message.side = report.side;
  message.time_in_force = report.time_in_force;
  message.order_price = report.order_price;
  message.quantity =
      get_total_quantity(report.cum_executed_quantity, report.leaves_quantity);
  message.client_order_id = report.client_order_id;
  // LastQty is not used in the generator. DO WE NEED IT?
  // assign(message.LastQty, report.executed_quantity);
  return message;
}

}  // namespace Simulator::Generator