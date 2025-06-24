#ifndef SIMULATOR_HTTP_IH_REDIRECT_DESTINATION_HPP_
#define SIMULATOR_HTTP_IH_REDIRECT_DESTINATION_HPP_

#include <string>
#include <utility>

namespace Simulator::Http::Redirect {

class Destination
{
public:

    Destination() = delete;

    Destination(std::string _host, int _port) noexcept
        :   m_host { std::move(_host) }
        ,   m_port { _port }
    {}

    std::string const & getHost() const noexcept
    {
        return m_host;
    }

    int getPort() const noexcept
    {
        return m_port;
    }

private:

    std::string m_host;
    int m_port;
};

} // Simulator::Http::Redirect

#endif // SIMULATOR_HTTP_IH_REDIRECT_DESTINATION_HPP_
