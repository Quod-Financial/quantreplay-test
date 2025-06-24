#include "ih/historical/adapters/data_access_adapter.hpp"

#include <stdexcept>
#include <utility>

#include "data_layer/api/models/datasource.hpp"
#include "ih/historical/adapters/csv_reader.hpp"
#include "ih/historical/adapters/postgresql_connector.hpp"
#include "ih/historical/data/record.hpp"
#include "ih/historical/mapping/params.hpp"
#include "ih/historical/parsing/params.hpp"
#include "log/logging.hpp"

namespace Simulator::Generator::Historical {

auto DataAccessAdapter::accept(RecordVisitor const& _visitor) noexcept -> void
{
    while (hasNextRecord()) {
        Record::Builder builder{};

        try {
            parseNextRecord(builder);
        } catch (std::exception const& _ex) {
            simulator::log::warn("failed to parse historical datasource row: {}",
                                 _ex.what());
            continue;
        }

        try {
            Record record = Record::Builder::construct(std::move(builder));
            _visitor(std::move(record));
        } catch (std::exception const& _ex) {
            simulator::log::warn("failed to process historical data record: {}",
                                 _ex.what());
        }
    }
}

auto DataAccessAdapterFactoryImpl::createDataAdapter(
        DataLayer::Datasource const & _datasource
) const -> std::unique_ptr<DataAccessAdapter>
{
    using Format = DataLayer::Datasource::Format;
    switch (_datasource.format()) {
        case Format::Csv:
            return createCsvReader(_datasource);
        case Format::Postgres:
            return createPostgresDbReader(_datasource);
    }

    throw std::runtime_error{"unable to determine datasource format"};
}

auto DataAccessAdapterFactoryImpl::createCsvReader(
    DataLayer::Datasource const& _datasource
) -> std::unique_ptr<DataAccessAdapter>
{
    CsvParsingParams parsing = make_csv_parsing_params(_datasource);
    MappingParams mapping = make_mapping_params(_datasource);
    return CsvReader::create(std::move(parsing), std::move(mapping));
}

auto DataAccessAdapterFactoryImpl::createPostgresDbReader(
    DataLayer::Datasource const& _datasource
) -> std::unique_ptr<DataAccessAdapter>
{
    DatabaseParsingParams parsing = make_database_parsing_params(_datasource);
    MappingParams mapping = make_mapping_params(_datasource);
    return PostgresConnector::create(std::move(parsing), std::move(mapping));
}

} // namespace Simulator::Generator::Historical
