#ifndef SIMULATOR_GENERATOR_IH_CONTEXT_ORDER_MARKET_DATA_PROVIDER_HPP_
#define SIMULATOR_GENERATOR_IH_CONTEXT_ORDER_MARKET_DATA_PROVIDER_HPP_

#include "core/domain/instrument_descriptor.hpp"

namespace Simulator::Generator {

struct MarketState {
  std::optional<double> bestBidPrice;
  std::optional<double> bestOfferPrice;
  std::optional<std::uint32_t> bidDepthLevels;
  std::optional<std::uint32_t> offerDepthLevels;
};

class OrderMarketDataProvider {
 public:
  explicit OrderMarketDataProvider(
      simulator::InstrumentDescriptor order_instrument);

  [[nodiscard]] auto getMarketState() const -> MarketState;

 private:
  simulator::InstrumentDescriptor instrument_descriptor_;
};

}  // namespace Simulator::Generator

#endif  // SIMULATOR_GENERATOR_IH_CONTEXT_ORDER_MARKET_DATA_PROVIDER_HPP_
