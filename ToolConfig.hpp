#pragma once

// Tool Efficiency Configuration System
// This file allows easy modification of tool breaking speeds for different materials and block types
// Higher values = faster breaking speed

namespace ToolConfig {

    // Base efficiency multipliers for different tool materials
    struct MaterialEfficiency {
        float hand;        // No tool (hand)
        float wood;        // Wooden tools
        float stone;       // Stone tools  
        float iron;        // Iron tools
        float gold;        // Gold tools
        float diamond;     // Diamond tools (emerald in code)
    };

    // Efficiency for different block types when using the correct tool
    // These are multiplied by the material efficiency above
    extern const MaterialEfficiency PICKAXE_EFFICIENCY;  // Stone, ores, etc.
    extern const MaterialEfficiency SHOVEL_EFFICIENCY;   // Dirt, sand, gravel, etc.
    extern const MaterialEfficiency AXE_EFFICIENCY;      // Wood, leaves, etc.
    extern const MaterialEfficiency SWORD_EFFICIENCY;    // Cobwebs, etc.
    extern const MaterialEfficiency HOE_EFFICIENCY;      // Farmland, etc.

    // Wrong tool penalty - efficiency when using wrong tool type for a block
    extern const MaterialEfficiency WRONG_TOOL_EFFICIENCY;

    // Initialize the efficiency values (call this in Item::initItems or similar)
    void initializeToolEfficiency();
    
    // Get the efficiency multiplier for a specific tool and block combination
    float getToolEfficiency(int toolItemId, int blockId);
    
    // Check if a tool is the correct type for a block
    bool isCorrectTool(int toolItemId, int blockId);
    
    // Helper functions to identify tool types
    bool isPickaxe(int itemId);
    bool isShovel(int itemId);
    bool isAxe(int itemId);
    bool isSword(int itemId);
    bool isHoe(int itemId);
}