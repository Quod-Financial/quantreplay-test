#ifndef SIMULATOR_HTTP_IH_MARSHALLING_JSON_HALT_HPP_
#define SIMULATOR_HTTP_IH_MARSHALLING_JSON_HALT_HPP_

#include <string_view>

#include "protocol/admin/trading_phase.hpp"

namespace Simulator::Http::Json {

class HaltUnmarshaller {
 public:
  static auto unmarshall(std::string_view json,
                         simulator::protocol::HaltPhaseRequest& request)
      -> void;
};

}  // namespace Simulator::Http::Json

#endif  // SIMULATOR_HTTP_IH_MARSHALLING_JSON_HALT_HPP_
