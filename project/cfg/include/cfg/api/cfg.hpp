#ifndef SIMULATOR_CFG_API_CFG_HPP_
#define SIMULATOR_CFG_API_CFG_HPP_

#include <boost/date_time/posix_time/posix_time.hpp>
#include <string>

namespace Simulator::Cfg {

struct DbConfiguration {
  std::string name;
  std::string user;
  std::string password;
  std::string host;
  std::string port;
  std::string venue;
};

struct QuickFIXConfiguration {
  std::string sessionSettings;
};

struct VenueConfiguration {
  std::string name;
  boost::posix_time::ptime startTime;
};

struct LogConfiguration {
  std::string level;

  int maxSize = 0;
  int maxFiles = 0;
};

struct GeneratorConfiguration {
  bool enableTracing = false;
};

struct HttpConfiguration {
  enum class PeerHostResolution : std::uint8_t { Localhost, VenueId };

  PeerHostResolution peer_resolution = PeerHostResolution::Localhost;
  bool check_api_version = true;
};

auto init(const std::string& path) -> void;

auto init() -> void;

auto db() -> const DbConfiguration&;

auto quickfix() -> const QuickFIXConfiguration&;

auto venue() -> const VenueConfiguration&;

auto log() -> const LogConfiguration&;

auto generator() -> const GeneratorConfiguration&;

auto http() -> const HttpConfiguration&;

}  // namespace Simulator::Cfg

#endif  // SIMULATOR_CFG_API_CFG_HPP_
