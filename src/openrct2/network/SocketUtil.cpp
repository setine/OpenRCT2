/*****************************************************************************
 * Copyright (c) 2014-2018 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

// TODO Cleanup headers

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

#    include "SocketUtil.h"

#    ifdef _WIN32
static bool _wsaInitialised = false;
#    endif


bool InitialiseWSA()
{
#    ifdef _WIN32
    if (!_wsaInitialised)
    {
        log_verbose("Initialising WSA");
        WSADATA wsa_data;
        if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
        {
            log_error("Unable to initialise winsock.");
            return false;
        }
        _wsaInitialised = true;
    }
    return _wsaInitialised;
#    else
    return true;
#    endif
}

void DisposeWSA()
{
#    ifdef _WIN32
    if (_wsaInitialised)
    {
        WSACleanup();
        _wsaInitialised = false;
    }
#    endif
}

namespace Convert
{
    uint16_t HostToNetwork(uint16_t value)
    {
        return htons(value);
    }

    uint16_t NetworkToHost(uint16_t value)
    {
        return ntohs(value);
    }
} // namespace Convert

#endif
