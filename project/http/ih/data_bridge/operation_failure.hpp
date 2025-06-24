#ifndef SIMULATOR_HTTP_IH_DATA_BRIDGE_OPERATION_FAILURE_HPP_
#define SIMULATOR_HTTP_IH_DATA_BRIDGE_OPERATION_FAILURE_HPP_

namespace Simulator::Http::DataBridge {

enum class Failure : std::uint8_t {
  MalformedInput,
  DatabaseConnectionError,
  ResponseDecodingError,
  ResponseCardinalityError,
  DataIntegrityViolationError,
  UnknownError
};

struct FailureInfo {
  Failure failure;
  std::string message;
};

}  // namespace Simulator::Http::DataBridge

#endif  // SIMULATOR_HTTP_IH_DATA_BRIDGE_OPERATION_FAILURE_HPP_
