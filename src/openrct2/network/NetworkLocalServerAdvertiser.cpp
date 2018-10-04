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
#    include "network.h" // TODO Make sure no include loops
#    include "../core/Json.hpp"

#    include "../Diagnostic.h"
#    include "UdpSocket.h"

#    include <iostream>
#    include <string>
#    include <memory>
#    include <array>

static const std::string SERVER_QUERY = "OPENRCT2_SERVER_QUERY";

class NetworkLocalServerAdvertiser final : public INetworkLocalServerAdvertiser
{
public:

    NetworkLocalServerAdvertiser(const NetworkConfiguration& config)
        : _socket(CreateUdpSocket())
    {
        try
        {
            _socket->Bind(config.advertise_locally_address, config.advertise_locally_port);
            _socket->JoinMulticastGroup(config.advertise_locally_address, config.advertise_locally_port);
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
                if(msg == SERVER_QUERY)
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
        json_object_set_new(obj.get(), "version", json_string(network_get_version().c_str()));
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

    std::unique_ptr<IUdpSocket> _socket;
    std::array<uint8_t,1024> _buffer;
};

INetworkLocalServerAdvertiser* CreateLocalServerAdvertiser(const NetworkConfiguration& config)
{
    return new NetworkLocalServerAdvertiser(config);
}

#endif // DISABLE_NETWORK
