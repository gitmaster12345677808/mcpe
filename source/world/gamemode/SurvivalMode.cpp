/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "world/gamemode/SurvivalMode.hpp"
#include "world/item/Item.hpp"
#include "common/Utils.hpp"
#include "client/app/Minecraft.hpp"

SurvivalMode::SurvivalMode(Minecraft* pMC, Level& level) : GameMode(pMC, level),
	m_destroyingPos(-1, -1, -1),
	m_destroyProgress(0.0f),
	m_lastDestroyProgress(0.0f),
	m_destroyTicks(0),
	m_destroyCooldown(0)
{
}

void SurvivalMode::initPlayer(Player* p)
{
	p->m_rot.x = -180.0f;
	p->m_pInventory->prepareSurvivalInventory();
}

bool SurvivalMode::canHurtPlayer()
{
	return true;
}

bool SurvivalMode::startDestroyBlock(Player* player, const TilePos& pos, Facing::Name face)
{
	ItemInstance* item = player->getSelectedItem();
	if (item && item->getItem() == Item::bow)
		return true;

	TileID tile = _level.getTile(pos);

	if (tile <= 0)
		return false;

	if (m_destroyProgress == 0.0f)
	{
		Tile::tiles[tile]->attack(&_level, pos, player);
	}

	if (Tile::tiles[tile]->getDestroyProgress(player) >= 1.0f)
	{
		return destroyBlock(player, pos, face);
	}

	return false;
}

bool SurvivalMode::destroyBlock(Player* player, const TilePos& pos, Facing::Name face)
{
	m_pMinecraft->m_pParticleEngine->destroyEffect(pos);

	TileID tile = _level.getTile(pos);
	int    data = _level.getData(pos);

	if (!GameMode::destroyBlock(player, pos, face))
		return false;

	//@HUH: check too late?
	bool bCanDestroy = m_pMinecraft->m_pLocalPlayer->canDestroy(Tile::tiles[tile]);
	
	// Check if the player has the appropriate tool to harvest this block
	bool bCanHarvest = canHarvestBlock(m_pMinecraft->m_pLocalPlayer, tile);

	if (bCanDestroy && bCanHarvest)
	{
		Tile::tiles[tile]->playerDestroy(&_level, m_pMinecraft->m_pLocalPlayer, pos, data);

		if (m_pMinecraft->isOnline())
		{
			m_pMinecraft->m_pRakNetInstance->send(new RemoveBlockPacket(m_pMinecraft->m_pLocalPlayer->m_EntityID, pos));
		}
	}

	return true;
}

bool SurvivalMode::continueDestroyBlock(Player* player, const TilePos& pos, Facing::Name face)
{
	if (m_destroyCooldown > 0)
	{
		m_destroyCooldown--;
		return false;
	}

	if (m_destroyingPos != pos)
	{
		m_destroyProgress     = 0.0f;
		m_lastDestroyProgress = 0.0f;
		m_destroyTicks = 0;
		m_destroyingPos = pos;
		return false;
	}

	TileID tile = _level.getTile(m_destroyingPos);
	if (!tile)
		return false;

	Tile* pTile = Tile::tiles[tile];
	float destroyProgress = pTile->getDestroyProgress(m_pMinecraft->m_pLocalPlayer);
	m_destroyProgress += getDestroyModifier() * destroyProgress;
	m_destroyTicks++;

	if ((m_destroyTicks & 3) == 1)
	{
		_level.playSound(pos + 0.5f, "step." + pTile->m_pSound->m_name,
			0.125f * (1.0f + pTile->m_pSound->volume), 0.5f * pTile->m_pSound->pitch);
	}

	if (m_destroyProgress >= 1.0f)
	{
		m_destroyTicks    = 0;
		m_destroyCooldown = 5;
		m_destroyProgress     = 0.0f;
		m_lastDestroyProgress = 0.0f;
		return destroyBlock(player, m_destroyingPos, face);
	}

	return false;
}

void SurvivalMode::stopDestroyBlock()
{
	m_destroyProgress = 0.0f;
	m_destroyCooldown = 0;
}

void SurvivalMode::tick()
{
	m_lastDestroyProgress = m_destroyProgress;
	//m_pMinecraft->m_pSoundEngine->playMusicTick(); // also on MultiPlayerGameMode
	GameMode::tick();
}

void SurvivalMode::render(float f)
{
	if (m_destroyProgress <= 0.0f)
	{
		m_pMinecraft->m_gui.field_8 = 0.0f;
		m_pMinecraft->m_pLevelRenderer->field_10 = 0.0f;
	}
	else
	{
		float x = m_lastDestroyProgress + (m_destroyProgress - m_lastDestroyProgress) * f;
		m_pMinecraft->m_gui.field_8 = x;
		m_pMinecraft->m_pLevelRenderer->field_10 = x;
	}
}

bool SurvivalMode::canHarvestBlock(Player* player, TileID tileId) const
{
	// Get the player's current held item
	ItemInstance* heldItem = player->m_pInventory->getSelectedItem();
	int heldItemId = heldItem ? heldItem->m_itemID : -1;
	
	// Blocks that require specific tools to drop items
	switch (tileId) {
		// Stone-type blocks require a pickaxe
		case TILE_STONE:
		case TILE_STONEBRICK:
		case TILE_ORE_COAL:
		case TILE_ORE_IRON:
		case TILE_ORE_GOLD:
		case TILE_ORE_EMERALD:
		case TILE_BLOCK_IRON:
		case TILE_BLOCK_GOLD:
		case TILE_BLOCK_EMERALD:
		case TILE_OBSIDIAN:
		case TILE_BRICKS:
		case TILE_FURNACE:
		case TILE_FURNACE_LIT:
			return (heldItemId == ITEM_PICKAXE_WOOD ||
			        heldItemId == ITEM_PICKAXE_STONE ||
			        heldItemId == ITEM_PICKAXE_IRON ||
			        heldItemId == ITEM_PICKAXE_GOLD ||
			        heldItemId == ITEM_PICKAXE_EMERALD);
		
		// Wood-type blocks require an axe (but can be broken by hand slowly)
		case TILE_TREE_TRUNK:
		case TILE_WOOD:
		case TILE_CHEST:
		case TILE_WORKBENCH:
		case TILE_BOOKSHELF:
			// Allow hand breaking for wood blocks (just slower)
			return true;
		
		// Dirt/grass-type blocks require a shovel (but can be broken by hand)
		case TILE_DIRT:
		case TILE_GRASS:
		case TILE_SAND:
		case TILE_GRAVEL:
		case TILE_CLAY:
			// Allow hand breaking for dirt blocks
			return true;
		
		// Blocks that can always be broken by hand
		case TILE_LEAVES:
		case TILE_SAPLING:
		case TILE_FLOWER:
		case TILE_ROSE:
		case TILE_MUSHROOM_1:
		case TILE_MUSHROOM_2:
		case TILE_TORCH:
		case TILE_LADDER:
		case TILE_SIGN:
		case TILE_DOOR_WOOD:
		case TILE_GLASS:
		case TILE_TALLGRASS:
		case TILE_DEAD_BUSH:
		case TILE_WHEAT:
		case TILE_REEDS:
		case TILE_CACTUS:
		case TILE_TNT:
		case TILE_SPONGE:
		default:
			return true;
	}
}
