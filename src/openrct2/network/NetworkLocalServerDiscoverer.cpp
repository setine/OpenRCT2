/*****************************************************************************
 * Copyright (c) 2014-2018 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#ifndef DISABLE_NETWORK

#    include "NetworkLocalServerDiscoverer.h"

#    include "../config/Config.h"
#    include "../core/Console.hpp"
#    include "../core/Json.hpp"

#    include "../Diagnostic.h"
#    include "UdpSocket.h"
#    include "NetworkTypes.h"

#    include <array>
#    include <memory>
#    include <set>
#    include <string>


class NetworkLocalServerDiscoverer final : public INetworkLocalServerDiscoverer
{
public:
    NetworkLocalServerDiscoverer(const NetworkConfiguration& config)
        : _multicastEndpointIPv4(config.advertise_locally_address_ipv4, config.default_port)
        , _multicastEndpointIPv6(config.advertise_locally_address_ipv6, config.default_port)
        , _listenPort(config.default_port)
        , _socket(CreateUdpSocket())
    {
    }

    ~NetworkLocalServerDiscoverer()
    {
        StopQuery();
    }

    void StartQuery() override
    {
        try
        {
            if (_socket->GetStatus() == UDP_SOCKET_STATUS_CLOSED)
            {
                _socket->Bind(UdpEndpoint(_listenPort));

                if(_socket->GetType() == UDP_SOCKET_TYPE_IPV4)
                    _multicastEndpoint = _multicastEndpointIPv4;
                else if(_socket->GetType() == UDP_SOCKET_TYPE_IPV6)
                    _multicastEndpoint = _multicastEndpointIPv6;
                else
                    throw SocketException("Unknown UDP socket type encountered!");

                _socket->JoinMulticastGroup(_multicastEndpoint);
            }

            _socket->SendDataTo(_multicastEndpoint, NETWORK_COMMAD_LOCAL_SERVER_QUERY.data(), NETWORK_COMMAD_LOCAL_SERVER_QUERY.size());
        }
        catch (const std::exception& ex)
        {
            Console::Error::WriteLine(ex.what());
        }
    }

    void StopQuery() override
    {
        if (_socket->GetStatus() == UDP_SOCKET_STATUS_CLOSED)
            return;

        _socket->Close();
        _knownServers.clear();
    }

    std::vector<server_entry> Update() override
    {
        std::vector<server_entry> result(_knownServers.begin(), _knownServers.end());

        if (_socket->GetStatus() == UDP_SOCKET_STATUS_CLOSED)
            return result;

        size_t readBytes = 0;
        auto [status, endpoint] = _socket->ReceiveDataFrom(_buffer.data(), _buffer.size(), &readBytes);

        // TODO What if more than one message available
        if (status == NETWORK_READPACKET_SUCCESS)
        {
            std::string msg(_buffer.begin(), _buffer.begin() + readBytes);

            server_entry entry;
            bool successful = parseServerReply(msg, endpoint, entry);
            if (successful)
            {
                auto [it, wasInserted] = _knownServers.insert(entry);
                if (!wasInserted)
                {
                    _knownServers.erase(it);
                    _knownServers.insert(entry);
                }

                result = std::vector<server_entry>(_knownServers.begin(), _knownServers.end());
            }
        }

        return result;
    }

private:
    bool parseServerReply(const std::string& reply, const UdpEndpoint& serverEndpoint, server_entry& entry)
    {
        try
        {
            std::shared_ptr<json_t> root(Json::FromString(reply), json_decref);

            json_t* port = json_object_get(root.get(), "port");
            json_t* name = json_object_get(root.get(), "name");
            json_t* description = json_object_get(root.get(), "description");
            json_t* requiresPassword = json_object_get(root.get(), "requiresPassword");
            json_t* version = json_object_get(root.get(), "version");
            json_t* players = json_object_get(root.get(), "players");
            json_t* maxPlayers = json_object_get(root.get(), "maxplayers");

            if (name == nullptr || version == nullptr)
            {
                log_verbose("Cowardly refusing to add server without name or version specified.");
                return false;
            }

            entry.address = serverEndpoint.address + ":" + std::to_string(json_integer_value(port));
            entry.name = json_string_value(name);
            entry.description = json_string_value(description);
            entry.requiresPassword = json_boolean_value(requiresPassword);
            entry.version = json_string_value(version);
            entry.players = static_cast<uint8_t>(json_integer_value(players));
            entry.maxplayers = static_cast<uint8_t>(json_integer_value(maxPlayers));
        }
        catch (const JsonException&)
        {
            return false;
        }

        return true;
    }

    const UdpEndpoint _multicastEndpointIPv4;
    const UdpEndpoint _multicastEndpointIPv6;

    uint16_t _listenPort;
    UdpEndpoint _multicastEndpoint;

    std::unique_ptr<IUdpSocket> _socket;
    std::array<uint8_t, 1024> _buffer;

    std::set<server_entry> _knownServers;
};

INetworkLocalServerDiscoverer* CreateLocalServerDiscoverer(const NetworkConfiguration& config)
{
    return new NetworkLocalServerDiscoverer(config);
}

#endif // DISABLE_NETWORK
