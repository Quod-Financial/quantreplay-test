#ifndef SIMULATOR_GENERATOR_SRC_HISTORICAL_PROVIDERS_DATA_PROVIDER_HPP_
#define SIMULATOR_GENERATOR_SRC_HISTORICAL_PROVIDERS_DATA_PROVIDER_HPP_

#include <chrono>
#include <deque>
#include <functional>
#include <memory>

#include "ih/historical/adapters/data_access_adapter.hpp"
#include "ih/historical/data/record.hpp"
#include "ih/historical/data/time.hpp"

namespace Simulator::DataLayer {

class Datasource;

} // namespace Simulator::DataLayer

namespace Simulator::Generator::Historical {

class DataProvider {
  public:
    using ActionPuller = std::function<void(Historical::Action)>;

    DataProvider() = default;

    DataProvider(DataProvider const&) = default;
    auto operator=(DataProvider const&) -> DataProvider& = default;

    DataProvider(DataProvider&&) = default;
    auto operator=(DataProvider&&) -> DataProvider& = default;

    virtual ~DataProvider() = default;


    auto prepare(Historical::DataAccessAdapter& _adapter) -> std::size_t;

    auto pullAction(ActionPuller const& _puller) -> void;


    [[nodiscard]]
    virtual auto isEmpty() const noexcept -> bool = 0;

    virtual auto initializeTimeOffset() noexcept -> void = 0;


    [[nodiscard]]
    auto hasTimeOffset() const noexcept -> bool;

    [[nodiscard]]
    auto getTimeOffset() const noexcept -> Historical::Duration;

  protected:
    auto setTimeOffset(Historical::Duration _offset) noexcept -> void;

  private:
    virtual auto add(Historical::Record _record) -> void = 0;

    virtual auto pullInto(Action::Builder& _pulledActionBuilder) -> void = 0;


    std::optional<Historical::Duration> mOptTimeOffset;
};

class FiniteProvider final : public Historical::DataProvider {
  public:
    [[nodiscard]]
    auto isEmpty() const noexcept -> bool override;

    auto initializeTimeOffset() noexcept -> void override;

  private:
    auto add(Historical::Record _record) -> void override;

    auto pullInto(Historical::Action::Builder& _pulledActionBuilder)
        -> void override;


    std::deque<Historical::Record> mRecords;
};

class RepeatingProvider final : public Historical::DataProvider {
  public:
    [[nodiscard]]
    auto isEmpty() const noexcept -> bool override;

    auto initializeTimeOffset() noexcept -> void override;

  private:
    void add(Historical::Record _record) override;

    void pullInto(Historical::Action::Builder& _pulledActionBuilder) override;


    std::deque<Historical::Record> mRecords;
    std::deque<Historical::Record> mProcessedRecords;
};


class DataProvidersFactory {
  public:
    DataProvidersFactory() = default;

    DataProvidersFactory(DataProvidersFactory const&) = default;
    auto operator=(DataProvidersFactory const&)
        -> DataProvidersFactory& = default;

    DataProvidersFactory(DataProvidersFactory&&) = default;
    auto operator=(DataProvidersFactory&&) -> DataProvidersFactory& = default;

    virtual ~DataProvidersFactory() = default;

    [[nodiscard]]
    virtual auto createProvider(DataLayer::Datasource const& _datasource) const
        -> std::unique_ptr<DataProvider> = 0;
};


class DataProvidersFactoryImpl final : public DataProvidersFactory {
  public:
    DataProvidersFactoryImpl();

    explicit DataProvidersFactoryImpl(
        std::unique_ptr<DataAccessAdapterFactory> _pDataAdaptersFactory
    ) noexcept;

    [[nodiscard]]
    auto createProvider(DataLayer::Datasource const& _datasource) const
        -> std::unique_ptr<DataProvider> override;

  private:
    std::unique_ptr<DataAccessAdapterFactory> mDataAdapterFactory;
};

} // namespace Simulator::Generator::Historical

#endif // SIMULATOR_GENERATOR_SRC_HISTORICAL_PROVIDERS_DATA_PROVIDER_HPP_
