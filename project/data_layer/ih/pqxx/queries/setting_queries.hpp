#ifndef SIMULATOR_DATA_LAYER_IH_PQXX_QUERIES_SETTING_QUERIES_HPP_
#define SIMULATOR_DATA_LAYER_IH_PQXX_QUERIES_SETTING_QUERIES_HPP_

#include <string>

#include "api/models/setting.hpp"
#include "api/inspectors/setting.hpp"
#include "ih/pqxx/common/names/database_entries.hpp"
#include "ih/pqxx/queries/detail/insert_query_builder.hpp"
#include "ih/pqxx/queries/detail/select_query_builder.hpp"
#include "ih/pqxx/queries/detail/update_query_builder.hpp"

namespace Simulator::DataLayer::Pqxx::SettingQuery {

class Insert {
  public:
    using PatchType = DataLayer::Setting::Patch;

    template<typename Sanitizer>
    static auto prepare(PatchType const& _snapshot, Sanitizer& _sanitizer)
        -> Insert
    {
        Insert query{};
        query.build(_snapshot, _sanitizer);
        return query;
    }

    auto returningKey() -> Insert&
    {
        constexpr auto column = Setting::Attribute::Key;
        mBuilder.withReturning(column);
        return *this;
    }

    [[nodiscard]]
    auto compose() const -> std::string
    {
        return mBuilder.compose();
    }

  private:
    Insert() :
        mBuilder(std::string{Table::Setting})
    {}

    template<typename Sanitizer>
    void build(PatchType const& _snapshot, Sanitizer& _sanitizer)
    {
        auto dataExtractor = mBuilder.makeDataExtractor(_sanitizer);

        using PatchReader = SettingPatchReader<decltype(dataExtractor)>;
        PatchReader reader{dataExtractor};
        reader.read(_snapshot);

        mBuilder.build(dataExtractor);
    }

    Detail::InsertQueryBuilder mBuilder;
};

class Select {
  public:
    using Predicate = Predicate::Expression<Setting>;

    static auto prepare() -> Select { return Select{}; }

    template<typename Sanitizer>
    auto by(Predicate const& _predicate, Sanitizer& _sanitizer) -> Select&
    {
        mBuilder.withPredicateExpression(_predicate, _sanitizer);
        return *this;
    }

    template<typename Sanitizer>
    auto byKey(std::string const& _key, Sanitizer& _sanitizer) -> Select&
    {
        constexpr auto column = Setting::Attribute::Key;
        mBuilder.withEqPredicate(column, _key, _sanitizer);
        return *this;
    }

    [[nodiscard]]
    auto compose() const -> std::string
    {
        return mBuilder.compose();
    }

  private:
    Select() :
        mBuilder(std::string{Table::Setting})
    {}

    Detail::SelectQueryBuilder mBuilder;
};

class Update {
  public:
    using PatchType = DataLayer::Setting::Patch;
    using Predicate = Predicate::Expression<Setting>;

    template<typename Sanitizer>
    static auto prepare(PatchType const& _snapshot, Sanitizer& _sanitizer)
        -> Update
    {
        Update query{};
        query.build(_snapshot, _sanitizer);
        return query;
    }

    template<typename Sanitizer>
    auto by(Predicate const& _predicate, Sanitizer& _sanitizer) -> Update&
    {
        mBuilder.withPredicateExpression(_predicate, _sanitizer);
        return *this;
    }

    auto returningKey() -> Update&
    {
        constexpr auto column = Setting::Attribute::Key;
        mBuilder.withReturning(column);
        return *this;
    }

    [[nodiscard]]
    auto compose() const -> std::string
    {
        return mBuilder.compose();
    }

  private:
    Update() :
        mBuilder(std::string{Table::Setting})
    {}

    template<typename Sanitizer>
    void build(PatchType const& _snapshot, Sanitizer& _sanitizer)
    {
        auto dataExtractor = mBuilder.makeDataExtractor(_sanitizer);

        using PatchReader = SettingPatchReader<decltype(dataExtractor)>;
        PatchReader reader{dataExtractor};
        reader.read(_snapshot);

        mBuilder.build(dataExtractor);
    }

    Detail::UpdateQueryBuilder mBuilder;
};

} // namespace Simulator::DataLayer::Pqxx::SettingQuery

#endif // SIMULATOR_DATA_LAYER_IH_PQXX_QUERIES_SETTING_QUERIES_HPP_
