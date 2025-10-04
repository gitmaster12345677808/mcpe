/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "FoodItem.hpp"
#include "world/level/Level.hpp"
#include "world/entity/Player.hpp"

FoodItem::FoodItem(int itemId, int healAmount) : Item(itemId), m_healAmount(healAmount)
{
	setMaxStackSize(64);
}

ItemInstance* FoodItem::use(ItemInstance* instance, Level* level, Player* player)
{
	// Only allow eating if player is damaged
	if (player->m_health < 20)
	{
		// Heal the player
		player->heal(m_healAmount);
		
		// Play eating sound
		level->playSound(player->m_pos, "eat", 0.5f, 1.0f);
		
		// Consume one item
		instance->m_count--;
		if (instance->m_count <= 0)
		{
			return nullptr; // Item stack consumed
		}
	}
	
	return instance;
}