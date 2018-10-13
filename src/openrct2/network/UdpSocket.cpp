/*****************************************************************************
 * Copyright (c) 2014-2018 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#ifndef DISABLE_NETWORK

#    include <array>
#    include <cstring>
#    include <memory>
#    include <string>

// clang-format off
// MSVC: include <math.h> here otherwise PI gets defined twice
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
    #define ip_mreq ip_mreq_source
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

class UdpSocket final : public IUdpSocket
{
private:
    UDP_SOCKET_STATUS _status = UDP_SOCKET_STATUS_CLOSED;
    UDP_SOCKET_TYPE _type = UDP_SOCKET_TYPE_UNDEFINED;
    SOCKET _socket = INVALID_SOCKET;

public:
    UdpSocket() = default;

    ~UdpSocket() override
    {
        CloseSocket();
    }

    void Bind(const UdpEndpoint& endpoint) override
    {
        if (_status != UDP_SOCKET_STATUS_CLOSED)
        {
            throw std::runtime_error("Socket not closed.");
        }

        sockaddr_storage ss{};
        int32_t ss_len;
        auto [success, type] = ResolveAddress(endpoint, &ss, &ss_len);
        if (!success)
            throw SocketException("Unable to resolve address.");

        _type = type;

        // Create the listening socket
        _socket = socket(ss.ss_family, SOCK_DGRAM, 0);
        if (_socket == INVALID_SOCKET)
        {
            throw SocketException("Unable to create socket.");
        }

        // Turn off IPV6_V6ONLY so we can accept both v4 and v6 connections
        if (_type == UDP_SOCKET_TYPE_IPV6)
        {
            int32_t value = 0;
            if (setsockopt(_socket, IPPROTO_IPV6, IPV6_V6ONLY, (const char*)&value, sizeof(value)) != 0)
            {
                log_error("IPV6_V6ONLY failed. %d", LAST_SOCKET_ERROR());
            }
        }

        int32_t value = 1;
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

        _status = UDP_SOCKET_STATUS_BOUND;
    }

    virtual void JoinMulticastGroup(const UdpEndpoint& endpoint) override
    {
        sockaddr_storage ss{};
        int32_t ss_len;
        auto [success, type] = ResolveAddress(endpoint, &ss, &ss_len);
        if (!success)
            throw SocketException("Unable to resolve address.");

        if (type == UDP_SOCKET_TYPE_IPV4)
        {
            const sockaddr_in& ss_in = (const sockaddr_in&)ss;

            if (!IN_MULTICAST(ntohl(ss_in.sin_addr.s_addr)))
                throw SocketException("Address " + endpoint.address + " isn't a multicast address");

            ip_mreq mreq{};
            mreq.imr_interface.s_addr = htonl(INADDR_ANY);
            mreq.imr_multiaddr = ss_in.sin_addr;
#    ifdef _WIN32
            if (setsockopt(_socket, IPPROTO_IP, IP_ADD_SOURCE_MEMBERSHIP, (const char*)&mreq, sizeof(mreq)) < 0)
#    else
            if (setsockopt(_socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)
#    endif
                throw SocketException("Failed to set multicast mode.");
        }
        else if (type == UDP_SOCKET_TYPE_IPV6)
        {
            const sockaddr_in6& ss_in = (const sockaddr_in6&)ss;
            if (!IN6_IS_ADDR_MULTICAST(&ss_in.sin6_addr))
                throw SocketException("Address " + endpoint.address + " isn't a multicast address");

            ipv6_mreq mreq{};
            mreq.ipv6mr_multiaddr = ss_in.sin6_addr;

            if (setsockopt(_socket, IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq)) < 0)
                throw SocketException("Failed to set multicast mode.");
        }
        else
        {
            throw SocketException("Unknown UDP socket type encountered!");
        }
    }

    UDP_SOCKET_TYPE GetType() const override
    {
        return _type;
    }

    UDP_SOCKET_STATUS GetStatus() const override
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
        auto [success, type] = ResolveAddress(remote, &ss, &ss_len);
        if (!success)
            throw SocketException("Unable to resolve address.");

        size_t totalSent = 0;
        do
        {
            const char* bufferStart = (const char*)buffer + totalSent;
            size_t remainingSize = size - totalSent;
            int32_t sentBytes = sendto(_socket, bufferStart, (int32_t)remainingSize, 0, (const sockaddr*)&ss, ss_len);

            if (sentBytes == SOCKET_ERROR)
            {
                return totalSent;
            }
            totalSent += sentBytes;
        } while (totalSent < size);
        return totalSent;
    }

    std::tuple<NETWORK_READPACKET, UdpEndpoint> ReceiveDataFrom(void* buffer, size_t size, size_t* sizeReceived) override
    {
        UdpEndpoint srcEndpoint;

        if (_status != UDP_SOCKET_STATUS_BOUND)
        {
            throw std::runtime_error("Socket not bound.");
        }

        sockaddr_storage ss{};
        socklen_t srcAddressSize = sizeof(ss);

        int32_t readBytes = recvfrom(_socket, (char*)buffer, (int32_t)size, 0, (sockaddr*)&ss, &srcAddressSize);

        if (_type == UDP_SOCKET_TYPE_IPV4)
        {
            std::array<char, INET_ADDRSTRLEN + 1> address{};
            const sockaddr_in& ss_in = (sockaddr_in&)ss;
            if (inet_ntop(AF_INET, &(ss_in.sin_addr), address.data(), address.size()))
                srcEndpoint.address = std::string(address.begin(), address.end());

            srcEndpoint.port = Convert::NetworkToHost(ss_in.sin_port);
        }
        else if (_type == UDP_SOCKET_TYPE_IPV6)
        {
            std::array<char, INET6_ADDRSTRLEN + 1> address{};
            const sockaddr_in6& ss_in = (const sockaddr_in6&)ss;

            if (inet_ntop(AF_INET6, &(ss_in.sin6_addr), address.data(), address.size()))
                srcEndpoint.address = std::string(address.begin(), address.end());

            srcEndpoint.port = Convert::NetworkToHost(ss_in.sin6_port);
        }
        else
        {
            throw SocketException("Unknown UDP socket type encountered!");
        }

        if (readBytes == 0)
        {
            *sizeReceived = 0;
            return { NETWORK_READPACKET_DISCONNECTED, srcEndpoint };
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
                return { NETWORK_READPACKET_DISCONNECTED, srcEndpoint };
            }
            else
            {
                return { NETWORK_READPACKET_NO_DATA, UdpEndpoint() };
            }
        }
        else
        {
            *sizeReceived = readBytes;
            return { NETWORK_READPACKET_SUCCESS, srcEndpoint };
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

    std::tuple<bool, UDP_SOCKET_TYPE> ResolveAddress(const UdpEndpoint& endpoint, sockaddr_storage* ss, int32_t* ss_len) const
    {
        const char* endpointAddress = endpoint.address.c_str();
        std::string serviceName = std::to_string(endpoint.port);

        addrinfo hints = {};
        hints.ai_family = AF_UNSPEC;
        if (endpoint.address.empty())
        {
            hints.ai_flags = AI_PASSIVE;
            endpointAddress = nullptr;
        }

        addrinfo* result = nullptr;
        int errorcode = 0;
        {
            errorcode = getaddrinfo(endpointAddress, serviceName.c_str(), &hints, &result);
        }
        if (errorcode != 0)
        {
            log_error("Resolving address failed: Code %d.", errorcode);
            log_error("Resolution error message: %s.", gai_strerror(errorcode));
            return { false, UDP_SOCKET_TYPE_UNDEFINED };
        }
        if (result == nullptr)
        {
            return { false, UDP_SOCKET_TYPE_UNDEFINED };
        }
        else
        {
            std::memcpy(ss, result->ai_addr, result->ai_addrlen);
            *ss_len = static_cast<int32_t>(result->ai_addrlen);
            auto family = result->ai_family;
            freeaddrinfo(result);

            if (family == AF_INET)
                return { true, UDP_SOCKET_TYPE_IPV4 };
            else if (family == AF_INET6)
                return { true, UDP_SOCKET_TYPE_IPV6 };
            else
                return { true, UDP_SOCKET_TYPE_UNDEFINED };
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
