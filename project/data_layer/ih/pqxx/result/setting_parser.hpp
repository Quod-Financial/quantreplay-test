#ifndef SIMULATOR_PROJECT_DATA_LAYER_IH_PQXX_RESULT_SETTING_PARSER_HPP_
#define SIMULATOR_PROJECT_DATA_LAYER_IH_PQXX_RESULT_SETTING_PARSER_HPP_

#include <pqxx/row>

#include "api/exceptions/exceptions.hpp"
#include "api/inspectors/setting.hpp"
#include "api/models/setting.hpp"
#include "ih/pqxx/result/detail/basic_row_parser.hpp"

namespace Simulator::DataLayer::Pqxx {

class SettingParser {
  public:
    explicit SettingParser(pqxx::row const& _databaseRow) noexcept :
        mRowParser(_databaseRow)
    {}

    auto parseInto(Setting::Patch& _destinationPatch) -> void
    {
        SettingPatchWriter<decltype(mRowParser)> writer{mRowParser};
        writer.write(_destinationPatch);
    }


    static auto parse(pqxx::row const& _databaseRow) -> Setting::Patch
    {
        // Warning:
        // GCC reports false-positive warning
        // that parser is used before being initialized
        // in release builds
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"

        Setting::Patch parsed{};
        SettingParser parser{_databaseRow};
        parser.parseInto(parsed);
        return parsed;

#pragma GCC diagnostic pop
    }

    static auto parseKey(pqxx::row const& _databaseRow) -> std::string
    {
        Detail::BasicRowParser rowParser{_databaseRow};

        std::string key{};
        if (rowParser(Setting::Attribute::Key, key)) {
            return key;
        }

        throw DataDecodingError(
            "failed to decode a setting record key "
            "from the database row"
        );
    }

  private:
    Detail::BasicRowParser mRowParser;
};

} // namespace Simulator::DataLayer::Pqxx

#endif // SIMULATOR_PROJECT_DATA_LAYER_IH_PQXX_RESULT_SETTING_PARSER_HPP_
