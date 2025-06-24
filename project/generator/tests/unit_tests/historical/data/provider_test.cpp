#include <chrono>
#include <initializer_list>
#include <stdexcept>
#include <thread>
#include <tuple>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ih/historical/data/provider.hpp"
#include "ih/historical/data/record.hpp"
#include "ih/historical/data/time.hpp"
#include "mocks/historical/data_access_adapter.hpp"
#include "mocks/historical/provider.hpp"
#include "tests/test_utils/historical_data_utils.hpp"

namespace Simulator::Generator::Historical {
namespace {

using ::testing::Return;

class Generator_Historical_DataProvider : public testing::Test {
  public:
    using RecordAttributes =
        std::tuple<Historical::Timepoint, std::string, std::uint64_t>;

    static auto makeTestRecordBuilder() -> Historical::Record::Builder
    {
        static std::string const defaultInstrument{"AAPL"};
        constexpr std::uint64_t defaultSourceRow = 3;
        constexpr auto defaultTime =
            Historical::Timepoint{Historical::Duration{100}};

        Historical::Record::Builder recordBuilder{};
        recordBuilder.with_receive_time(defaultTime)
            .with_instrument(defaultInstrument)
            .with_source_row(defaultSourceRow);

        return recordBuilder;
    }

    static auto makeTestRecordBuilder(RecordAttributes const& _attributes)
        -> Historical::Record::Builder
    {
        Historical::Record::Builder builder{};
        builder.with_receive_time(std::get<0>(_attributes));
        builder.with_instrument(std::get<1>(_attributes));
        builder.with_source_row(std::get<2>(_attributes));
        return builder;
    }

    static auto makeTestRecord() -> Historical::Record
    {
        return Historical::Record::Builder::construct(makeTestRecordBuilder());
    }

    static auto makeFakeDataAdapter(std::uint32_t _numStoredRecords)
        -> Fake::DataAccessAdapter
    {
        Fake::DataAccessAdapter adapter{};

        for (std::uint32_t recNum = 0; recNum < _numStoredRecords; ++recNum) {
            adapter.pushRecordBuilder(makeTestRecordBuilder());
        }

        return adapter;
    }

    static auto makeFakeDataAdapter(
        std::initializer_list<RecordAttributes> _storedRecordsAttributes
    ) -> Fake::DataAccessAdapter
    {
        Fake::DataAccessAdapter adapter{};
        for (RecordAttributes const& attributes : _storedRecordsAttributes) {
            adapter.pushRecordBuilder(makeTestRecordBuilder(attributes));
        }
        return adapter;
    }
};

class Generator_Historical_FiniteProvider
    : public Generator_Historical_DataProvider {};

class Generator_Historical_RepeatingProvider
    : public Generator_Historical_DataProvider {};

ACTION_P(ConstructActionWith, HistoricalRecord)
{
    constexpr auto timeOffset = Historical::Duration{100};
    Historical::Action::Builder& actionBuilder = arg0;
    Historical::Record record = HistoricalRecord;

    actionBuilder.add(std::move(record), timeOffset);
}

TEST_F(Generator_Historical_DataProvider, TimeOffset)
{
    constexpr Historical::Duration offset{100};

    Mock::DataProvider provider{};
    EXPECT_FALSE(provider.hasTimeOffset());
    EXPECT_EQ(provider.getTimeOffset(), Historical::Duration{0});

    provider.assignTimeOffset(offset);
    EXPECT_TRUE(provider.hasTimeOffset());
    EXPECT_EQ(provider.getTimeOffset(), offset);
}

TEST_F(Generator_Historical_DataProvider, Prepare_Empty)
{
    Fake::DataAccessAdapter adapter = makeFakeDataAdapter(0);
    ASSERT_FALSE(adapter.hasNextRecord());

    Mock::DataProvider provider{};
    EXPECT_CALL(provider, add).Times(0);

    EXPECT_EQ(provider.prepare(adapter), 0);
}

TEST_F(Generator_Historical_DataProvider, Prepare_SingleRecord)
{
    Fake::DataAccessAdapter adapter = makeFakeDataAdapter(1);
    ASSERT_TRUE(adapter.hasNextRecord());

    Mock::DataProvider provider{};
    EXPECT_CALL(provider, add).Times(1);

    EXPECT_EQ(provider.prepare(adapter), 1);
}

TEST_F(Generator_Historical_DataProvider, Prepare_MultipleRecords)
{
    Fake::DataAccessAdapter adapter = makeFakeDataAdapter(5);
    ASSERT_TRUE(adapter.hasNextRecord());

    Mock::DataProvider provider{};
    EXPECT_CALL(provider, add).Times(5);

    EXPECT_EQ(provider.prepare(adapter), 5);
}

TEST_F(Generator_Historical_DataProvider, PullAction_Empty)
{
    Mock::DataProvider provider{};
    EXPECT_CALL(provider, isEmpty).Times(1).WillOnce(Return(true));

    // TimeOffset is initialized by implementation, so `hasTimeOffset`
    // always returns 'false', which triggers `initializeTimeOffset`
    EXPECT_CALL(provider, initializeTimeOffset).Times(0);

    EXPECT_THROW(
        provider.pullAction([]([[maybe_unused]] auto _action) {}),
        std::logic_error
    );
}

TEST_F(Generator_Historical_DataProvider, PullAction_Single)
{
    Historical::Record const record = makeTestRecord();

    Mock::DataProvider provider{};
    EXPECT_CALL(provider, isEmpty).Times(1).WillOnce(Return(false));
    EXPECT_CALL(provider, initializeTimeOffset).Times(1);
    EXPECT_CALL(provider, pullInto(testing::_))
        .Times(1)
        .WillOnce(ConstructActionWith(record));

    EXPECT_NO_THROW(provider.pullAction([]([[maybe_unused]] auto _action) {}));
}

TEST_F(Generator_Historical_DataProvider, PullAction_Multiple)
{
    Historical::Record const record = makeTestRecord();

    Mock::DataProvider provider{};
    EXPECT_CALL(provider, isEmpty).Times(3).WillRepeatedly(Return(false));
    EXPECT_CALL(provider, initializeTimeOffset).Times(3);
    EXPECT_CALL(provider, pullInto(testing::_))
        .Times(3)
        .WillRepeatedly(ConstructActionWith(record));

    EXPECT_NO_THROW(provider.pullAction([]([[maybe_unused]] auto _action) {}));
    EXPECT_NO_THROW(provider.pullAction([]([[maybe_unused]] auto _action) {}));
    EXPECT_NO_THROW(provider.pullAction([]([[maybe_unused]] auto _action) {}));
}

TEST_F(Generator_Historical_FiniteProvider, TimeOffset_InitializeWhenEmpty)
{
    FiniteProvider provider{};
    ASSERT_TRUE(provider.isEmpty());

    EXPECT_FALSE(provider.hasTimeOffset());
    provider.initializeTimeOffset();
    EXPECT_FALSE(provider.hasTimeOffset());
}

TEST_F(Generator_Historical_FiniteProvider, TimeOffset_InitializeWhenFilled)
{
    // 2023-06-13 13:10:52 GMT
    constexpr Historical::Timepoint recordTime = make_time(1686661852000000000);

    Fake::DataAccessAdapter adapter =
        makeFakeDataAdapter({RecordAttributes{recordTime, "AAPL", 1}});

    FiniteProvider provider{};
    provider.prepare(adapter);
    ASSERT_FALSE(provider.isEmpty());

    EXPECT_FALSE(provider.hasTimeOffset());
    provider.initializeTimeOffset();
    EXPECT_TRUE(provider.hasTimeOffset());
}

TEST_F(Generator_Historical_FiniteProvider, Pull_Empty)
{
    auto const puller = []([[maybe_unused]] auto const& _action) {};

    FiniteProvider provider{};
    ASSERT_TRUE(provider.isEmpty());

    EXPECT_THROW(provider.pullAction(puller), std::logic_error);
}

TEST_F(Generator_Historical_FiniteProvider, Pull_SingleAction)
{
    // 2023-06-13 13:10:52 GMT
    constexpr Historical::Timepoint firstRecTime =
        make_time(1686661852000000000);
    // 2023-06-13 13:10:53 GMT
    constexpr Historical::Timepoint secondRecTime =
        make_time(1686661853000000000);

    Fake::DataAccessAdapter adapter = makeFakeDataAdapter(
        {RecordAttributes(firstRecTime, "AAPL", 1),
         RecordAttributes(secondRecTime, "TSLA", 2)}
    );

    auto const puller = []([[maybe_unused]] auto const& _action) {};

    FiniteProvider provider{};
    provider.prepare(adapter);
    ASSERT_FALSE(provider.isEmpty());

    EXPECT_FALSE(provider.hasTimeOffset());
    EXPECT_NO_THROW(provider.pullAction(puller));
    EXPECT_FALSE(provider.isEmpty());
    EXPECT_TRUE(provider.hasTimeOffset());
}

TEST_F(Generator_Historical_FiniteProvider, Pull_AllActions)
{
    // 2023-06-13 13:10:52 GMT
    constexpr Historical::Timepoint firstRecTime =
        make_time(1686661852000000000);
    // 2023-06-13 13:10:53 GMT
    constexpr Historical::Timepoint secondRecTime =
        make_time(1686661853000000000);

    Fake::DataAccessAdapter adapter = makeFakeDataAdapter(
        {RecordAttributes(firstRecTime, "AAPL", 1),
         RecordAttributes(secondRecTime, "TSLA", 2)}
    );

    auto const puller = []([[maybe_unused]] auto const& _action) {};

    FiniteProvider provider{};
    provider.prepare(adapter);
    ASSERT_FALSE(provider.isEmpty());

    EXPECT_NO_THROW(provider.pullAction(puller));
    EXPECT_FALSE(provider.isEmpty());

    EXPECT_NO_THROW(provider.pullAction(puller));
    EXPECT_TRUE(provider.isEmpty());
}

TEST_F(Generator_Historical_FiniteProvider, Pull_MergeRecords)
{
    // 2023-06-13 13:10:52 GMT
    constexpr Historical::Timepoint recordsTime = make_time(1686661852000000000);

    Fake::DataAccessAdapter adapter = makeFakeDataAdapter(
        {RecordAttributes(recordsTime, "AAPL", 1),
         RecordAttributes(recordsTime, "TSLA", 2)}
    );

    auto const puller = [&](Historical::Action const& _action) {
        std::size_t numRecords = 0;
        _action.visit_records(
            [&numRecords]([[maybe_unused]] auto const& _record) {
              ++numRecords;
            });

        EXPECT_EQ(numRecords, 2);
    };

    FiniteProvider provider{};
    provider.prepare(adapter);
    ASSERT_FALSE(provider.isEmpty());

    EXPECT_NO_THROW(provider.pullAction(puller));
    EXPECT_TRUE(provider.isEmpty());
}

TEST_F(Generator_Historical_RepeatingProvider, TimeOffset_InitializeWhenEmpty)
{
    RepeatingProvider provider{};
    ASSERT_TRUE(provider.isEmpty());

    EXPECT_FALSE(provider.hasTimeOffset());
    provider.initializeTimeOffset();
    EXPECT_FALSE(provider.hasTimeOffset());
}

TEST_F(Generator_Historical_RepeatingProvider, TimeOffset_InitializeWhenFilled)
{
    // 2023-06-13 13:10:52 GMT
    constexpr Historical::Timepoint recordTime = make_time(1686661852000000000);

    Fake::DataAccessAdapter adapter =
        makeFakeDataAdapter({RecordAttributes{recordTime, "AAPL", 1}});

    RepeatingProvider provider{};
    provider.prepare(adapter);
    ASSERT_FALSE(provider.isEmpty());

    EXPECT_FALSE(provider.hasTimeOffset());
    provider.initializeTimeOffset();
    EXPECT_TRUE(provider.hasTimeOffset());
}

TEST_F(Generator_Historical_RepeatingProvider, Pull_SingleAction)
{
    // 2023-06-13 13:10:52 GMT
    constexpr Historical::Timepoint firstRecTime =
        make_time(1686661852000000000);
    // 2023-06-13 13:10:53 GMT
    constexpr Historical::Timepoint secondRecTime =
        make_time(1686661853000000000);

    Fake::DataAccessAdapter adapter = makeFakeDataAdapter(
        {RecordAttributes(firstRecTime, "AAPL", 1),
         RecordAttributes(secondRecTime, "TSLA", 2)}
    );

    auto const puller = []([[maybe_unused]] auto const& _action) {};

    RepeatingProvider provider{};
    provider.prepare(adapter);
    ASSERT_FALSE(provider.isEmpty());

    EXPECT_FALSE(provider.hasTimeOffset());
    EXPECT_NO_THROW(provider.pullAction(puller));
    EXPECT_FALSE(provider.isEmpty());
    EXPECT_TRUE(provider.hasTimeOffset());
}

TEST_F(Generator_Historical_RepeatingProvider, Pull_MultipleActions)
{
    // 2023-06-13 13:10:52 GMT
    constexpr Historical::Timepoint firstRecTime =
        make_time(1686661852000000000);
    // 2023-06-13 13:10:53 GMT
    constexpr Historical::Timepoint secondRecTime =
        make_time(1686661853000000000);

    Fake::DataAccessAdapter adapter = makeFakeDataAdapter(
        {RecordAttributes(firstRecTime, "AAPL", 1),
         RecordAttributes(secondRecTime, "TSLA", 2)}
    );

    auto const puller = []([[maybe_unused]] auto const& _action) {};

    RepeatingProvider provider{};
    provider.prepare(adapter);
    ASSERT_FALSE(provider.isEmpty());

    // 1st action
    EXPECT_FALSE(provider.hasTimeOffset());
    EXPECT_NO_THROW(provider.pullAction(puller));
    EXPECT_FALSE(provider.isEmpty());
    EXPECT_TRUE(provider.hasTimeOffset());

    // 2nd action
    ASSERT_FALSE(provider.isEmpty());
    EXPECT_NO_THROW(provider.pullAction(puller));
    EXPECT_FALSE(provider.isEmpty());

    Historical::Duration const firstTimeOffset = provider.getTimeOffset();

    // Suspend test for 1 historical time point to catch the difference
    // in first and second time offsets
    std::this_thread::sleep_for(Historical::Duration{1});

    // 3d action (copy of 1st one)
    EXPECT_NO_THROW(provider.pullAction(puller));
    EXPECT_FALSE(provider.isEmpty());
    EXPECT_TRUE(provider.hasTimeOffset());

    // 4th action (copy of 2nd one)
    ASSERT_FALSE(provider.isEmpty());
    EXPECT_NO_THROW(provider.pullAction(puller));
    EXPECT_FALSE(provider.isEmpty());

    Historical::Duration const secondTimeOffset = provider.getTimeOffset();

    EXPECT_TRUE(secondTimeOffset != firstTimeOffset);
}

TEST_F(Generator_Historical_RepeatingProvider, Pull_MergeRecords)
{
    // 2023-06-13 13:10:52 GMT
    constexpr Historical::Timepoint recordsTime = make_time(1686661852000000000);

    Fake::DataAccessAdapter adapter = makeFakeDataAdapter(
        {RecordAttributes(recordsTime, "AAPL", 1),
         RecordAttributes(recordsTime, "TSLA", 2)}
    );

    auto const puller = [&](Historical::Action const& _action) {
        std::size_t numRecords = 0;
        _action.visit_records(
            [&numRecords]([[maybe_unused]] auto const& _record) {
              ++numRecords;
            });

        EXPECT_EQ(numRecords, 2);
    };

    RepeatingProvider provider{};
    provider.prepare(adapter);
    ASSERT_FALSE(provider.isEmpty());

    EXPECT_NO_THROW(provider.pullAction(puller));
    EXPECT_FALSE(provider.isEmpty());
}

} // namespace
} // namespace Simulator::Generator::Historical