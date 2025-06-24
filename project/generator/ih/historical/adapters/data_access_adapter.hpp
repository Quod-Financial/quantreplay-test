#ifndef SIMULATOR_GENERATOR_SRC_HISTORICAL_ADAPTERS_DATA_ACCESS_ADAPTER_HPP_
#define SIMULATOR_GENERATOR_SRC_HISTORICAL_ADAPTERS_DATA_ACCESS_ADAPTER_HPP_

#include <cstddef>
#include <functional>
#include <memory>
#include <vector>

#include "ih/historical/data/record.hpp"

namespace Simulator::DataLayer {

class Datasource;

} // namespace Simulator::DataLayer

namespace Simulator::Generator::Historical {

class DataAccessAdapter {
  public:
    using RecordVisitor = std::function<void(Historical::Record)>;

    DataAccessAdapter() = default;

    DataAccessAdapter(DataAccessAdapter const&) = default;
    auto operator=(DataAccessAdapter const&) -> DataAccessAdapter& = default;

    DataAccessAdapter(DataAccessAdapter&&) = default;
    auto operator=(DataAccessAdapter&&) -> DataAccessAdapter& = default;

    virtual ~DataAccessAdapter() = default;

    void accept(RecordVisitor const& _visitor) noexcept;

  private:
    [[nodiscard]]
    virtual auto hasNextRecord() const noexcept -> bool = 0;

    virtual auto parseNextRecord(Record::Builder& _builder) -> void = 0;
};

class DataAccessAdapterFactory {
  public:
    DataAccessAdapterFactory() = default;

    DataAccessAdapterFactory(DataAccessAdapterFactory const&) = default;
    auto operator=(DataAccessAdapterFactory const&)
        -> DataAccessAdapterFactory& = default;

    DataAccessAdapterFactory(DataAccessAdapterFactory&&) = default;
    auto operator=(DataAccessAdapterFactory&&)
        -> DataAccessAdapterFactory& = default;

    virtual ~DataAccessAdapterFactory() = default;

    [[nodiscard]]
    virtual auto createDataAdapter(DataLayer::Datasource const& _datasource
    ) const -> std::unique_ptr<DataAccessAdapter> = 0;
};

class DataAccessAdapterFactoryImpl : public DataAccessAdapterFactory {
  public:
    [[nodiscard]]
    auto createDataAdapter(DataLayer::Datasource const& _datasource) const
        -> std::unique_ptr<DataAccessAdapter> override;

  private:
    static auto createCsvReader(
         DataLayer::Datasource const & _datasource
    ) -> std::unique_ptr<DataAccessAdapter>;

    static auto createPostgresDbReader(
        DataLayer::Datasource const & _datasource
    ) -> std::unique_ptr<DataAccessAdapter>;
};

} // namespace Simulator::Generator::Historical

#endif // SIMULATOR_GENERATOR_SRC_HISTORICAL_ADAPTERS_DATA_ACCESS_ADAPTER_HPP_
