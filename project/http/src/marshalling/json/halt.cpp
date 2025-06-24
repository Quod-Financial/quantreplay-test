#include "ih/marshalling/json/halt.hpp"

#include <rapidjson/document.h>

#include "ih/marshalling/json/detail/unmarshaller.hpp"

namespace Simulator::Http::Json {

auto HaltUnmarshaller::unmarshall(
    std::string_view json, simulator::protocol::HaltPhaseRequest& request)
    -> void {
  rapidjson::Document document;
  document.Parse(json.data());
  if (!document.IsObject()) {
    throw std::runtime_error{"failed to parse Halt JSON object"};
  }

  Unmarshaller unmarshaller{document};
  if (!unmarshaller("allowCancels", request.allow_cancels)) {
    throw std::invalid_argument{"missing required field \"allowCancels\""};
  }
}

}  // namespace Simulator::Http::Json