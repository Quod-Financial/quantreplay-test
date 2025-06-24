#include "ih/marshalling/json/datasource.hpp"

#include <rapidjson/document.h>

#include "data_layer/api/inspectors/column_mapping.hpp"
#include "data_layer/api/inspectors/datasource.hpp"

#include "ih/marshalling/json/detail/keys.hpp"
#include "ih/marshalling/json/detail/marshaller.hpp"
#include "ih/marshalling/json/detail/unmarshaller.hpp"
#include "ih/marshalling/json/detail/utils.hpp"

namespace Simulator::Http::Json {

auto DatasourceMarshaller::marshall(DataLayer::Datasource const& _datasource)
    -> std::string
{
    rapidjson::Document datasourceDoc{};
    marshall(_datasource, datasourceDoc);
    return Json::encode(datasourceDoc);
}

auto DatasourceMarshaller::marshall(
    std::vector<DataLayer::Datasource> const& _datasources
) -> std::string
{
    rapidjson::Document root{};
    root.SetObject();
    auto& allocator = root.GetAllocator();

    rapidjson::Document datasourcesList{std::addressof(allocator)};
    datasourcesList.SetObject().SetArray();
    for (auto const& datasource : _datasources) {
        rapidjson::Document datasourceDoc{std::addressof(allocator)};
        marshall(datasource, datasourceDoc);
        datasourcesList.PushBack(datasourceDoc, allocator);
    }

    constexpr auto key = DatasourceKey::Datasources;
    root.AddMember(makeKey(key), datasourcesList, allocator);

    return Json::encode(root);
}

auto DatasourceMarshaller::marshall(
    DataLayer::Datasource const& _datasource,
    rapidjson::Document& _dest
) -> void
{
    Json::Marshaller marshaller{_dest};
    DataLayer::DatasourceReader<decltype(marshaller)> reader{marshaller};
    reader.read(_datasource);
    DatasourceMarshaller::marshall(_datasource.columns_mapping(), _dest);
}

auto DatasourceMarshaller::marshall(
    std::vector<DataLayer::ColumnMapping> const& _columnsMapping,
    rapidjson::Document& _parent
) -> void
{
    auto& allocator = _parent.GetAllocator();
    rapidjson::Document columnsMappingList{std::addressof(allocator)};
    columnsMappingList.SetObject().SetArray();

    for (auto const& columnMapping : _columnsMapping) {
        rapidjson::Document columnMappingDoc{std::addressof(allocator)};
        Json::Marshaller marshaller{columnMappingDoc};

        DataLayer::ColumnMappingReader<decltype(marshaller)> reader{marshaller};
        reader.read(columnMapping);

        columnsMappingList.PushBack(columnMappingDoc, allocator);
    }

    constexpr auto key = DatasourceKey::ColumnMapping;
    _parent.AddMember(Json::makeKey(key), columnsMappingList, allocator);
}


auto DatasourceUnmarshaller::unmarshall(
    std::string_view _json,
    DataLayer::Datasource::Patch& _dest
) -> void
{
    using DataLayer::DatasourcePatchWriter;

    rapidjson::Document document{};
    document.Parse(_json.data());
    if (!document.IsObject()) {
        throw std::runtime_error{"failed to parse Datasource JSON object"};
    }

    Unmarshaller unmarshaller{document};
    DatasourcePatchWriter<decltype(unmarshaller)> writer{unmarshaller};
    writer.write(_dest);

    unmarshallColumnMapping(document, _dest);
}

auto DatasourceUnmarshaller::unmarshallColumnMapping(
    rapidjson::Document const& _datasourceDoc,
    DataLayer::Datasource::Patch& _dest
) -> void
{
    using DataLayer::ColumnMappingPatchWriter;
    constexpr auto listKey = DatasourceKey::ColumnMapping;

    if (!_datasourceDoc.HasMember(listKey.data())) {
        return;
    }

    auto const& columnMappingsList = _datasourceDoc[listKey.data()];
    if (!columnMappingsList.IsArray()) {
        throw std::runtime_error{
            "can not parse `columnMapping' key in Datasource JSON, which "
            "is not a JSON array"};
    }

    auto const array = columnMappingsList.GetArray();
    if (array.Empty()) {
        _dest.without_column_mapping();
        return;
    }

    for (auto const& object : array) {
        if (!object.IsObject()) {
            throw std::runtime_error{
                "can not parse a JSON object in `columnMapping' JSON array"};
        }

        DataLayer::ColumnMapping::Patch columnMapping{};
        Unmarshaller unmarshaller{object};
        ColumnMappingPatchWriter<decltype(unmarshaller)> writer{unmarshaller};
        writer.write(columnMapping);

        _dest.with_column_mapping(std::move(columnMapping));
    }
}

} // namespace Simulator::Http::Json
