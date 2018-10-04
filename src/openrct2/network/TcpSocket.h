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

enum TCP_SOCKET_STATUS
{
    TCP_SOCKET_STATUS_CLOSED,
    TCP_SOCKET_STATUS_RESOLVING,
    TCP_SOCKET_STATUS_CONNECTING,
    TCP_SOCKET_STATUS_CONNECTED,
    TCP_SOCKET_STATUS_LISTENING,
};

/**
 * Represents a TCP socket / connection or listener.
 */
interface ITcpSocket
{
public:
    virtual ~ITcpSocket()
    {
    }

    virtual TCP_SOCKET_STATUS GetStatus() abstract;
    virtual const char* GetError() abstract;
    virtual const char* GetHostName() const abstract;

    virtual void Listen(uint16_t port) abstract;
    virtual void Listen(const char* address, uint16_t port) abstract;
    virtual ITcpSocket* Accept() abstract;

    virtual void Connect(const char* address, uint16_t port) abstract;
    virtual void ConnectAsync(const char* address, uint16_t port) abstract;

    virtual size_t SendData(const void* buffer, size_t size) abstract;
    virtual NETWORK_READPACKET ReceiveData(void* buffer, size_t size, size_t* sizeReceived) abstract;

    virtual void Disconnect() abstract;
    virtual void Close() abstract;
};

ITcpSocket* CreateTcpSocket();
