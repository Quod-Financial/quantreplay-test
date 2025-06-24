#include "ih/historical/mapping/datasource_params.hpp"

namespace Simulator::Generator::Historical {

const DatasourceParams DatasourceParams::Postgres{
    DataLayer::Datasource::Format::Postgres, std::nullopt};
const DatasourceParams DatasourceParams::CsvNoHeader{
    DataLayer::Datasource::Format::Csv, false};
const DatasourceParams DatasourceParams::CsvHasHeader{
    DataLayer::Datasource::Format::Csv, true};

}  // namespace Simulator::Generator::Historical
