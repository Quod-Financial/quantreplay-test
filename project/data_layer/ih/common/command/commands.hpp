#ifndef SIMULATOR_DATA_LAYER_IH_COMMON_COMMAND_COMMANDS_HPP_
#define SIMULATOR_DATA_LAYER_IH_COMMON_COMMAND_COMMANDS_HPP_

#include <optional>
#include <stdexcept>
#include <utility>

#include "api/predicate/expression.hpp"

namespace Simulator::DataLayer::Command {

template<typename Model>
class Insert {
  public:
    using ModelType = Model;
    using PatchType = typename ModelType::Patch;

    Insert() = delete;

    static auto create(PatchType _initialPatch) -> Insert
    {
        return Insert{std::move(_initialPatch)};
    }

    [[nodiscard]]
    auto getInitialPatch() const noexcept -> PatchType const&
    {
        return mInitialPatch;
    }

    [[nodiscard]]
    auto takeResult() -> ModelType&
    {
        if (!mResult.has_value()) {
            throw std::logic_error{
                "no result has been set into the `Insert' database "
                "command"};
        }
        return *mResult;
    }

    auto setResult(ModelType _result) -> void { mResult = std::move(_result); }


  private:
    explicit Insert(PatchType _initialPatch) noexcept :
        mInitialPatch(std::move(_initialPatch))
    {}

    std::optional<ModelType> mResult;
    PatchType mInitialPatch;
};

template<typename Model>
class SelectOne {
  public:
    using ModelType = Model;
    using PredicateType = Predicate::Expression<ModelType>;

    SelectOne() = delete;

    static auto create(PredicateType _predicate) -> SelectOne
    {
        return SelectOne{std::move(_predicate)};
    }

    [[nodiscard]]
    auto getPredicate() noexcept -> PredicateType const&
    {
        return mPredicate;
    }

    [[nodiscard]]
    auto takeResult() -> ModelType&
    {
        if (!mResult.has_value()) {
            throw std::logic_error{
                "BUG: no result has been set into the `SelectOne` database "
                "command"};
        }
        return *mResult;
    }

    auto setResult(ModelType _result) -> void { mResult = std::move(_result); }

  private:
    explicit SelectOne(PredicateType _predicate) noexcept :
        mPredicate(std::move(_predicate))
    {}

    PredicateType mPredicate;
    std::optional<ModelType> mResult;
};

template<typename Model>
class SelectAll {
  public:
    using ModelType = Model;
    using PredicateType = Predicate::Expression<ModelType>;

    SelectAll() = delete;

    static auto create(std::optional<PredicateType> _predicate) -> SelectAll
    {
        return SelectAll{std::move(_predicate)};
    }

    [[nodiscard]]
    auto getPredicate() noexcept -> std::optional<PredicateType> const&
    {
        return mPredicate;
    }

    [[nodiscard]]
    auto takeResult() -> std::vector<ModelType>&
    {
        return mResult;
    }

    auto setResult(std::vector<ModelType> _result) -> void
    {
        mResult = std::move(_result);
    }

  private:
    explicit SelectAll(std::optional<PredicateType> _predicate) noexcept :
        mPredicate(std::move(_predicate))
    {}

    std::optional<PredicateType> mPredicate;
    std::vector<ModelType> mResult;
};

template<typename Model>
class UpdateOne {
  public:
    using ModelType = Model;
    using PatchType = typename ModelType::Patch;
    using PredicateType = Predicate::Expression<ModelType>;

    UpdateOne() = delete;

    [[nodiscard]]
    static auto create(PatchType _patch, PredicateType _predicate) -> UpdateOne
    {
        return UpdateOne{std::move(_patch), std::move(_predicate)};
    }

    auto getPatch() const noexcept -> PatchType const& { return mPatch; }

    [[nodiscard]]
    auto getPredicate() noexcept -> PredicateType const&
    {
        return mPredicate;
    }

    [[nodiscard]]
    auto takeResult() -> ModelType&
    {
        if (!mResult.has_value()) {
            throw std::logic_error{
                "no result has been set into the `UpdateOne` database "
                "command"};
        }
        return *mResult;
    }

    auto setResult(ModelType _result) -> void { mResult = std::move(_result); }

  private:
    explicit UpdateOne(PatchType _patch, PredicateType _predicate) noexcept :
        mPatch(std::move(_patch)), mPredicate(std::move(_predicate))
    {}

    PatchType mPatch;
    PredicateType mPredicate;
    std::optional<ModelType> mResult;
};

template<typename Model>
class UpdateAll {
  public:
    using ModelType = Model;
    using PatchType = typename ModelType::Patch;
    using PredicateType = Predicate::Expression<ModelType>;

    UpdateAll() = delete;

    [[nodiscard]]
    static auto create(
        PatchType _patch,
        std::optional<PredicateType> _predicate
    ) -> UpdateAll
    {
        return UpdateAll{std::move(_patch), std::move(_predicate)};
    }

    auto getPatch() const noexcept -> PatchType const& { return mPatch; }

    [[nodiscard]]
    auto getPredicate() noexcept -> std::optional<PredicateType> const&
    {
        return mPredicate;
    }

    [[nodiscard]]
    auto takeResult() -> std::vector<ModelType>&
    {
        return mResult;
    }

    auto setResult(std::vector<ModelType> _result) -> void
    {
        mResult = std::move(_result);
    }

  private:
    explicit UpdateAll(
        PatchType _patch,
        std::optional<PredicateType> _predicate
    ) noexcept :
        mPatch(std::move(_patch)), mPredicate(std::move(_predicate))
    {}

    PatchType mPatch;
    std::optional<PredicateType> mPredicate;
    std::vector<ModelType> mResult;
};

template<typename Model>
class DeleteOne {
  public:
    using ModelType = Model;
    using PredicateType = Predicate::Expression<ModelType>;

    DeleteOne() = delete;

    static auto create(PredicateType _predicate) -> DeleteOne
    {
        return DeleteOne{std::move(_predicate)};
    }

    [[nodiscard]]
    auto getPredicate() noexcept -> PredicateType const&
    {
        return mPredicate;
    }

  private:
    explicit DeleteOne(PredicateType _predicate) noexcept :
        mPredicate(std::move(_predicate))
    {}

    PredicateType mPredicate;
};

template<typename Model>
class DeleteAll {
  public:
    using ModelType = Model;
    using PredicateType = Predicate::Expression<ModelType>;

    DeleteAll() = delete;

    static auto create(std::optional<PredicateType> _predicate) -> DeleteAll
    {
        return DeleteAll{std::move(_predicate)};
    }

    [[nodiscard]]
    auto getPredicate() noexcept -> std::optional<PredicateType> const&
    {
        return mPredicate;
    }

  private:
    explicit DeleteAll(std::optional<PredicateType> _predicate) noexcept :
        mPredicate(std::move(_predicate))
    {}

    std::optional<PredicateType> mPredicate;
};

} // namespace Simulator::DataLayer::Command

#endif // SIMULATOR_DATA_LAYER_IH_COMMON_COMMAND_COMMANDS_HPP_
