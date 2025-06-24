#ifndef SIMULATOR_PROJECT_DATA_LAYER_IH_COMMON_EXCEPTIONS_HPP_
#define SIMULATOR_PROJECT_DATA_LAYER_IH_COMMON_EXCEPTIONS_HPP_

#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>

#include <fmt/format.h>

#include "api/exceptions/exceptions.hpp"

// Internal exception that are handled inside the component and are not visible
// to client code
namespace Simulator::DataLayer {

class ConnectionPropertyMissing : public ConfigurationError {
  public:
    explicit ConnectionPropertyMissing(std::string_view _property) noexcept;

  private:
    static auto formatMessage(std::string_view _connectionProperty) noexcept
        -> std::string;
};

// Thrown when client code tries to create a model from a patch with required
// attribute missing
class RequiredAttributeMissing : public MalformedPatch {
  public:
    explicit RequiredAttributeMissing(
        std::string_view _modelName,
        std::string_view _attributeName
    ) noexcept;

  private:
    static auto formatMessage(
        std::string_view _modelName,
        std::string_view _attributeName
    ) noexcept -> std::string;
};

// Thrown when client code tries to access a not-initialized model attribute
class UnspecifiedAttributeAccess : public std::logic_error {
  public:
    explicit UnspecifiedAttributeAccess(
        std::string_view _modelName,
        std::string_view _attributeName
    ) noexcept;

  private:
    static auto formatMessage(
        std::string_view _model,
        std::string_view _attribute
    ) noexcept -> std::string;
};

class EnumDecodingError : public DataDecodingError {
  public:
    EnumDecodingError(
        std::string_view _enumName,
        std::string_view _badValue
    ) noexcept;

  private:
    static auto formatMessage(
        std::string_view _enumName,
        std::string_view _badValue
    ) noexcept -> std::string;
};

class ColumnNameDecodingError : public DataDecodingError {
  public:
    ColumnNameDecodingError(std::string_view _columnName) noexcept;

  private:
    static auto formatMessage(std::string_view _badColumnName) noexcept
        -> std::string;
};

// General data conversion error occurred during data serialization into DBMS
// format
class DataEncodingError : public InternalError {
  public:
    explicit DataEncodingError(std::string_view _message) noexcept;

  private:
    static auto formatMessage(std::string_view _message) noexcept
        -> std::string;
};

class EnumEncodingError : public DataEncodingError {
  public:
    template<typename T, std::enable_if_t<std::is_enum_v<T>>* = nullptr>
    EnumEncodingError(std::string_view _enumName, T _badValue) :
        DataEncodingError(formatMessage(_enumName, _badValue))
    {}

  private:
    template<typename T, std::enable_if_t<std::is_enum_v<T>>* = nullptr>
    static auto formatMessage(std::string_view _enumName, T _badValue)
        -> std::enable_if_t<std::is_enum_v<T>, std::string>
    {
        using UnderlyingType = std::underlying_type_t<T>;
        return fmt::format(
            "can not encode an unknown {} enumeration value ({:#x})",
            _enumName,
            static_cast<UnderlyingType>(_badValue)
        );
    }
};

class ColumnNameEncodingError : public DataEncodingError {
  public:
    template<typename T, std::enable_if_t<std::is_enum_v<T>>* = nullptr>
    ColumnNameEncodingError(std::string_view _tableName, T _badValue) :
        DataEncodingError(formatMessage(_tableName, _badValue))
    {}

  private:
    template<typename T, std::enable_if_t<std::is_enum_v<T>>* = nullptr>
    static auto formatMessage(std::string_view _tableName, T _badAttribute)
        -> std::enable_if_t<std::is_enum_v<T>, std::string>
    {
        using UnderlyingType = std::underlying_type_t<T>;
        return fmt::format(
            "can not encode {:#x} value into a column name for {} table",
            static_cast<UnderlyingType>(_badAttribute),
            _tableName
        );
    }
};

} // namespace Simulator::DataLayer

#endif // SIMULATOR_PROJECT_DATA_LAYER_IH_COMMON_EXCEPTIONS_HPP_
