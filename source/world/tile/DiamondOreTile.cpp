/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "DiamondOreTile.hpp"
#include "common/Utils.hpp"

DiamondOreTile::DiamondOreTile(int id, int texture) : OreTile(id, texture)
{
}

DiamondOreTile::~DiamondOreTile()
{
}

int DiamondOreTile::getResource(TileData data, Random*) const
{
	// Return diamond item ID instead of diamond ore block
	return ITEM_EMERALD; // Diamond item (emerald in code)
}

int DiamondOreTile::getResourceCount(Random*) const
{
	// Drop 1 diamond item
	return 1;
}

int DiamondOreTile::getSpawnResourcesAuxValue(int) const
{
	// No special aux value needed for diamond items
	return 0;
}