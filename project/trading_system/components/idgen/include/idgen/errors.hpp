#ifndef SIMULATOR_TRADING_SYSTEM_COMPONENTS_IDGEN_ERRORS_HPP_
#define SIMULATOR_TRADING_SYSTEM_COMPONENTS_IDGEN_ERRORS_HPP_

#include <cstdint>
#include <nonstd/expected.hpp>

namespace simulator::trading_system::idgen {

enum class GenerationError : std::uint8_t { CollisionDetected };

template <typename T>
using ExpectedId = nonstd::expected<T, GenerationError>;

}  // namespace simulator::trading_system::idgen

#endif  // SIMULATOR_TRADING_SYSTEM_COMPONENTS_IDGEN_ERRORS_HPP_
