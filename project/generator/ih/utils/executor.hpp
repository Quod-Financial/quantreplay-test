#ifndef SIMULATOR_GENERATOR_SRC_UTILS_EXECUTOR_HPP_
#define SIMULATOR_GENERATOR_SRC_UTILS_EXECUTOR_HPP_

#include <atomic>
#include <thread>
#include <memory>

#include "ih/context/component_context.hpp"
#include "ih/utils/executable.hpp"

namespace Simulator::Generator {

class Executor
{
public:

    Executor() = delete;

    Executor(
            std::unique_ptr<Executable> _pExecutable
        ,   std::shared_ptr<ComponentContext> _pGlobalContext
    ) noexcept;

    static std::unique_ptr<Executor> create(
            std::unique_ptr<Executable> _pExecutable
        ,   std::shared_ptr<ComponentContext> _pGlobalContext
    );

    void launch();

    void terminate() noexcept;

private:

    [[nodiscard]]
    bool isExecuting() const noexcept;

    void start();

    void resetExecutingThread() noexcept;

    void execute() noexcept;


    std::shared_ptr<ComponentContext> m_pGlobalContext;

    std::unique_ptr<Executable> m_pExecutable;
    std::unique_ptr<std::thread> m_pExecutingThread;

    std::atomic<bool> m_terminated { false };
};

} // namespace Simulator::Generator

#endif // SIMULATOR_GENERATOR_SRC_UTILS_EXECUTOR_HPP_
