/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "FurnaceScreen.hpp"
#include "IngameBlockSelectionScreen.hpp"
#include "compat/KeyCodes.hpp"
#include "world/entity/Player.hpp"
#include "world/item/Inventory.hpp"
#include "world/item/Item.hpp"
#include "common/Utils.hpp"
#include "client/renderer/Tesselator.hpp"
#include "client/gui/components/Button.hpp"
#include <algorithm>
#include <string>

FurnaceScreen::FurnaceScreen(Player* player, const TilePos& pos)
    : m_pPlayer(player), m_furnacePos(pos), m_scrollOffset(0), m_maxVisibleItems(MAX_VISIBLE_ITEMS), m_playerFuelCount(0)
{
    m_btnDone = nullptr;
    m_btnScrollUp = nullptr;
    m_btnScrollDown = nullptr;
}

FurnaceScreen::~FurnaceScreen()
{
    // Cleanup is handled by Screen destructor
}

void FurnaceScreen::init()
{
    // Create UI buttons
    m_btnDone = new Button(1, m_width - 80, 10, 70, 20, "Done");
    m_buttons.push_back(m_btnDone);
    
    m_btnScrollUp = new Button(2, m_width - 30, ITEM_START_Y, 20, 20, "Up");
    m_buttons.push_back(m_btnScrollUp);
    
    m_btnScrollDown = new Button(3, m_width - 30, ITEM_START_Y + (MAX_VISIBLE_ITEMS * ITEM_HEIGHT), 20, 20, "Down");
    m_buttons.push_back(m_btnScrollDown);
    
    // Find smeltable items in player inventory
    m_smeltableItems.clear();
    Inventory* inventory = m_pPlayer->m_pInventory;
    m_playerFuelCount = 0;
    
    for (int i = 0; i < inventory->getNumItems(); i++) {
        ItemInstance* item = inventory->getItem(i);
        if (item && item->m_count > 0) {
            // Count fuel items
            if (item->m_itemID == 263 || item->m_itemID == TILE_WOOD || item->m_itemID == TILE_SAPLING) {
                m_playerFuelCount += item->m_count;
            }
            
            // Add smeltable items
            if (canSmelt(item)) {
                SmeltingItem smeltItem;
                smeltItem.item = item;
                smeltItem.inventorySlot = i;
                smeltItem.progress = 0.0f;
                smeltItem.smelting = false;
                smeltItem.fuelTime = 0.0f;
                m_smeltableItems.push_back(smeltItem);
            }
        }
    }
    
    updateSmeltButtons();
}

void FurnaceScreen::render(int mouseX, int mouseY, float f)
{
    renderBackground(0);
    
    // Draw title
    m_pMinecraft->m_pFont->draw("Furnace", m_width / 2 - 25, 10, 0xFFFFFF);
    
    // Show fuel count
    char fuelText[64];
    snprintf(fuelText, sizeof(fuelText), "Fuel available: %d", m_playerFuelCount);
    m_pMinecraft->m_pFont->draw(fuelText, 10, 20, 0xCCCCCC);
    
    // Update smelting progress
    updateSmelting();
    
    // Render the smeltable items list
    renderSmeltableItems();
    
    // Call parent render for buttons
    Screen::render(mouseX, mouseY, f);
}

void FurnaceScreen::renderSmeltableItems()
{
    int yPos = ITEM_START_Y;
    int endIndex = std::min((int)m_smeltableItems.size(), m_scrollOffset + m_maxVisibleItems);
    
    for (int i = m_scrollOffset; i < endIndex; i++) {
        renderSmeltingItem(i, m_smeltableItems[i], ITEM_START_X, yPos);
        yPos += ITEM_HEIGHT;
    }
}

void FurnaceScreen::renderSmeltingItem(int index, const SmeltingItem& smeltItem, int x, int y)
{
    // Draw item background
    Tesselator& t = Tesselator::instance;
    
    // Background color based on state
    int bgColor;
    if (smeltItem.smelting) {
        bgColor = 0xFF404040; // Dark gray for smelting
    } else if (m_playerFuelCount > 0) {
        bgColor = 0xFF2F2F2F; // Medium gray if can smelt
    } else {
        bgColor = 0xFF1F1F1F; // Dark gray if no fuel
    }
    
    t.begin();
    t.color(bgColor);
    t.vertex(x + ITEM_WIDTH, y + ITEM_HEIGHT, 0.0f);
    t.vertex(x + ITEM_WIDTH, y, 0.0f);
    t.vertex(x, y, 0.0f);
    t.vertex(x, y + ITEM_HEIGHT, 0.0f);
    t.draw();
    
    // Draw item info
    ItemInstance* result = getSmeltResult(smeltItem.item);
    std::string itemName = getItemName(smeltItem.item->m_itemID);
    std::string resultName = result ? getItemName(result->m_itemID) : "Unknown";
    
    char itemText[128];
    snprintf(itemText, sizeof(itemText), "%s (x%d) -> %s", 
             itemName.c_str(), smeltItem.item->m_count, resultName.c_str());
    
    int textColor = (m_playerFuelCount > 0) ? 0xFFFFFF : 0x888888;
    m_pMinecraft->m_pFont->draw(itemText, x + 5, y + 8, textColor);
    
    // Draw smelting progress bar if smelting
    if (smeltItem.smelting && smeltItem.progress > 0.0f) {
        int progressBarX = x + 5;
        int progressBarY = y + 20;
        int progressBarWidth = 100;
        int progressBarHeight = 6;
        
        // Progress bar background
        t.begin();
        t.color(0xFF333333);
        t.vertex(progressBarX + progressBarWidth, progressBarY + progressBarHeight, 0.0f);
        t.vertex(progressBarX + progressBarWidth, progressBarY, 0.0f);
        t.vertex(progressBarX, progressBarY, 0.0f);
        t.vertex(progressBarX, progressBarY + progressBarHeight, 0.0f);
        t.draw();
        
        // Progress bar fill
        int fillWidth = (int)(progressBarWidth * smeltItem.progress);
        if (fillWidth > 0) {
            t.begin();
            t.color(0xFF00FF00); // Green progress
            t.vertex(progressBarX + fillWidth, progressBarY + progressBarHeight, 0.0f);
            t.vertex(progressBarX + fillWidth, progressBarY, 0.0f);
            t.vertex(progressBarX, progressBarY, 0.0f);
            t.vertex(progressBarX, progressBarY + progressBarHeight, 0.0f);
            t.draw();
        }
    }
    
    if (result) delete result;
}

void FurnaceScreen::updateSmeltButtons()
{
    // Clear existing smelt buttons
    for (Button* btn : m_smeltButtons) {
        auto it = std::find(m_buttons.begin(), m_buttons.end(), btn);
        if (it != m_buttons.end()) {
            m_buttons.erase(it);
        }
        delete btn;
    }
    m_smeltButtons.clear();
    
    // Create smelt buttons for visible items
    int yPos = ITEM_START_Y;
    int endIndex = std::min((int)m_smeltableItems.size(), m_scrollOffset + m_maxVisibleItems);
    
    for (int i = m_scrollOffset; i < endIndex; i++) {
        const SmeltingItem& smeltItem = m_smeltableItems[i];
        bool canStartSmelting = !smeltItem.smelting && m_playerFuelCount > 0;
        
        // Create button at end of item row
        int buttonX = ITEM_START_X + ITEM_WIDTH - 60;
        int buttonY = yPos + 5;
        int buttonId = 100 + i; // Start smelt button IDs at 100
        
        const char* buttonText;
        if (smeltItem.smelting) {
            buttonText = "Smelting...";
        } else if (m_playerFuelCount > 0) {
            buttonText = "Smelt";
        } else {
            buttonText = "No Fuel";
        }
        
        Button* smeltBtn = new Button(buttonId, buttonX, buttonY, 55, 20, buttonText);
        smeltBtn->m_bEnabled = canStartSmelting;
        
        m_smeltButtons.push_back(smeltBtn);
        m_buttons.push_back(smeltBtn);
        
        yPos += ITEM_HEIGHT;
    }
}

void FurnaceScreen::buttonClicked(Button* pButton)
{
    if (pButton->m_buttonId == 1) { // Done button
        m_pMinecraft->setScreen(new IngameBlockSelectionScreen());
    } else if (pButton->m_buttonId == 2) { // Scroll Up button
        handleScroll(-1);
    } else if (pButton->m_buttonId == 3) { // Scroll Down button
        handleScroll(1);
    } else if (pButton->m_buttonId >= 100) { // Smelt button
        int itemIndex = pButton->m_buttonId - 100;
        if (itemIndex >= 0 && itemIndex < m_smeltableItems.size()) {
            SmeltingItem& smeltItem = m_smeltableItems[itemIndex];
            if (!smeltItem.smelting && hasFuel()) {
                // Start smelting this item
                smeltItem.smelting = true;
                smeltItem.progress = 0.0f;
                smeltItem.fuelTime = 200.0f; // 10 seconds of smelting time
                m_activeSmeltingItems[smeltItem.inventorySlot] = &smeltItem;
                
                // Consume fuel
                consumeFuel();
                
                // Update buttons
                updateSmeltButtons();
            }
        }
    }
}

void FurnaceScreen::mouseClicked(int x, int y, int type)
{
    Screen::mouseClicked(x, y, type);
}

void FurnaceScreen::keyPressed(int key)
{
    if (key == SDLVK_ESCAPE) {
        m_pMinecraft->setScreen(new IngameBlockSelectionScreen());
        return;
    }
    
    Screen::keyPressed(key);
}

void FurnaceScreen::removed()
{
    // Nothing special needed
}

void FurnaceScreen::updateSmelting()
{
    // Update all active smelting items
    for (auto it = m_activeSmeltingItems.begin(); it != m_activeSmeltingItems.end();) {
        SmeltingItem* smeltItem = it->second;
        
        if (smeltItem->smelting) {
            smeltItem->progress += 0.05f; // Increase progress each frame
            
            if (smeltItem->progress >= 1.0f) {
                // Smelting complete!
                ItemInstance* result = getSmeltResult(smeltItem->item);
                if (result) {
                    // Reduce source item count
                    smeltItem->item->m_count--;
                    
                    // Add result to inventory
                    m_pPlayer->m_pInventory->addItem(*result);
                    delete result;
                    
                    // Reset smelting state
                    smeltItem->smelting = false;
                    smeltItem->progress = 0.0f;
                    
                    // Remove from active list
                    it = m_activeSmeltingItems.erase(it);
                    
                    // Refresh the screen if item count is now 0
                    if (smeltItem->item->m_count <= 0) {
                        init(); // Refresh smeltable items list
                        return;
                    }
                    
                    updateSmeltButtons();
                    continue;
                }
            }
        }
        ++it;
    }
}

bool FurnaceScreen::canSmelt(ItemInstance* item)
{
    if (!item) return false;
    
    switch (item->m_itemID) {
        case TILE_ORE_IRON:
        case TILE_ORE_GOLD:
        case TILE_ORE_COAL:
        case TILE_ORE_EMERALD: // Diamond ore
            return true;
        default:
            return false;
    }
}

ItemInstance* FurnaceScreen::getSmeltResult(ItemInstance* item)
{
    if (!item) return nullptr;
    
    switch (item->m_itemID) {
        case TILE_ORE_IRON:
            return new ItemInstance(265, 1, 0); // Iron ingot
        case TILE_ORE_GOLD:
            return new ItemInstance(266, 1, 0); // Gold ingot
        case TILE_ORE_COAL:
            return new ItemInstance(263, 1, 0); // Coal
        case TILE_ORE_EMERALD:
            return new ItemInstance(264, 1, 0); // Diamond
        default:
            return nullptr;
    }
}

bool FurnaceScreen::hasFuel()
{
    return m_playerFuelCount > 0;
}

void FurnaceScreen::consumeFuel()
{
    // Find and consume one fuel item from inventory
    Inventory* inventory = m_pPlayer->m_pInventory;
    
    for (int i = 0; i < inventory->getNumItems(); i++) {
        ItemInstance* item = inventory->getItem(i);
        if (item && item->m_count > 0) {
            if (item->m_itemID == 263 || item->m_itemID == TILE_WOOD || item->m_itemID == TILE_SAPLING) {
                item->m_count--;
                m_playerFuelCount--;
                if (item->m_count <= 0) {
                    inventory->setItem(i, nullptr);
                    delete item;
                }
                return;
            }
        }
    }
}

void FurnaceScreen::handleScroll(int direction)
{
    m_scrollOffset += direction;
    if (m_scrollOffset < 0) m_scrollOffset = 0;
    
    int maxScroll = std::max(0, (int)m_smeltableItems.size() - m_maxVisibleItems);
    if (m_scrollOffset > maxScroll) m_scrollOffset = maxScroll;
    
    updateSmeltButtons();
}

std::string FurnaceScreen::getItemName(int itemId)
{
    switch (itemId) {
        case TILE_ORE_IRON: return "Iron Ore";
        case TILE_ORE_GOLD: return "Gold Ore";  
        case TILE_ORE_COAL: return "Coal Ore";
        case TILE_ORE_EMERALD: return "Diamond Ore";
        case 265: return "Iron Ingot";
        case 266: return "Gold Ingot";
        case 263: return "Coal";
        case 264: return "Diamond";
        default: return "Unknown Item";
    }
}