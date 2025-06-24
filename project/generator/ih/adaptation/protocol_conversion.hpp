#ifndef SIMULATOR_GENERATOR_IH_ADAPTATION_PROTOCOL_CONVERSION_HPP_
#define SIMULATOR_GENERATOR_IH_ADAPTATION_PROTOCOL_CONVERSION_HPP_

#include "core/domain/instrument_descriptor.hpp"
#include "data_layer/api/models/listing.hpp"
#include "ih/adaptation/generated_message.hpp"
#include "protocol/app/execution_report.hpp"
#include "protocol/app/order_cancellation_confirmation.hpp"
#include "protocol/app/order_cancellation_request.hpp"
#include "protocol/app/order_modification_confirmation.hpp"
#include "protocol/app/order_modification_request.hpp"
#include "protocol/app/order_placement_confirmation.hpp"
#include "protocol/app/order_placement_reject.hpp"
#include "protocol/app/order_placement_request.hpp"

namespace Simulator::Generator {

[[nodiscard]] auto convert_to_instrument_descriptor(
    const DataLayer::Listing& listing) -> simulator::InstrumentDescriptor;

[[nodiscard]] auto convert_to_order_placement_request(
    const GeneratedMessage& message,
    const simulator::InstrumentDescriptor& instrument)
    -> simulator::protocol::OrderPlacementRequest;

[[nodiscard]] auto convert_to_order_modification_request(
    const GeneratedMessage& message,
    const simulator::InstrumentDescriptor& instrument)
    -> simulator::protocol::OrderModificationRequest;

[[nodiscard]] auto convert_to_order_cancellation_request(
    const GeneratedMessage& message,
    const simulator::InstrumentDescriptor& instrument)
    -> simulator::protocol::OrderCancellationRequest;

[[nodiscard]]
auto convert_to_generated_message(
    simulator::protocol::OrderPlacementConfirmation const& confirmation)
    -> GeneratedMessage;

[[nodiscard]]
auto convert_to_generated_message(
    simulator::protocol::OrderPlacementReject const& reject)
    -> GeneratedMessage;

[[nodiscard]]
auto convert_to_generated_message(
    simulator::protocol::OrderModificationConfirmation const& confirmation)
    -> GeneratedMessage;

[[nodiscard]]
auto convert_to_generated_message(
    simulator::protocol::OrderCancellationConfirmation const& confirmation)
    -> GeneratedMessage;

[[nodiscard]]
auto convert_to_generated_message(
    simulator::protocol::ExecutionReport const& report) -> GeneratedMessage;

}  // namespace Simulator::Generator

#endif  // SIMULATOR_GENERATOR_IH_ADAPTATION_PROTOCOL_CONVERSION_HPP_
