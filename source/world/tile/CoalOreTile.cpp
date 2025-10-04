/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "CoalOreTile.hpp"
#include "common/Utils.hpp"

CoalOreTile::CoalOreTile(TileID id, int texture) : OreTile(id, texture)
{
}

int CoalOreTile::getResource(TileData data, Random* random) const
{
	// Drop coal item instead of coal ore block
	return ITEM_COAL;
}

int CoalOreTile::getResourceCount(Random* random) const
{
	// Drop 1 coal item
	return 1;
}