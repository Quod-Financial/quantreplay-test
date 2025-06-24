#ifndef SIMULATOR_PROJECT_V2_HTTP_IH_SERVER_HPP_
#define SIMULATOR_PROJECT_V2_HTTP_IH_SERVER_HPP_

#include <pistache/endpoint.h>

#include "data_layer/api/database/context.hpp"
#include "http/http.hpp"

namespace simulator::http {

struct Server::Implementation {
 public:
  Implementation(std::uint16_t accept_port,
                 Simulator::DataLayer::Database::Context database);

  auto launch() -> void;

  auto terminate() -> void;

 private:
  static auto create_endpoint(std::uint16_t accept_port)
      -> std::unique_ptr<Pistache::Http::Endpoint>;

  auto register_custom_headers() -> void;

  auto setup_handler(Simulator::DataLayer::Database::Context database) -> void;

  std::unique_ptr<Pistache::Http::Endpoint> endpoint_;
};

}  // namespace simulator::http

#endif  // SIMULATOR_PROJECT_V2_HTTP_IH_SERVER_HPP_