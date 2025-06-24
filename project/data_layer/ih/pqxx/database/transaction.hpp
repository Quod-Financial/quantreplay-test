#ifndef SIMULATOR_DATA_LAYER_IH_PQXX_DATABASE_TRANSACTION_HPP_
#define SIMULATOR_DATA_LAYER_IH_PQXX_DATABASE_TRANSACTION_HPP_

#include <pqxx/connection>
#include <pqxx/result>
#include <pqxx/row>
#include <pqxx/transaction>

#include <functional>
#include <utility>

#include "api/exceptions/exceptions.hpp"
#include "log/logging.hpp"

namespace Simulator::DataLayer::Pqxx {

template<typename TransactionImpl>
class TransactionHandler {
  public:
    TransactionHandler() = delete;

    explicit TransactionHandler(TransactionImpl& _impl) :
        mImplementation{_impl}
    {}

    auto exec0(std::string_view _query)
    {
        auto const trxExecutor = [&] {
            return impl().exec0(pqxx::zview(_query));
        };
        return handleErrors(trxExecutor);
    }

    auto exec1(std::string_view _query)
    {
        auto const trxExecutor = [&] {
            return impl().exec1(pqxx::zview(_query));
        };
        return handleErrors(trxExecutor);
    }

    auto exec(std::string_view _query)
    {
        auto const trxExecutor = [&] {
            return impl().exec(pqxx::zview(_query));
        };
        return handleErrors(trxExecutor);
    }

  private:
    template<typename TransactionExecutor>
    auto handleErrors(TransactionExecutor _executor)
    {
        try {
            return _executor();
        } catch (pqxx::broken_connection const& _ex) {
            simulator::log::warn(
                "database transaction failed with "
                "pqxx::broken_connection error: `{}'",
                _ex.what()
            );
            throw DataLayer::ConnectionFailure(_ex.what());
        } catch (pqxx::unexpected_rows const& _ex) {
            simulator::log::warn(
                "database transaction failed with "
                "pqxx::unexpected_rows error: `{}'",
                _ex.what()
            );
            throw CardinalityViolationError();
        } catch (pqxx::unique_violation const& _ex) {
            simulator::log::warn(
                "database transaction failed with "
                "pqxx::unique_violation error: `{}'",
                _ex.what()
            );
            throw DataLayer::DataIntegrityError("unique");
        } catch (pqxx::foreign_key_violation const& _ex) {
            simulator::log::warn(
                "database transaction failed with "
                "pqxx::foreign_key_violation error: `{}'",
                _ex.what()
            );
            throw DataLayer::DataIntegrityError("foreign key");
        } catch (pqxx::not_null_violation const& _ex) {
            simulator::log::warn(
                "database transaction failed with "
                "pqxx::not_null_violation error: `{}'",
                _ex.what()
            );
            throw DataLayer::DataIntegrityError("not null");
        } catch (pqxx::integrity_constraint_violation const& _ex) {
            // We don't care about other types of database constraints, so
            // we don't report about an exact violated constraint type.
            // Additional info may be present in logs.
            simulator::log::warn(
                "database transaction failed with "
                "pqxx::integrity_constraint_violation error: `{}'",
                _ex.what()
            );
            throw DataLayer::DataIntegrityError("data integrity");
        } catch (pqxx::data_exception const& _ex) {
            simulator::log::err(
                "database transaction failed with pqxx::data_exception, "
                "SQL request may be ill-formed, pqxx error: `{}', "
                "malformed SQL query - `{}'",
                _ex.what(),
                _ex.query()
            );
            throw InternalError("malformed SQL query rejected by the database");
        } catch (std::exception const& _ex) {
            simulator::log::err(
                "An unexpected error occurred while performing database "
                "operation: `{}'",
                _ex.what()
            );
            throw InternalError(_ex.what());
        } catch (...) {
            simulator::log::err(
                "An undefined error occurred while performing database "
                "operation"
            );
            throw InternalError("unknown error");
        }
    }

    auto impl() noexcept -> TransactionImpl& { return mImplementation.get(); }

    std::reference_wrapper<TransactionImpl> mImplementation;
};


class Transaction {
    using DbmsTransaction = pqxx::work;

  public:
    using Handler = TransactionHandler<DbmsTransaction>;

    explicit Transaction(pqxx::connection& _connection) :
        mTransaction(_connection), mHandler(mTransaction)
    {}

    auto handler() noexcept -> Handler { return mHandler; }

    auto commit() -> void { mTransaction.commit(); }

  private:
    DbmsTransaction mTransaction;
    Handler mHandler;
};

} // namespace Simulator::DataLayer::Pqxx

#endif // SIMULATOR_DATA_LAYER_IH_PQXX_DATABASE_TRANSACTION_HPP_
