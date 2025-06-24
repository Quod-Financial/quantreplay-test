#include "ih/historical/processor.hpp"

#include <memory>
#include <string_view>
#include <vector>

#include "ih/adaptation/protocol_conversion.hpp"
#include "ih/context/instrument_context.hpp"
#include "ih/historical/data/record.hpp"
#include "ih/historical/record_applier.hpp"
#include "log/logging.hpp"
#include "middleware/routing/trading_request_channel.hpp"

namespace Simulator::Generator::Historical {
namespace {

auto send_message(const GeneratedMessage& message,
                  const simulator::InstrumentDescriptor& instrument) -> void {
  try {
    if (message.message_type == MessageType::NewOrderSingle) {
      simulator::middleware::send_trading_request(
          convert_to_order_placement_request(message, instrument));
    } else if (message.message_type == MessageType::OrderCancelReplaceRequest) {
      simulator::middleware::send_trading_request(
          convert_to_order_modification_request(message, instrument));
    } else if (message.message_type == MessageType::OrderCancelRequest) {
      simulator::middleware::send_trading_request(
          convert_to_order_cancellation_request(message, instrument));
    }
  } catch (const simulator::middleware::ChannelUnboundError&) {
    simulator::log::err(
        "failed to send message from historical replier - trading request "
        "channel is not bound");
  }
}

}  // namespace

ActionProcessor::ContextsRegistry::ContextsRegistry(
    ActionProcessor::Contexts const& _availableContexts) noexcept {
  for (auto const& pContext : _availableContexts) {
    if (!pContext) {
      continue;
    }

    const auto& symbol = pContext->getInstrument().getSymbol();
    if (symbol && !symbol->empty()) {
      mRegistry.emplace(*symbol, pContext);
    } else {
      simulator::log::err(
          "failed to insert listing with listing_id {} into registry because its symbol is empty",
          pContext->getInstrument().getListingId());
    }
  }
}

auto ActionProcessor::ContextsRegistry::resolveContext(
    Historical::Record const& _record) const
    -> std::shared_ptr<OrderInstrumentContext> {
  std::string_view const instrument = _record.instrument();
  auto targetIt = mRegistry.find(instrument);
  return targetIt != std::end(mRegistry) ? targetIt->second : nullptr;
}

ActionProcessor::ActionProcessor(
    ActionProcessor::Contexts const& _availableContexts)
    : mCtxRegistry{_availableContexts} {}

auto ActionProcessor::process(Historical::Action _action) -> void {
  _action.steal_records(
      [this](Historical::Record _record) { process(std::move(_record)); });
}

auto ActionProcessor::process(Historical::Record _record) const -> void {
  ContextPointer const pContext = mCtxRegistry.resolveContext(_record);
  if (!pContext) {
    simulator::log::warn(
        "can not find corresponding instrument generation context, "
        "skipping historical {}",
        _record);
    return;
  }

  std::vector<GeneratedMessage> const historicalMessages =
      RecordApplier::apply(std::move(_record), pContext);
  const simulator::InstrumentDescriptor& instrument =
      pContext->getInstrumentDescriptor();

  for (auto const& historicalRequest : historicalMessages) {
    send_message(historicalRequest, instrument);
  }
}

}  // namespace Simulator::Generator::Historical
