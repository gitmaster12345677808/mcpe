/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#pragma once

#include "OreTile.hpp"

class DiamondOreTile : public OreTile
{
public:
	DiamondOreTile(int id, int texture);
	virtual ~DiamondOreTile();
	
	int getResource(TileData data, Random*) const override;
	int getResourceCount(Random*) const override;
	int getSpawnResourcesAuxValue(int) const override;
};