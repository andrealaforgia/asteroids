## Project Structure

This project is organized to separate the reusable 2D engine from game-specific code:

```
asteroids/
├── engine/core/           # Reusable 2D engine components
│   ├── graphics/         # Rendering, text, colors
│   ├── audio/            # Sound system
│   ├── input/            # Keyboard and event handling
│   ├── math/             # Geometry, physics, animation
│   └── utils/            # Clock, FPS tracking, command-line parsing
├── game/                  # Asteroids game-specific code
│   ├── src/
│   │   ├── entities/     # Ship, asteroids, bullets, saucers
│   │   ├── stages/       # Intro, playing, game over screens
│   │   ├── systems/      # Collision, scoring, rendering
│   │   └── main/         # Game initialization and main loop
│   ├── include/          # Game header files
│   └── assets/sounds/    # Audio files
└── Makefile              # Build system
```

## Install dependencies
```
make install
```

## Build the game
```
make clean && make
```

### Show run options
```
./asteroids --help
```

## Run the game
```
./asteroids
```

### Run the game with custom options
Select the display and resolution from the `graphics-info` list:
```
./asteroids --graphics-info
```

Example: Running the game with FPS display, custom volume, for mode 11 of display 1:
```
./asteroids --show-fps --volume=64 --display=1 --display-mode=11
```

### Window Modes

The game supports four different window modes:

| Mode | Value | Description |
|------|-------|-------------|
| **Windowed** | `--window-mode=0` | Normal resizable window |
| **Fullscreen** | `--window-mode=1` | True fullscreen mode (default) |
| **Borderless** | `--window-mode=2` | Borderless fullscreen window |
| **Maximized** | `--window-mode=3` | Maximized resizable window |

**Default:** Fullscreen mode (1)

**Examples:**

Windowed mode:
```
./asteroids --window-mode=0
```

Fullscreen (default):
```
./asteroids --window-mode=1
```

Borderless fullscreen (great for multi-monitor setups):
```
./asteroids --window-mode=2
```

Maximized window:
```
./asteroids --window-mode=3
```

### VSync (Vertical Synchronization)

VSync synchronizes the game's frame rendering with your display's refresh rate to eliminate screen tearing and provide smoother visuals.

**Default:** VSync is **disabled** by default for maximum performance and minimal input latency.

**Enable VSync:**
```
./asteroids --vsync
```

**When to use VSync:**
- You notice screen tearing (horizontal lines across the screen during fast movement)
- You prefer smoother, more consistent frame pacing
- Your system easily maintains 60+ FPS

**When to disable VSync (default):**
- You want the absolute lowest input latency for responsive controls
- You have a variable refresh rate display (FreeSync/G-Sync/Adaptive Sync)
- You're experiencing stuttering or performance issues with VSync enabled
- You prefer higher framerates over perfect frame synchronization

**Technical details:**
- VSync caps the framerate at your display's refresh rate (typically 60 FPS)
- Without VSync, the game uses manual frame limiting to target 60 FPS
- VSync adds ~1-2 frames of input latency but eliminates tearing
- On high-refresh-rate displays (120Hz+), VSync will cap at the higher rate

**Example - VSync with windowed mode:**
```
./asteroids --window-mode=1 --vsync
```

### FPS Display

Display real-time frames-per-second statistics in the top-left corner during gameplay.

**Default:** FPS display is **disabled** by default.

**Enable FPS display:**
```
./asteroids --show-fps
```

**Use cases:**
- Performance monitoring and debugging
- Verifying VSync is working correctly (should show 60 FPS or your display's refresh rate)
- Diagnosing performance issues or frame drops
- Comparing performance between different window modes or displays

**Example - FPS display with VSync:**
```
./asteroids --show-fps --vsync
```

### Target Frame Rate

Control the game's target frame rate (FPS). The game will attempt to maintain this frame rate through frame limiting.

**Default:** 60 FPS

**Set custom frame rate:**
```
./asteroids --fps=120   # High refresh rate displays
./asteroids --fps=30    # Lower performance systems
./asteroids --fps=144   # Gaming monitors
```

**Valid range:** 1-300 FPS

**Notes:**
- When VSync is enabled, the actual frame rate will be capped at your display's refresh rate
- Higher FPS values provide smoother animation but require more CPU/GPU power
- Lower FPS values reduce resource usage but may feel less responsive
- Most displays are 60Hz, so 60 FPS is optimal for most users

**Example - 120 FPS for high refresh rate display:**
```
./asteroids --fps=120
```

### Audio Volume

Control the game's audio volume at startup.

**Default:** Volume is set to **32** (out of 128) for comfortable listening.

**Set custom volume:**
```
./asteroids --volume=64   # Half of maximum
./asteroids --volume=0    # Mute all sound
./asteroids --volume=128  # Maximum volume
```

**Valid range:** 0-128 (0 = silent, 128 = maximum)

**Note:** You can also toggle sound on/off during gameplay by pressing the **S** key.

## Game Controls

### Gameplay
- **Up Arrow** - Thrust forward
- **Left Arrow** - Rotate left
- **Right Arrow** - Rotate right
- **Space** - Fire weapon

### System Controls
- **S** - Toggle sound on/off
- **F11** - Toggle fullscreen mode
- **ESC** - Quit game
- **Return/Enter** - Start game (from intro screen)
