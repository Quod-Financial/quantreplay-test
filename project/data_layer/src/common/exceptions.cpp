#include "ih/common/exceptions.hpp"

#include <stdexcept>
#include <string>
#include <string_view>

#include <fmt/format.h>

namespace Simulator::DataLayer {

ConnectionPropertyMissing::ConnectionPropertyMissing(std::string_view _property
) noexcept :
    ConfigurationError(formatMessage(_property))
{}

auto ConnectionPropertyMissing::formatMessage(
    std::string_view _connectionProperty
) noexcept -> std::string
{
    return fmt::format(
        "required database connection property `{}' is absent",
        _connectionProperty
    );
}

RequiredAttributeMissing::RequiredAttributeMissing(
    std::string_view _modelName,
    std::string_view _attributeName
) noexcept :
    MalformedPatch(formatMessage(_modelName, _attributeName))
{}

auto RequiredAttributeMissing::formatMessage(
    std::string_view _modelName,
    std::string_view _attributeName
) noexcept -> std::string
{
    return fmt::format(
        "can not create a {} model, "
        "required {} attribute is missing in the patch",
        _modelName,
        _attributeName
    );
}

UnspecifiedAttributeAccess::UnspecifiedAttributeAccess(
    std::string_view _modelName,
    std::string_view _attributeName
) noexcept :
    std::logic_error(formatMessage(_modelName, _attributeName))
{}

auto UnspecifiedAttributeAccess::formatMessage(
    std::string_view _model,
    std::string_view _attribute
) noexcept -> std::string
{
    return fmt::format(
        "illegal access attempt to an unspecified {}.{} attribute value",
        _model,
        _attribute
    );
}

DataEncodingError::DataEncodingError(std::string_view _message) noexcept :
    InternalError(formatMessage(_message))
{}

auto DataEncodingError::formatMessage(std::string_view _message) noexcept
    -> std::string
{
    return fmt::format("failed to encode data into DBMS format - {}", _message);
}

EnumDecodingError::EnumDecodingError(
    std::string_view _enumName,
    std::string_view _badValue
) noexcept :
    DataDecodingError(formatMessage(_enumName, _badValue))
{}

auto EnumDecodingError::formatMessage(
    std::string_view _enumName,
    std::string_view _badValue
) noexcept -> std::string
{
    return fmt::format(
        "can not convert `{}' to {} enumerable type",
        _badValue,
        _enumName
    );
}

ColumnNameDecodingError::ColumnNameDecodingError(
    std::string_view _badDatabaseColumnName
) noexcept :
    DataDecodingError(formatMessage(_badDatabaseColumnName))
{}

auto ColumnNameDecodingError::formatMessage(
    std::string_view _badDatabaseColumnName
) noexcept -> std::string
{
    return fmt::format(
        "can not decode `{}' database column name",
        _badDatabaseColumnName
    );
}

} // namespace Simulator::DataLayer