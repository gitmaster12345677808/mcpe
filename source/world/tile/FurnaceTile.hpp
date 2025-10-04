/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#pragma once

#include "Tile.hpp"

class FurnaceTile : public Tile
{
public:
	FurnaceTile(TileID id, bool lit = false);
	
	virtual int getTexture(Facing::Name face) const override;
	virtual int getTexture(Facing::Name face, TileData data) const override;
	virtual void onPlace(Level* level, const TilePos& pos) override;
	virtual int use(Level* level, const TilePos& pos, Player* player) override;
	virtual void neighborChanged(Level* level, const TilePos& pos, TileID tile) override;
	virtual void tick(Level* level, const TilePos& pos, Random* random) override;
	virtual int getResource(TileData data, Random* random) const override;
	virtual int getResourceCount(Random* random) const override;

protected:
	bool m_lit;
	
private:
	void updateFurnaceState(Level* level, const TilePos& pos, bool shouldBeLit);
	static void setLitState(Level* level, const TilePos& pos, bool lit);
};