#ifndef SIMULATOR_CFG_SRC_CFG_IMPL_HPP_
#define SIMULATOR_CFG_SRC_CFG_IMPL_HPP_

#include <memory>
#include <mutex>

#include "cfg/api/cfg.hpp"

namespace tinyxml2 {
class XMLElement;
}

namespace Simulator::Cfg {

class ConfigurationImpl {
 public:
  static auto instance(bool mock = false, const std::string& path = {})
      -> ConfigurationImpl&;

  ConfigurationImpl() = default;

  explicit ConfigurationImpl(const std::string& path);

  DbConfiguration db_;

  QuickFIXConfiguration quickfix_;

  VenueConfiguration venue_;

  LogConfiguration log_;

  GeneratorConfiguration generator_;

  HttpConfiguration http_;

 private:
  auto init_db_configuration(const tinyxml2::XMLElement* element) -> void;

  auto init_quickfix_configuration(const tinyxml2::XMLElement* element) -> void;

  auto init_venue_configuration(const tinyxml2::XMLElement* element) -> void;

  auto init_log_configuration(const tinyxml2::XMLElement* element) -> void;

  auto init_generator_configuration(const tinyxml2::XMLElement* element)
      -> void;

  auto init_http_configuration(const tinyxml2::XMLElement* element) -> void;

  static std::unique_ptr<ConfigurationImpl> configuration_instance;
  static std::once_flag config_init_flag;
};

}  // namespace Simulator::Cfg

#endif  // SIMULATOR_CFG_SRC_CFG_IMPL_HPP_
