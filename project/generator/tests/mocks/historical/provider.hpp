#ifndef SIMULATOR_PROJECT_GENERATOR_TESTS_MOCKS_HISTORICAL_PROVIDER_HPP_
#define SIMULATOR_PROJECT_GENERATOR_TESTS_MOCKS_HISTORICAL_PROVIDER_HPP_

#include <gmock/gmock.h>

#include "ih/historical/data/time.hpp"
#include "ih/historical/data/provider.hpp"
#include "ih/historical/data/record.hpp"

namespace Simulator::Generator::Mock {

class DataProvider
    :   public Historical::DataProvider
{
public:

    MOCK_METHOD(bool, isEmpty, (), (const, noexcept, override));

    MOCK_METHOD(void, initializeTimeOffset, (), (noexcept, override));

    MOCK_METHOD(void, add, (Historical::Record), (override));

    MOCK_METHOD(void, pullInto, (Historical::Action::Builder &), (override));

    void assignTimeOffset(Historical::Duration _offset)
    {
        setTimeOffset(_offset);
    }
};

} // namespace Simulator::Generator::Mock

#endif// SIMULATOR_PROJECT_GENERATOR_TESTS_MOCKS_HISTORICAL_PROVIDER_HPP_
