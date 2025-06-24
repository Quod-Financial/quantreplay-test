#ifndef SIMULATOR_DATA_LAYER_TESTS_COMMON_MARSHALLER_HPP_
#define SIMULATOR_DATA_LAYER_TESTS_COMMON_MARSHALLER_HPP_

#include <cstdint>
#include <string>
#include <type_traits>

#include <gmock/gmock.h>

#include "api/models/column_mapping.hpp"
#include "api/models/datasource.hpp"
#include "api/models/market_phase.hpp"
#include "api/models/price_seed.hpp"
#include "api/models/venue.hpp"

namespace Simulator::DataLayer {

template<typename Model>
class Marshaller {
  public:
    using Attribute = typename Model::Attribute;

    MOCK_METHOD(void, boolean, (Attribute, bool));
    MOCK_METHOD(void, character, (Attribute, char));
    MOCK_METHOD(void, int32, (Attribute, std::int32_t));
    MOCK_METHOD(void, uint16, (Attribute, std::uint16_t));
    MOCK_METHOD(void, uint32, (Attribute, std::uint32_t));
    MOCK_METHOD(void, uint64, (Attribute, std::uint64_t));
    MOCK_METHOD(void, real, (Attribute, double));
    MOCK_METHOD(void, string, (Attribute, std::string));
    MOCK_METHOD(void, engineType, (Attribute, Venue::EngineType));
    MOCK_METHOD(void, datasourceFormat, (Attribute, Datasource::Format));
    MOCK_METHOD(void, datasourceType, (Attribute, Datasource::Type));
    MOCK_METHOD(void, marketPhaseType, (Attribute, MarketPhase::Phase));


#define DEFINE_MARSHALLING_OPERATOR(TYPE, ACCEPTOR)                            \
    auto operator()(Attribute _field, TYPE _val) -> void                       \
    {                                                                          \
        ACCEPTOR(_field, _val);                                                \
    }

    DEFINE_MARSHALLING_OPERATOR(bool, boolean);
    DEFINE_MARSHALLING_OPERATOR(char, character);
    DEFINE_MARSHALLING_OPERATOR(std::int32_t, int32);
    DEFINE_MARSHALLING_OPERATOR(std::uint16_t, uint16);
    DEFINE_MARSHALLING_OPERATOR(std::uint32_t, uint32);
    DEFINE_MARSHALLING_OPERATOR(std::uint64_t, uint64);
    DEFINE_MARSHALLING_OPERATOR(double, real);
    DEFINE_MARSHALLING_OPERATOR(std::string const&, string);
    DEFINE_MARSHALLING_OPERATOR(Venue::EngineType, engineType);
    DEFINE_MARSHALLING_OPERATOR(Datasource::Format, datasourceFormat);
    DEFINE_MARSHALLING_OPERATOR(Datasource::Type, datasourceType);
    DEFINE_MARSHALLING_OPERATOR(MarketPhase::Phase, marketPhaseType);

#undef DEFINE_MARSHALLING_OPERATOR
};


template<typename Model>
class Unmarshaller {
  public:
    using Attribute = typename Model::Attribute;

    MOCK_METHOD(bool, boolean, (Attribute, bool&));
    MOCK_METHOD(bool, character, (Attribute, char&));
    MOCK_METHOD(bool, int32, (Attribute, std::int32_t&));
    MOCK_METHOD(bool, uint16, (Attribute, std::uint16_t&));
    MOCK_METHOD(bool, uint32, (Attribute, std::uint32_t&));
    MOCK_METHOD(bool, uint64, (Attribute, std::uint64_t&));
    MOCK_METHOD(bool, real, (Attribute, double&));
    MOCK_METHOD(bool, string, (Attribute, std::string&));
    MOCK_METHOD(bool, engineType, (Attribute, Venue::EngineType&));
    MOCK_METHOD(bool, datasourceFormat, (Attribute, Datasource::Format&));
    MOCK_METHOD(bool, datasourceType, (Attribute, Datasource::Type&));
    MOCK_METHOD(bool, marketPhaseType, (Attribute, MarketPhase::Phase&));

#define DEFINE_UNMARSHALLING_OPERATOR(TYPE, ACCEPTOR)                          \
    auto operator()(Attribute _field, TYPE _type) -> bool                      \
    {                                                                          \
        return ACCEPTOR(_field, _type);                                        \
    }

    DEFINE_UNMARSHALLING_OPERATOR(bool&, boolean);
    DEFINE_UNMARSHALLING_OPERATOR(char&, character);
    DEFINE_UNMARSHALLING_OPERATOR(std::int32_t&, int32);
    DEFINE_UNMARSHALLING_OPERATOR(std::uint16_t&, uint16);
    DEFINE_UNMARSHALLING_OPERATOR(std::uint32_t&, uint32);
    DEFINE_UNMARSHALLING_OPERATOR(std::uint64_t&, uint64);
    DEFINE_UNMARSHALLING_OPERATOR(double&, real);
    DEFINE_UNMARSHALLING_OPERATOR(std::string&, string);
    DEFINE_UNMARSHALLING_OPERATOR(Venue::EngineType&, engineType);
    DEFINE_UNMARSHALLING_OPERATOR(Datasource::Format&, datasourceFormat);
    DEFINE_UNMARSHALLING_OPERATOR(Datasource::Type&, datasourceType);
    DEFINE_UNMARSHALLING_OPERATOR(MarketPhase::Phase&, marketPhaseType);

#undef DEFINE_UNMARSHALLING_OPERATOR
};

} // namespace Simulator::DataLayer

#endif // SIMULATOR_DATA_LAYER_TESTS_COMMON_MARSHALLER_HPP_
