#pragma once
#include "world/crafting/CraftingGrid.hpp"
#include "world/item/ItemInstance.hpp"

class Player;

class CraftingInterface {
public:
    CraftingInterface(Player* player);
    
    void open();
    void close();
    bool isOpen() const;
    
    void setGridItem(int x, int y, ItemInstance* item);
    ItemInstance* getGridItem(int x, int y) const;
    
    bool attemptCraft();
    void clearGrid();
    
    CraftingGrid& getGrid();
    
private:
    Player* m_pPlayer;
    CraftingGrid m_grid;
    bool m_bOpen;
};