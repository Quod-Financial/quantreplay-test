#include "ih/config/config.hpp"

#include <date/date.h>

#include <chrono>
#include <istream>
#include <optional>

#include "core/tools/time.hpp"
#include "ih/config/phase_entry_reader.hpp"
#include "ih/config/venue_entry_reader.hpp"
#include "log/logging.hpp"

namespace data = Simulator::DataLayer;
using namespace std::chrono_literals;

namespace simulator::trading_system {
namespace {

auto parse_time_of_day(std::istream& buffer)
    -> std::optional<std::chrono::minutes> {
  std::optional<std::chrono::minutes> parsed;

  date::fields<std::chrono::seconds> fields;
  buffer >> date::parse("%H:%M", fields);

  if (buffer.good() && fields.has_tod) {
    parsed = std::chrono::floor<std::chrono::minutes>(fields.tod.to_duration());
  }

  return parsed;
}

auto determine_trading_phase(data::MarketPhase::Phase value)
    -> std::optional<std::variant<TradingPhase, TradingStatus>> {
  switch (value) {
    case data::MarketPhase::Phase::Open:
      return TradingPhase::Option::Open;
    case data::MarketPhase::Phase::Closed:
      return TradingPhase::Option::Closed;
    case data::MarketPhase::Phase::OpeningAuction:
      return TradingPhase::Option::OpeningAuction;
    case data::MarketPhase::Phase::ClosingAuction:
      return TradingPhase::Option::ClosingAuction;
    case data::MarketPhase::Phase::IntradayAuction:
      return TradingPhase::Option::IntradayAuction;
    case data::MarketPhase::Phase::Halted:
      return TradingStatus::Option::Halt;
  }
  return std::nullopt;
}

}  // namespace

auto PhaseEntryReader::operator()(const data::MarketPhase& record) -> void {
  time_parse_buf_.str(record.start_time());
  const auto begin = parse_time_of_day(time_parse_buf_);
  if (!begin) {
    on_error(record, "failed to parse start time value");
    return;
  }

  time_parse_buf_.str(record.end_time());
  const auto end = parse_time_of_day(time_parse_buf_);
  if (!end) {
    on_error(record, "failed to parse end time value");
    return;
  }

  const auto phase = determine_trading_phase(record.phase());
  if (!phase) {
    on_error(record, "failed to recognize the trading phase type");
    return;
  }

  const auto end_range =
      std::chrono::seconds(std::abs(record.end_time_range().value_or(0)));

  const auto allow_cancels = record.allow_cancels().value_or(false);

  ies::PhaseRecord converted{.begin = *begin,
                             .end = *end,
                             .end_range = end_range,
                             .phase = *phase,
                             .allow_cancels_on_halt = allow_cancels};

  if (converted.begin >= converted.end) {
    on_error(record, "phase start time is not less than end time");
    return;
  }

  if (converted.begin > 24h || converted.end > 24h) {
    on_error(record, "phase start/end time value is not in acceptable range");
    return;
  }

  destination_->add_trading_phase(converted);
  log::info("venue trading phase scheduled: {}", converted);
}

auto PhaseEntryReader::on_error(const data::MarketPhase& data,
                                std::string_view reason) -> void {
  log::warn(
      "cannot process market phase record: {}, ignoring phase with "
      "start_time={}, end_time={}",
      reason,
      data.start_time(),
      data.end_time());
}

auto VenueEntryReader::operator()(const data::Venue& record) const -> void {
  {
    const bool value = record.getSupportTifDayFlag().value_or(true);
    destination_->set_support_day_orders(value);
    log::info("venue supports Day orders: {}", value);
  }

  {
    const bool value = record.getSupportTifIocFlag().value_or(true);
    destination_->set_support_ioc_orders(value);
    log::info("venue supports IoC orders: {}", value);
  }

  {
    const bool value = record.getSupportTifFokFlag().value_or(true);
    log::info("venue supports FoK orders: {}", value);
    destination_->set_support_fok_orders(value);
  }

  {
    const bool value = record.getCancelOnDisconnectFlag().value_or(false);
    log::info("venue supports cancel on disconnect: {}", value);
    destination_->set_cod(value);
  }

  {
    const bool value = record.getTnsEnabledFlag().value_or(false);
    log::info("venue supports trades streaming: {}", value);
    destination_->set_trade_streaming(value);
  }

  {
    const bool value = record.getTnsQtyEnabledFlag().value_or(false);
    log::info("venue supports trades volume streaming: {}", value);
    destination_->set_trade_volume_streaming(value);
  }

  {
    const bool value = record.getTnsPartiesEnabledFlag().value_or(false);
    log::info("venue supports trades parties streaming: {}", value);
    destination_->set_trade_parties_streaming(value);
  }

  {
    const bool value = record.getTnsSideEnabledFlag().value_or(false);
    log::info("venue supports trades aggressor streaming: {}", value);
    destination_->set_trade_aggressor_streaming(value);
  }

  {
    const bool value = not record.getIncludeOwnOrdersFlag().value_or(true);
    log::info("venue supports client's depth orders exclusion: {}", value);
    destination_->set_depth_orders_exclusion(value);
  }

  {
    const bool value = record.getPersistenceEnabledFlag().value_or(false);
    log::info("venue supports persistence: {}", value);
    destination_->set_persistence(value);
  }

  if (const auto& persistence_path = record.getPersistenceFilePath()) {
    log::info("venue persistence file path: {}", *persistence_path);
    destination_->set_persistence_file_path(*persistence_path);
  }

  if (const auto& timezone = record.getTimezone()) {
    log::info("venue timezone: {}", *timezone);
    destination_->set_timezone_clock(core::TzClock{*timezone});
  }

  PhaseEntryReader reader(*destination_);
  for (const auto& phase : record.getMarketPhases()) {
    reader(phase);
  }
}

}  // namespace simulator::trading_system
