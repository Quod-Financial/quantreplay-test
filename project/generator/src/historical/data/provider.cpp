#include "ih/historical/data/provider.hpp"

#include <cassert>
#include <memory>
#include <optional>
#include <utility>

#include "data_layer/api/models/datasource.hpp"
#include "ih/historical/adapters/data_access_adapter.hpp"
#include "ih/historical/data/record.hpp"
#include "ih/historical/data/time.hpp"
#include "log/logging.hpp"

namespace Simulator::Generator::Historical {

auto DataProvider::prepare(Historical::DataAccessAdapter& _adapter)
    -> std::size_t
{
    std::size_t recordsAccepted = 0;
    _adapter.accept([this, &recordsAccepted](Historical::Record _record) {
        add(std::move(_record));
        ++recordsAccepted;
    });
    return recordsAccepted;
}

auto DataProvider::pullAction(DataProvider::ActionPuller const& _puller) -> void
{
    if (isEmpty()) {
        throw std::logic_error{
            "unable to pull action from data provider - "
            "data provider has no data to provide"};
    }

    if (!hasTimeOffset()) {
        initializeTimeOffset();
    }

    Action::Builder pulledActionBuilder{};
    pullInto(pulledActionBuilder);

    auto action = Action::Builder::construct(std::move(pulledActionBuilder));
    _puller(std::move(action));
}

auto DataProvider::hasTimeOffset() const noexcept -> bool
{
    return mOptTimeOffset.has_value();
}

auto DataProvider::getTimeOffset() const noexcept -> Historical::Duration
{
    return mOptTimeOffset.value_or(Historical::Duration{0});
}

auto DataProvider::setTimeOffset(Historical::Duration _offset) noexcept -> void
{
    mOptTimeOffset = _offset;
}


auto FiniteProvider::isEmpty() const noexcept -> bool
{
    return mRecords.empty();
}

auto FiniteProvider::initializeTimeOffset() noexcept -> void
{
    if (isEmpty()) {
        return;
    }

    assert(!mRecords.empty());
    auto const& nextRecTime = mRecords.front().receive_time();
    auto const timeOffset = Historical::Time::makeOffset(nextRecTime);
    DataProvider::setTimeOffset(timeOffset);
}

auto FiniteProvider::add(Historical::Record _record) -> void
{
    mRecords.emplace_back(std::move(_record));
}

auto FiniteProvider::pullInto(Historical::Action::Builder& _pulledActionBuilder)
    -> void
{
    // Should be checked in parent template method
    assert(hasTimeOffset());
    assert(!mRecords.empty());

    auto const timeOffset = getTimeOffset();
    std::optional<Historical::Timepoint> prevRecTime{};
    while (!mRecords.empty()) {
        auto const nextRecTime = mRecords.front().receive_time();
        if (prevRecTime.value_or(nextRecTime) != nextRecTime) {
            break;
        }

        prevRecTime = nextRecTime;
        auto nextRec = std::move(mRecords.front());
        mRecords.pop_front();

        _pulledActionBuilder.add(std::move(nextRec), timeOffset);
    }
}


auto RepeatingProvider::isEmpty() const noexcept -> bool
{
    return mRecords.empty() && mProcessedRecords.empty();
}

auto RepeatingProvider::initializeTimeOffset() noexcept -> void
{
    if (isEmpty()) {
        return;
    }

    if (mRecords.empty()) {
        assert(!mProcessedRecords.empty());
        std::swap(mRecords, mProcessedRecords);
    }

    assert(!mRecords.empty());
    auto const& nextRecTime = mRecords.front().receive_time();
    auto const timeOffset = Historical::Time::makeOffset(nextRecTime);
    DataProvider::setTimeOffset(timeOffset);
}

auto RepeatingProvider::add(Historical::Record _record) -> void
{
    mRecords.emplace_back(std::move(_record));
}


auto RepeatingProvider::pullInto(
    Historical::Action::Builder& _pulledActionBuilder
) -> void
{
    // Should be checked in parent template method
    assert(!isEmpty());

    if (mRecords.empty()) {
        assert(!mProcessedRecords.empty());
        std::swap(mRecords, mProcessedRecords);
        initializeTimeOffset();
    }

    assert(hasTimeOffset());
    assert(!mRecords.empty());

    auto const timeOffset = getTimeOffset();
    std::optional<Historical::Timepoint> prevRecTime{};
    while (!mRecords.empty()) {
        auto const nextRecTime = mRecords.front().receive_time();
        if (prevRecTime.value_or(nextRecTime) != nextRecTime) {
            break;
        }

        prevRecTime = nextRecTime;

        auto nextRec = std::move(mRecords.front());
        mRecords.pop_front();
        mProcessedRecords.push_back(nextRec);

        _pulledActionBuilder.add(std::move(nextRec), timeOffset);
    }
}


DataProvidersFactoryImpl::DataProvidersFactoryImpl() :
    DataProvidersFactoryImpl(std::make_unique<DataAccessAdapterFactoryImpl>())
{}

DataProvidersFactoryImpl::DataProvidersFactoryImpl(
    std::unique_ptr<DataAccessAdapterFactory> _pDataAdaptersFactory
) noexcept :
    mDataAdapterFactory{std::move(_pDataAdaptersFactory)}
{
    assert(mDataAdapterFactory);
}

auto DataProvidersFactoryImpl::createProvider(
    DataLayer::Datasource const& _datasource
) const -> std::unique_ptr<DataProvider>
{
    auto pDataAdapter = mDataAdapterFactory->createDataAdapter(_datasource);
    auto pDataProvider = [&]() -> std::unique_ptr<DataProvider> {
        if (_datasource.repeat_flag().value_or(false)) {
            return std::make_unique<RepeatingProvider>();
        }
        return std::make_unique<FiniteProvider>();
    }();

    assert(pDataAdapter);
    assert(pDataProvider);

    std::size_t const recordsRead = pDataProvider->prepare(*pDataAdapter);

    simulator::log::info(
        "created a data provider for a `{}' datasource (DatasourceID: "
        "{} connection: {}) with {} historical records prepared",
        _datasource.name(),
        _datasource.datasource_id(),
        _datasource.connection(),
        recordsRead);

    return pDataProvider;
}

} // namespace Simulator::Generator::Historical
