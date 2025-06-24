#include <fmt/format.h>
#include <gtest/gtest.h>

#include <string>

#include "api/exceptions/exceptions.hpp"
#include "api/models/column_mapping.hpp"
#include "api/models/datasource.hpp"
#include "ih/pqxx/queries/datasource_queries.hpp"

namespace Simulator::DataLayer {
namespace {

// NOLINTBEGIN(*magic-numbers*)

struct SanitizerStub {
  template <typename T>
  auto operator()(T&& _value) -> std::string {
    return fmt::format("`{}`", std::forward<T>(_value));
  }
};

struct DataLayerPqxxDatasourceQueryInsert : public ::testing::Test {
  using Query = Pqxx::DatasourceQuery::Insert;

  Datasource::Patch patch;
  SanitizerStub sanitizer;
};

TEST_F(DataLayerPqxxDatasourceQueryInsert,
       ThrowsExceptionIfComposedFromEmptyPatch) {
  const auto query = Query::prepare(patch, sanitizer);
  EXPECT_THROW((void)query.compose(), MalformedPatch);
}

TEST_F(DataLayerPqxxDatasourceQueryInsert, ComposedFromNonEmptyPatch) {
  patch.with_venue_id("LSE").with_name("LSE Data");

  const auto query = Query::prepare(patch, sanitizer).returningId();
  EXPECT_EQ(query.compose(),
            "INSERT INTO data_source (name, venue_id) "
            "VALUES (`LSE Data`, `LSE`) "
            "RETURNING data_source_id");
}

struct DataLayerPqxxDatasourceQuerySelect : public ::testing::Test {
  using Query = Pqxx::DatasourceQuery::Select;

  SanitizerStub sanitizer;
};

TEST_F(DataLayerPqxxDatasourceQuerySelect, ComposedWithoutPredicate) {
  const auto query = Query::prepare();
  EXPECT_EQ(query.compose(), "SELECT * FROM data_source");
}

TEST_F(DataLayerPqxxDatasourceQuerySelect, ComposedWithPredicateExpression) {
  using Column = Datasource::Attribute;

  const auto predicate =
      DatasourceCmp::eq(Column::Format, Datasource::Format::Csv) &&
      DatasourceCmp::eq(Column::Enabled, false);

  const auto query = Query::prepare().by(predicate, sanitizer);
  EXPECT_EQ(query.compose(),
            "SELECT * FROM data_source "
            "WHERE format = `CSV` AND enabled = `false`");
}

TEST_F(DataLayerPqxxDatasourceQuerySelect,
       ComposedWithByDatasourceIdPredicate) {
  const auto query = Query::prepare().byDatasourceId(42, sanitizer);
  EXPECT_EQ(query.compose(),
            "SELECT * FROM data_source WHERE data_source_id = `42`");
}

struct DataLayerPqxxDatasourceQueryUpdate : public ::testing::Test {
  using Query = Pqxx::DatasourceQuery::Update;

  Datasource::Patch patch;
  SanitizerStub sanitizer;
};

TEST_F(DataLayerPqxxDatasourceQueryUpdate,
       ThrowsExceptionIfComposedFromEmptyPatch) {
  const auto query = Query::prepare(patch, sanitizer);
  EXPECT_THROW((void)query.compose(), MalformedPatch);
}

TEST_F(DataLayerPqxxDatasourceQueryUpdate, ComposedFromNonEmptyPatch) {
  patch.with_venue_id("LSE").with_name("LSE Data");

  const auto query = Query::prepare(patch, sanitizer);
  EXPECT_EQ(query.compose(),
            "UPDATE data_source SET name = `LSE Data`, venue_id = `LSE`");
}

TEST_F(DataLayerPqxxDatasourceQueryUpdate, ComposedWithPredicate) {
  using Column = Datasource::Attribute;

  patch.with_venue_id("LSE").with_name("LSE Data");
  const auto predicate = DatasourceCmp::eq(Column::DatasourceId, 42);

  const auto query = Query::prepare(patch, sanitizer).by(predicate, sanitizer);

  EXPECT_EQ(query.compose(),
            "UPDATE data_source "
            "SET name = `LSE Data`, venue_id = `LSE` "
            "WHERE data_source_id = `42`");
}

TEST_F(DataLayerPqxxDatasourceQueryUpdate, ComposedWithDatasourceIdReturning) {
  patch.with_venue_id("LSE").with_name("LSE Data");

  const auto query = Query::prepare(patch, sanitizer).returningId();
  EXPECT_EQ(query.compose(),
            "UPDATE data_source "
            "SET name = `LSE Data`, venue_id = `LSE` "
            "RETURNING data_source_id");
}

struct DataLayerPqxxColumnMappingQueryInsert : public ::testing::Test {
  ColumnMapping::Patch patch;
  SanitizerStub sanitizer;
};

TEST_F(DataLayerPqxxColumnMappingQueryInsert, ComposedFromEmptyColumnMapping) {
  const auto mapping = ColumnMapping::create(patch, 42);

  const auto query =
      Pqxx::ColumnMappingQuery::Insert::prepare(mapping, sanitizer);

  EXPECT_EQ(query.compose(),
            "INSERT INTO "
            "column_mapping (data_source_id, column_from, column_to) "
            "VALUES (`42`, ``, ``)");
}

TEST_F(DataLayerPqxxColumnMappingQueryInsert, ComposedFromColumnMapping) {
  patch.with_column_from("ReceivedTimestamp").with_column_to("timestamp");
  const auto mapping = ColumnMapping::create(patch, 42);

  const auto query =
      Pqxx::ColumnMappingQuery::Insert::prepare(mapping, sanitizer);

  EXPECT_EQ(query.compose(),
            "INSERT INTO "
            "column_mapping (data_source_id, column_from, column_to) "
            "VALUES (`42`, `ReceivedTimestamp`, `timestamp`)");
}

TEST(DataLayerPqxxColumnMappingQuerySelect, ComposedWithoutPredicate) {
  const auto query = Pqxx::ColumnMappingQuery::Select::prepare();
  EXPECT_EQ(query.compose(), "SELECT * FROM column_mapping");
}

TEST(DataLayerPqxxColumnMappingQuerySelect, ComposedWithPredicate) {
  SanitizerStub sanitizer;
  auto query = Pqxx::ColumnMappingQuery::Select::prepare();
  query.byDatasourceId(42, sanitizer);

  EXPECT_EQ(query.compose(),
            "SELECT * FROM column_mapping WHERE data_source_id = `42`");
}

TEST(DataLayerPqxxColumnMappingQueryDelete, ComposesWithoutPredicate) {
  const auto query = Pqxx::ColumnMappingQuery::Delete::prepare();
  EXPECT_EQ(query.compose(), "DELETE FROM column_mapping");
}

TEST(DataLayerPqxxColumnMappingQueryDelete, ComposesWithPredicate) {
  SanitizerStub sanitizer;
  auto query = Pqxx::ColumnMappingQuery::Delete::prepare();
  query.byDatasourceId(42, sanitizer);

  EXPECT_EQ(query.compose(),
            "DELETE FROM column_mapping WHERE data_source_id = `42`");
}

// NOLINTEND(*magic-numbers*)

}  // namespace
}  // namespace Simulator::DataLayer