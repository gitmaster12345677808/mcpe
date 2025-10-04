/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "CropTile.hpp"
#include "world/level/Level.hpp"
#include "world/item/Item.hpp"
#include "world/entity/Player.hpp"
#include "world/entity/ItemEntity.hpp"
#include <algorithm>

CropTile::CropTile(TileID id, int texture) : Bush(id, texture)
{
	setTicking(true);
	setShape(0.0f, 0.0f, 0.0f, 1.0f, 0.25f, 1.0f);
}

bool CropTile::mayPlace(const Level* level, const TilePos& pos) const
{
	TileID belowTile = level->getTile(pos.below());
	return belowTile == Tile::farmland->m_ID;
}

bool CropTile::canSurvive(const Level* level, const TilePos& pos) const
{
	return mayPlace(level, pos);
}

void CropTile::tick(Level* level, const TilePos& pos, Random* random)
{
	// Call parent tick for survival check
	Bush::tick(level, pos, random);

	if (level->getRawBrightness(pos.above()) >= 9)
	{
		TileData data = level->getData(pos);
		if (data < 7) // Not fully grown
		{
			int growthSpeed = getGrowthSpeed(level, pos);
			
			if (random->nextInt(((100 / growthSpeed) + 1)) == 0)
			{
				level->setData(pos, data + 1);
			}
		}
	}
}

int CropTile::getGrowthSpeed(Level* level, const TilePos& pos) const
{
	int speed = 1;
	TilePos farmlandPos = pos.below();
	
	// Check farmland moisture level (0 = dry, 7 = wet)
	TileData farmlandData = level->getData(farmlandPos);
	if (farmlandData > 0) // Moist farmland
	{
		// Growth speed increases with moisture level
		speed += farmlandData; // Speed ranges from 1 (dry) to 8 (fully wet)
	}
	
	// Additional bonus for water directly nearby (legacy check)
	if (hasWater(level, farmlandPos))
	{
		speed += 2; // Extra boost for direct water proximity
	}
	
	return speed;
}

bool CropTile::hasWater(Level* level, const TilePos& pos) const
{
	// Check 4 blocks around for water
	for (int x = -4; x <= 4; x++)
	{
		for (int z = -4; z <= 4; z++)
		{
			TilePos checkPos = pos + TilePos(x, 0, z);
			TileID tile = level->getTile(checkPos);
			if (tile == Tile::water->m_ID || tile == Tile::calmWater->m_ID)
			{
				return true;
			}
		}
	}
	return false;
}

int CropTile::getTexture(Facing::Name face, TileData data) const
{
	// Return different textures based on growth stage (0-7)
	return m_TextureFrame + (data > 7 ? 7 : data);
}

void CropTile::playerDestroy(Level* level, Player* player, const TilePos& pos, TileData data)
{
	Bush::playerDestroy(level, player, pos, data);
	
	if (!level->m_bIsClientSide)
	{
		// Always drop seeds
		Vec3 spawnPos = Vec3(pos) + Vec3(0.5f, 0.1f, 0.5f);
		ItemInstance* seedInst = new ItemInstance(ITEM_SEEDS, 1, 0);
		ItemEntity* seedEntity = new ItemEntity(level, spawnPos, seedInst);
		seedEntity->m_throwTime = 10;
		level->addEntity(seedEntity);
		
		// Drop wheat if fully grown
		if (isFullyGrown(data))
		{
			ItemInstance* wheatInst = new ItemInstance(ITEM_WHEAT, 1, 0);
			ItemEntity* wheatEntity = new ItemEntity(level, spawnPos, wheatInst);
			wheatEntity->m_throwTime = 10;
			level->addEntity(wheatEntity);
			
			// Sometimes drop extra seeds
			Random random;
			if (random.nextInt(8) == 0)
			{
				ItemInstance* extraSeedInst = new ItemInstance(ITEM_SEEDS, 1, 0);
				ItemEntity* extraSeedEntity = new ItemEntity(level, spawnPos, extraSeedInst);
				extraSeedEntity->m_throwTime = 10;
				level->addEntity(extraSeedEntity);
			}
		}
	}
}

int CropTile::getResource(TileData data, Random* random) const
{
	// Always return seeds as the basic resource
	return ITEM_SEEDS;
}

bool CropTile::isFullyGrown(TileData data) const
{
	return data >= 7;
}

bool CropTile::isFertilizable(Level* level, const TilePos& pos) const
{
	TileData data = level->getData(pos);
	return !isFullyGrown(data);
}

void CropTile::fertilize(Level* level, const TilePos& pos, Random* random)
{
	TileData data = level->getData(pos);
	if (data < 7)
	{
		// Randomly advance 1-4 growth stages
		int advancement = 1 + random->nextInt(4);
		data = std::min(7, data + advancement);
		level->setData(pos, data);
	}
}