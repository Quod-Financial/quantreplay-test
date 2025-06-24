#ifndef SIMULATOR_GENERATOR_TESTS_MOCKS_HISTORICAL_DATA_ACCESS_ADAPTER_HPP_
#define SIMULATOR_GENERATOR_TESTS_MOCKS_HISTORICAL_DATA_ACCESS_ADAPTER_HPP_

#include <cassert>
#include <deque>

#include <gmock/gmock.h>

#include "ih/historical/adapters/data_access_adapter.hpp"
#include "ih/historical/data/record.hpp"

namespace Simulator::Generator::Mock {

class DataAccessAdapter
    :   public Historical::DataAccessAdapter
{
public:

    MOCK_METHOD(bool, hasNextRecord, (), (const, noexcept, override));

    MOCK_METHOD(void, parseNextRecord, (Historical::Record::Builder &), (override));
};

} // namespace Simulator::Generator::Mock

namespace Simulator::Generator::Fake {

class DataAccessAdapter
    :   public Historical::DataAccessAdapter
{
public:

    [[nodiscard]]
    bool hasNextRecord() const noexcept override
    {
        return !m_assignedRecords.empty();
    }

    void parseNextRecord(Historical::Record::Builder & _builder) override
    {
        assert(hasNextRecord());
        _builder = std::move(m_assignedRecords.front());
        m_assignedRecords.pop_front();
    }

    void pushRecordBuilder(Historical::Record::Builder _builder)
    {
        m_assignedRecords.emplace_back(std::move(_builder));
    }

private:

    std::deque<Historical::Record::Builder> m_assignedRecords;
};

} // namespace Simulator::Generator::Fake

#endif // SIMULATOR_GENERATOR_TESTS_MOCKS_HISTORICAL_DATA_ACCESS_ADAPTER_HPP_
