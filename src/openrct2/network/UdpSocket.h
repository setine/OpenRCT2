/*****************************************************************************
 * Copyright (c) 2014-2018 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#pragma once

#include "../common.h"
#include "SocketUtil.h"
#include <tuple>

enum UDP_SOCKET_STATUS
{
    UDP_SOCKET_STATUS_BOUND,
    UDP_SOCKET_STATUS_CLOSED,
};


enum UDP_SOCKET_TYPE
{
    UDP_SOCKET_TYPE_CLOSED,
    UDP_SOCKET_TYPE_IPV4,
    UDP_SOCKET_TYPE_IPV6
};

struct UdpEndpoint {

    UdpEndpoint(){}

    UdpEndpoint(const std::string& address_, uint16_t port_)
        : address(address_)
        , port(port_)
    {}

    std::string address;
    uint16_t port;
};


/**
 * Represents a UDP socket.
 */
interface IUdpSocket
{
public:
    virtual ~IUdpSocket() = default;

    virtual void Bind(uint16_t port) abstract;
    virtual void Bind(const char* address, uint16_t port) abstract;
    virtual void JoinMulticastGroup(const char* address, uint16_t port) abstract;

    virtual UDP_SOCKET_TYPE GetType() abstract;
    virtual UDP_SOCKET_STATUS GetStatus() abstract;


    virtual size_t SendDataTo(const UdpEndpoint& remote, const void* buffer, size_t size) abstract;
    virtual std::tuple<NETWORK_READPACKET,UdpEndpoint> ReceiveDataFrom(void* buffer, size_t size, size_t* sizeReceived) abstract;

    virtual void Close() abstract;
};

IUdpSocket* CreateUdpSocket();
