/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#pragma once

#include "OreTile.hpp"

class CoalOreTile : public OreTile
{
public:
	CoalOreTile(TileID id, int texture);

	int getResource(TileData data, Random* random) const override;
	int getResourceCount(Random* random) const override;
};