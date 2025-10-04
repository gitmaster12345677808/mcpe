/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#pragma once

#include "../Screen.hpp"
#include "world/crafting/CraftingGrid.hpp"
#include "world/crafting/CraftingManager.hpp"
#include "world/crafting/CraftingInterface.hpp"
#include "world/item/ItemInstance.hpp"

class Player;

class CraftingScreen : public Screen
{
public:
	CraftingScreen(Player* player);
	virtual ~CraftingScreen();

	virtual void init() override;
	virtual void render(int x, int y, float f) override;
	virtual void buttonClicked(Button* pButton) override;
	virtual void mouseClicked(int x, int y, int type) override;
	virtual void mouseReleased(int x, int y, int type) override;
	virtual void keyPressed(int key) override;
	virtual void removed() override;
	virtual bool isPauseScreen() override { return false; }
	virtual bool isInGameScreen() override { return true; }

private:
	void renderRecipeList();
	void renderRecipe(int index, const CraftingRecipe& recipe, int x, int y, bool canCraft);
	bool canCraftRecipe(const CraftingRecipe& recipe);
	void craftRecipe(const CraftingRecipe& recipe);
	int getRecipeAt(int mouseX, int mouseY);
	void updateCraftButtons();
	void handleScroll(int direction);
	std::string getItemName(int itemId);
	
	Player* m_pPlayer;
	Button* m_btnDone;
	Button* m_btnScrollUp;
	Button* m_btnScrollDown;
	std::vector<Button*> m_craftButtons;
	std::vector<const CraftingRecipe*> m_availableRecipes;
	int m_selectedRecipe;
	int m_scrollOffset;
	int m_maxVisibleRecipes;
	
	static const int RECIPE_HEIGHT = 30;
	static const int RECIPE_START_X = 20;
	static const int RECIPE_START_Y = 40;
	static const int RECIPE_WIDTH = 200;
	static const int SCROLLBAR_WIDTH = 10;
	static const int SCROLLBAR_X = 240;
};