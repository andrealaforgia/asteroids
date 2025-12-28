## Install dependencies
```
make install
```

## Build the game
```
make clean && make && rm *.o
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

e.g. Running the game with no sounds, showing the frame-per-second rate, for mode 11 of display 1:
```
./asteroids --no-sound --show-fps --display=1 --display-mode=11
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
