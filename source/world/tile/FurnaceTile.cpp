/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "world/tile/FurnaceTile.hpp"
#include "world/level/Level.hpp"
#include "world/entity/Player.hpp"
#include "world/entity/LocalPlayer.hpp"
#include "client/gui/screens/FurnaceScreen.hpp"

FurnaceTile::FurnaceTile(TileID id, bool lit) : Tile(id, Material::stone)
{
	m_lit = lit;
}

int FurnaceTile::getTexture(Facing::Name face) const
{
	// Front face has special texture, others use side texture
	if (face == Facing::NORTH) // Front face
		return m_lit ? TEXTURE_FURNACE_LIT : TEXTURE_FURNACE_FRONT;
	else if (face == Facing::UP || face == Facing::DOWN) // Top and bottom
		return TEXTURE_FURNACE_TOP;
	else // Sides
		return TEXTURE_FURNACE_SIDE;
}

int FurnaceTile::getTexture(Facing::Name face, TileData data) const
{
	// For now, ignore data and use basic textures
	return getTexture(face);
}

void FurnaceTile::onPlace(Level* level, const TilePos& pos)
{
	// Basic placement - no special behavior needed
}

int FurnaceTile::use(Level* level, const TilePos& pos, Player* player)
{
	// Open furnace GUI when player interacts with furnace
	if (player) {
		// Check if this is a LocalPlayer (client-side)
		LocalPlayer* localPlayer = dynamic_cast<LocalPlayer*>(player);
		if (localPlayer && localPlayer->m_pMinecraft) {
			// Open the furnace screen with the tile position
			localPlayer->m_pMinecraft->setScreen(new FurnaceScreen(localPlayer, pos));
			return 1; // Indicate interaction was handled
		}
	}
	return 0;
}

void FurnaceTile::neighborChanged(Level* level, const TilePos& pos, TileID tile)
{
	// Basic implementation - no special neighbor behavior needed for now
}

void FurnaceTile::tick(Level* level, const TilePos& pos, Random* random)
{
	// TODO: Implement smelting logic
	// For now, just basic tick behavior
}

int FurnaceTile::getResource(TileData data, Random* random) const
{
	// Always drop furnace item when broken
	return TILE_FURNACE;
}

int FurnaceTile::getResourceCount(Random* random) const
{
	return 1;
}

void FurnaceTile::updateFurnaceState(Level* level, const TilePos& pos, bool shouldBeLit)
{
	// TODO: Implement state changes between lit/unlit
}

void FurnaceTile::setLitState(Level* level, const TilePos& pos, bool lit)
{
	TileID newTile = lit ? TILE_FURNACE_LIT : TILE_FURNACE;
	level->setTileNoUpdate(pos, newTile);
}