#include "ih/context/generation_manager.hpp"

#include <fmt/format.h>

#include <chrono>
#include <list>

#include "ih/constants.hpp"
#include "log/logging.hpp"

namespace Simulator::Generator {

GenerationState::GenerationState(bool _activateOnCreation) noexcept
    :   m_state { _activateOnCreation ? State::Active : State::Suspended }
{}


bool GenerationState::isRunning() const noexcept
{
    return m_state.load() == State::Active;
}


void GenerationState::setRunning() noexcept
{
    if (!isTerminated())
    {
        m_state = State::Active;
    }
}


void GenerationState::setStopped() noexcept
{
    if (!isTerminated())
    {
        m_state = State::Suspended;
    }
}


bool GenerationState::isTerminated() const noexcept
{
    return m_state.load() == State::Terminated;
}


void GenerationState::setTerminated() noexcept
{
    m_state = State::Terminated;
}


std::size_t GeneratedMessagesCounter::incrementOrderMessagesCount() noexcept
{
    return ++m_numOrderMessagesGenerated;
}


IdentifierGenerator::IdentifierGenerator()
    :   m_nextIdentifier {
            static_cast<std::size_t>(
                std::chrono::system_clock::now().time_since_epoch().count()
            )
        }
{}


std::string IdentifierGenerator::generateIdentifier() noexcept
{
    return fmt::format("SIM-{}", m_nextIdentifier++);
}


GenerationManager::GenerationManager(DataLayer::Venue target_venue)
    :   target_venue_ { std::move(target_venue) }
    ,   m_generationState { target_venue_.getOrderOnStartupFlag().value_or(Constant::DefaultVenueOrderOnStartup) }
{}


std::shared_ptr<GenerationManager> GenerationManager::create(
    DataLayer::Venue const & target_venue
)
{
    return std::make_shared<GenerationManager>(target_venue);
}


bool GenerationManager::isComponentRunning() const noexcept
{
    return m_generationState.isRunning();
}


DataLayer::Venue const & GenerationManager::getVenue() const noexcept
{
    return target_venue_;
}


std::string GenerationManager::generateIdentifier() noexcept
{
    return m_idGenerator.generateIdentifier();
}


std::size_t GenerationManager::nextGeneratedOrderMessageNumber() noexcept
{
    return m_messagesCounter.incrementOrderMessagesCount();
}


void GenerationManager::callOnLaunch(OnStartupCallback const & _callback)
{
    if (!m_generationState.isTerminated())
    {
        [[maybe_unused]] std::unique_lock<std::mutex> const lock {
            m_eventsListenersMutex
        };

        m_launchEventListeners.emplace_back(_callback);
    }
}


void GenerationManager::launch()
{
    if (m_generationState.isRunning())
    {
        return;
    }

    if (m_generationState.isTerminated())
    {
        simulator::log::warn(
          "unable to launch generation, as it has been terminated previously");
        return;
    }

    m_generationState.setRunning();

    std::list<OnStartupCallback> pendingCallbacks {};
    {
        [[maybe_unused]] std::unique_lock<std::mutex> const lock {
            m_eventsListenersMutex
        };

        pendingCallbacks = std::move(m_launchEventListeners);
        m_launchEventListeners.clear();
    }
    for (auto const & onLaunchCallback : pendingCallbacks)
    {
        onLaunchCallback();
    }
}

void GenerationManager::suspend()
{
    m_generationState.setStopped();
}


void GenerationManager::terminate() noexcept
{
    m_generationState.setTerminated();
}

} // namespace Simulator::GeneratorImpl
