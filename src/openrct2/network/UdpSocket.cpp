/*****************************************************************************
 * Copyright (c) 2014-2018 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/


// TODO Cleanup includes
#ifndef DISABLE_NETWORK

#    include <chrono>
#    include <cmath>
#    include <cstring>
#    include <future>
#    include <string>
#    include <thread>

// clang-format off
// MSVC: include <math.h> here otherwise PI gets defined twice
#include <cmath>

#ifdef _WIN32
    // winsock2 must be included before windows.h
    #include <winsock2.h>
    #include <ws2tcpip.h>

    #define LAST_SOCKET_ERROR() WSAGetLastError()
    #undef EWOULDBLOCK
    #define EWOULDBLOCK WSAEWOULDBLOCK
    #ifndef SHUT_RD
        #define SHUT_RD SD_RECEIVE
    #endif
    #ifndef SHUT_RDWR
        #define SHUT_RDWR SD_BOTH
    #endif
    #define FLAG_NO_PIPE 0
#else
    #include <cerrno>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <netinet/tcp.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <fcntl.h>
    #include "../common.h"
    using SOCKET = int32_t;
    #define SOCKET_ERROR -1
    #define INVALID_SOCKET -1
    #define LAST_SOCKET_ERROR() errno
    #define closesocket close
    #define ioctlsocket ioctl
    #if defined(__linux__)
        #define FLAG_NO_PIPE MSG_NOSIGNAL
    #else
        #define FLAG_NO_PIPE 0
    #endif // defined(__linux__)
#endif // _WIN32
// clang-format on

#    include "UdpSocket.h"

class UdpSocket;

class UdpSocket final : public IUdpSocket
{
private:
    UDP_SOCKET_STATUS _status = UDP_SOCKET_STATUS_CLOSED;
    uint16_t _listeningPort = 0;
    SOCKET _socket = INVALID_SOCKET;


public:
    UdpSocket() = default;

    ~UdpSocket() override
    {
        CloseSocket();
    }

    void Bind(uint16_t port) override
    {
        Bind(nullptr, port);
    }

    void Bind(const char* address, uint16_t port) override
    {
        if (_status != UDP_SOCKET_STATUS_CLOSED)
        {
            throw std::runtime_error("Socket not closed.");
        }

        sockaddr_storage ss{};
        int32_t ss_len;
        if (!ResolveAddress(address, port, &ss, &ss_len))
        {
            throw SocketException("Unable to resolve address.");
        }

        // Create the listening socket
        _socket = socket(ss.ss_family, SOCK_DGRAM, 0);
        if (_socket == INVALID_SOCKET)
        {
            throw SocketException("Unable to create socket.");
        }

        // Turn off IPV6_V6ONLY so we can accept both v4 and v6 connections
        int32_t value = 0;
        if (setsockopt(_socket, IPPROTO_IPV6, IPV6_V6ONLY, (const char*)&value, sizeof(value)) != 0)
        {
            log_error("IPV6_V6ONLY failed. %d", LAST_SOCKET_ERROR());
        }

        value = 1;
        if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&value, sizeof(value)) != 0)
        {
            log_error("SO_REUSEADDR failed. %d", LAST_SOCKET_ERROR());
        }

        try
        {
            // Bind to address:port
            if (bind(_socket, (sockaddr*)&ss, ss_len) != 0)
            {
                throw SocketException("Unable to bind to socket.");
            }

            if (!SetNonBlocking(_socket, true))
            {
                throw SocketException("Failed to set non-blocking mode.");
            }
        }
        catch (const std::exception&)
        {
            CloseSocket();
            throw;
        }

        _listeningPort = port;
        _status = UDP_SOCKET_STATUS_BOUND;
    }

    virtual void JoinMulticastGroup(const char* address, uint16_t port) override
    {
        sockaddr_storage ss{};
        int32_t ss_len;
        if (!ResolveAddress(address, port, &ss, &ss_len))
        {
            throw SocketException("Unable to resolve address.");
        }

        // TODO ipv6
        const sockaddr_in& ss_in = (const sockaddr_in&)ss;

        if(IN_MULTICAST(ntohl(ss_in.sin_addr.s_addr)))
        {
            ip_mreq mreq{};
            mreq.imr_interface.s_addr = htonl(INADDR_ANY);
            mreq.imr_multiaddr = ss_in.sin_addr;

            if(setsockopt(_socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
                throw SocketException("Failed to set multicast mode.");
            }
        }
    }

    UDP_SOCKET_STATUS GetStatus() override
    {
        return _status;
    }

    size_t SendDataTo(const UdpEndpoint& remote, const void* buffer, size_t size) override
    {
        if (_status != UDP_SOCKET_STATUS_BOUND)
        {
            throw std::runtime_error("Socket not bound.");
        }

        sockaddr_storage ss{};
        int32_t ss_len = sizeof(ss);
        if(!ResolveAddress(remote.address.c_str(), remote.port, &ss, &ss_len))
        {
            throw SocketException("Unable to resolve address.");
        }

        size_t totalSent = 0;
        do
        {
            const char* bufferStart = (const char*)buffer + totalSent;
            size_t remainingSize = size - totalSent;
            int32_t sentBytes = sendto(_socket, bufferStart, (int32_t)remainingSize, 0,
                    (const sockaddr*)&ss, ss_len);

            if (sentBytes == SOCKET_ERROR)
            {
                return totalSent;
            }
            totalSent += sentBytes;
        } while (totalSent < size);
        return totalSent;
    }

    std::tuple<NETWORK_READPACKET,UdpEndpoint> ReceiveDataFrom(void* buffer, size_t size, size_t* sizeReceived) override
    {
        UdpEndpoint srcEndpoint;

        if (_status != UDP_SOCKET_STATUS_BOUND)
        {
            throw std::runtime_error("Socket not bound.");
        }

        //TODO ipv6
        sockaddr_storage ss{};
        socklen_t srcAddressSize = sizeof(ss);

        int32_t readBytes = recvfrom(_socket, (char*)buffer, (int32_t)size, 0,
                (sockaddr*)&ss, &srcAddressSize);

        {
            std::array<char,INET_ADDRSTRLEN+1> address{};
            const sockaddr_in& ss_in = (sockaddr_in&)ss;
            if(inet_ntop(AF_INET, &(ss_in.sin_addr), address.data(), address.size()))
            {
                srcEndpoint.address = std::string(address.begin(), address.end());
            }

            srcEndpoint.port = Convert::NetworkToHost(ss_in.sin_port);
        }

        if (readBytes == 0)
        {
            *sizeReceived = 0;
            return {NETWORK_READPACKET_DISCONNECTED, srcEndpoint};
        }
        else if (readBytes == SOCKET_ERROR)
        {
            *sizeReceived = 0;
#    ifndef _WIN32
            // Removing the check for EAGAIN and instead relying on the values being the same allows turning on of
            // -Wlogical-op warning.
            // This is not true on Windows, see:
            // * https://msdn.microsoft.com/en-us/library/windows/desktop/ms737828(v=vs.85).aspx
            // * https://msdn.microsoft.com/en-us/library/windows/desktop/ms741580(v=vs.85).aspx
            // * https://msdn.microsoft.com/en-us/library/windows/desktop/ms740668(v=vs.85).aspx
            static_assert(
                EWOULDBLOCK == EAGAIN,
                "Portability note: your system has different values for EWOULDBLOCK "
                "and EAGAIN, please extend the condition below");
#    endif // _WIN32
            if (LAST_SOCKET_ERROR() != EWOULDBLOCK)
            {
                return {NETWORK_READPACKET_DISCONNECTED, srcEndpoint};
            }
            else
            {
                return {NETWORK_READPACKET_NO_DATA, UdpEndpoint()};
            }
        }
        else
        {
            *sizeReceived = readBytes;
            return {NETWORK_READPACKET_SUCCESS, srcEndpoint};
        }
    }

    void Close() override
    {
        CloseSocket();
    }

private:
    explicit UdpSocket(SOCKET socket)
    {
        _socket = socket;
        _status = UDP_SOCKET_STATUS_CLOSED;
    }

    void CloseSocket()
    {
        if (_socket != INVALID_SOCKET)
        {
            closesocket(_socket);
            _socket = INVALID_SOCKET;
        }
        _status = UDP_SOCKET_STATUS_CLOSED;
    }

    // TODO move to utils
    bool ResolveAddress(const char* address, uint16_t port, sockaddr_storage* ss, int32_t* ss_len)
    {
        std::string serviceName = std::to_string(port);

        addrinfo hints = {};
        hints.ai_family = AF_UNSPEC;
        if (address == nullptr)
        {
            hints.ai_flags = AI_PASSIVE;
        }

        addrinfo* result = nullptr;
        int errorcode = getaddrinfo(address, serviceName.c_str(), &hints, &result);
        if (errorcode != 0)
        {
            log_error("Resolving address failed: Code %d.", errorcode);
            log_error("Resolution error message: %s.", gai_strerror(errorcode));
            return false;
        }
        if (result == nullptr)
        {
            return false;
        }
        else
        {
            memcpy(ss, result->ai_addr, result->ai_addrlen);
            *ss_len = (int32_t)result->ai_addrlen;
            freeaddrinfo(result);
            return true;
        }
    }

    static bool SetNonBlocking(SOCKET socket, bool on)
    {
#    ifdef _WIN32
        u_long nonBlocking = on;
        return ioctlsocket(socket, FIONBIO, &nonBlocking) == 0;
#    else
        int32_t flags = fcntl(socket, F_GETFL, 0);
        return fcntl(socket, F_SETFL, on ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK)) == 0;
#    endif
    }

};

IUdpSocket* CreateUdpSocket()
{
    return new UdpSocket();
}

#endif
