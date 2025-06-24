#include "src/cfg_impl.hpp"

#include <fmt/format.h>
#include <tinyxml2.h>

#include <boost/date_time/microsec_time_clock.hpp>
#include <boost/lexical_cast.hpp>
#include <exception>
#include <iostream>
#include <mutex>
#include <ostream>
#include <stdexcept>
#include <string_view>

#include "api/cfg.hpp"

namespace Simulator::Cfg {
namespace {

[[noreturn]]
auto configuration_parsing_error(std::string const& token) -> void {
  throw std::runtime_error(
      fmt::format("Could not find \"{}\" token in configuration", token));
}

auto set_config(const tinyxml2::XMLElement* element,
                std::string& value,
                const std::string& config_token,
                bool warn = true) -> bool {
  auto* child_element = element->FirstChildElement(config_token.c_str());
  if (child_element != nullptr) {
    value = child_element->GetText();
    return true;
  }

  if (warn) {
    configuration_parsing_error(config_token);
  }
  return false;
}

auto set_config(const tinyxml2::XMLElement* element,
                int& value,
                std::string const& config_token,
                bool warn = true) -> void {
  std::string value_str;
  if (set_config(element, value_str, config_token, warn)) {
    value = std::stoi(value_str);
  }
}

auto set_config(const tinyxml2::XMLElement* element,
                bool& value,
                std::string const& config_token,
                bool warn = true) -> void {
  constexpr std::string_view CfgTrueValue{"true"};
  constexpr std::string_view CfgFalseValue{"false"};

  std::string_view configuredValue{};
  if (element != nullptr) {
    auto const* pConfig = element->FirstChildElement(config_token.c_str());

    if (pConfig != nullptr) {
      configuredValue = pConfig->GetText();
    }
  }

  if (configuredValue == CfgTrueValue) {
    value = true;
  } else if (configuredValue == CfgFalseValue) {
    value = false;
  } else if (warn) {
    configuration_parsing_error(config_token);
  }
}

}  // namespace

void init() { ConfigurationImpl::instance(true); }

void init(const std::string& path) { ConfigurationImpl::instance(false, path); }

auto db() -> const DbConfiguration& {
  return ConfigurationImpl::instance().db_;
}

auto quickfix() -> const QuickFIXConfiguration& {
  return ConfigurationImpl::instance().quickfix_;
}

auto venue() -> const VenueConfiguration& {
  return ConfigurationImpl::instance().venue_;
}

auto log() -> const LogConfiguration& {
  return ConfigurationImpl::instance().log_;
}

auto generator() -> const GeneratorConfiguration& {
  return ConfigurationImpl::instance().generator_;
}

auto http() -> const HttpConfiguration& {
  return ConfigurationImpl::instance().http_;
}

auto ConfigurationImpl::instance(bool mock, const std::string& path)
    -> ConfigurationImpl& {
  std::call_once(config_init_flag, [mock, &path]() -> void {
    ConfigurationImpl::configuration_instance =
        mock ? std::make_unique<ConfigurationImpl>()
             : std::make_unique<ConfigurationImpl>(path);
  });

  if (configuration_instance) {
    return *configuration_instance;
  }

  std::cerr << "[BUG]: Configuration error: configuration is expected to be "
               "initialized, but a configuration instance does not exist. "
               "Terminating program."
            << std::endl;

  std::terminate();
}

ConfigurationImpl::ConfigurationImpl(std::string const& path) {
  tinyxml2::XMLDocument xmlDoc;
  if (xmlDoc.LoadFile(path.c_str()) != tinyxml2::XML_SUCCESS) {
    throw std::runtime_error(
        fmt::format("Could not find config token: {}", xmlDoc.ErrorStr()));
  }

  tinyxml2::XMLNode* root = xmlDoc.FirstChildElement("mktsimulator");
  if (root == nullptr) {
    throw std::runtime_error("Unable to extract config root");
  }

  auto* database = root->FirstChildElement("database");
  init_db_configuration(database);

  auto* quickfix = root->FirstChildElement("config");
  init_quickfix_configuration(quickfix);

  auto* venue = root->FirstChildElement("venue");
  init_venue_configuration(venue);

  auto* log = root->FirstChildElement("logger");
  init_log_configuration(log);

  auto* generator = root->FirstChildElement("generator");
  init_generator_configuration(generator);

  auto* http = root->FirstChildElement("http");
  init_http_configuration(http);
}

auto ConfigurationImpl::init_db_configuration(
    const tinyxml2::XMLElement* element) -> void {
  if (element == nullptr) {
    throw std::runtime_error("Could not find database token in configuration");
  }

  set_config(element, db_.name, "name");
  set_config(element, db_.user, "user");
  set_config(element, db_.password, "password");
  set_config(element, db_.host, "host", false);
  set_config(element, db_.port, "port");
}

auto ConfigurationImpl::init_quickfix_configuration(
    const tinyxml2::XMLElement* element) -> void {
  if (element == nullptr) {
    configuration_parsing_error("config");
  }

  quickfix_.sessionSettings = element->GetText();
}

auto ConfigurationImpl::init_venue_configuration(
    const tinyxml2::XMLElement* element) -> void {
  if (element == nullptr) {
    configuration_parsing_error("venue");
  }

  venue_.name = element->GetText();
  venue_.startTime = boost::posix_time::microsec_clock::local_time();
}

auto ConfigurationImpl::init_log_configuration(
    const tinyxml2::XMLElement* element) -> void {
  if (element == nullptr) {
    configuration_parsing_error("logger");
  }

  set_config(element, log_.level, "level");
  set_config(element, log_.maxSize, "maxSize");
  set_config(element, log_.maxFiles, "maxFiles");

  if (log_.maxFiles <= 0 || log_.maxSize <= 0) {
    throw std::runtime_error(
        "maxSize and maxFiles must be integer value greater than 0");
  }
  log_.maxFiles--;
}

auto ConfigurationImpl::init_generator_configuration(
    const tinyxml2::XMLElement* element) -> void {
  if (element == nullptr) {
    return;
  }

  set_config(element, generator_.enableTracing, "enableTracing", false);
}

auto ConfigurationImpl::init_http_configuration(
    const tinyxml2::XMLElement* element) -> void {
  if (element == nullptr) {
    return;
  }

  {
    std::string peer_resolution;
    set_config(element, peer_resolution, "peerHostResolution", false);

    if (!peer_resolution.empty()) {
      if (peer_resolution == "localhost") {
        http_.peer_resolution =
            HttpConfiguration::PeerHostResolution::Localhost;
      } else if (peer_resolution == "venue") {
        http_.peer_resolution = HttpConfiguration::PeerHostResolution::VenueId;
      } else {
        throw std::runtime_error(
            "unknown value for peerHostResolution config token");
      }
    }
  }

  set_config(element, http_.check_api_version, "checkApiVersion", false);
}

std::unique_ptr<ConfigurationImpl> ConfigurationImpl::configuration_instance{
    nullptr};

std::once_flag ConfigurationImpl::config_init_flag;

}  // namespace Simulator::Cfg
