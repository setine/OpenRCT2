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
#    include <optional>
#    include <set>
#    include <string>

constexpr int NUM_DISCOVERY_PACKETS = 2;

class NetworkLocalServerDiscoverer final : public INetworkLocalServerDiscoverer
{
public:
    NetworkLocalServerDiscoverer(const NetworkConfiguration& config)
        : _multicastEndpointIPv4(config.advertise_locally_address_ipv4, config.advertise_locally_port)
        , _multicastEndpointIPv6(config.advertise_locally_address_ipv6, config.advertise_locally_port)
        , _listenPort(config.advertise_locally_port)
        , _numDiscoveryPacketsSent(0)
    {
    }

    ~NetworkLocalServerDiscoverer()
    {
        StopQuery();
    }

    void StartQuery() override
    {
        _numDiscoveryPacketsSent = 0;

        try
        {
            if(_sockets.empty()){
                auto endpoints =  IUdpSocket::AvailableEndpoints(_listenPort);
                for(const UdpEndpoint& endpoint : endpoints)
                {
                    _sockets.emplace_back(CreateUdpSocket());
                    _sockets.back()->Bind(endpoint);
                }
            }

        }
        catch (const std::exception& ex)
        {
            Console::Error::WriteLine(ex.what());
        }
    }

    void StopQuery() override
    {
        for(auto& socket : _sockets)
        {
            if (socket->GetStatus() == UDP_SOCKET_STATUS_CLOSED)
                return;

            socket->Close();
        }
        _knownServers.clear();
    }

    void ClearServers() override
    {
        _knownServers.clear();
    }

    std::vector<server_entry> Update() override
    {
        if(_numDiscoveryPacketsSent < NUM_DISCOVERY_PACKETS)
        {
            sendDiscoveryPackets();
            _numDiscoveryPacketsSent++;
        }

        std::vector<server_entry> result(_knownServers.begin(), _knownServers.end());

        for(auto& socket : _sockets)
        {
            if (socket->GetStatus() == UDP_SOCKET_STATUS_CLOSED)
                continue;

            size_t readBytes = 0;
            auto [status, endpoint] = socket->ReceiveDataFrom(_buffer.data(), _buffer.size(), &readBytes);

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

                }
            }
        }

        result = std::vector<server_entry>(_knownServers.begin(), _knownServers.end());
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

    void sendDiscoveryPackets()
    {
        for(auto& socket : _sockets)
        {
            if (socket->GetStatus() == UDP_SOCKET_STATUS_CLOSED)
                continue;

            std::optional<UdpEndpoint> multicastEndpoint;
            if(socket->GetType() == UDP_SOCKET_TYPE_IPV4)
                multicastEndpoint = _multicastEndpointIPv4;
            else if(socket->GetType() == UDP_SOCKET_TYPE_IPV6)
                multicastEndpoint = _multicastEndpointIPv6;
            else
                throw SocketException("Unknown UDP socket type encountered!");

            socket->SendDataTo(*multicastEndpoint, NETWORK_COMMAD_LOCAL_SERVER_QUERY.data(), NETWORK_COMMAD_LOCAL_SERVER_QUERY.size());
        }
    }

    const UdpEndpoint _multicastEndpointIPv4;
    const UdpEndpoint _multicastEndpointIPv6;

    uint16_t _listenPort;

    int _numDiscoveryPacketsSent;

    std::vector<std::unique_ptr<IUdpSocket>> _sockets;
    std::array<uint8_t, 1024> _buffer;

    std::set<server_entry> _knownServers;
};

INetworkLocalServerDiscoverer* CreateLocalServerDiscoverer(const NetworkConfiguration& config)
{
    return new NetworkLocalServerDiscoverer(config);
}

#endif // DISABLE_NETWORK
