#ifndef SIMULATOR_GENERATOR_IH_HISTORICAL_ADAPTERS_POSTGRESQL_CONNECTOR_HPP_
#define SIMULATOR_GENERATOR_IH_HISTORICAL_ADAPTERS_POSTGRESQL_CONNECTOR_HPP_

#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include <pqxx/connection>
#include <pqxx/row>

#include "ih/historical/adapters/data_access_adapter.hpp"
#include "ih/historical/mapping/params.hpp"
#include "ih/historical/parsing/params.hpp"

namespace Simulator::Generator::Historical {

class PostgresConnector : public Historical::DataAccessAdapter {
  public:
    PostgresConnector() = delete;

    PostgresConnector(
        Historical::DatabaseParsingParams _params,
        Historical::MappingParams _mappingParams,
        pqxx::connection _connection
    );

    [[nodiscard]]
    static auto create(
        Historical::DatabaseParsingParams _params,
        Historical::MappingParams _mappingParams
    ) -> std::unique_ptr<PostgresConnector>;

  private:
    [[nodiscard]]
    auto hasNextRecord() const noexcept -> bool override;

    auto parseNextRecord(Record::Builder& _builder) -> void override;


    auto loadData(pqxx::connection& _databaseConnection) -> void;

    auto make_depth_config(pqxx::connection& database_connection) const -> Mapping::DepthConfig;

    auto initMappingParams(Mapping::DepthConfig depth_config) -> void;

    auto extractData() -> void;

    auto columns_number(pqxx::connection& database_connection) const -> std::uint32_t;

    MappingParams mMappingParams;
    DatabaseParsingParams mParsingParams;
    std::uint32_t depth_{0};

    pqxx::result mData;

    std::vector<std::pair<std::uint64_t, pqxx::row>> mExtractedRows;
    std::uint64_t mNextRowIdx{0};
};

} // namespace Simulator::Generator::Historical

#endif // SIMULATOR_GENERATOR_IH_HISTORICAL_ADAPTERS_POSTGRESQL_CONNECTOR_HPP_
