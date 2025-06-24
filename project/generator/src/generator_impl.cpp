#include "ih/generator_impl.hpp"

#include <memory>

#include "data_layer/api/data_access_layer.hpp"
#include "ih/adaptation/protocol_conversion.hpp"
#include "ih/context/generation_manager.hpp"
#include "ih/context/order_generation_context_impl.hpp"
#include "ih/factory/executable_factory_impl.hpp"
#include "ih/registry/registry_updater.hpp"
#include "ih/utils/executor.hpp"
#include "ih/utils/validator.hpp"
#include "log/logging.hpp"

namespace Simulator::Generator {

GeneratorImpl::GeneratorImpl(
    DataLayer::Venue const& _targetVenue,
    DataLayer::Database::Context _databaseContext
) :
    mDatabaseContext(std::move(_databaseContext)),
    mGenerationManager{GenerationManager::create(_targetVenue)},
    mRndExecutorFactory{InstrumentRandomGeneratorFactoryImpl::create()},
    mHistExecutorFactory{std::make_unique<HistoricalReplierFactoryImpl>()}
{
    assert(mGenerationManager);

    initializeInstruments();
    initializeRandomGenerationExecutors();
    initializeHistoricalExecutor();

    simulator::log::info("generator initialized successfully");
}


GeneratorImpl::~GeneratorImpl() noexcept { terminateGenerator(); }

auto GeneratorImpl::status() -> bool
{
    return mGenerationManager->isComponentRunning();
}

auto GeneratorImpl::suspend() -> void { mGenerationManager->suspend(); }

auto GeneratorImpl::resume() -> void { mGenerationManager->launch(); }

auto GeneratorImpl::start() -> void
{
    for (auto const& pRandomOrdersExecutable : mListingsRandomGenerators) {
        pRandomOrdersExecutable->launch();
    }

    if (mHistoricalReplier) {
        mHistoricalReplier->launch();
    }
}

auto GeneratorImpl::initializeInstruments() -> void
{
    for(const auto& listing : DataLayer::selectAllListings(mDatabaseContext)) {
        initializeInstrument(listing);
    }
}

auto GeneratorImpl::initializeInstrument(DataLayer::Listing const& _listing)
    -> void
{
    auto const& currentVenue = mGenerationManager->getVenue();
    if (_listing.getVenueId() != currentVenue.getVenueId()) {
        simulator::log::debug(
          "generator ignores listing `{}' (id: {}), "
          "which does not belong to current venue `{}'",
          _listing.getSymbol(),
          _listing.getListingId(),
          currentVenue.getVenueId());
      return;
    }

    if (!Validator::isAcceptable(_listing)) {
        return;
    }

    const auto internal_instrument_id = _listing.getListingId();
    simulator::InstrumentDescriptor descriptor =
        convert_to_instrument_descriptor(_listing);
    descriptor.requester_instrument_id =
        simulator::RequesterInstrumentId{internal_instrument_id};

    const auto pContext = OrderInstrumentContextImpl::create(
            _listing
        ,   descriptor
        ,   mGenerationManager
    );

    mOrderListingsContexts.emplace_back(pContext);
    mContextLookup.emplace(internal_instrument_id, pContext);

    simulator::log::debug(
      "configured order generation context for the `{}'",
      _listing.getSymbol());
}

auto GeneratorImpl::initializeRandomGenerationExecutors() -> void
{
    for (auto const& pInstrumentCtx : mOrderListingsContexts) {
        auto const& instrument = pInstrumentCtx->getInstrument();

        if (!instrument.getSymbol().has_value()) {
            simulator::log::info(
                "can not initialize random orders generation executable for "
                "instrument id `{}' - the instrument does not have a symbol",
                instrument.getListingId());
            continue;
        }

        std::unique_ptr<Executable> pExecutable;
        try {
            const auto pxSeed = DataLayer::selectOnePriceSeed(
                mDatabaseContext,
                DataLayer::Predicate::eq<DataLayer::PriceSeed>(
                    DataLayer::PriceSeed::Attribute::Symbol,
                    *instrument.getSymbol()));

            pExecutable =
                mRndExecutorFactory->createOrdersExecutable(pInstrumentCtx, pxSeed);
        }
        catch (std::logic_error& e) {
            simulator::log::info(
                "can not initialize random orders generation executable for "
                "`{}' instrument - no price seed entry has been found "
                "for the instrument",
                instrument.getSymbol());
            continue;
        }

        if (pExecutable) {
            mListingsRandomGenerators.emplace_back(
                Executor::create(std::move(pExecutable), mGenerationManager)
            );
        }
    }
}

auto GeneratorImpl::initializeHistoricalExecutor() -> void
{
    if (mOrderListingsContexts.empty()) {
        mHistoricalReplier = nullptr;
        return;
    }

    using DataLayer::DatasourceCmp;
    using DataLayer::Datasource;
    using Attribute = DataLayer::Datasource::Attribute;
    auto const& operatingVenueId = mGenerationManager->getVenue().getVenueId();
    auto pred = DatasourceCmp::eq(Attribute::VenueId, operatingVenueId) &&
                DatasourceCmp::eq(Attribute::Enabled, true);

    std::vector<Datasource> const datasources =
        DataLayer::selectAllDatasources(mDatabaseContext, std::move(pred));

    auto pExecutable = mHistExecutorFactory->createOrdersExecutable(
        datasources,
        mOrderListingsContexts
    );

    mHistoricalReplier =
        Executor::create(std::move(pExecutable), mGenerationManager);
}

auto GeneratorImpl::terminate() noexcept -> void { terminateGenerator(); }

auto GeneratorImpl::process_reply(std::uint64_t instrument_id,
                                  const GeneratedMessage& reply) -> void {
  const auto context_iter = mContextLookup.find(instrument_id);
  if (context_iter == mContextLookup.end()) {
      simulator::log::warn(
          "can not process reply message for instrument {}, "
          "no context has been found",
          instrument_id);
      return;
  }

  OrderRegistryUpdater::update(context_iter->second->takeRegistry(), reply);

  simulator::log::debug("reply message for instrument {} processed",
             instrument_id);
}

auto GeneratorImpl::terminateGenerator() noexcept -> void
{
    if (m_wasTerminated) {
        return;
    }

    m_wasTerminated = true;
    for (auto const& pRandomOrdersExecutable : mListingsRandomGenerators) {
        pRandomOrdersExecutable->terminate();
    }
    mListingsRandomGenerators.clear();

    if (mHistoricalReplier) {
        mHistoricalReplier->terminate();
    }

    mGenerationManager->terminate();
}

} // namespace Simulator::Generator
