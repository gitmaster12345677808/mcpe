# Tool Efficiency System

This update adds a comprehensive tool efficiency system that makes different tools have significantly different breaking speeds for different block types. This makes it actually worth crafting better tools!

## What's New

### Different Breaking Speeds
- **Pickaxes** are now much faster at breaking stone, ores, and similar blocks
- **Shovels** work better on dirt, sand, gravel, and similar blocks  
- **Axes** are more efficient at breaking wood, logs, and similar blocks
- **Wrong tools** now have a penalty when used on inappropriate blocks
- **Hand breaking** is much slower for blocks that require tools

### Tool Material Differences
- **Wooden tools**: Basic efficiency, cheap but fragile
- **Stone tools**: Better than wood, moderate durability
- **Iron tools**: Good efficiency and durability balance
- **Gold tools**: Extremely fast but very fragile
- **Diamond tools**: Best combination of speed and durability

## Configuration

### Easy Configuration Files
You can easily modify tool speeds by editing `game/tool_config.txt`. This file contains all the efficiency values for different tool and material combinations.

Example:
```
# PICKAXE EFFICIENCY (for stone, ores, etc.)
pickaxe,hand,0.3     # Very slow with hand
pickaxe,wood,2.0     # 2x speed with wooden pickaxe
pickaxe,iron,6.0     # 6x speed with iron pickaxe
pickaxe,diamond,8.0  # 8x speed with diamond pickaxe
```

### Code Configuration
For more advanced configuration, you can modify the values in `ToolConfig.cpp`:

- `PICKAXE_EFFICIENCY`: Controls pickaxe speeds on stone-type blocks
- `SHOVEL_EFFICIENCY`: Controls shovel speeds on dirt-type blocks  
- `AXE_EFFICIENCY`: Controls axe speeds on wood-type blocks
- `WRONG_TOOL_EFFICIENCY`: Penalty for using wrong tool type

## Block Categories

### Pickaxe Blocks (stone-type)
- Stone, Cobblestone, Stone Bricks
- All Ores (Coal, Iron, Gold, Diamond)
- Metal Blocks (Iron, Gold, Diamond)
- Obsidian, Bricks, Furnaces

### Shovel Blocks (dirt-type)  
- Dirt, Grass, Sand, Gravel, Clay

### Axe Blocks (wood-type)
- Tree Logs, Wood Planks
- Chests, Workbenches, Bookshelves

## Implementation Details

### Files Modified
- `ToolConfig.hpp`: Configuration system header
- `ToolConfig.cpp`: Tool efficiency implementation
- `source/world/entity/Player.hpp`: Added tool-aware destroy speed method
- `source/world/entity/Player.cpp`: Implemented tool efficiency lookup
- `source/world/tile/Tile.cpp`: Modified to use tool-aware destroy speed
- `source/client/app/NinecraftApp.cpp`: Initialize tool system
- `source/CMakeLists.txt`: Added ToolConfig to build system

### How It Works
1. When a player tries to break a block, the system checks their held item
2. It determines the correct tool type for the block being broken
3. It calculates the efficiency multiplier based on tool material and block type
4. Wrong tools get a penalty, correct tools get a bonus
5. The breaking speed is multiplied by this efficiency value

### Benefits
- **Makes tool progression meaningful**: Better tools are noticeably faster
- **Encourages proper tool usage**: Using the right tool for the job matters
- **Easily configurable**: Change speeds without recompiling code
- **Backwards compatible**: Doesn't break existing save files or gameplay

## Customization Examples

### Make diamond tools even faster:
```
pickaxe,diamond,12.0  # Change from 8.0 to 12.0
```

### Make hand breaking stone impossible:
```
pickaxe,hand,0.0  # Change from 0.3 to 0.0
```

### Make all tools equally fast:
```
pickaxe,wood,5.0
pickaxe,stone,5.0  
pickaxe,iron,5.0
pickaxe,gold,5.0
pickaxe,diamond,5.0
```

The tool efficiency system makes Minecraft PE more engaging by giving players meaningful choices about which tools to craft and use!