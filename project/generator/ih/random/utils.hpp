#ifndef SIMULATOR_GENERATOR_IH_RANDOM_UTILS_HPP_
#define SIMULATOR_GENERATOR_IH_RANDOM_UTILS_HPP_

#include "core/domain/attributes.hpp"
#include "ih/context/order_market_data_provider.hpp"

namespace Simulator::Generator::Random::Utils {

inline auto opposite(simulator::Side side) -> simulator::Side {
  return side == simulator::Side::Option::Buy ? simulator::Side::Option::Sell
                                              : simulator::Side::Option::Buy;
}

inline auto select_price(const MarketState& mktState, simulator::Side side)
    -> std::optional<double> {
  return side == simulator::Side::Option::Buy ? mktState.bestBidPrice
                                              : mktState.bestOfferPrice;
}

}  //  namespace Simulator::Generator::Random::Utils

#endif  // SIMULATOR_GENERATOR_IH_RANDOM_UTILS_HPP_
