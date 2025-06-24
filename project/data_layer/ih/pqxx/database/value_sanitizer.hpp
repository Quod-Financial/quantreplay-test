#ifndef SIMULATOR_DATA_LAYER_IH_PQXX_DATABASE_VALUE_SANITIZER_HPP_
#define SIMULATOR_DATA_LAYER_IH_PQXX_DATABASE_VALUE_SANITIZER_HPP_

#include <functional>

#include <pqxx/connection>

namespace Simulator::DataLayer::Pqxx {

class ValueSanitizer {
  public:
    explicit ValueSanitizer(pqxx::connection const& _connection) noexcept :
        mConnection(_connection)
    {}

    template<typename T>
    auto operator()(T const& _value) -> std::string
    {
        return mConnection.get().quote(_value);
    }

  private:
    std::reference_wrapper<pqxx::connection const> mConnection;
};

} // namespace Simulator::DataLayer::Pqxx

#endif // SIMULATOR_DATA_LAYER_IH_PQXX_DATABASE_VALUE_SANITIZER_HPP_
