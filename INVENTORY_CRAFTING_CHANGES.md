# Inventory Crafting Button Changes

## Overview
Changed the crafting menu access from the C key to a button in the inventory screen.

## Changes Made

### 1. Removed C Key Crafting (`source/client/gui/Gui.cpp`)
- Removed the `KM_CRAFT` key handling that opened the crafting screen
- Added comment explaining the change

### 2. Added Crafting Button to Inventory (`source/client/gui/screens/IngameBlockSelectionScreen.*`)

#### Header Changes (`IngameBlockSelectionScreen.hpp`)
- Added `Button m_btnCraft;` member variable

#### Implementation Changes (`IngameBlockSelectionScreen.cpp`)
- Added `#include "CraftingScreen.hpp"`
- Updated constructor to initialize craft button: `m_btnCraft(2, "Craft")`
- Modified `init()` method to position buttons:
  - Craft button: Top right corner (50px wide)
  - Chat button: Next to craft button with 5px gap
  - Pause button: Top left corner
- Updated `buttonClicked()` method to handle craft button clicks
- Removed touchscreen-only restrictions for better cross-platform support

## Button Layout
```
[Pause]                    [Chat] [Craft]
+------------------------------------------+
|                                          |
|            Inventory Grid                |
|                                          |
+------------------------------------------+
```

## How It Works
1. Open inventory with E key (unchanged)
2. Click the "Craft" button in the top right to access crafting
3. C key no longer opens crafting menu directly

## Benefits
- More intuitive UI flow (inventory → crafting)
- Consistent with modern Minecraft UX patterns
- Cleaner keyboard controls
- Works on all platforms (not just touchscreen)

## Testing
To test the changes:
1. Build the project: `cd build && make -j$(nproc)`
2. Run the game
3. Press E to open inventory
4. Verify C key doesn't open crafting directly
5. Click "Craft" button to open crafting menu