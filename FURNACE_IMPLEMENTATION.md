# Furnace Implementation and Crash Fix

## Overview
Fixed the game crash when opening inventory with furnace items and implemented basic furnace functionality.

## Issues Identified and Fixed

### 1. Inventory Crash Issue
**Problem**: The game crashed when opening inventory with furnace items due to missing texture mapping in `g_ItemFrames` array.

**Root Cause**: 
- Furnace (TILE_FURNACE = 61) had a value of 0 in `g_ItemFrames[61]`, meaning no texture
- This caused rendering issues when the inventory tried to display furnace items

**Fix**: Updated `g_ItemFrames` array to include furnace texture (value 59 at index 61)

### 2. Missing Furnace Tile Implementation
**Problem**: `FurnaceTile.hpp` was commented out and no furnace tile class existed.

**Solution**: Created complete furnace tile implementation

## Files Created/Modified

### New Files Created:
1. **`source/world/tile/FurnaceTile.hpp`** - Furnace tile header
2. **`source/world/tile/FurnaceTile.cpp`** - Furnace tile implementation

### Files Modified:

#### `source/client/renderer/entity/ItemRenderer.cpp`
- Fixed `g_ItemFrames` array to include furnace texture mapping
- Added texture value 59 for TILE_FURNACE at index 61
- This prevents inventory rendering crashes

#### `source/world/tile/Tile.cpp`
- Uncommented `#include "FurnaceTile.hpp"`
- Added furnace tile initialization in `initTiles()`:
  - `TILE_FURNACE` (normal furnace)
  - `TILE_FURNACE_LIT` (lit furnace with light emission)

#### `source/CMakeLists.txt`
- Added `world/tile/FurnaceTile.cpp` to build system

#### `source/world/item/Inventory.cpp`
- Added furnace to creative inventory for testing

## Furnace Features Implemented

### Basic Functionality
- **Placement**: Furnaces can be placed in the world
- **Breaking**: Furnaces drop furnace item when broken
- **Textures**: Proper front/side/top textures based on face direction
- **Lighting**: Lit furnaces emit light (0.625f emission level)
- **Material**: Stone material with appropriate hardness (3.5f destroy time)
- **Sound**: Stone breaking/placing sounds

### Advanced Features (Stub Implementation)
- **Interaction**: `use()` method exists but GUI not yet implemented
- **State Changes**: Framework for switching between lit/unlit states
- **Smelting Logic**: `tick()` method ready for future smelting implementation

## Technical Details

### Texture Mapping
- Front face: `TEXTURE_FURNACE_FRONT` (normal) / `TEXTURE_FURNACE_LIT` (when lit)
- Sides: `TEXTURE_FURNACE_SIDE`
- Top/Bottom: `TEXTURE_FURNACE_TOP`

### Tile IDs
- `TILE_FURNACE` (61): Normal furnace
- `TILE_FURNACE_LIT` (62): Lit furnace with light emission

### Material Properties
- Material: Stone
- Hardness: 3.5f (moderate breaking time)
- Sound: Stone sounds
- Light emission: 0.625f (when lit)

## Testing

To test the furnace implementation:

1. **Build the project**: `cd build && make -j$(nproc)`
2. **Open creative inventory**: Press E in-game
3. **Find furnace**: Look for furnace block in creative inventory
4. **Place furnace**: Should place without crashing
5. **Break furnace**: Should drop furnace item
6. **Inventory test**: Opening inventory with furnace items should not crash

## Future Enhancements

The basic furnace implementation provides a foundation for:

1. **Smelting System**:
   - Fuel consumption logic
   - Recipe system for smelting
   - Progress tracking

2. **GUI Interface**:
   - Furnace inventory screen (input, fuel, output slots)
   - Progress bar display
   - Button interactions

3. **Advanced Features**:
   - Particle effects for smoke
   - Sound effects for smelting
   - Redstone compatibility

4. **Multiplayer Support**:
   - Furnace state synchronization
   - Multi-player inventory management

The current implementation ensures furnaces work as basic placeable blocks and won't crash the game, providing a stable foundation for future functionality.