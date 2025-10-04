#include "CraftingManager.hpp"
#include "world/item/Item.hpp"
#include "world/tile/Tile.hpp"

void CraftingManager::initializeRecipes() {
    CraftingRecipe recipe;
    
    // Stick recipe (2 wooden planks vertically)
    recipe.pattern = {{Tile::wood->m_ID}, {Tile::wood->m_ID}};
    recipe.resultItemId = Item::stick->m_itemID;
    recipe.resultCount = 4;
    addRecipe(recipe);
    
    // Wooden planks recipe (1 log -> 4 planks)
    recipe.pattern = {{Tile::treeTrunk->m_ID}};
    recipe.resultItemId = Tile::wood->m_ID;
    recipe.resultCount = 4;
    addRecipe(recipe);
    
    // Ladder recipe (7 sticks in H pattern)
    recipe.pattern = {
        {Item::stick->m_itemID, -1, Item::stick->m_itemID},
        {Item::stick->m_itemID, Item::stick->m_itemID, Item::stick->m_itemID},
        {Item::stick->m_itemID, -1, Item::stick->m_itemID}
    };
    recipe.resultItemId = Tile::ladder->m_ID;
    recipe.resultCount = 3;
    addRecipe(recipe);
    
    // Sugar from sugar cane
    recipe.pattern = {{Item::wheat->m_itemID}};
    recipe.resultItemId = Item::sugar->m_itemID;
    recipe.resultCount = 1;
    addRecipe(recipe);
}