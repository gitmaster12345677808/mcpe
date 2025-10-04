#pragma once
#include <vector>
#include "world/item/ItemInstance.hpp"

class CraftingGrid {
public:
    CraftingGrid(int width, int height);
    void setItem(int x, int y, ItemInstance* item);
    ItemInstance* getItem(int x, int y) const;
    std::vector<std::vector<ItemInstance*>> getGrid() const;
    int getWidth() const;
    int getHeight() const;
private:
    int m_width;
    int m_height;
    std::vector<std::vector<ItemInstance*>> m_grid;
};
