#pragma once
#include <vector>
#include <map>
#include <string>
#include "world/item/ItemInstance.hpp"

struct CraftingRecipe {
    std::vector<std::vector<int>> pattern; // 2D grid of item IDs
    int resultItemId;
    int resultCount;
};

class CraftingManager {
public:
    static CraftingManager& getInstance();
    void addRecipe(const CraftingRecipe& recipe);
    const CraftingRecipe* match(const std::vector<std::vector<ItemInstance*>>& grid) const;
    const std::vector<CraftingRecipe>& getRecipes() const;
    void initializeRecipes();
private:
    std::vector<CraftingRecipe> m_recipes;
    CraftingManager() = default;
};
