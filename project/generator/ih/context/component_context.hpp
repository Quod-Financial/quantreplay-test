#ifndef SIMULATOR_GENERATOR_IH_CONTEXT_COMPONENT_CONTEXT_HPP_
#define SIMULATOR_GENERATOR_IH_CONTEXT_COMPONENT_CONTEXT_HPP_

#include <cstddef>
#include <functional>

#include "data_layer/api/models/venue.hpp"

namespace Simulator::Generator {

class ComponentContext
{
public:

    using OnStartupCallback = std::function<void()>;

    virtual ~ComponentContext() = default;

    [[nodiscard]]
    virtual bool isComponentRunning() const noexcept = 0;

    [[nodiscard]]
    virtual DataLayer::Venue const & getVenue() const noexcept = 0;

    virtual std::string generateIdentifier() noexcept = 0;

    virtual std::size_t nextGeneratedOrderMessageNumber() noexcept = 0;

    virtual void callOnLaunch(OnStartupCallback const & _callback) = 0;
};

} // namespace Simulator::Generator

#endif // SIMULATOR_GENERATOR_IH_CONTEXT_COMPONENT_CONTEXT_HPP_
