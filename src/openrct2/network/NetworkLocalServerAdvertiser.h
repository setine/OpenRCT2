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

struct NetworkConfiguration;

interface INetworkLocalServerAdvertiser
{
    virtual ~INetworkLocalServerAdvertiser() = default;

    virtual void Update(size_t numPlayers, bool requiresPassword) abstract;
};

INetworkLocalServerAdvertiser* CreateLocalServerAdvertiser(const NetworkConfiguration& config);
