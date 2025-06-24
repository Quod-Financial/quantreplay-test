#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <pqxx/except>

#include "api/exceptions/exceptions.hpp"
#include "ih/pqxx/database/transaction.hpp"

using namespace Simulator;
using namespace Simulator::DataLayer;

using ::testing::Throw;

namespace {

class MockTransactionImpl {
  public:
    MOCK_METHOD(void, exec0, (pqxx::zview));
    MOCK_METHOD(void, exec1, (pqxx::zview));
    MOCK_METHOD(void, exec, (pqxx::zview));
};

} // namespace

class DataLayer_Pqxx_TransactionHandler : public ::testing::Test {
  public:
    using TransactionHandler = Pqxx::TransactionHandler<MockTransactionImpl>;

    auto transactionImpl() -> MockTransactionImpl& { return mTransactionImpl; }

    auto makeTransactionHandler() -> TransactionHandler
    {
        return TransactionHandler{mTransactionImpl};
    }

  private:
    MockTransactionImpl mTransactionImpl;
};


TEST_F(DataLayer_Pqxx_TransactionHandler, Exec0_ConnectionBrokenThrown)
{
    TransactionHandler handler = makeTransactionHandler();

    EXPECT_CALL(transactionImpl(), exec0)
        .WillOnce(Throw(pqxx::broken_connection{""}));

    EXPECT_THROW(handler.exec0(""), DataLayer::ConnectionFailure);
}

TEST_F(DataLayer_Pqxx_TransactionHandler, Exec0_UnexpectedRowsThrown)
{
    TransactionHandler handler = makeTransactionHandler();

    EXPECT_CALL(transactionImpl(), exec0)
        .WillOnce(Throw(pqxx::unexpected_rows{""}));

    EXPECT_THROW(handler.exec0(""), DataLayer::CardinalityViolationError);
}

TEST_F(DataLayer_Pqxx_TransactionHandler, Exec0_UniqueConstraintViolation)
{
    TransactionHandler handler = makeTransactionHandler();

    EXPECT_CALL(transactionImpl(), exec0)
        .WillOnce(Throw(pqxx::unique_violation{""}));

    EXPECT_THROW(handler.exec0(""), DataLayer::DataIntegrityError);
}

TEST_F(DataLayer_Pqxx_TransactionHandler, Exec0_ForeignKeyViolation)
{
    TransactionHandler handler = makeTransactionHandler();

    EXPECT_CALL(transactionImpl(), exec0)
        .WillOnce(Throw(pqxx::foreign_key_violation{""}));

    EXPECT_THROW(handler.exec0(""), DataLayer::DataIntegrityError);
}

TEST_F(DataLayer_Pqxx_TransactionHandler, Exec0_NotNullViolation) {
    TransactionHandler handler = makeTransactionHandler();

    EXPECT_CALL(transactionImpl(), exec0)
        .WillOnce(Throw(pqxx::not_null_violation{""}));

    EXPECT_THROW(handler.exec0(""), DataLayer::DataIntegrityError);
}

TEST_F(DataLayer_Pqxx_TransactionHandler, Exec0_DataIntegrityViolation)
{
    TransactionHandler handler = makeTransactionHandler();

    EXPECT_CALL(transactionImpl(), exec0)
        .WillOnce(Throw(pqxx::integrity_constraint_violation{""}));

    EXPECT_THROW(handler.exec0(""), DataLayer::DataIntegrityError);
}

TEST_F(DataLayer_Pqxx_TransactionHandler, Exec0_PqxxDataError)
{
    TransactionHandler handler = makeTransactionHandler();

    EXPECT_CALL(transactionImpl(), exec0)
        .WillOnce(Throw(pqxx::data_exception{""}));

    EXPECT_THROW(handler.exec0(""), DataLayer::InternalError);
}

TEST_F(DataLayer_Pqxx_TransactionHandler, Exec0_GenericException)
{
    TransactionHandler handler = makeTransactionHandler();

    EXPECT_CALL(transactionImpl(), exec0)
        .WillOnce(Throw(std::runtime_error{""}));

    EXPECT_THROW(handler.exec0(""), DataLayer::InternalError);
}

TEST_F(DataLayer_Pqxx_TransactionHandler, Exec0_UnkonwError)
{
    TransactionHandler handler = makeTransactionHandler();

    EXPECT_CALL(transactionImpl(), exec0)
        .WillOnce(Throw(42)); // NOLINT: test value

    EXPECT_THROW(handler.exec0(""), DataLayer::InternalError);
}

TEST_F(DataLayer_Pqxx_TransactionHandler, Exec1_ConnectionBrokenThrown)
{
    TransactionHandler handler = makeTransactionHandler();

    EXPECT_CALL(transactionImpl(), exec1)
        .WillOnce(Throw(pqxx::broken_connection{""}));

    EXPECT_THROW(handler.exec1(""), DataLayer::ConnectionFailure);
}

TEST_F(DataLayer_Pqxx_TransactionHandler, Exec1_UnexpectedRowsThrown)
{
    TransactionHandler handler = makeTransactionHandler();

    EXPECT_CALL(transactionImpl(), exec1)
        .WillOnce(Throw(pqxx::unexpected_rows{""}));

    EXPECT_THROW(handler.exec1(""), DataLayer::CardinalityViolationError);
}

TEST_F(DataLayer_Pqxx_TransactionHandler, Exec1_UniqueConstraintViolation)
{
    TransactionHandler handler = makeTransactionHandler();

    EXPECT_CALL(transactionImpl(), exec1)
        .WillOnce(Throw(pqxx::unique_violation{""}));

    EXPECT_THROW(handler.exec1(""), DataLayer::DataIntegrityError);
}

TEST_F(DataLayer_Pqxx_TransactionHandler, Exec1_ForeignKeyViolation)
{
    TransactionHandler handler = makeTransactionHandler();

    EXPECT_CALL(transactionImpl(), exec1)
        .WillOnce(Throw(pqxx::foreign_key_violation{""}));

    EXPECT_THROW(handler.exec1(""), DataLayer::DataIntegrityError);
}

TEST_F(DataLayer_Pqxx_TransactionHandler, Exec1_NotNullViolation) {
    TransactionHandler handler = makeTransactionHandler();

    EXPECT_CALL(transactionImpl(), exec1)
        .WillOnce(Throw(pqxx::not_null_violation{""}));

    EXPECT_THROW(handler.exec1(""), DataLayer::DataIntegrityError);
}

TEST_F(DataLayer_Pqxx_TransactionHandler, Exec1_DataIntegrityViolation)
{
    TransactionHandler handler = makeTransactionHandler();

    EXPECT_CALL(transactionImpl(), exec1)
        .WillOnce(Throw(pqxx::integrity_constraint_violation{""}));

    EXPECT_THROW(handler.exec1(""), DataLayer::DataIntegrityError);
}

TEST_F(DataLayer_Pqxx_TransactionHandler, Exec1_PqxxDataError)
{
    TransactionHandler handler = makeTransactionHandler();

    EXPECT_CALL(transactionImpl(), exec1)
        .WillOnce(Throw(pqxx::data_exception{""}));

    EXPECT_THROW(handler.exec1(""), DataLayer::InternalError);
}

TEST_F(DataLayer_Pqxx_TransactionHandler, Exec1_GenericException)
{
    TransactionHandler handler = makeTransactionHandler();

    EXPECT_CALL(transactionImpl(), exec1)
        .WillOnce(Throw(std::runtime_error{""}));

    EXPECT_THROW(handler.exec1(""), DataLayer::InternalError);
}

TEST_F(DataLayer_Pqxx_TransactionHandler, Exec1_UnkonwError)
{
    TransactionHandler handler = makeTransactionHandler();

    EXPECT_CALL(transactionImpl(), exec1)
        .WillOnce(Throw(42)); // NOLINT: Test value

    EXPECT_THROW(handler.exec1(""), DataLayer::InternalError);
}

TEST_F(DataLayer_Pqxx_TransactionHandler, Exec_ConnectionBrokenThrown)
{
    TransactionHandler handler = makeTransactionHandler();

    EXPECT_CALL(transactionImpl(), exec)
        .WillOnce(Throw(pqxx::broken_connection{""}));

    EXPECT_THROW(handler.exec(""), DataLayer::ConnectionFailure);
}

TEST_F(DataLayer_Pqxx_TransactionHandler, Exec_UniqueConstraintViolation)
{
    TransactionHandler handler = makeTransactionHandler();

    EXPECT_CALL(transactionImpl(), exec)
        .WillOnce(Throw(pqxx::unique_violation{""}));

    EXPECT_THROW(handler.exec(""), DataLayer::DataIntegrityError);
}

TEST_F(DataLayer_Pqxx_TransactionHandler, Exec_ForeignKeyViolation)
{
    TransactionHandler handler = makeTransactionHandler();

    EXPECT_CALL(transactionImpl(), exec)
        .WillOnce(Throw(pqxx::foreign_key_violation{""}));

    EXPECT_THROW(handler.exec(""), DataLayer::DataIntegrityError);
}

TEST_F(DataLayer_Pqxx_TransactionHandler, Exec_NotNullViolation) {
    TransactionHandler handler = makeTransactionHandler();

    EXPECT_CALL(transactionImpl(), exec)
        .WillOnce(Throw(pqxx::not_null_violation{""}));

    EXPECT_THROW(handler.exec(""), DataLayer::DataIntegrityError);
}

TEST_F(DataLayer_Pqxx_TransactionHandler, Exec_DataIntegrityViolation)
{
    TransactionHandler handler = makeTransactionHandler();

    EXPECT_CALL(transactionImpl(), exec)
        .WillOnce(Throw(pqxx::integrity_constraint_violation{""}));

    EXPECT_THROW(handler.exec(""), DataLayer::DataIntegrityError);
}

TEST_F(DataLayer_Pqxx_TransactionHandler, Exec_PqxxDataError)
{
    TransactionHandler handler = makeTransactionHandler();

    EXPECT_CALL(transactionImpl(), exec)
        .WillOnce(Throw(pqxx::data_exception{""}));

    EXPECT_THROW(handler.exec(""), DataLayer::InternalError);
}

TEST_F(DataLayer_Pqxx_TransactionHandler, Exec_GenericException)
{
    TransactionHandler handler = makeTransactionHandler();

    EXPECT_CALL(transactionImpl(), exec)
        .WillOnce(Throw(std::runtime_error{""}));

    EXPECT_THROW(handler.exec(""), DataLayer::InternalError);
}

TEST_F(DataLayer_Pqxx_TransactionHandler, Exec_UnkonwError)
{
    TransactionHandler handler = makeTransactionHandler();

    EXPECT_CALL(transactionImpl(), exec)
        .WillOnce(Throw(42)); // NOLINT: Test value

    EXPECT_THROW(handler.exec(""), DataLayer::InternalError);
}