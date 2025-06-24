#include <optional>
#include <stdexcept>
#include <vector>

#include <gtest/gtest.h>

#include "api/predicate/predicate.hpp"
#include "common/model.hpp"
#include "ih/common/command/commands.hpp"

using namespace Simulator::DataLayer;

using Attribute = TestModel::Attribute;
using InsertCommand = Command::Insert<TestModel>;
using SelectOneCommand = Command::SelectOne<TestModel>;
using SelectAllCommand = Command::SelectAll<TestModel>;
using UpdateOneCommand = Command::UpdateOne<TestModel>;
using UpdateAllCommand = Command::UpdateAll<TestModel>;
using DeleteAllCommand = Command::DeleteAll<TestModel>;


TEST(DataLayer_Common_InsertCommand, GetResult_ResultNotSet)
{
    auto command = InsertCommand::create(TestModel::Patch{});
    EXPECT_THROW((void)command.takeResult(), std::logic_error);
}

TEST(DataLayer_Common_InsertCommand, GetResult_ResultSet)
{
    auto command = InsertCommand::create(TestModel::Patch{});

    command.setResult(TestModel{});
    EXPECT_NO_THROW((void)command.takeResult());
}


TEST(DataLayer_Common_SelectOneCommand, GetResult_ResultNotSet)
{
    auto const pred = Predicate::eq<TestModel>(Attribute::BooleanField, true);
    auto command = SelectOneCommand::create(pred);

    EXPECT_THROW((void)command.takeResult(), std::logic_error);
}

TEST(DataLayer_Common_SelectOneCommand, GetResult_ResultSet)
{
    auto const pred = Predicate::eq<TestModel>(Attribute::BooleanField, true);
    auto command = SelectOneCommand::create(pred);

    command.setResult(TestModel{});
    EXPECT_NO_THROW((void)command.takeResult());
}


TEST(DataLayer_Common_SelectAllCommand, GetPredicate_NotSpecified)
{
    auto command = SelectAllCommand::create(std::nullopt);
    EXPECT_FALSE(command.getPredicate().has_value());
}

TEST(DataLayer_Common_SelectAllCommand, GetPredicate_Specified)
{
    auto const pred = Predicate::eq<TestModel>(Attribute::BooleanField, true);
    auto command = SelectAllCommand::create(pred);
    EXPECT_TRUE(command.getPredicate().has_value());
}

TEST(DataLayer_Common_SelectAllCommand, GetResult_ResultNotSet)
{
    auto command = SelectAllCommand::create(std::nullopt);
    EXPECT_TRUE(command.takeResult().empty());
}

TEST(DataLayer_Common_SelectAllCommand, GetResult_ResultSet)
{
    auto command = SelectAllCommand::create(std::nullopt);

    command.setResult(std::vector<TestModel>{TestModel{}});
    EXPECT_EQ(command.takeResult().size(), 1);
}


TEST(DataLayer_Common_UpdateOneCommand, GetResult_ResultNotSet)
{
    auto const pred = Predicate::eq<TestModel>(Attribute::BooleanField, true);
    auto command = UpdateOneCommand::create(TestModel::Patch{}, pred);

    EXPECT_THROW((void)command.takeResult(), std::logic_error);
}

TEST(DataLayer_Common_UpdateOneCommand, GetResult_ResultSet)
{
    auto const pred = Predicate::eq<TestModel>(Attribute::BooleanField, true);
    auto command = UpdateOneCommand::create(TestModel::Patch{}, pred);

    command.setResult(TestModel{});
    EXPECT_NO_THROW((void)command.takeResult());
}


TEST(DataLayer_Common_UpdateAllCommand, GetPredicate_NotSpecified)
{
    auto command = UpdateAllCommand::create(TestModel::Patch{}, std::nullopt);
    EXPECT_FALSE(command.getPredicate().has_value());
}

TEST(DataLayer_Common_UpdateAllCommand, GetPredicate_Specified)
{
    auto const pred = Predicate::eq<TestModel>(Attribute::BooleanField, true);
    auto command = UpdateAllCommand::create(TestModel::Patch{}, pred);
    EXPECT_TRUE(command.getPredicate().has_value());
}

TEST(DataLayer_Common_UpdateAllCommand, GetResult_ResultNotSet)
{
    auto command = UpdateAllCommand::create(TestModel::Patch{}, std::nullopt);
    EXPECT_TRUE(command.takeResult().empty());
}

TEST(DataLayer_Common_UpdateAllCommand, GetResult_ResultSet)
{
    auto command = UpdateAllCommand::create(TestModel::Patch{}, std::nullopt);

    command.setResult(std::vector<TestModel>{TestModel{}});
    EXPECT_EQ(command.takeResult().size(), 1);
}


TEST(DataLayer_Common_DeleteAllCommand, GetPredicate_NotSpecified)
{
    auto command = DeleteAllCommand::create(std::nullopt);
    EXPECT_FALSE(command.getPredicate().has_value());
}

TEST(DataLayer_Common_DeleteAllCommand, GetPredicate_Specified)
{
    auto const pred = Predicate::eq<TestModel>(Attribute::BooleanField, true);
    auto command = DeleteAllCommand::create(pred);
    EXPECT_TRUE(command.getPredicate().has_value());
}