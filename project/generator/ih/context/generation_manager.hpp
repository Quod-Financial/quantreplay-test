#ifndef SIMULATOR_GENERATOR_SRC_CONTEXT_GENERATION_MANAGER_HPP_
#define SIMULATOR_GENERATOR_SRC_CONTEXT_GENERATION_MANAGER_HPP_

#include <atomic>
#include <cstddef>
#include <list>
#include <memory>
#include <mutex>
#include <string>

#include "data_layer/api/models/venue.hpp"

#include "ih/context/component_context.hpp"

namespace Simulator::Generator {

class GenerationState
{
    enum class State
    {
        Active,
        Suspended,
        Terminated
    };

public:

    GenerationState() = delete;

    explicit GenerationState(bool _activateOnCreation) noexcept;

    [[nodiscard]]
    bool isRunning() const noexcept;

    void setRunning() noexcept;

    void setStopped() noexcept;

    [[nodiscard]]
    bool isTerminated() const noexcept;

    void setTerminated() noexcept;

private:

    std::atomic<State> m_state { State::Suspended };
};


class GeneratedMessagesCounter
{
public:

    std::size_t incrementOrderMessagesCount() noexcept;

private:

    std::atomic<std::size_t> m_numOrderMessagesGenerated { 0 };
};


class IdentifierGenerator
{
public:

    IdentifierGenerator();

    std::string generateIdentifier() noexcept;

private:

    std::atomic<std::size_t> m_nextIdentifier;
};


class GenerationManager
    :   public ComponentContext
{
public:

    GenerationManager() = delete;

    explicit GenerationManager(DataLayer::Venue target_venue);

    static std::shared_ptr<GenerationManager> create(
        DataLayer::Venue const & _targetVenue
    );


    [[nodiscard]]
    bool isComponentRunning() const noexcept override;

    [[nodiscard]]
    DataLayer::Venue const & getVenue() const noexcept override;

    std::string generateIdentifier() noexcept override;

    std::size_t nextGeneratedOrderMessageNumber() noexcept override;

    void callOnLaunch(OnStartupCallback const & _callback) override;


    void launch();

    void suspend();

    void terminate() noexcept;

private:

    DataLayer::Venue target_venue_;

    std::list<OnStartupCallback> m_launchEventListeners;
    std::mutex m_eventsListenersMutex;

    GeneratedMessagesCounter m_messagesCounter;
    IdentifierGenerator m_idGenerator;
    GenerationState m_generationState;

    bool m_terminated { false };
};

} // namespace Simulator::Generator

#endif // SIMULATOR_GENERATOR_SRC_CONTEXT_GENERATION_MANAGER_HPP_

