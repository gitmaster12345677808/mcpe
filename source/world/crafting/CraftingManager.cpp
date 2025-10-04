#include "CraftingManager.hpp"
#include "common/Utils.hpp"

CraftingManager& CraftingManager::getInstance() {
    static CraftingManager instance;
    if (instance.m_recipes.empty()) {
        instance.initializeRecipes();
    }
    return instance;
}

void CraftingManager::addRecipe(const CraftingRecipe& recipe) {
    m_recipes.push_back(recipe);
}

const std::vector<CraftingRecipe>& CraftingManager::getRecipes() const {
    return m_recipes;
}

const CraftingRecipe* CraftingManager::match(const std::vector<std::vector<ItemInstance*>>& grid) const {
    for (const auto& recipe : m_recipes) {
        // Simple matching: check if grid matches recipe pattern
        if (grid.size() != recipe.pattern.size()) continue;
        bool match = true;
        for (size_t i = 0; i < grid.size(); ++i) {
            if (grid[i].size() != recipe.pattern[i].size()) { match = false; break; }
            for (size_t j = 0; j < grid[i].size(); ++j) {
                int id = grid[i][j] ? grid[i][j]->m_itemID : -1;
                if (id != recipe.pattern[i][j]) { match = false; break; }
            }
            if (!match) break;
        }
        if (match) return &recipe;
    }
    return nullptr;
}

void CraftingManager::initializeRecipes() {
    CraftingRecipe recipe;
    
    // =================
    // BASIC MATERIALS
    // =================
    
    // Wooden planks recipe (1 log -> 4 planks)
    recipe.pattern = {{TILE_TREE_TRUNK}}; // Tree trunk ID = 17
    recipe.resultItemId = TILE_WOOD; // Wood plank ID = 5
    recipe.resultCount = 4;
    addRecipe(recipe);
    
    // Stick recipe (2 wooden planks vertically)
    recipe.pattern = {{TILE_WOOD}, {TILE_WOOD}}; // Wood plank ID = 5
    recipe.resultItemId = ITEM_STICK; // Stick item ID = 280
    recipe.resultCount = 4;
    addRecipe(recipe);
    
    // =================
    // WOODEN TOOLS
    // =================
    
    // Wooden Pickaxe
    recipe.pattern = {
        {TILE_WOOD, TILE_WOOD, TILE_WOOD},
        {-1, ITEM_STICK, -1},
        {-1, ITEM_STICK, -1}
    };
    recipe.resultItemId = ITEM_PICKAXE_WOOD;
    recipe.resultCount = 1;
    addRecipe(recipe);
    
    // Wooden Sword
    recipe.pattern = {
        {TILE_WOOD},
        {TILE_WOOD},
        {ITEM_STICK}
    };
    recipe.resultItemId = ITEM_SWORD_WOOD;
    recipe.resultCount = 1;
    addRecipe(recipe);
    
    // Wooden Shovel
    recipe.pattern = {
        {TILE_WOOD},
        {ITEM_STICK},
        {ITEM_STICK}
    };
    recipe.resultItemId = ITEM_SHOVEL_WOOD;
    recipe.resultCount = 1;
    addRecipe(recipe);
    
    // Wooden Axe
    recipe.pattern = {
        {TILE_WOOD, TILE_WOOD},
        {TILE_WOOD, ITEM_STICK},
        {-1, ITEM_STICK}
    };
    recipe.resultItemId = ITEM_HATCHET_WOOD;
    recipe.resultCount = 1;
    addRecipe(recipe);
    
    // Wooden Hoe
    recipe.pattern = {
        {TILE_WOOD, TILE_WOOD},
        {-1, ITEM_STICK},
        {-1, ITEM_STICK}
    };
    recipe.resultItemId = ITEM_HOE_WOOD;
    recipe.resultCount = 1;
    addRecipe(recipe);
    
    // =================
    // STONE TOOLS
    // =================
    
    // Stone Pickaxe
    recipe.pattern = {
        {TILE_STONEBRICK, TILE_STONEBRICK, TILE_STONEBRICK},
        {-1, ITEM_STICK, -1},
        {-1, ITEM_STICK, -1}
    };
    recipe.resultItemId = ITEM_PICKAXE_STONE;
    recipe.resultCount = 1;
    addRecipe(recipe);
    
    // Stone Sword
    recipe.pattern = {
        {TILE_STONEBRICK},
        {TILE_STONEBRICK},
        {ITEM_STICK}
    };
    recipe.resultItemId = ITEM_SWORD_STONE;
    recipe.resultCount = 1;
    addRecipe(recipe);
    
    // Stone Shovel
    recipe.pattern = {
        {TILE_STONEBRICK},
        {ITEM_STICK},
        {ITEM_STICK}
    };
    recipe.resultItemId = ITEM_SHOVEL_STONE;
    recipe.resultCount = 1;
    addRecipe(recipe);
    
    // Stone Axe
    recipe.pattern = {
        {TILE_STONEBRICK, TILE_STONEBRICK},
        {TILE_STONEBRICK, ITEM_STICK},
        {-1, ITEM_STICK}
    };
    recipe.resultItemId = ITEM_HATCHET_STONE;
    recipe.resultCount = 1;
    addRecipe(recipe);
    
    // Stone Hoe
    recipe.pattern = {
        {TILE_STONEBRICK, TILE_STONEBRICK},
        {-1, ITEM_STICK},
        {-1, ITEM_STICK}
    };
    recipe.resultItemId = ITEM_HOE_STONE;
    recipe.resultCount = 1;
    addRecipe(recipe);
    
    // =================
    // IRON TOOLS
    // =================
    
    // Iron Pickaxe
    recipe.pattern = {
        {ITEM_INGOT_IRON, ITEM_INGOT_IRON, ITEM_INGOT_IRON},
        {-1, ITEM_STICK, -1},
        {-1, ITEM_STICK, -1}
    };
    recipe.resultItemId = ITEM_PICKAXE_IRON;
    recipe.resultCount = 1;
    addRecipe(recipe);
    
    // Iron Sword
    recipe.pattern = {
        {ITEM_INGOT_IRON},
        {ITEM_INGOT_IRON},
        {ITEM_STICK}
    };
    recipe.resultItemId = ITEM_SWORD_IRON;
    recipe.resultCount = 1;
    addRecipe(recipe);
    
    // Iron Shovel
    recipe.pattern = {
        {ITEM_INGOT_IRON},
        {ITEM_STICK},
        {ITEM_STICK}
    };
    recipe.resultItemId = ITEM_SHOVEL_IRON;
    recipe.resultCount = 1;
    addRecipe(recipe);
    
    // Iron Axe
    recipe.pattern = {
        {ITEM_INGOT_IRON, ITEM_INGOT_IRON},
        {ITEM_INGOT_IRON, ITEM_STICK},
        {-1, ITEM_STICK}
    };
    recipe.resultItemId = ITEM_HATCHET_IRON;
    recipe.resultCount = 1;
    addRecipe(recipe);
    
    // Iron Hoe
    recipe.pattern = {
        {ITEM_INGOT_IRON, ITEM_INGOT_IRON},
        {-1, ITEM_STICK},
        {-1, ITEM_STICK}
    };
    recipe.resultItemId = ITEM_HOE_IRON;
    recipe.resultCount = 1;
    addRecipe(recipe);
    
    // =================
    // GOLD TOOLS
    // =================
    
    // Gold Pickaxe
    recipe.pattern = {
        {ITEM_INGOT_GOLD, ITEM_INGOT_GOLD, ITEM_INGOT_GOLD},
        {-1, ITEM_STICK, -1},
        {-1, ITEM_STICK, -1}
    };
    recipe.resultItemId = ITEM_PICKAXE_GOLD;
    recipe.resultCount = 1;
    addRecipe(recipe);
    
    // Gold Sword
    recipe.pattern = {
        {ITEM_INGOT_GOLD},
        {ITEM_INGOT_GOLD},
        {ITEM_STICK}
    };
    recipe.resultItemId = ITEM_SWORD_GOLD;
    recipe.resultCount = 1;
    addRecipe(recipe);
    
    // Gold Shovel
    recipe.pattern = {
        {ITEM_INGOT_GOLD},
        {ITEM_STICK},
        {ITEM_STICK}
    };
    recipe.resultItemId = ITEM_SHOVEL_GOLD;
    recipe.resultCount = 1;
    addRecipe(recipe);
    
    // Gold Axe
    recipe.pattern = {
        {ITEM_INGOT_GOLD, ITEM_INGOT_GOLD},
        {ITEM_INGOT_GOLD, ITEM_STICK},
        {-1, ITEM_STICK}
    };
    recipe.resultItemId = ITEM_HATCHET_GOLD;
    recipe.resultCount = 1;
    addRecipe(recipe);
    
    // Gold Hoe
    recipe.pattern = {
        {ITEM_INGOT_GOLD, ITEM_INGOT_GOLD},
        {-1, ITEM_STICK},
        {-1, ITEM_STICK}
    };
    recipe.resultItemId = ITEM_HOE_GOLD;
    recipe.resultCount = 1;
    addRecipe(recipe);
    
    // =================
    // DIAMOND/EMERALD TOOLS
    // =================
    
    // Diamond Pickaxe
    recipe.pattern = {
        {ITEM_EMERALD, ITEM_EMERALD, ITEM_EMERALD},
        {-1, ITEM_STICK, -1},
        {-1, ITEM_STICK, -1}
    };
    recipe.resultItemId = ITEM_PICKAXE_EMERALD;
    recipe.resultCount = 1;
    addRecipe(recipe);
    
    // Diamond Sword
    recipe.pattern = {
        {ITEM_EMERALD},
        {ITEM_EMERALD},
        {ITEM_STICK}
    };
    recipe.resultItemId = ITEM_SWORD_EMERALD;
    recipe.resultCount = 1;
    addRecipe(recipe);
    
    // Diamond Shovel
    recipe.pattern = {
        {ITEM_EMERALD},
        {ITEM_STICK},
        {ITEM_STICK}
    };
    recipe.resultItemId = ITEM_SHOVEL_EMERALD;
    recipe.resultCount = 1;
    addRecipe(recipe);
    
    // Diamond Axe
    recipe.pattern = {
        {ITEM_EMERALD, ITEM_EMERALD},
        {ITEM_EMERALD, ITEM_STICK},
        {-1, ITEM_STICK}
    };
    recipe.resultItemId = ITEM_HATCHET_EMERALD;
    recipe.resultCount = 1;
    addRecipe(recipe);
    
    // Diamond Hoe
    recipe.pattern = {
        {ITEM_EMERALD, ITEM_EMERALD},
        {-1, ITEM_STICK},
        {-1, ITEM_STICK}
    };
    recipe.resultItemId = ITEM_HOE_EMERALD;
    recipe.resultCount = 1;
    addRecipe(recipe);
    
    // =================
    // BUILDING BLOCKS
    // =================
    
    // Ladder recipe (7 sticks in H pattern)
    recipe.pattern = {
        {ITEM_STICK, -1, ITEM_STICK},
        {ITEM_STICK, ITEM_STICK, ITEM_STICK},
        {ITEM_STICK, -1, ITEM_STICK}
    };
    recipe.resultItemId = TILE_LADDER; // Ladder ID
    recipe.resultCount = 3;
    addRecipe(recipe);
    
    // Torch recipe (coal + stick)
    recipe.pattern = {
        {ITEM_COAL},
        {ITEM_STICK}
    };
    recipe.resultItemId = TILE_TORCH;
    recipe.resultCount = 4;
    addRecipe(recipe);
    
    // Crafting Table recipe (4 planks)
    recipe.pattern = {
        {TILE_WOOD, TILE_WOOD},
        {TILE_WOOD, TILE_WOOD}
    };
    recipe.resultItemId = TILE_WORKBENCH;
    recipe.resultCount = 1;
    addRecipe(recipe);
    
    // Furnace recipe (8 cobblestone)
    recipe.pattern = {
        {TILE_STONEBRICK, TILE_STONEBRICK, TILE_STONEBRICK},
        {TILE_STONEBRICK, -1, TILE_STONEBRICK},
        {TILE_STONEBRICK, TILE_STONEBRICK, TILE_STONEBRICK}
    };
    recipe.resultItemId = TILE_FURNACE;
    recipe.resultCount = 1;
    addRecipe(recipe);
    
    // Chest recipe (8 planks)
    recipe.pattern = {
        {TILE_WOOD, TILE_WOOD, TILE_WOOD},
        {TILE_WOOD, -1, TILE_WOOD},
        {TILE_WOOD, TILE_WOOD, TILE_WOOD}
    };
    recipe.resultItemId = TILE_CHEST;
    recipe.resultCount = 1;
    addRecipe(recipe);
    
    // =================
    // UTILITY ITEMS
    // =================
    
    // Bow recipe
    recipe.pattern = {
        {-1, ITEM_STICK, ITEM_STRING},
        {ITEM_STICK, -1, ITEM_STRING},
        {-1, ITEM_STICK, ITEM_STRING}
    };
    recipe.resultItemId = ITEM_BOW;
    recipe.resultCount = 1;
    addRecipe(recipe);
    
    // Arrow recipe
    recipe.pattern = {
        {ITEM_FLINT},
        {ITEM_STICK},
        {ITEM_FEATHER}
    };
    recipe.resultItemId = ITEM_ARROW;
    recipe.resultCount = 4;
    addRecipe(recipe);
    
    // Bowl recipe
    recipe.pattern = {
        {TILE_WOOD, -1, TILE_WOOD},
        {-1, TILE_WOOD, -1}
    };
    recipe.resultItemId = ITEM_BOWL;
    recipe.resultCount = 4;
    addRecipe(recipe);
    
    // Bread recipe
    recipe.pattern = {
        {ITEM_WHEAT, ITEM_WHEAT, ITEM_WHEAT}
    };
    recipe.resultItemId = ITEM_BREAD;
    recipe.resultCount = 1;
    addRecipe(recipe);
    
    // Paper recipe
    recipe.pattern = {
        {ITEM_REEDS, ITEM_REEDS, ITEM_REEDS}
    };
    recipe.resultItemId = ITEM_PAPER;
    recipe.resultCount = 3;
    addRecipe(recipe);
    
    // Book recipe
    recipe.pattern = {
        {ITEM_PAPER},
        {ITEM_PAPER},
        {ITEM_PAPER}
    };
    recipe.resultItemId = ITEM_BOOK;
    recipe.resultCount = 1;
    addRecipe(recipe);
    
    // Sign recipe
    recipe.pattern = {
        {TILE_WOOD, TILE_WOOD, TILE_WOOD},
        {TILE_WOOD, TILE_WOOD, TILE_WOOD},
        {-1, ITEM_STICK, -1}
    };
    recipe.resultItemId = ITEM_SIGN;
    recipe.resultCount = 3;
    addRecipe(recipe);
    
    // Wooden Door recipe
    recipe.pattern = {
        {TILE_WOOD, TILE_WOOD},
        {TILE_WOOD, TILE_WOOD},
        {TILE_WOOD, TILE_WOOD}
    };
    recipe.resultItemId = ITEM_DOOR_WOOD;
    recipe.resultCount = 1;
    addRecipe(recipe);
    
    // Fertilizer recipe (3 logs + 3 dirt)
    recipe.pattern = {
        {TILE_TREE_TRUNK, TILE_DIRT, TILE_TREE_TRUNK},
        {TILE_DIRT, TILE_TREE_TRUNK, TILE_DIRT}
    };
    recipe.resultItemId = ITEM_FERTILIZER;
    recipe.resultCount = 6; // Makes 6 fertilizer
    addRecipe(recipe);
}
