#ifndef SIMULATOR_DATA_LAYER_INCLUDE_DATA_LAYER_API_EXCEPTIONS_EXCEPTIONS_HPP_
#define SIMULATOR_DATA_LAYER_INCLUDE_DATA_LAYER_API_EXCEPTIONS_EXCEPTIONS_HPP_

#include <stdexcept>
#include <string_view>

namespace Simulator::DataLayer {

class ConfigurationError : public std::runtime_error {
  public:
    explicit ConfigurationError(std::string const& _message) noexcept;
};

class ConnectionFailure : public std::runtime_error {
  public:
    explicit ConnectionFailure(std::string_view _driverMessage) noexcept;

  private:
    static auto formatMessage(std::string_view _driverMessage) noexcept
        -> std::string;
};

// Thrown on received database data decoding error
class DataDecodingError : public std::runtime_error {
  public:
    explicit DataDecodingError(std::string const& _message) noexcept;
};

// Thrown when a patch can not be used to create a model or is not processable
// by data access layer
class MalformedPatch : public std::invalid_argument {
  public:
    explicit MalformedPatch(std::string const& _message) noexcept;
};

// Thrown when a database returned a not expected number of records, f.e.
// selecting a single record from table returned several records or didn't
// return anything
class CardinalityViolationError : public std::out_of_range {
  public:
    explicit CardinalityViolationError() noexcept;
};

// Thrown when a database reported about data integrity constraint violation.
// If data passed by a user violates unique constraint, foreign_key constraint,
// not_null constraint, or other constraints data layer throws this exception.
class DataIntegrityError : public std::runtime_error {
  public:
    explicit DataIntegrityError(std::string_view _constraintName) noexcept;

  private:
    static auto formatMessage(std::string_view _constraintName) noexcept
        -> std::string;
};

// This exception indicates that an error occurred in the Data Level.
// May be interpreted as an unexpected behaviour (bug)
class InternalError : public std::logic_error {
  public:
    explicit InternalError(std::string_view _message) noexcept;

  private:
    static auto formatMessage(std::string_view _message) noexcept
        -> std::string;
};

} // namespace Simulator::DataLayer

#endif // SIMULATOR_DATA_LAYER_INCLUDE_DATA_LAYER_API_EXCEPTIONS_EXCEPTIONS_HPP_
