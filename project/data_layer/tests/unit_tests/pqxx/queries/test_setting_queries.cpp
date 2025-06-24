#include <string>

#include <fmt/format.h>
#include <gtest/gtest.h>

#include "api/exceptions/exceptions.hpp"
#include "api/models/setting.hpp"
#include "api/predicate/predicate.hpp"
#include "ih/pqxx/queries/setting_queries.hpp"

using namespace Simulator::DataLayer;

namespace {

struct SanitizerStub {
    template<typename T>
    auto operator()(T&& _value) -> std::string
    {
        return fmt::format("`{}`", std::forward<T>(_value));
    }
};

} // namespace


TEST(DataLayer_Pqxx_SettingQuery_Insert, Compose_FromEmptyPatch)
{
    Setting::Patch const patch{};
    SanitizerStub sanitizer{};

    auto const query = Pqxx::SettingQuery::Insert::prepare(patch, sanitizer);
    EXPECT_THROW((void)query.compose(), MalformedPatch);
}

TEST(DataLayer_Pqxx_SettingQuery_Insert, Compose_FromNonEmptyPatch)
{
    SanitizerStub sanitizer{};
    Setting::Patch patch{};
    patch.withKey("Key").withValue("Value");

    auto const query = Pqxx::SettingQuery::Insert::prepare(patch, sanitizer);
    EXPECT_EQ(
        query.compose(),
        "INSERT INTO setting (key, value) "
        "VALUES (`Key`, `Value`)"
    );
}

TEST(DataLayer_Pqxx_SettingQuery_Insert, Compose_WithKeyReturning)
{
    SanitizerStub sanitizer{};
    Setting::Patch patch{};
    patch.withKey("Key").withValue("Value");

    auto const query =
        Pqxx::SettingQuery::Insert::prepare(patch, sanitizer).returningKey();
    EXPECT_EQ(
        query.compose(),
        "INSERT INTO setting (key, value) "
        "VALUES (`Key`, `Value`) "
        "RETURNING key"
    );
}

TEST(DataLayer_Pqxx_SettingQuery_Select, Compose_Empty)
{
    using Query = Pqxx::SettingQuery::Select;
    auto const query = Query::prepare();
    EXPECT_EQ(query.compose(), "SELECT * FROM setting");
}

TEST(DataLayer_Pqxx_SettingQuery_Select, Compose_WithPredicateExpression)
{
    using Column = Setting::Attribute;
    using Query = Pqxx::SettingQuery::Select;

    SanitizerStub sanitizer{};
    auto const predicate = SettingCmp::eq(Column::Key, std::string{"Key"}) &&
                           SettingCmp::eq(Column::Value, std::string{"Value"});

    auto const query = Query::prepare().by(predicate, sanitizer);
    EXPECT_EQ(
        query.compose(),
        "SELECT * FROM setting "
        "WHERE key = `Key` AND value = `Value`"
    );
}

TEST(DataLayer_Pqxx_SettingQuery_Select, Compose_WithByKeyPredicate)
{
    using Query = Pqxx::SettingQuery::Select;

    SanitizerStub sanitizer{};
    auto const query = Query::prepare().byKey("Key", sanitizer);
    EXPECT_EQ(query.compose(), "SELECT * FROM setting WHERE key = `Key`");
}

TEST(DataLayer_Pqxx_SettingQuery_Update, Compose_FromEmptyPatch)
{
    Setting::Patch const patch{};
    SanitizerStub sanitizer{};

    auto const query = Pqxx::SettingQuery::Update::prepare(patch, sanitizer);
    EXPECT_THROW((void)query.compose(), MalformedPatch);
}

TEST(DataLayer_Pqxx_SettingQuery_Update, Compose_FromNonEmptyPatch)
{
    SanitizerStub sanitizer{};
    Setting::Patch patch{};
    patch.withKey("Key").withValue("Value");

    auto const query = Pqxx::SettingQuery::Update::prepare(patch, sanitizer);
    EXPECT_EQ(
        query.compose(),
        "UPDATE setting SET key = `Key`, value = `Value`"
    );
}

TEST(DataLayer_Pqxx_SettingQuery_Update, Compose_WithPredicate)
{
    using Column = Setting::Attribute;
    using Query = Pqxx::SettingQuery::Update;

    SanitizerStub sanitizer{};
    Setting::Patch patch{};
    patch.withKey("Key").withValue("Value");

    auto const predicate = SettingCmp::eq(Column::Key, std::string{"Key1"}) &&
                           SettingCmp::eq(Column::Value, std::string{"Value1"});

    auto const query =
        Query::prepare(patch, sanitizer).by(predicate, sanitizer);

    EXPECT_EQ(
        query.compose(),
        "UPDATE setting "
        "SET key = `Key`, value = `Value` "
        "WHERE key = `Key1` AND value = `Value1`"
    );
}

TEST(DataLayer_Pqxx_SettingQuery_Update, Compose_WithKeyReturning)
{
    using Column = Setting::Attribute;
    using Query = Pqxx::SettingQuery::Update;

    SanitizerStub sanitizer{};
    Setting::Patch patch{};
    patch.withKey("Key").withValue("Value");

    auto const predicate = SettingCmp::eq(Column::Key, std::string{"Key1"}) &&
                           SettingCmp::eq(Column::Value, std::string{"Value1"});

    auto const query = Query::prepare(patch, sanitizer)
                           .by(predicate, sanitizer)
                           .returningKey();

    EXPECT_EQ(
        query.compose(),
        "UPDATE setting "
        "SET key = `Key`, value = `Value` "
        "WHERE key = `Key1` AND value = `Value1` "
        "RETURNING key"
    );
}
