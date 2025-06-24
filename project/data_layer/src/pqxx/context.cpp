#include "ih/pqxx/context.hpp"

#include <string>
#include <string_view>
#include <utility>

#include <fmt/format.h>

#include "cfg/api/cfg.hpp"

#include "ih/common/exceptions.hpp"

namespace Simulator::DataLayer::Pqxx {

namespace {

void validateConnectionAttribute(
    std::string_view _name,
    std::string const& _value
)
{
    if (_value.empty()) {
        throw ConnectionPropertyMissing{_name};
    }
}

} // namespace

std::string_view const Context::Configurator::msConnectionStringFormat{
    "postgresql://{user}:{password}@{host}:{port}/{dbname}"};

void Context::Configurator::withHost(std::string _host) noexcept
{
    mHost = std::move(_host);
}

void Context::Configurator::withPort(std::string _port) noexcept
{
    mPort = std::move(_port);
}

void Context::Configurator::withUser(std::string _user) noexcept
{
    mUser = std::move(_user);
}

void Context::Configurator::withDatabase(std::string _databaseName) noexcept
{
    mDatabase = std::move(_databaseName);
}

void Context::Configurator::withPassword(std::string _password) noexcept
{
    mPassword = std::move(_password);
}

auto Context::Configurator::formatConnectionString() const -> std::string
{
    validateConnectionAttribute("host", mHost);
    validateConnectionAttribute("port", mPort);
    validateConnectionAttribute("user", mUser);
    validateConnectionAttribute("databaseName", mDatabase);
    validateConnectionAttribute("password", mPassword);

    return fmt::format(
        fmt::runtime(msConnectionStringFormat),
        fmt::arg("user", mUser),
        fmt::arg("password", mPassword),
        fmt::arg("host", mHost),
        fmt::arg("port", mPort),
        fmt::arg("dbname", mDatabase)
    );
}

Context::Context(std::string _connectionString) noexcept :
    mConnection(std::move(_connectionString))
{}

auto Context::getConnectionString() const noexcept -> std::string const&
{
    return mConnection;
}

void Context::configure(Cfg::DbConfiguration const& _cfg)
{
    Configurator configurator;
    configurator.withHost(_cfg.host);
    configurator.withPort(_cfg.port);
    configurator.withUser(_cfg.user);
    configurator.withPassword(_cfg.password);
    configurator.withDatabase(_cfg.name);

    mConnection = configurator.formatConnectionString();
}

} // namespace Simulator::DataLayer::Pqxx