/*****************************************************************************
 * Copyright (c) 2014-2018 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#ifndef DISABLE_NETWORK

#    include "NetworkLocalServerAdvertiser.h"

#    include "../config/Config.h"
#    include "../core/Console.hpp"
#    include "../core/Json.hpp"

#    include "../Diagnostic.h"
#    include "UdpSocket.h"
#    include "NetworkTypes.h"

#    include <array>
#    include <memory>
#    include <string>

class NetworkLocalServerAdvertiser final : public INetworkLocalServerAdvertiser
{
public:

    NetworkLocalServerAdvertiser(const NetworkConfiguration& config, const std::string networkVersion)
        : _networkVersion(networkVersion)
        , _socket(CreateUdpSocket())
    {
        try
        {
            _socket->Bind(config.default_port);

            if(_socket->GetType() == UDP_SOCKET_TYPE_IPV4)
                _socket->JoinMulticastGroup(UdpEndpoint(config.advertise_locally_address_ipv4, config.default_port));
            else if(_socket->GetType() == UDP_SOCKET_TYPE_IPV6)
                _socket->JoinMulticastGroup(UdpEndpoint(config.advertise_locally_address_ipv6, config.default_port));
            else
                throw SocketException("Unknown UDP socket type encountered!");

        }
        catch (const std::exception& ex)
        {
            Console::Error::WriteLine(ex.what());
        }
    }

    ~NetworkLocalServerAdvertiser()
    {
        if(_socket->GetStatus() != UDP_SOCKET_STATUS_CLOSED)
            _socket->Close();
    }

    void Update(size_t numPlayers, bool requiresPassword) override
    {
        if(_socket->GetStatus() == UDP_SOCKET_STATUS_BOUND)
        {
            size_t readBytes = 0;
            auto [status, endpoint] = _socket->ReceiveDataFrom(_buffer.data(),
                    _buffer.size(), &readBytes);

            if(status == NETWORK_READPACKET_SUCCESS) {
                std::string msg(_buffer.begin(), _buffer.begin() + readBytes);
                if(msg == NETWORK_COMMAD_LOCAL_SERVER_QUERY)
                    DoResponse(endpoint, numPlayers, requiresPassword);
            }
        }
    }

private:

    void DoResponse(const UdpEndpoint& client, size_t numPlayers, bool requiresPassword)
    {
        std::shared_ptr<json_t> obj(json_object(), json_decref);
        json_object_set_new(obj.get(), "port", json_integer(gConfigNetwork.default_port));
        json_object_set_new(obj.get(), "name", json_string(gConfigNetwork.server_name));
        json_object_set_new(obj.get(), "description", json_string(gConfigNetwork.server_description));
        json_object_set_new(obj.get(), "requiresPassword", json_boolean(requiresPassword));
        json_object_set_new(obj.get(), "version", json_string(_networkVersion.c_str()));
        json_object_set_new(obj.get(), "players", json_integer(numPlayers));
        json_object_set_new(obj.get(), "maxPlayers", json_integer(gConfigNetwork.maxplayers));

        std::unique_ptr<const char> msg(json_dumps(obj.get(), 0));
        std::string msgString(msg.get());

        try
        {
            _socket->SendDataTo(client, msgString.data(), msgString.size());
        }
        catch (const std::exception& ex)
        {
            Console::Error::WriteLine(ex.what());
        }
    }

    std::string _networkVersion;
    std::unique_ptr<IUdpSocket> _socket;
    std::array<uint8_t,1024> _buffer;
};

INetworkLocalServerAdvertiser* CreateLocalServerAdvertiser(const NetworkConfiguration& config, const std::string& networkVersion)
{
    return new NetworkLocalServerAdvertiser(config, networkVersion);
}

#endif // DISABLE_NETWORK
