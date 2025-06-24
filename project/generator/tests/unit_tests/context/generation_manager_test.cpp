#include <chrono>
#include <functional>
#include <future>
#include <memory>
#include <regex>
#include <string>
#include <string_view>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "cfg/api/cfg.hpp"

#include "ih/context/generation_manager.hpp"

namespace Simulator::Generator {
namespace {

class NoopCallback
{
public:

    MOCK_METHOD(void, call, ());
};


class Generator_IdentifierGenerator
    :   public testing::Test
{
public:

    static void parseIdentifier(
            std::string_view _generatedID
        ,   std::size_t & _numberPart
        ,   std::string & _idPrefix
    )
    {
        constexpr char delimiter = '-';
        auto const prefixEndPos = _generatedID.find(delimiter);
        ASSERT_TRUE(prefixEndPos != std::string_view::npos);

        _idPrefix = _generatedID.substr(0, prefixEndPos + 1);
        auto const _idNumber = std::string{
            _generatedID.substr(prefixEndPos + 1)
        };

        ASSERT_NO_THROW(_numberPart = std::stoull(_idNumber));
    }
};

class Generator_GenerationManager
    :   public testing::Test
{
public:
    static auto makeManager(const std::string& venue_id)
        -> std::shared_ptr<GenerationManager>
    {
        DataLayer::Venue::Patch patch;
        patch.withVenueId(venue_id);
        return GenerationManager::create(DataLayer::Venue::create(std::move(patch)));
    }

    static auto makeManager(bool order_on_startup_enabled)
        -> std::shared_ptr<GenerationManager>
    {
        static std::string const defaultVenueID{"XNSE"};

        DataLayer::Venue::Patch patch;
        patch.withVenueId(defaultVenueID)
            .withOrdersOnStartupFlag(order_on_startup_enabled);
        return GenerationManager::create(DataLayer::Venue::create(std::move(patch)));
    }

protected:

    void SetUp() override
    {
        Cfg::init();
    }
};

TEST(Generator_GenerationState, Suspended)
{
    GenerationState const state{ false };

    EXPECT_FALSE(state.isRunning());
    EXPECT_FALSE(state.isTerminated());
}

TEST(Generator_GenerationState, Active)
{
    GenerationState const state{ true };

    EXPECT_TRUE(state.isRunning());
    EXPECT_FALSE(state.isTerminated());
}

TEST(Generator_GenerationState, Change_SuspendedToActive)
{
    GenerationState state{ false };

    ASSERT_FALSE(state.isRunning());
    ASSERT_FALSE(state.isTerminated());

    state.setRunning();

    EXPECT_TRUE(state.isRunning());
    EXPECT_FALSE(state.isTerminated());
}

TEST(Generator_GenerationState, Change_ActiveToSuspended)
{
    GenerationState state{ true };

    ASSERT_TRUE(state.isRunning());
    ASSERT_FALSE(state.isTerminated());

    state.setStopped();

    EXPECT_FALSE(state.isRunning());
    EXPECT_FALSE(state.isTerminated());
}

TEST(Generator_GenerationState, Terminate_Suspended)
{
    GenerationState state{ false };

    ASSERT_FALSE(state.isRunning());
    ASSERT_FALSE(state.isTerminated());

    state.setTerminated();

    EXPECT_FALSE(state.isRunning());
    EXPECT_TRUE(state.isTerminated());
}

TEST(Generator_GenerationState, Terminate_Active)
{
    GenerationState state{ true };

    ASSERT_TRUE(state.isRunning());
    ASSERT_FALSE(state.isTerminated());

    state.setTerminated();

    EXPECT_FALSE(state.isRunning());
    EXPECT_TRUE(state.isTerminated());
}

TEST(Generator_GeneratedMessagesCounter, Increment)
{
    constexpr std::size_t maxCount = 5;
    GeneratedMessagesCounter counter{};

    for (std::size_t count = 1; count <= maxCount; ++count)
    {
        EXPECT_EQ(counter.incrementOrderMessagesCount(), count);
    }
}

TEST(Generator_GeneratedMessagesCounter, ConcurentIncrement)
{
    constexpr std::size_t minIncrement = 1;
    constexpr std::size_t maxIncrements = 10;

    GeneratedMessagesCounter counter{};
    auto const incrementor = [&]() -> std::size_t {
        return counter.incrementOrderMessagesCount();
    };

    std::set<std::size_t> createdIds{};
    auto const incrementCheck = [&](std::size_t _increment) {
        EXPECT_TRUE(minIncrement <= _increment);
        EXPECT_TRUE(_increment <= maxIncrements);
        EXPECT_TRUE(createdIds.emplace(_increment).second);
    };

    auto asyncIncrement1 = std::async(std::launch::async, incrementor);
    auto asyncIncrement2 = std::async(std::launch::async, incrementor);
    auto asyncIncrement3 = std::async(std::launch::async, incrementor);
    auto asyncIncrement4 = std::async(std::launch::async, incrementor);
    auto asyncIncrement5 = std::async(std::launch::async, incrementor);
    auto asyncIncrement6 = std::async(std::launch::async, incrementor);
    auto asyncIncrement7 = std::async(std::launch::async, incrementor);
    auto asyncIncrement8 = std::async(std::launch::async, incrementor);
    auto asyncIncrement9 = std::async(std::launch::async, incrementor);
    auto asyncIncrement10 = std::async(std::launch::async, incrementor);

    incrementCheck(asyncIncrement1.get());
    incrementCheck(asyncIncrement2.get());
    incrementCheck(asyncIncrement3.get());
    incrementCheck(asyncIncrement4.get());
    incrementCheck(asyncIncrement5.get());
    incrementCheck(asyncIncrement6.get());
    incrementCheck(asyncIncrement7.get());
    incrementCheck(asyncIncrement8.get());
    incrementCheck(asyncIncrement9.get());
    incrementCheck(asyncIncrement10.get());
}

TEST_F(Generator_IdentifierGenerator, CreateIdentifier)
{
    using std::chrono::system_clock;

    std::string const expectedPrefix{ "SIM-" };
    auto const minNumber = static_cast<std::size_t>(
        system_clock::now().time_since_epoch().count()
    );

    IdentifierGenerator idGenerator{};

    std::string const generatedID = idGenerator.generateIdentifier();
    EXPECT_FALSE(generatedID.empty());

    std::string prefix{};
    std::size_t numberPart = 0;
    parseIdentifier(generatedID, numberPart, prefix);

    EXPECT_EQ(prefix, expectedPrefix);
    EXPECT_TRUE(numberPart > minNumber);
}

TEST_F(Generator_IdentifierGenerator, CreateIdentifierConcurently)
{
    using std::chrono::system_clock;

    std::string const expectedPrefix{ "SIM-" };
    auto const minNumber = static_cast<std::size_t>(
        system_clock::now().time_since_epoch().count()
    );

    IdentifierGenerator generator{};
    auto const generateID = [&]() -> std::string {
        return generator.generateIdentifier();
    };

    auto asyncGenID1 = std::async(std::launch::async, generateID);
    auto asyncGenID2 = std::async(std::launch::async, generateID);
    auto asyncGenID3 = std::async(std::launch::async, generateID);
    auto asyncGenID4 = std::async(std::launch::async, generateID);
    auto asyncGenID5 = std::async(std::launch::async, generateID);
    auto asyncGenID6 = std::async(std::launch::async, generateID);
    auto asyncGenID7 = std::async(std::launch::async, generateID);
    auto asyncGenID8 = std::async(std::launch::async, generateID);
    auto asyncGenID9 = std::async(std::launch::async, generateID);
    auto asyncGenID10 = std::async(std::launch::async, generateID);

    auto const maxNumber = static_cast<std::size_t>(
        system_clock::now().time_since_epoch().count()
    );

    std::set<std::string> createdIds{};
    auto const checkID = [&](std::string const &_id) {
        std::string prefixPart{};
        std::size_t idNumber = 0;

        parseIdentifier(_id, idNumber, prefixPart);
        EXPECT_TRUE(idNumber > minNumber);
        EXPECT_TRUE(idNumber < maxNumber);
        EXPECT_EQ(prefixPart, expectedPrefix);
        EXPECT_TRUE(createdIds.emplace(_id).second);
    };

    checkID(asyncGenID1.get());
    checkID(asyncGenID2.get());
    checkID(asyncGenID3.get());
    checkID(asyncGenID4.get());
    checkID(asyncGenID5.get());
    checkID(asyncGenID6.get());
    checkID(asyncGenID7.get());
    checkID(asyncGenID8.get());
    checkID(asyncGenID9.get());
    checkID(asyncGenID10.get());
}

TEST_F(Generator_GenerationManager, Create)
{
    std::string const venueID{ "XBOM" };
    auto const pManager = makeManager(venueID);
    ASSERT_TRUE(pManager);

    EXPECT_EQ(pManager->getVenue().getVenueId(), venueID);
}

TEST_F(Generator_GenerationManager, Create_OrderOnStartup_Disabled)
{
    auto const pManager = makeManager(false);
    ASSERT_TRUE(pManager);

    EXPECT_FALSE(pManager->isComponentRunning());
}

TEST_F(Generator_GenerationManager, Create_OrderOnStartup_Enabled)
{
    auto const pManager = makeManager(true);
    ASSERT_TRUE(pManager);

    EXPECT_TRUE(pManager->isComponentRunning());
}

TEST_F(Generator_GenerationManager, Generate_Order_MessageNumber)
{
    auto const pManager = makeManager("XNSE");
    ASSERT_TRUE(pManager);

    EXPECT_EQ(pManager->nextGeneratedOrderMessageNumber(), 1);
    EXPECT_EQ(pManager->nextGeneratedOrderMessageNumber(), 2);
    EXPECT_EQ(pManager->nextGeneratedOrderMessageNumber(), 3);
}

TEST_F(Generator_GenerationManager, Generate_Identifier)
{
    // standard sim generated prefix 'SIM-'
    // and a 19-digit number (based on UNIX nanosecond time)
    std::regex const idRegex{ "SIM-[0-9]{19}", std::regex::ECMAScript };
    auto const pManager = makeManager("XLSE");
    ASSERT_TRUE(pManager);

    EXPECT_TRUE(std::regex_match(pManager->generateIdentifier(), idRegex));
}

TEST_F(Generator_GenerationManager, Suspend_Suspended)
{
    auto const pManager = makeManager(false);
    ASSERT_TRUE(pManager);
    ASSERT_FALSE(pManager->isComponentRunning());

    pManager->suspend();

    EXPECT_FALSE(pManager->isComponentRunning());
}

TEST_F(Generator_GenerationManager, Suspend_Active)
{
    auto const pManager = makeManager(true);
    ASSERT_TRUE(pManager);
    ASSERT_TRUE(pManager->isComponentRunning());

    pManager->suspend();

    EXPECT_FALSE(pManager->isComponentRunning());
}

TEST_F(Generator_GenerationManager, Launch_Suspended)
{
    auto const pManager = makeManager(false);
    ASSERT_TRUE(pManager);
    ASSERT_FALSE(pManager->isComponentRunning());

    pManager->launch();

    EXPECT_TRUE(pManager->isComponentRunning());
}

TEST_F(Generator_GenerationManager, Launch_Active)
{
    auto const pManager = makeManager(true);
    ASSERT_TRUE(pManager);
    ASSERT_TRUE(pManager->isComponentRunning());

    pManager->launch();

    EXPECT_TRUE(pManager->isComponentRunning());
}

TEST_F(Generator_GenerationManager, Terminate_Suspended)
{
    auto const pManager = makeManager(false);
    ASSERT_TRUE(pManager);
    ASSERT_FALSE(pManager->isComponentRunning());

    pManager->terminate();

    EXPECT_FALSE(pManager->isComponentRunning());

    pManager->launch();

    EXPECT_FALSE(pManager->isComponentRunning());
}

TEST_F(Generator_GenerationManager, Terminate_Active)
{
    auto const pManager = makeManager(true);
    ASSERT_TRUE(pManager);
    ASSERT_TRUE(pManager->isComponentRunning());

    pManager->terminate();

    EXPECT_FALSE(pManager->isComponentRunning());

    pManager->launch();

    EXPECT_FALSE(pManager->isComponentRunning());
}

TEST_F(Generator_GenerationManager, CallOnLaunch_Suspended)
{
    auto const pManager = makeManager(false);
    ASSERT_TRUE(pManager);
    ASSERT_FALSE(pManager->isComponentRunning());

    NoopCallback callback{};
    EXPECT_CALL(callback, call).Times(1);

    pManager->callOnLaunch([&callback]() { callback.call(); });
    pManager->launch();

    pManager->suspend();
    pManager->launch();
}

TEST_F(Generator_GenerationManager, CallOnLaunch_Active)
{
    auto const pManager = makeManager(true);
    ASSERT_TRUE(pManager);
    ASSERT_TRUE(pManager->isComponentRunning());

    NoopCallback callback{};
    EXPECT_CALL(callback, call).Times(1);

    pManager->callOnLaunch([&callback]() { callback.call(); });
    pManager->suspend();
    pManager->launch();

    pManager->suspend();
    pManager->launch();
}

TEST_F(Generator_GenerationManager, CallOnLaunch_Terminated)
{
    auto const pManager = makeManager(true);
    ASSERT_TRUE(pManager);
    ASSERT_TRUE(pManager->isComponentRunning());

    NoopCallback callback{};
    EXPECT_CALL(callback, call).Times(0);

    pManager->callOnLaunch([&callback]() { callback.call(); });
    pManager->terminate();

    pManager->launch();
}

}  // namespace
}  // namespace Simulator::Generator