#include "CraftingInterface.hpp"
#include "world/entity/Player.hpp"
#include "CraftingManager.hpp"
#include "network/packets/CraftingPacket.hpp"

CraftingInterface::CraftingInterface(Player* player) 
    : m_pPlayer(player), m_grid(3, 3), m_bOpen(false) {}

void CraftingInterface::open() {
    m_bOpen = true;
    m_pPlayer->m_pInventory->openCraftingTable();
}

void CraftingInterface::close() {
    m_bOpen = false;
    clearGrid();
    m_pPlayer->m_pInventory->closeCraftingTable();
}

bool CraftingInterface::isOpen() const {
    return m_bOpen;
}

void CraftingInterface::setGridItem(int x, int y, ItemInstance* item) {
    m_grid.setItem(x, y, item);
}

ItemInstance* CraftingInterface::getGridItem(int x, int y) const {
    return m_grid.getItem(x, y);
}

bool CraftingInterface::attemptCraft() {
    const CraftingRecipe* recipe = CraftingManager::getInstance().match(m_grid.getGrid());
    if (!recipe) return false;
    
    // TODO: Implement proper network packet for multiplayer when CraftingPacket is available
    // For now, just log the crafting action
    printf("Player %d crafted item %d\n", m_pPlayer->m_EntityID, recipe->resultItemId);
    
    // For singleplayer, directly perform crafting
    return m_pPlayer->m_pInventory->craft(m_grid);
}

void CraftingInterface::clearGrid() {
    for (int y = 0; y < m_grid.getHeight(); ++y) {
        for (int x = 0; x < m_grid.getWidth(); ++x) {
            m_grid.setItem(x, y, nullptr);
        }
    }
}

CraftingGrid& CraftingInterface::getGrid() {
    return m_grid;
}