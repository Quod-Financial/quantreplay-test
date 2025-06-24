#ifndef SIMULATOR_DATA_LAYER_INCLUDE_DATA_LAYER_API_DATABASE_CONTEXT_HPP_
#define SIMULATOR_DATA_LAYER_INCLUDE_DATA_LAYER_API_DATABASE_CONTEXT_HPP_

#include <memory>

namespace Simulator::DataLayer::Database {

class ContextResolver;

class Context final {
    friend class Driver;
    friend class ContextResolver;
    class Implementation;

  public:
    Context(Context const&) = default;
    auto operator=(Context const&) -> Context& = default;

    Context(Context&&) = default;
    auto operator=(Context&&) -> Context& = default;

    ~Context() = default;

  private:
    Context() = default;

    void accept(ContextResolver& _resolver) const;

    std::shared_ptr<Implementation> mImplementation;
};

} // namespace Simulator::DataLayer::Database

#endif // SIMULATOR_DATA_LAYER_INCLUDE_DATA_LAYER_API_DATABASE_CONTEXT_HPP_
