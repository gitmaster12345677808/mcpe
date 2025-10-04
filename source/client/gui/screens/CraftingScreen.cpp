/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "CraftingScreen.hpp"
#include "client/app/Minecraft.hpp"
#include "client/renderer/Tesselator.hpp"
#include "client/gui/Gui.hpp"
#include "world/entity/Player.hpp"
#include "world/item/Item.hpp"
#include <map>
#include <algorithm>
#include <SDL2/SDL.h>
#include <chrono>

CraftingScreen::CraftingScreen(Player* player) 
	: m_pPlayer(player), m_btnDone(nullptr), m_btnScrollUp(nullptr), m_btnScrollDown(nullptr), 
	  m_selectedRecipe(-1), m_scrollOffset(0), m_maxVisibleRecipes(10)
{
}

CraftingScreen::~CraftingScreen()
{
	if (m_btnDone) delete m_btnDone;
	// Clean up craft buttons
	for (Button* btn : m_craftButtons) {
		delete btn;
	}
}

void CraftingScreen::init()
{
	m_buttons.clear();
	
	
	m_btnDone = new Button(1, m_width / 2 - 50, m_height - 30, 100, 20, "Done");
	m_buttons.push_back(m_btnDone);
	
	// Add scroll up button
	m_btnScrollUp = new Button(2, RECIPE_START_X + RECIPE_WIDTH + 10, RECIPE_START_Y, 30, 25, "Up");
	m_buttons.push_back(m_btnScrollUp);
	
	// Add scroll down button
	m_btnScrollDown = new Button(3, RECIPE_START_X + RECIPE_WIDTH + 10, RECIPE_START_Y + 30, 30, 25, "Down");
	m_buttons.push_back(m_btnScrollDown);
	
	// Get all available recipes
	m_availableRecipes.clear();
	const std::vector<CraftingRecipe>& allRecipes = CraftingManager::getInstance().getRecipes();
	for (const auto& recipe : allRecipes) {
		m_availableRecipes.push_back(&recipe);
	}
	
	// Create initial craft buttons
	updateCraftButtons();
	
	// Open the player's crafting interface
	m_pPlayer->m_pCraftingInterface->open();
}

void CraftingScreen::render(int mouseX, int mouseY, float f)
{
	renderBackground(0);
	
		// Draw title
	m_pMinecraft->m_pFont->draw("Crafting", m_width / 2 - 20, 10, 0xFFFFFF);
	
	// Draw simple controls help
	m_pMinecraft->m_pFont->draw("Click 'Craft' buttons to craft, use Up/Down to scroll", 10, m_height - 50, 0xCCCCCC);
	
	// Render the recipe list
	renderRecipeList();
	
	// Call parent render for buttons
	Screen::render(mouseX, mouseY, f);
}

void CraftingScreen::renderRecipeList()
{
	int yPos = RECIPE_START_Y;
	int endIndex = std::min((int)m_availableRecipes.size(), m_scrollOffset + m_maxVisibleRecipes);
	
	for (int i = m_scrollOffset; i < endIndex; i++) {
		const CraftingRecipe* recipe = m_availableRecipes[i];
		bool canCraft = canCraftRecipe(*recipe);
		renderRecipe(i, *recipe, RECIPE_START_X, yPos, canCraft);
		yPos += RECIPE_HEIGHT;
	}
	
	// Scrollbar removed - using buttons instead
}

void CraftingScreen::renderRecipe(int index, const CraftingRecipe& recipe, int x, int y, bool canCraft)
{
	// Draw recipe background
	Tesselator& t = Tesselator::instance;
	t.begin();
	if (canCraft) {
		t.color(0x00FF00); // Green if craftable
	} else {
		t.color(0xFF0000); // Red if not craftable
	}
	t.vertex(x, y + RECIPE_HEIGHT, 0.0f);
	t.vertex(x + RECIPE_WIDTH, y + RECIPE_HEIGHT, 0.0f);
	t.vertex(x + RECIPE_WIDTH, y, 0.0f);
	t.vertex(x, y, 0.0f);
	t.draw();

	// Draw recipe text
	std::string resultName = getItemName(recipe.resultItemId);
	char recipeText[128];
	snprintf(recipeText, sizeof(recipeText), "%s (x%d)", resultName.c_str(), recipe.resultCount);
	m_pMinecraft->m_pFont->draw(recipeText, x + 5, y + 8, canCraft ? 0x000000 : 0xFFFFFF);

	// Show ingredients
	std::string ingredientText = "Needs: ";
	bool first = true;
	for (int row = 0; row < recipe.pattern.size(); row++) {
		for (int col = 0; col < recipe.pattern[row].size(); col++) {
			int itemId = recipe.pattern[row][col];
			if (itemId > 0) {
				if (!first) ingredientText += ", ";
				ingredientText += getItemName(itemId);
				first = false;
			}
		}
	}
	m_pMinecraft->m_pFont->draw(ingredientText.c_str(), x + 5, y + 18, canCraft ? 0x000000 : 0xFFFFFF);
}

// Removed renderScrollbar method - using buttons instead

void CraftingScreen::updateCraftButtons()
{
	// Clear existing craft buttons
	for (Button* btn : m_craftButtons) {
		// Remove from main button list
		auto it = std::find(m_buttons.begin(), m_buttons.end(), btn);
		if (it != m_buttons.end()) {
			m_buttons.erase(it);
		}
		delete btn;
	}
	m_craftButtons.clear();
	
	// Create craft buttons for visible recipes
	int yPos = RECIPE_START_Y;
	int endIndex = std::min((int)m_availableRecipes.size(), m_scrollOffset + m_maxVisibleRecipes);
	
	for (int i = m_scrollOffset; i < endIndex; i++) {
		const CraftingRecipe* recipe = m_availableRecipes[i];
		bool canCraft = canCraftRecipe(*recipe);
		
		// Create button at end of recipe row
		int buttonX = RECIPE_START_X + RECIPE_WIDTH - 55;
		int buttonY = yPos + 2;
		int buttonId = 100 + i; // Start craft button IDs at 100
		
		const char* buttonText = canCraft ? "Craft" : "Can't";
		Button* craftBtn = new Button(buttonId, buttonX, buttonY, 50, 26, buttonText);
		craftBtn->m_bEnabled = canCraft;
		
		m_craftButtons.push_back(craftBtn);
		m_buttons.push_back(craftBtn);
		
		yPos += RECIPE_HEIGHT;
	}
}

void CraftingScreen::buttonClicked(Button* pButton)
{
	if (pButton->m_buttonId == 1) { // Done button
		m_pMinecraft->setScreen(nullptr);
	} else if (pButton->m_buttonId == 2) { // Scroll Up button
		handleScroll(-1);
	} else if (pButton->m_buttonId == 3) { // Scroll Down button
		handleScroll(1);
	} else if (pButton->m_buttonId >= 100) { // Craft button
		int recipeIndex = pButton->m_buttonId - 100;
		if (recipeIndex >= 0 && recipeIndex < m_availableRecipes.size()) {
			const CraftingRecipe* recipe = m_availableRecipes[recipeIndex];
			if (canCraftRecipe(*recipe)) {
				craftRecipe(*recipe);
				// Refresh craft buttons after crafting to update "Can't"/"Craft" states
				updateCraftButtons();
			}
		}
	}
}

void CraftingScreen::mouseClicked(int x, int y, int type)
{
	Screen::mouseClicked(x, y, type);
	// All craft button handling is now done via proper Button objects in buttonClicked()
}

void CraftingScreen::mouseReleased(int x, int y, int type)
{
	Screen::mouseReleased(x, y, type);
	// Simplified - no complex drag scrolling needed since we have buttons
}

void CraftingScreen::removed()
{
	// Close the crafting interface when screen is closed
	m_pPlayer->m_pCraftingInterface->close();
}

bool CraftingScreen::canCraftRecipe(const CraftingRecipe& recipe)
{
	// Check if player has all required items
	std::map<int, int> requiredItems;
	
	// Count required items from recipe pattern
	for (const auto& row : recipe.pattern) {
		for (int itemId : row) {
			if (itemId > 0) {
				requiredItems[itemId]++;
			}
		}
	}
	
	printf("Checking recipe - needs: ");
	for (const auto& pair : requiredItems) {
		printf("%dx%d ", pair.second, pair.first);
	}
	printf("\n");
	
	// Check if player has enough of each item
	Inventory* inventory = m_pPlayer->m_pInventory;
	for (const auto& pair : requiredItems) {
		int itemId = pair.first;
		int requiredCount = pair.second;
		int playerCount = 0;
		
		// Count items in player's inventory
		for (int i = 0; i < inventory->getNumItems(); i++) {
			ItemInstance* item = inventory->getItem(i);
			if (item && item->m_itemID == itemId) {
				playerCount += item->m_count;
			}
		}
		
		printf("Item %d: need %d, have %d\n", itemId, requiredCount, playerCount);
		if (playerCount < requiredCount) {
			printf("Missing item %d! Need %d, have %d\n", itemId, requiredCount, playerCount);
			return false;
		}
	}
	
	printf("Can craft this recipe!\n");
	return true;
}

void CraftingScreen::craftRecipe(const CraftingRecipe& recipe)
{
	if (!canCraftRecipe(recipe)) return;
	
	// Remove required items from inventory
	std::map<int, int> requiredItems;
	for (const auto& row : recipe.pattern) {
		for (int itemId : row) {
			if (itemId > 0) {
				requiredItems[itemId]++;
			}
		}
	}
	
	Inventory* inventory = m_pPlayer->m_pInventory;
	for (const auto& pair : requiredItems) {
		int itemId = pair.first;
		int requiredCount = pair.second;
		
		// Remove items from inventory
		for (int i = 0; i < inventory->getNumItems() && requiredCount > 0; i++) {
			ItemInstance* item = inventory->getItem(i);
			if (item && item->m_itemID == itemId) {
				int removeCount = std::min(requiredCount, (int)item->m_count);
				item->m_count -= removeCount;
				requiredCount -= removeCount;
				
				if (item->m_count <= 0) {
					delete item;
					inventory->setItem(i, nullptr);
				}
			}
		}
	}
	
	// Add result item to inventory
	ItemInstance resultItem(recipe.resultItemId, recipe.resultCount, 0);
	printf("Created result item: %d x%d\n", resultItem.m_itemID, resultItem.m_count);
	
	bool addResult = inventory->addItem(resultItem);
	if (addResult) {
		printf("Successfully crafted item %d x%d!\n", recipe.resultItemId, recipe.resultCount);
	} else {
		printf("Failed to add crafted item to inventory - inventory might be full!\n");
	}
	
	// Print final inventory state
	printf("Final inventory: ");
	for (int i = 0; i < inventory->getNumItems(); i++) {
		ItemInstance* item = inventory->getItem(i);
		if (item && item->m_count > 0) {
			printf("%dx%d ", item->m_count, item->m_itemID);
		}
	}
	printf("\n");
}

int CraftingScreen::getRecipeAt(int mouseX, int mouseY)
{
	if (mouseX < RECIPE_START_X || mouseX > RECIPE_START_X + RECIPE_WIDTH) {
		return -1;
	}
	
	int yOffset = mouseY - RECIPE_START_Y;
	if (yOffset < 0) return -1;
	
	int visibleIndex = yOffset / RECIPE_HEIGHT;
	if (visibleIndex >= 0 && visibleIndex < m_maxVisibleRecipes) {
		int actualIndex = m_scrollOffset + visibleIndex;
		if (actualIndex < m_availableRecipes.size()) {
			return actualIndex;
		}
	}
	
	return -1;
}

void CraftingScreen::handleScroll(int direction)
{
	if (m_availableRecipes.size() <= m_maxVisibleRecipes) return;
	
	m_scrollOffset += direction;
	if (m_scrollOffset < 0) {
		m_scrollOffset = 0;
	}
	int maxScroll = m_availableRecipes.size() - m_maxVisibleRecipes;
	if (m_scrollOffset > maxScroll) {
		m_scrollOffset = maxScroll;
	}
	
	// Update craft buttons for new visible recipes
	updateCraftButtons();
}

void CraftingScreen::keyPressed(int key)
{
	// Let parent handle key presses (like Escape)
	Screen::keyPressed(key);
}

std::string CraftingScreen::getItemName(int itemId)
{
	// Map item IDs to user-friendly names
	switch (itemId) {
		// Blocks/Tiles
		case TILE_WOOD: return "Wooden Planks";
		case TILE_TREE_TRUNK: return "Wood Log";
		case TILE_STONEBRICK: return "Cobblestone";
		case TILE_STONE: return "Stone";
		case TILE_DIRT: return "Dirt";
		case TILE_GRASS: return "Grass Block";
		case TILE_SAND: return "Sand";
		case TILE_GRAVEL: return "Gravel";
		case TILE_TORCH: return "Torch";
		case TILE_LADDER: return "Ladder";
		case TILE_WORKBENCH: return "Crafting Table";
		case TILE_FURNACE: return "Furnace";
		case TILE_CHEST: return "Chest";
		case TILE_GLASS: return "Glass";
		case TILE_BRICKS: return "Bricks";
		case TILE_TNT: return "TNT";
		case TILE_BOOKSHELF: return "Bookshelf";
		case TILE_OBSIDIAN: return "Obsidian";
		case TILE_ORE_COAL: return "Coal Ore";
		case TILE_ORE_IRON: return "Iron Ore";
		case TILE_ORE_GOLD: return "Gold Ore";
		case TILE_ORE_EMERALD: return "Diamond Ore";
		case TILE_BLOCK_IRON: return "Iron Block";
		case TILE_BLOCK_GOLD: return "Gold Block";
		case TILE_BLOCK_EMERALD: return "Diamond Block";
		
		// Tools
		case ITEM_STICK: return "Stick";
		case ITEM_PICKAXE_WOOD: return "Wooden Pickaxe";
		case ITEM_PICKAXE_STONE: return "Stone Pickaxe";
		case ITEM_PICKAXE_IRON: return "Iron Pickaxe";
		case ITEM_PICKAXE_GOLD: return "Gold Pickaxe";
		case ITEM_PICKAXE_EMERALD: return "Diamond Pickaxe";
		case ITEM_SWORD_WOOD: return "Wooden Sword";
		case ITEM_SWORD_STONE: return "Stone Sword";
		case ITEM_SWORD_IRON: return "Iron Sword";
		case ITEM_SWORD_GOLD: return "Gold Sword";
		case ITEM_SWORD_EMERALD: return "Diamond Sword";
		case ITEM_SHOVEL_WOOD: return "Wooden Shovel";
		case ITEM_SHOVEL_STONE: return "Stone Shovel";
		case ITEM_SHOVEL_IRON: return "Iron Shovel";
		case ITEM_SHOVEL_GOLD: return "Gold Shovel";
		case ITEM_SHOVEL_EMERALD: return "Diamond Shovel";
		case ITEM_HATCHET_WOOD: return "Wooden Axe";
		case ITEM_HATCHET_STONE: return "Stone Axe";
		case ITEM_HATCHET_IRON: return "Iron Axe";
		case ITEM_HATCHET_GOLD: return "Gold Axe";
		case ITEM_HATCHET_EMERALD: return "Diamond Axe";
		case ITEM_HOE_WOOD: return "Wooden Hoe";
		case ITEM_HOE_STONE: return "Stone Hoe";
		case ITEM_HOE_IRON: return "Iron Hoe";
		case ITEM_HOE_GOLD: return "Gold Hoe";
		case ITEM_HOE_EMERALD: return "Diamond Hoe";
		
		// Materials
		case ITEM_COAL: return "Coal";
		case ITEM_EMERALD: return "Diamond";
		case ITEM_INGOT_IRON: return "Iron Ingot";
		case ITEM_INGOT_GOLD: return "Gold Ingot";
		case ITEM_FLINT: return "Flint";
		case ITEM_STRING: return "String";
		case ITEM_FEATHER: return "Feather";
		case ITEM_LEATHER: return "Leather";
		case ITEM_PAPER: return "Paper";
		case ITEM_REEDS: return "Sugar Cane";
		case ITEM_WHEAT: return "Wheat";
		case ITEM_SEEDS: return "Seeds";
		case ITEM_CLAY: return "Clay";
		case ITEM_BRICK: return "Brick";
		case ITEM_BONE: return "Bone";
		case ITEM_SUGAR: return "Sugar";
		
		// Weapons & Tools
		case ITEM_BOW: return "Bow";
		case ITEM_ARROW: return "Arrow";
		case ITEM_FLINT_AND_STEEL: return "Flint and Steel";
		
		// Food
		case ITEM_APPLE: return "Apple";
		case ITEM_BREAD: return "Bread";
		case ITEM_PORKCHOP_RAW: return "Raw Porkchop";
		case ITEM_PORKCHOP_COOKED: return "Cooked Porkchop";
		case ITEM_FISH_RAW: return "Raw Fish";
		case ITEM_FISH_COOKED: return "Cooked Fish";
		case ITEM_APPLE_GOLD: return "Golden Apple";
		case ITEM_STEW_MUSHROOM: return "Mushroom Stew";
		case ITEM_COOKIE: return "Cookie";
		case ITEM_CAKE: return "Cake";
		
		// Utility
		case ITEM_BOWL: return "Bowl";
		case ITEM_BOOK: return "Book";
		case ITEM_SIGN: return "Sign";
		case ITEM_DOOR_WOOD: return "Wooden Door";
		case ITEM_DOOR_IRON: return "Iron Door";
		case ITEM_BUCKET: return "Bucket";
		case ITEM_BUCKET_WATER: return "Water Bucket";
		case ITEM_BUCKET_LAVA: return "Lava Bucket";
		case ITEM_BUCKET_MILK: return "Milk Bucket";
		case ITEM_FERTILIZER: return "Fertilizer";
		case ITEM_MINECART: return "Minecart";
		case ITEM_BOAT: return "Boat";
		case ITEM_COMPASS: return "Compass";
		case ITEM_CLOCK: return "Clock";
		case ITEM_FISHING_ROD: return "Fishing Rod";
		
		default:
			char buffer[32];
			snprintf(buffer, sizeof(buffer), "Item %d", itemId);
			return std::string(buffer);
	}
}
