#include "ih/random/instrument_generator.hpp"

#include <fmt/chrono.h>

#include <cassert>
#include <chrono>
#include <cmath>
#include <memory>
#include <ratio>

#include "ih/adaptation/protocol_conversion.hpp"
#include "ih/constants.hpp"
#include "log/logging.hpp"
#include "middleware/routing/trading_request_channel.hpp"

namespace Simulator::Generator::Random {
namespace {

auto send_message(const GeneratedMessage& message,
                  const simulator::InstrumentDescriptor& instrument) -> void {
  try {
    if (message.message_type == MessageType::NewOrderSingle) {
      simulator::middleware::send_trading_request(
          convert_to_order_placement_request(message, instrument));
    } else if (message.message_type ==
               MessageType::OrderCancelReplaceRequest) {
      simulator::middleware::send_trading_request(
          convert_to_order_modification_request(message, instrument));
    } else if (message.message_type == MessageType::OrderCancelRequest) {
      simulator::middleware::send_trading_request(
          convert_to_order_cancellation_request(message, instrument));
    }
  } catch (const simulator::middleware::ChannelUnboundError&) {
      simulator::log::err(
          "failed to send message from random generator - trading request "
          "channel is not bound");
  }
}

}  // namespace

OrderGenerator::OrderGenerator(
        std::shared_ptr<OrderInstrumentContext> _pInstrumentContext
    ,   std::unique_ptr<GenerationAlgorithm> _pRandomGenerationAlgorithm
)
    :   m_pInstrumentContext { std::move(_pInstrumentContext) }
    ,   m_pGenerationAlgorithm { std::move(_pRandomGenerationAlgorithm) }
{
    assert(m_pInstrumentContext);
    assert(m_pGenerationAlgorithm);

    initExecutionRate();

    auto const & listing = m_pInstrumentContext->getInstrument();
    simulator::log::info(
        "successfully initialized random orders generator for `{}' instrument "
        "(id: {}), execution rate is set to {}",
        listing.getSymbol(),
        listing.getListingId(),
        m_executionRate);
}


bool OrderGenerator::finished() const noexcept
{
    // A random instrument generator is an infinite process.
    return false;
}


void OrderGenerator::prepare() noexcept
{}


void OrderGenerator::execute()
{
    auto const & listing = m_pInstrumentContext->getInstrument();
    simulator::log::debug(
        "executing random order message generation operation for `{}' (id: {})",
        listing.getSymbol(),
        listing.getListingId());

    bool const publish = m_pGenerationAlgorithm->generate(m_generatedMessage);
    if (!publish) {
      return;
    }

    send_message(m_generatedMessage,
                 m_pInstrumentContext->getInstrumentDescriptor());
}


std::chrono::microseconds OrderGenerator::nextExecTimeout() const
{
    return m_executionRate;
}


void OrderGenerator::initExecutionRate()
{
    constexpr double normNumerator = 3.0;
    constexpr double normDenominator = 2.0;

    auto const & instrument = m_pInstrumentContext->getInstrument();
    auto const updateRate = instrument.getRandomOrdersRate().value_or(
        Constant::DefaultListingRandomOrdersRate);
    assert(updateRate > 0);

    double const normalizeCoefficient =
        updateRate * normNumerator / normDenominator;
    assert(normalizeCoefficient > 0.0);

    auto const normalizedRate = std::chrono::microseconds {
        static_cast<std::uint32_t>(
            std::round(std::micro::den / normalizeCoefficient)
        )
    };

    simulator::log::debug(
        "normalized random generation execution rate for `{}' instrument "
        "(id: {}) is calculated as 1 random message per {} "
        "(rate normalization coefficient is {})",
        instrument.getSymbol(),
        instrument.getListingId(),
        normalizedRate,
        normalizeCoefficient);

    m_executionRate = normalizedRate;
}

} // namespace Simulator::Generator::Random
