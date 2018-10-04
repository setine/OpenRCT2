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

#include <string>
#include <stdexcept>


enum NETWORK_READPACKET
{
    NETWORK_READPACKET_SUCCESS,
    NETWORK_READPACKET_NO_DATA,
    NETWORK_READPACKET_MORE_DATA,
    NETWORK_READPACKET_DISCONNECTED
};

bool InitialiseWSA();
void DisposeWSA();

class SocketException : public std::runtime_error
{
public:
    explicit SocketException(const std::string& message)
        : std::runtime_error(message)
    {
    }
};

namespace Convert
{
    uint16_t HostToNetwork(uint16_t value);
    uint16_t NetworkToHost(uint16_t value);
} // namespace Convert
