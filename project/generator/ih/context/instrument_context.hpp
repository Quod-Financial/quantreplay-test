#ifndef SIMULATOR_GENERATOR_IH_CONTEXT_INSTRUMENT_CONTEXT_HPP_
#define SIMULATOR_GENERATOR_IH_CONTEXT_INSTRUMENT_CONTEXT_HPP_

#include <string>

#include "core/domain/instrument_descriptor.hpp"
#include "data_layer/api/models/listing.hpp"
#include "data_layer/api/models/venue.hpp"
#include "ih/registry/generated_orders_registry.hpp"

namespace Simulator::Generator {

class InstrumentContext
{
public:

    virtual ~InstrumentContext() = default;

    [[nodiscard]]
    virtual std::string getSyntheticIdentifier() noexcept = 0;

    [[nodiscard]]
    virtual DataLayer::Venue const & getVenue() const noexcept = 0;

    [[nodiscard]]
    virtual DataLayer::Listing const & getInstrument() const noexcept = 0;

    virtual simulator::InstrumentDescriptor const& getInstrumentDescriptor()
        const noexcept = 0;
};


class OrderInstrumentContext
    :   public InstrumentContext
{
public:

    [[nodiscard]]
    virtual std::size_t nextMessageNumber() noexcept = 0;

    [[nodiscard]]
    virtual GeneratedOrdersRegistry & takeRegistry() noexcept = 0;
};

} // namespace Simulator::Generator

#endif // SIMULATOR_GENERATOR_IH_CONTEXT_INSTRUMENT_CONTEXT_HPP_

