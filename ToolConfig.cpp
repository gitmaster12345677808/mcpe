#include "ToolConfig.hpp"
#include "source/common/Utils.hpp"

namespace ToolConfig {

    // ====================================================================
    // TOOL EFFICIENCY CONFIGURATION
    // Modify these values to change how fast tools break blocks!
    // ====================================================================

    // Pickaxe efficiency for stone-type blocks (stone, ores, cobblestone, etc.)
    const MaterialEfficiency PICKAXE_EFFICIENCY = {
        .hand = 0.3f,      // Very slow with hand
        .wood = 2.0f,      // Wooden pickaxe
        .stone = 4.0f,     // Stone pickaxe  
        .iron = 6.0f,      // Iron pickaxe
        .gold = 12.0f,     // Gold pickaxe (fast but fragile)
        .diamond = 8.0f    // Diamond pickaxe (fast and durable)
    };

    // Shovel efficiency for dirt-type blocks (dirt, sand, gravel, etc.)
    const MaterialEfficiency SHOVEL_EFFICIENCY = {
        .hand = 1.0f,      // Normal speed with hand
        .wood = 2.0f,      // Wooden shovel
        .stone = 4.0f,     // Stone shovel
        .iron = 6.0f,      // Iron shovel
        .gold = 12.0f,     // Gold shovel
        .diamond = 8.0f    // Diamond shovel
    };

    // Axe efficiency for wood-type blocks (logs, wood planks, etc.)
    const MaterialEfficiency AXE_EFFICIENCY = {
        .hand = 0.8f,      // Slow with hand
        .wood = 2.0f,      // Wooden axe
        .stone = 4.0f,     // Stone axe
        .iron = 6.0f,      // Iron axe
        .gold = 12.0f,     // Gold axe
        .diamond = 8.0f    // Diamond axe
    };

    // Sword efficiency for special blocks (cobwebs, etc.)
    const MaterialEfficiency SWORD_EFFICIENCY = {
        .hand = 1.0f,      // Normal speed
        .wood = 1.5f,      // Wooden sword
        .stone = 1.5f,     // Stone sword
        .iron = 1.5f,      // Iron sword
        .gold = 1.5f,      // Gold sword
        .diamond = 1.5f    // Diamond sword
    };

    // Hoe efficiency for farmland
    const MaterialEfficiency HOE_EFFICIENCY = {
        .hand = 1.0f,      // Normal speed
        .wood = 2.0f,      // Wooden hoe
        .stone = 4.0f,     // Stone hoe
        .iron = 6.0f,      // Iron hoe
        .gold = 12.0f,     // Gold hoe
        .diamond = 8.0f    // Diamond hoe
    };

    // Penalty for using wrong tool type
    const MaterialEfficiency WRONG_TOOL_EFFICIENCY = {
        .hand = 1.0f,      // Hand speed (normal)
        .wood = 0.5f,      // Wrong wooden tool
        .stone = 0.5f,     // Wrong stone tool
        .iron = 0.5f,      // Wrong iron tool
        .gold = 0.5f,      // Wrong gold tool
        .diamond = 0.5f    // Wrong diamond tool
    };

    // ====================================================================
    // IMPLEMENTATION FUNCTIONS
    // ====================================================================

    void initializeToolEfficiency() {
        // This function can be called to initialize any dynamic values
        // Currently all values are static const, but this could be extended
        // to load from config files, etc.
    }

    bool isPickaxe(int itemId) {
        return (itemId == ITEM_PICKAXE_WOOD ||
                itemId == ITEM_PICKAXE_STONE ||
                itemId == ITEM_PICKAXE_IRON ||
                itemId == ITEM_PICKAXE_GOLD ||
                itemId == ITEM_PICKAXE_EMERALD);
    }

    bool isShovel(int itemId) {
        return (itemId == ITEM_SHOVEL_WOOD ||
                itemId == ITEM_SHOVEL_STONE ||
                itemId == ITEM_SHOVEL_IRON ||
                itemId == ITEM_SHOVEL_GOLD ||
                itemId == ITEM_SHOVEL_EMERALD);
    }

    bool isAxe(int itemId) {
        return (itemId == ITEM_HATCHET_WOOD ||
                itemId == ITEM_HATCHET_STONE ||
                itemId == ITEM_HATCHET_IRON ||
                itemId == ITEM_HATCHET_GOLD ||
                itemId == ITEM_HATCHET_EMERALD);
    }

    bool isSword(int itemId) {
        return (itemId == ITEM_SWORD_WOOD ||
                itemId == ITEM_SWORD_STONE ||
                itemId == ITEM_SWORD_IRON ||
                itemId == ITEM_SWORD_GOLD ||
                itemId == ITEM_SWORD_EMERALD);
    }

    bool isHoe(int itemId) {
        return (itemId == ITEM_HOE_WOOD ||
                itemId == ITEM_HOE_STONE ||
                itemId == ITEM_HOE_IRON ||
                itemId == ITEM_HOE_GOLD ||
                itemId == ITEM_HOE_EMERALD);
    }

    // Helper function to get material efficiency from item ID
    float getMaterialEfficiency(int itemId, const MaterialEfficiency& efficiency) {
        // Wooden tools
        if (itemId == ITEM_PICKAXE_WOOD || itemId == ITEM_SHOVEL_WOOD || 
            itemId == ITEM_HATCHET_WOOD || itemId == ITEM_SWORD_WOOD || itemId == ITEM_HOE_WOOD) {
            return efficiency.wood;
        }
        // Stone tools
        else if (itemId == ITEM_PICKAXE_STONE || itemId == ITEM_SHOVEL_STONE || 
                 itemId == ITEM_HATCHET_STONE || itemId == ITEM_SWORD_STONE || itemId == ITEM_HOE_STONE) {
            return efficiency.stone;
        }
        // Iron tools
        else if (itemId == ITEM_PICKAXE_IRON || itemId == ITEM_SHOVEL_IRON || 
                 itemId == ITEM_HATCHET_IRON || itemId == ITEM_SWORD_IRON || itemId == ITEM_HOE_IRON) {
            return efficiency.iron;
        }
        // Gold tools
        else if (itemId == ITEM_PICKAXE_GOLD || itemId == ITEM_SHOVEL_GOLD || 
                 itemId == ITEM_HATCHET_GOLD || itemId == ITEM_SWORD_GOLD || itemId == ITEM_HOE_GOLD) {
            return efficiency.gold;
        }
        // Diamond tools (emerald in code)
        else if (itemId == ITEM_PICKAXE_EMERALD || itemId == ITEM_SHOVEL_EMERALD || 
                 itemId == ITEM_HATCHET_EMERALD || itemId == ITEM_SWORD_EMERALD || itemId == ITEM_HOE_EMERALD) {
            return efficiency.diamond;
        }
        // Hand or unknown tool
        else {
            return efficiency.hand;
        }
    }

    bool isCorrectTool(int toolItemId, int blockId) {
        // Stone-type blocks require pickaxe
        if (blockId == TILE_STONE || blockId == TILE_STONEBRICK || 
            blockId == TILE_ORE_COAL || blockId == TILE_ORE_IRON || 
            blockId == TILE_ORE_GOLD || blockId == TILE_ORE_EMERALD ||
            blockId == TILE_BLOCK_IRON || blockId == TILE_BLOCK_GOLD || 
            blockId == TILE_BLOCK_EMERALD || blockId == TILE_OBSIDIAN ||
            blockId == TILE_BRICKS || blockId == TILE_FURNACE || blockId == TILE_FURNACE_LIT) {
            return isPickaxe(toolItemId);
        }
        
        // Dirt-type blocks work better with shovel
        else if (blockId == TILE_DIRT || blockId == TILE_GRASS || 
                 blockId == TILE_SAND || blockId == TILE_GRAVEL || blockId == TILE_CLAY) {
            return isShovel(toolItemId);
        }
        
        // Wood-type blocks work better with axe
        else if (blockId == TILE_TREE_TRUNK || blockId == TILE_WOOD || 
                 blockId == TILE_CHEST || blockId == TILE_WORKBENCH || blockId == TILE_BOOKSHELF) {
            return isAxe(toolItemId);
        }
        
        // Special sword blocks (like cobwebs if they exist)
        // Currently no special sword blocks in this version
        
        // All other blocks can be broken by hand at normal speed
        return (toolItemId == -1); // -1 means hand/no tool
    }

    float getToolEfficiency(int toolItemId, int blockId) {
        // Handle hand (no tool)
        if (toolItemId == -1) {
            // Stone blocks are very slow with hand
            if (blockId == TILE_STONE || blockId == TILE_STONEBRICK || 
                blockId == TILE_ORE_COAL || blockId == TILE_ORE_IRON || 
                blockId == TILE_ORE_GOLD || blockId == TILE_ORE_EMERALD ||
                blockId == TILE_BLOCK_IRON || blockId == TILE_BLOCK_GOLD || 
                blockId == TILE_BLOCK_EMERALD || blockId == TILE_OBSIDIAN ||
                blockId == TILE_BRICKS || blockId == TILE_FURNACE || blockId == TILE_FURNACE_LIT) {
                return PICKAXE_EFFICIENCY.hand;
            }
            // Wood blocks are slow with hand
            else if (blockId == TILE_TREE_TRUNK || blockId == TILE_WOOD || 
                     blockId == TILE_CHEST || blockId == TILE_WORKBENCH || blockId == TILE_BOOKSHELF) {
                return AXE_EFFICIENCY.hand;
            }
            // Dirt blocks are normal speed with hand
            else {
                return 1.0f;
            }
        }

        // Check if it's the correct tool for the block
        if (isCorrectTool(toolItemId, blockId)) {
            // Stone-type blocks
            if (blockId == TILE_STONE || blockId == TILE_STONEBRICK || 
                blockId == TILE_ORE_COAL || blockId == TILE_ORE_IRON || 
                blockId == TILE_ORE_GOLD || blockId == TILE_ORE_EMERALD ||
                blockId == TILE_BLOCK_IRON || blockId == TILE_BLOCK_GOLD || 
                blockId == TILE_BLOCK_EMERALD || blockId == TILE_OBSIDIAN ||
                blockId == TILE_BRICKS || blockId == TILE_FURNACE || blockId == TILE_FURNACE_LIT) {
                return getMaterialEfficiency(toolItemId, PICKAXE_EFFICIENCY);
            }
            // Dirt-type blocks
            else if (blockId == TILE_DIRT || blockId == TILE_GRASS || 
                     blockId == TILE_SAND || blockId == TILE_GRAVEL || blockId == TILE_CLAY) {
                return getMaterialEfficiency(toolItemId, SHOVEL_EFFICIENCY);
            }
            // Wood-type blocks
            else if (blockId == TILE_TREE_TRUNK || blockId == TILE_WOOD || 
                     blockId == TILE_CHEST || blockId == TILE_WORKBENCH || blockId == TILE_BOOKSHELF) {
                return getMaterialEfficiency(toolItemId, AXE_EFFICIENCY);
            }
        }
        
        // Wrong tool penalty
        return getMaterialEfficiency(toolItemId, WRONG_TOOL_EFFICIENCY);
    }

} // namespace ToolConfig