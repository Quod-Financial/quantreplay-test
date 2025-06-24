#include "ih/context/order_market_data_provider.hpp"

#include "core/domain/instrument_descriptor.hpp"
#include "middleware/routing/trading_request_channel.hpp"

namespace Simulator::Generator {
namespace {

[[nodiscard]] auto convert_to_market_state(
    simulator::protocol::InstrumentState instrument_state) noexcept
    -> MarketState {
  MarketState market_state;
  if (instrument_state.best_bid_price.has_value()) {
    market_state.bestBidPrice = instrument_state.best_bid_price->value();
  }
  if (instrument_state.current_bid_depth.has_value()) {
    market_state.bidDepthLevels = instrument_state.current_bid_depth->value();
  }
  if (instrument_state.best_offer_price.has_value()) {
    market_state.bestOfferPrice = instrument_state.best_offer_price->value();
  }
  if (instrument_state.current_bid_depth.has_value()) {
    market_state.offerDepthLevels =
        instrument_state.current_offer_depth->value();
  }
  return market_state;
}

}  // namespace

OrderMarketDataProvider::OrderMarketDataProvider(
    simulator::InstrumentDescriptor order_instrument)
    : instrument_descriptor_(std::move(order_instrument)) {}

auto OrderMarketDataProvider::getMarketState() const -> MarketState {
  simulator::protocol::InstrumentStateRequest request;
  simulator::protocol::InstrumentState reply;

  request.instrument = instrument_descriptor_;
  try {
    simulator::middleware::send_trading_request(request, reply);
  } catch (...) {}

  return convert_to_market_state(reply);
}

}  // namespace Simulator::Generator
