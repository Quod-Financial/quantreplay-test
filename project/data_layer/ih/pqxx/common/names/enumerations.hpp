#ifndef SIMULATOR_DATA_LAYER_IH_PQXX_COMMON_NAMES_ENUMERATIONS_HPP_
#define SIMULATOR_DATA_LAYER_IH_PQXX_COMMON_NAMES_ENUMERATIONS_HPP_

#include <string_view>

namespace Simulator::DataLayer::Pqxx {
namespace DatasourceFormat {

constexpr std::string_view Csv{"CSV"};
constexpr std::string_view Postgres{"PSQL"};

} // namespace DatasourceFormat

namespace DatasourceType {

constexpr std::string_view OrderBook{"OrderBook"};

} // namespace DatasourceType

namespace MarketPhaseType {

constexpr std::string_view Open{"Open"};
constexpr std::string_view Closed{"Closed"};
constexpr std::string_view OpeningAuction{"PreOpen"};
constexpr std::string_view ClosingAuction{"PreClose"};
constexpr std::string_view IntradayAuction{"Auction"};
constexpr std::string_view Halted{"Halted"};

} // namespace MarketPhaseKind

namespace VenueEngineType {

constexpr std::string_view Matching{"Matching"};
constexpr std::string_view Quoting{"Quoting"};

} // namespace VenueEngineType
} // namespace Simulator::DataLayer::Pqxx

#endif // SIMULATOR_DATA_LAYER_IH_PQXX_COMMON_NAMES_ENUMERATIONS_HPP_
