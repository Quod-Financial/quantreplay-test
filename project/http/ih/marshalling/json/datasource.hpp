#ifndef SIMULATOR_HTTP_IH_MARSHALLING_JSON_DATASOURCE_HPP_
#define SIMULATOR_HTTP_IH_MARSHALLING_JSON_DATASOURCE_HPP_

#include <string>
#include <string_view>
#include <vector>

#include <rapidjson/document.h>

#include "data_layer/api/models/column_mapping.hpp"
#include "data_layer/api/models/datasource.hpp"

namespace Simulator::Http::Json {

class DatasourceMarshaller {
  public:
    static auto marshall(DataLayer::Datasource const& _datasource)
        -> std::string;

    static auto marshall(std::vector<DataLayer::Datasource> const& _datasources)
        -> std::string;

  private:
    static auto marshall(
        DataLayer::Datasource const& _datasource,
        rapidjson::Document& _dest
    ) -> void;

    static auto marshall(
        std::vector<DataLayer::ColumnMapping> const& _columnsMapping,
        rapidjson::Document& _parent
    ) -> void;
};


class DatasourceUnmarshaller {
  public:
    static auto unmarshall(
        std::string_view _json,
        DataLayer::Datasource::Patch& _dest
    ) -> void;

  private:
    static auto unmarshallColumnMapping(
        rapidjson::Document const& _datasourceDoc,
        DataLayer::Datasource::Patch& _dest
    ) -> void;
};

} // namespace Simulator::Http::Json

#endif // SIMULATOR_HTTP_IH_MARSHALLING_JSON_DATASOURCE_HPP_
