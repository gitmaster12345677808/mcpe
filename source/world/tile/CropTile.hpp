/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#pragma once

#include "Bush.hpp"

class CropTile : public Bush
{
public:
	CropTile(TileID id, int texture);

	bool mayPlace(const Level* level, const TilePos& pos) const override;
	void tick(Level* level, const TilePos& pos, Random* random) override;
	bool isFertilizable(Level* level, const TilePos& pos) const;
	void fertilize(Level* level, const TilePos& pos, Random* random);
	int getGrowthSpeed(Level* level, const TilePos& pos) const;
	int getTexture(Facing::Name face, TileData data) const override;
	void playerDestroy(Level* level, Player* player, const TilePos& pos, TileData data) override;
	int getResource(TileData data, Random* random) const override;
	bool canSurvive(const Level* level, const TilePos& pos) const override;

private:
	bool isFullyGrown(TileData data) const;
	bool hasWater(Level* level, const TilePos& pos) const;
};