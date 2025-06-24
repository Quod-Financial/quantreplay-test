#include <chrono>
#include <exception>
#include <functional>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ih/historical/adapters/data_access_adapter.hpp"
#include "ih/historical/data/record.hpp"
#include "mocks/historical/data_access_adapter.hpp"

using namespace Simulator;
using namespace Simulator::Generator;
using namespace Simulator::Generator::Historical;

using ::testing::Return;
using ::testing::Throw;

ACTION(CreateValidRecord)
{
    Historical::Record::Builder& recordBuilder = arg0;
    recordBuilder.with_receive_time(std::chrono::system_clock::now())
        .with_instrument("AAPL")
        .with_source_row(1);
}

ACTION(CreateMalformedRecord)
{
    Historical::Record::Builder& recordBuilder = arg0;
    recordBuilder.with_receive_time(std::chrono::system_clock::now())
        .with_source_row(1);
}

class NoopRecordVisitor {
  public:
    operator Historical::DataAccessAdapter::RecordVisitor()
    {
        return [this](auto const& _record) { accept(_record); };
    }

    MOCK_METHOD(void, accept, (Historical::Record const&));
};

TEST(Generator_Historical_DataAccessAdapter, Accept_Empty)
{
    Mock::DataAccessAdapter adapter{};
    EXPECT_CALL(adapter, hasNextRecord).Times(1).WillOnce(Return(false));
    EXPECT_CALL(adapter, parseNextRecord).Times(0);

    NoopRecordVisitor visitor{};
    EXPECT_CALL(visitor, accept).Times(0);

    EXPECT_NO_THROW(adapter.accept(visitor));
}

TEST(Generator_Historical_DataAccessAdapter, Accept_RecordParsingException)
{
    Mock::DataAccessAdapter adapter{};
    EXPECT_CALL(adapter, hasNextRecord)
        .Times(2)
        .WillOnce(Return(true))
        .WillRepeatedly(Return(false));
    EXPECT_CALL(adapter, parseNextRecord)
        .Times(1)
        .WillOnce(Throw(std::exception{}));

    NoopRecordVisitor visitor{};
    EXPECT_CALL(visitor, accept).Times(0);

    EXPECT_NO_THROW(adapter.accept(visitor));
}

TEST(Generator_Historical_DataAccessAdapter, Accept_MalformedRecord)
{
    Mock::DataAccessAdapter adapter{};
    EXPECT_CALL(adapter, hasNextRecord)
        .Times(2)
        .WillOnce(Return(true))
        .WillRepeatedly(Return(false));
    EXPECT_CALL(adapter, parseNextRecord)
        .Times(1)
        .WillOnce(CreateMalformedRecord());

    NoopRecordVisitor visitor{};
    EXPECT_CALL(visitor, accept).Times(0);

    EXPECT_NO_THROW(adapter.accept(visitor));
}

TEST(Generator_Historical_DataAccessAdapter, Accept_ValidRecord)
{
    Mock::DataAccessAdapter adapter{};
    EXPECT_CALL(adapter, hasNextRecord)
        .Times(2)
        .WillOnce(Return(true))
        .WillRepeatedly(Return(false));
    EXPECT_CALL(adapter, parseNextRecord)
        .Times(1)
        .WillOnce(CreateValidRecord());

    NoopRecordVisitor visitor{};
    EXPECT_CALL(visitor, accept).Times(1);

    EXPECT_NO_THROW(adapter.accept(visitor));
}
