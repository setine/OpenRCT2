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
#include "ServerList.h"
#include <vector>

struct NetworkConfiguration;

interface INetworkLocalServerDiscoverer
{
    virtual ~INetworkLocalServerDiscoverer() = default;

    virtual void StartQuery() abstract;
    virtual void StopQuery() abstract;

    virtual std::vector<server_entry> Update() abstract;

};

INetworkLocalServerDiscoverer* CreateLocalServerDiscoverer(const NetworkConfiguration& config);
