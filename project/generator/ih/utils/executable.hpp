#ifndef SIMULATOR_GENERATOR_IH_UTILS_EXECUTABLE_HPP_
#define SIMULATOR_GENERATOR_IH_UTILS_EXECUTABLE_HPP_

#include <chrono>

namespace Simulator::Generator {

class Executable
{
public:

    virtual ~Executable() = default;

    virtual void prepare() noexcept = 0;

    virtual void execute() = 0;

    [[nodiscard]]
    virtual bool finished() const noexcept = 0;

    [[nodiscard]]
    virtual std::chrono::microseconds nextExecTimeout() const = 0;
};

} // namespace Simulator::Generator

#endif // SIMULATOR_GENERATOR_IH_UTILS_EXECUTABLE_HPP_
