/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#pragma once

#include "../Screen.hpp"
#include "world/item/ItemInstance.hpp"
#include <vector>
#include <map>

class Player;
class TilePos;
class Level;

struct SmeltingItem {
    ItemInstance* item;
    int inventorySlot;
    float progress;
    bool smelting;
    float fuelTime;
};

class FurnaceScreen : public Screen
{
public:
	FurnaceScreen(Player* player, const TilePos& pos);
	virtual ~FurnaceScreen();

	virtual void init() override;
	virtual void render(int x, int y, float f) override;
	virtual void buttonClicked(Button* pButton) override;
	virtual void mouseClicked(int x, int y, int type) override;
	virtual void keyPressed(int key) override;
	virtual void removed() override;
	virtual bool isPauseScreen() override { return false; }
	virtual bool isInGameScreen() override { return true; }

private:
	void renderSmeltableItems();
	void renderSmeltingItem(int index, const SmeltingItem& smeltItem, int x, int y);
	void updateSmeltButtons();
	void updateSmelting();
	bool canSmelt(ItemInstance* item);
	ItemInstance* getSmeltResult(ItemInstance* item);
	bool hasFuel();
	void consumeFuel();
	void handleScroll(int direction);
	std::string getItemName(int itemId);
	
	Player* m_pPlayer;
	TilePos m_furnacePos;
	Button* m_btnDone;
	Button* m_btnScrollUp;
	Button* m_btnScrollDown;
	std::vector<Button*> m_smeltButtons;
	std::vector<SmeltingItem> m_smeltableItems;
	std::map<int, SmeltingItem*> m_activeSmeltingItems; // slot -> smelting item
	
	// Display constants
	static const int ITEM_START_X = 10;
	static const int ITEM_START_Y = 40;
	static const int ITEM_WIDTH = 300;
	static const int ITEM_HEIGHT = 35;
	static const int MAX_VISIBLE_ITEMS = 8;
	
	int m_scrollOffset;
	int m_maxVisibleItems;
	int m_playerFuelCount; // Track coal/fuel in inventory
};