#include "ih/utils/executor.hpp"

#include <cassert>
#include <exception>
#include <memory>
#include <thread>
#include <utility>

#include "ih/context/component_context.hpp"
#include "ih/utils/executable.hpp"
#include "log/logging.hpp"

namespace Simulator::Generator {

Executor::Executor(
        std::unique_ptr<Executable> _pExecutable
    ,   std::shared_ptr<ComponentContext> _pGlobalContext
) noexcept
    :   m_pGlobalContext{ std::move(_pGlobalContext) }
    ,   m_pExecutable { std::move(_pExecutable) }
{
    assert(m_pGlobalContext);
    assert(m_pExecutable);
}


std::unique_ptr<Executor> Executor::create(
        std::unique_ptr<Executable> _pExecutable
    ,   std::shared_ptr<ComponentContext> _pGlobalContext
)
{
    return std::make_unique<Executor>(
            std::move(_pExecutable)
        ,   std::move(_pGlobalContext)
    );
}


void Executor::launch()
{
    // Consider adding some kind of id or name to identify executors in logs

    if (m_terminated)
    {
        simulator::log::warn(
          "unable to launch a generation executor as it was terminated "
          "previously");
      return;
    }

    if (!m_pGlobalContext->isComponentRunning())
    {
        m_pGlobalContext->callOnLaunch([this] { launch(); });
        simulator::log::info("postponed launching of generation executor");
        return;
    }

    if (isExecuting())
    {
        simulator::log::warn(
          "unable to launch a generation executor as it is in executing state "
          "already");
        return;
    }

    start();
    simulator::log::info("generation executor was launched successfully");
}


void Executor::terminate() noexcept
{
    m_terminated = true;
    if (isExecuting())
    {
        resetExecutingThread();
    }

    simulator::log::info("generation executor was terminated successfully");
}


bool Executor::isExecuting() const noexcept
{
    return m_pExecutingThread != nullptr;
}


void Executor::start()
{
    resetExecutingThread();
    m_pExecutingThread = std::make_unique<std::thread>([this] { execute(); });
}


void Executor::resetExecutingThread() noexcept
{
    if (isExecuting())
    {
        simulator::log::debug("joining generation executor's thread");

        assert(m_pExecutingThread->joinable());
        try
        {
            m_pExecutingThread->join();
        }
        catch (std::exception const & _ex)
        {
            simulator::log::err(
              "failed to join generation executor's thread: {}",
              _ex.what());
        }
    }

    m_pExecutingThread.reset();
}


void Executor::execute() noexcept
{
    m_pExecutable->prepare();

    try
    {
        while (!m_terminated && m_pGlobalContext->isComponentRunning())
        {
            m_pExecutable->execute();
            if (m_pExecutable->finished())
            {
                m_terminated = true;
            }
            else
            {
                std::this_thread::sleep_for(m_pExecutable->nextExecTimeout());
            }
        }
    }
    catch (std::exception const & _ex)
    {
        simulator::log::err(
          "error occurred in the generation executor's thread: {}", _ex.what());

        // It may be useful to be able to restart a failed thread
        // Think about how to properly restart a failed thread
        // (we can't do it in the thread where an exception occurred as in
        // this case a thread will try to join itself -> ooops, deadlock =( )
        // Currently it is restarted by ComponentContext observer,
        // which requires a user direct action...
    }

    if (m_terminated)
    {
        simulator::log::debug(
          "generation executor's thread has been terminated");
        return;
    }

    try
    {
        m_pGlobalContext->callOnLaunch([this] { start(); });
        simulator::log::debug(
          "generation executor's thread has been suspended till "
          "next launch notification from the generation context");
    }
    catch (std::exception const & _ex)
    {
        simulator::log::err(
          "an error occurred while postponing generator executor's "
          "thread launch on next generator launch event: {}",
          _ex.what());
    }
}

} // namespace Simulator::Generator
