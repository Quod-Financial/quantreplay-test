#ifndef SIMULATOR_DATA_LAYER_TESTS_COMMON_PREDICATE_HPP_
#define SIMULATOR_DATA_LAYER_TESTS_COMMON_PREDICATE_HPP_

#include <string>
#include <tuple>

#include <gmock/gmock.h>

#include "api/predicate/expression.hpp"

struct TestModel {
    class Patch {};

    enum class Attribute {
        BooleanField,
        IntegerField,
        UnsignedIntegerField,
        DecimalField,
        StringField,
        CustomField
    };

    enum class CustomFieldType { Value1, Value2, Value3 };
};

template<>
struct Simulator::DataLayer::Predicate::ModelTraits<TestModel> {
    using AttributeType = ::TestModel::Attribute;
    using CustomFieldTypes = std::tuple<::TestModel::CustomFieldType>;
};

struct FakeColumnResolver {
    auto operator()(TestModel::Attribute _columnEnum) -> std::string
    {
        std::string_view columnName{};
        switch (_columnEnum) {
            case TestModel::Attribute::BooleanField:
                columnName = "BooleanField";
                break;
            case TestModel::Attribute::IntegerField:
                columnName = "IntegerField";
                break;
            case TestModel::Attribute::UnsignedIntegerField:
                columnName = "UnsignedIntegerField";
                break;
            case TestModel::Attribute::DecimalField:
                columnName = "DecimalField";
                break;
            case TestModel::Attribute::StringField:
                columnName = "StringField";
                break;
            case TestModel::Attribute::CustomField:
                columnName = "CustomField";
                break;
        }

        if (!columnName.empty()) {
            return std::string{columnName};
        }

        throw std::logic_error{"unknown test column (model attribute)"};
    }
};

struct FakeEnumerationResolver {
    auto operator()(TestModel::CustomFieldType _customField) -> std::string
    {
        std::string_view value{};
        switch (_customField) {
            case TestModel::CustomFieldType::Value1:
                value = "Value1";
                break;
            case TestModel::CustomFieldType::Value2:
                value = "Value2";
                break;
            case TestModel::CustomFieldType::Value3:
                value = "Value3";
                break;
        }

        if (!value.empty()) {
            return std::string{value};
        }

        throw std::logic_error{"unknown custom enumeration value"};
    }
};

#endif // SIMULATOR_DATA_LAYER_TESTS_COMMON_PREDICATE_HPP_
