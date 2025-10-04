#include "CraftingGrid.hpp"

CraftingGrid::CraftingGrid(int width, int height)
    : m_width(width), m_height(height), m_grid(height, std::vector<ItemInstance*>(width, nullptr)) {}

void CraftingGrid::setItem(int x, int y, ItemInstance* item) {
    if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
        m_grid[y][x] = item;
    }
}

ItemInstance* CraftingGrid::getItem(int x, int y) const {
    if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
        return m_grid[y][x];
    }
    return nullptr;
}

std::vector<std::vector<ItemInstance*>> CraftingGrid::getGrid() const {
    return m_grid;
}

int CraftingGrid::getWidth() const { return m_width; }
int CraftingGrid::getHeight() const { return m_height; }
