/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#pragma once

#include "Item.hpp"

class FoodItem : public Item
{
public:
	FoodItem(int itemId, int healAmount);

	ItemInstance* use(ItemInstance* instance, Level* level, Player* player) override;

private:
	int m_healAmount;
};