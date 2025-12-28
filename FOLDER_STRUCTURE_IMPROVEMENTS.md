# Folder Structure Improvements for 2D Engine Reusability

**Project:** Asteroids Clone - Refactoring for Engine Extraction
**Document Date:** 2025-12-28
**Goal:** Separate reusable 2D engine components from game-specific Asteroids code

---

## Table of Contents

1. [Current Structure Problems](#current-structure-problems)
2. [Proposed Structure](#proposed-structure)
3. [Detailed File Organization](#detailed-file-organization)
4. [Migration Plan](#migration-plan)
5. [Build System Updates](#build-system-updates)
6. [Benefits of New Structure](#benefits-of-new-structure)
7. [Future Enhancements](#future-enhancements)

---

## Current Structure Problems

### Current Flat Structure

```
asteroids_dev/
├── asteroids.c
├── animate.c, animate.h
├── asteroid.c, asteroid.h
├── audio.c, audio.h
├── bullet.c, bullet.h
├── clock.c, clock.h
├── collision.c, collision.h
├── color.h
├── ... (50+ more files in root)
├── Makefile
└── README.md
```

### Issues

1. **No Separation** - Engine and game code mixed together
2. **Discoverability** - Hard to find specific files (57 files in one directory)
3. **Reusability** - Can't easily extract engine for other projects
4. **Organization** - No logical grouping of related functionality
5. **Asset Management** - No clear location for assets vs. code
6. **Testing** - No dedicated space for tests
7. **Dependencies** - Unclear which files depend on what

---

## Proposed Structure

### New Hierarchical Organization

```
asteroids_project/
│
├── engine/                      # REUSABLE 2D ENGINE
│   ├── core/                    # Core engine systems
│   │   ├── graphics/
│   │   ├── audio/
│   │   ├── input/
│   │   ├── math/
│   │   └── utils/
│   └── include/                 # Public engine headers
│
├── game/                        # ASTEROIDS GAME
│   ├── src/                     # Game source code
│   │   ├── entities/
│   │   ├── stages/
│   │   ├── systems/
│   │   └── main/
│   ├── include/                 # Game headers
│   └── assets/                  # Game assets
│       ├── sounds/
│       ├── sprites/
│       └── config/
│
├── build/                       # Build outputs
│   ├── debug/
│   └── release/
│
├── docs/                        # Documentation
│   ├── engine/
│   └── game/
│
├── tests/                       # Test suites
│   ├── engine/
│   └── game/
│
├── tools/                       # Development tools
│   └── scripts/
│
├── Makefile                     # Updated build system
├── README.md
├── LICENSE
└── .gitignore
```

---

## Detailed File Organization

### 1. Engine Directory (`engine/`)

#### `engine/core/graphics/`

**Purpose:** All graphics and rendering functionality

```
engine/core/graphics/
├── graphics.c, graphics.h        # SDL2 window/renderer management
├── primitives.c, primitives.h    # Drawing primitives (lines, circles, etc.)
├── frame.c, frame.h              # Frame buffer management
├── text.c, text.h                # Text rendering system
├── color.c, color.h              # Color definitions and utilities
└── renderer.c, renderer.h        # Generic rendering interface
```

**Files to move:**
- `graphics.c, graphics.h` → `engine/core/graphics/`
- `frame.c, frame.h` → `engine/core/graphics/`
- `text.c, text.h` → `engine/core/graphics/`
- `color.h` → `engine/core/graphics/`

**New files to create:**
- `primitives.c, primitives.h` - Extract primitive drawing from graphics.c
- `renderer.c, renderer.h` - Abstract rendering interface

#### `engine/core/audio/`

**Purpose:** Audio playback and management

```
engine/core/audio/
├── audio.c, audio.h              # SDL_mixer wrapper (refactored)
├── sound.c, sound.h              # Sound effect management
└── audio_manager.c, audio_manager.h  # Resource management
```

**Files to move:**
- `audio.c, audio.h` → `engine/core/audio/` (needs refactoring)

**Refactoring needed:**
- Remove hardcoded Asteroids sound paths
- Make sound loading generic
- Support sound registration system

#### `engine/core/input/`

**Purpose:** Input handling (keyboard, mouse, gamepad)

```
engine/core/input/
├── keyboard.c, keyboard.h        # Keyboard state and input
├── events.c, events.h            # SDL event polling
└── input_manager.c, input_manager.h  # Unified input abstraction
```

**Files to move:**
- `keyboard.c, keyboard.h` → `engine/core/input/`
- `events.c, events.h` → `engine/core/input/`

**New files to create:**
- `input_manager.c, input_manager.h` - Abstraction over input devices

#### `engine/core/math/`

**Purpose:** Math and physics primitives

```
engine/core/math/
├── geometry.c, geometry.h        # Point, vector, distance calculations
├── physics.c, physics.h          # Velocity, motion
├── animate.c, animate.h          # Animation and movement
└── coords.h                      # Coordinate structures
```

**Files to move:**
- `geometry.c, geometry.h` → `engine/core/math/`
- `physics.c, physics.h` → `engine/core/math/`
- `animate.c, animate.h` → `engine/core/math/`
- `coords.h` → `engine/core/math/`

#### `engine/core/utils/`

**Purpose:** Utility functions and helpers

```
engine/core/utils/
├── clock.c, clock.h              # Timing utilities
├── fps_tracker.c, fps_tracker.h  # FPS tracking
├── command_line.c, command_line.h  # CLI argument parsing
├── inline.h                      # Compiler macros
└── logger.c, logger.h            # Logging system (new)
```

**Files to move:**
- `clock.c, clock.h` → `engine/core/utils/`
- `fps_tracker.c, fps_tracker.h` → `engine/core/utils/`
- `command_line.c, command_line.h` → `engine/core/utils/`
- `inline.h` → `engine/core/utils/`

**New files to create:**
- `logger.c, logger.h` - Centralized logging

#### `engine/include/`

**Purpose:** Public engine API headers

```
engine/include/
├── engine.h                      # Main engine header (includes all)
├── engine_graphics.h             # Graphics API
├── engine_audio.h                # Audio API
├── engine_input.h                # Input API
├── engine_math.h                 # Math API
└── engine_types.h                # Common engine types
```

**New files to create:**
- All files in this directory are new
- They re-export public APIs from core modules
- Provide clean, documented engine interface

---

### 2. Game Directory (`game/`)

#### `game/src/entities/`

**Purpose:** Game entity definitions

```
game/src/entities/
├── ship.c                        # Player ship
├── asteroid.c                    # Asteroid entity
├── bullet.c                      # Bullet projectile
├── saucer.c                      # Enemy UFO
├── sharpnel.c                    # Explosion debris
└── entity_common.h               # Shared entity utilities
```

**Files to move:**
- `ship.c, ship.h` → `game/src/entities/` + `game/include/`
- `asteroid.c, asteroid.h` → `game/src/entities/` + `game/include/`
- `bullet.c, bullet.h` → `game/src/entities/` + `game/include/`
- `saucer.c, saucer.h` → `game/src/entities/` + `game/include/`
- `sharpnel.c, sharpnel.h` → `game/src/entities/` + `game/include/`

#### `game/src/stages/`

**Purpose:** Game state/stage management

```
game/src/stages/
├── intro_stage.c                 # Title/intro screen
├── playing_stage.c               # Main gameplay
├── game_over_stage.c             # Game over screen
└── stage_common.h                # Shared stage utilities
```

**Files to move:**
- `intro_stage.c, intro_stage.h` → `game/src/stages/` + `game/include/`
- `playing_stage.c, playing_stage.h` → `game/src/stages/` + `game/include/`
- `game_over_stage.c, game_over_stage.h` → `game/src/stages/` + `game/include/`

#### `game/src/systems/`

**Purpose:** Game-specific systems

```
game/src/systems/
├── collision.c                   # Collision detection
├── score.c                       # Scoring system
└── render_entities.c             # Entity rendering (refactored from render.c)
```

**Files to move:**
- `collision.c, collision.h` → `game/src/systems/` + `game/include/`
- `score.c, score.h` → `game/src/systems/` + `game/include/`

**Files to refactor:**
- `render.c, render.h` → Split into:
  - `engine/core/graphics/renderer.c` (generic rendering)
  - `game/src/systems/render_entities.c` (asteroid/ship sprite data)

#### `game/src/main/`

**Purpose:** Game initialization and main loop

```
game/src/main/
├── main.c                        # Entry point (asteroids.c renamed)
├── game.c                        # Game state management
└── game_settings.c               # Settings and configuration
```

**Files to move:**
- `asteroids.c` → `game/src/main/main.c`
- `game.c, game.h` → `game/src/main/` + `game/include/`
- `game_settings.c, game_settings.h` → `game/src/main/` + `game/include/`

#### `game/include/`

**Purpose:** Game header files

```
game/include/
├── entities/
│   ├── ship.h
│   ├── asteroid.h
│   ├── bullet.h
│   ├── saucer.h
│   └── sharpnel.h
├── stages/
│   ├── intro_stage.h
│   ├── playing_stage.h
│   └── game_over_stage.h
├── systems/
│   ├── collision.h
│   ├── score.h
│   └── render_entities.h
└── main/
    ├── game.h
    └── game_settings.h
```

**Files to move:**
- All `.h` files move from src to corresponding include directories

#### `game/assets/`

**Purpose:** Game assets (currently missing directory)

```
game/assets/
├── sounds/                       # Audio files
│   ├── bangLarge.wav
│   ├── bangMedium.wav
│   ├── bangSmall.wav
│   ├── beat1.wav
│   ├── beat2.wav
│   ├── extraShip.wav
│   ├── fire.wav
│   ├── saucerBig.wav
│   ├── saucerSmall.wav
│   ├── thrust.wav
│   └── gameOver.wav
│
├── sprites/                      # Sprite data (new)
│   ├── asteroid_types.json       # Asteroid coordinate data
│   ├── ship_rotations.json       # Ship sprite data
│   └── saucer.json               # Saucer sprite data
│
└── config/                       # Configuration files (new)
    ├── game_config.ini           # Game settings
    ├── controls.ini              # Key bindings
    └── audio_config.ini          # Sound volume, etc.
```

**Files to move:**
- `sounds/` directory → `game/assets/sounds/`

**New files to create:**
- JSON files for sprite data (extract from render.c)
- Configuration files for game settings

---

### 3. Supporting Directories

#### `build/`

**Purpose:** Compiled outputs

```
build/
├── debug/                        # Debug builds
│   ├── asteroids                 # Debug executable
│   ├── engine.a                  # Engine static library
│   └── *.o                       # Object files
│
└── release/                      # Release builds
    ├── asteroids                 # Optimized executable
    └── engine.a                  # Engine static library
```

**Benefits:**
- Keeps source tree clean
- Separate debug/release builds
- Engine as reusable library

#### `docs/`

**Purpose:** Documentation

```
docs/
├── engine/
│   ├── API.md                    # Engine API reference
│   ├── ARCHITECTURE.md           # Engine architecture
│   └── EXAMPLES.md               # Usage examples
│
└── game/
    ├── DESIGN.md                 # Game design document
    └── DEVELOPMENT.md            # Development notes
```

**New files to create:**
- All documentation files

#### `tests/`

**Purpose:** Test suites

```
tests/
├── engine/
│   ├── test_geometry.c           # Math tests
│   ├── test_physics.c            # Physics tests
│   └── test_audio.c              # Audio tests
│
└── game/
    ├── test_collision.c          # Collision tests
    └── test_score.c              # Scoring tests
```

**New files to create:**
- Test infrastructure
- Unit tests for critical systems

#### `tools/`

**Purpose:** Development utilities

```
tools/
└── scripts/
    ├── format_code.sh            # Code formatting
    ├── run_tests.sh              # Test runner
    └── build_release.sh          # Release build script
```

**Files to move:**
- `clint.sh` → `tools/scripts/lint_c.sh`
- `hlint.sh` → `tools/scripts/lint_headers.sh`
- `train0.sh, train1.sh` → `tools/scripts/` (if still needed)

---

## Migration Plan

### Phase 1: Prepare (No Code Changes)

**Estimated Time:** 1 hour

1. **Create new directory structure**
   ```bash
   mkdir -p engine/core/{graphics,audio,input,math,utils}
   mkdir -p engine/include
   mkdir -p game/{src/{entities,stages,systems,main},include,assets/{sounds,sprites,config}}
   mkdir -p build/{debug,release}
   mkdir -p docs/{engine,game}
   mkdir -p tests/{engine,game}
   mkdir -p tools/scripts
   ```

2. **Back up current state**
   ```bash
   git add -A
   git commit -m "Backup before restructuring"
   git tag pre-restructure
   ```

3. **Move assets**
   ```bash
   mv sounds/ game/assets/sounds/
   ```

### Phase 2: Move Engine Files

**Estimated Time:** 2-3 hours

1. **Graphics subsystem**
   ```bash
   git mv graphics.{c,h} engine/core/graphics/
   git mv frame.{c,h} engine/core/graphics/
   git mv text.{c,h} engine/core/graphics/
   git mv color.h engine/core/graphics/
   ```

2. **Audio subsystem** (requires refactoring)
   ```bash
   git mv audio.{c,h} engine/core/audio/
   # Edit audio.c to remove hardcoded paths
   # Create audio_manager.c for resource loading
   ```

3. **Input subsystem**
   ```bash
   git mv keyboard.{c,h} engine/core/input/
   git mv events.{c,h} engine/core/input/
   ```

4. **Math subsystem**
   ```bash
   git mv geometry.{c,h} engine/core/math/
   git mv physics.{c,h} engine/core/math/
   git mv animate.{c,h} engine/core/math/
   git mv coords.h engine/core/math/
   ```

5. **Utils subsystem**
   ```bash
   git mv clock.{c,h} engine/core/utils/
   git mv fps_tracker.{c,h} engine/core/utils/
   git mv command_line.{c,h} engine/core/utils/
   git mv inline.h engine/core/utils/
   ```

### Phase 3: Move Game Files

**Estimated Time:** 2 hours

1. **Entities**
   ```bash
   git mv ship.{c,h} game/src/entities/ && mv game/src/entities/ship.h game/include/entities/
   git mv asteroid.{c,h} game/src/entities/ && mv game/src/entities/asteroid.h game/include/entities/
   git mv bullet.{c,h} game/src/entities/ && mv game/src/entities/bullet.h game/include/entities/
   git mv saucer.{c,h} game/src/entities/ && mv game/src/entities/saucer.h game/include/entities/
   git mv sharpnel.{c,h} game/src/entities/ && mv game/src/entities/sharpnel.h game/include/entities/
   ```

2. **Stages**
   ```bash
   git mv intro_stage.{c,h} game/src/stages/ && mv game/src/stages/intro_stage.h game/include/stages/
   git mv playing_stage.{c,h} game/src/stages/ && mv game/src/stages/playing_stage.h game/include/stages/
   git mv game_over_stage.{c,h} game/src/stages/ && mv game/src/stages/game_over_stage.h game/include/stages/
   ```

3. **Systems**
   ```bash
   git mv collision.{c,h} game/src/systems/ && mv game/src/systems/collision.h game/include/systems/
   git mv score.{c,h} game/src/systems/ && mv game/src/systems/score.h game/include/systems/
   ```

4. **Main**
   ```bash
   git mv asteroids.c game/src/main/main.c
   git mv game.{c,h} game/src/main/ && mv game/src/main/game.h game/include/main/
   git mv game_settings.{c,h} game/src/main/ && mv game/src/main/game_settings.h game/include/main/
   ```

### Phase 4: Refactor render.c

**Estimated Time:** 2-3 hours

**Current render.c has mixed concerns:**
- Generic rendering utilities → Engine
- Asteroids sprite data → Game

**Steps:**

1. **Extract sprite data to JSON**
   ```bash
   # Create game/assets/sprites/asteroid_types.json with OBJECT_COORDS data
   # Create game/assets/sprites/ship_rotations.json with ship coordinate data
   # Create game/assets/sprites/saucer.json with saucer data
   ```

2. **Split render.c**
   ```bash
   # Create engine/core/graphics/renderer.c with generic object rendering
   # Create game/src/systems/render_entities.c with entity-specific rendering
   ```

3. **Create sprite loader**
   ```bash
   # Create game/src/systems/sprite_loader.c to load JSON sprite data
   ```

### Phase 5: Update Build System

**Estimated Time:** 2-3 hours

**New Makefile structure:**

```makefile
# Platform detection (same as before)
UNAME_S := $(shell uname -s)
# ...

# Directories
ENGINE_SRC_DIR = engine/core
ENGINE_INC_DIR = engine/include
GAME_SRC_DIR = game/src
GAME_INC_DIR = game/include
BUILD_DIR = build/debug
RELEASE_DIR = build/release

# Engine sources (all .c files in engine/core subdirectories)
ENGINE_SRCS = $(shell find $(ENGINE_SRC_DIR) -name '*.c')
ENGINE_OBJS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(notdir $(ENGINE_SRCS)))

# Game sources
GAME_SRCS = $(shell find $(GAME_SRC_DIR) -name '*.c')
GAME_OBJS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(notdir $(GAME_SRCS)))

# Include paths
INCLUDES = -I$(ENGINE_INC_DIR) -I$(ENGINE_SRC_DIR) -I$(GAME_INC_DIR)

# Compiler flags
CFLAGS := -ggdb3 -Ofast --std=c99 -Wall -Wextra -pedantic-errors $(INCLUDES) $(SDL2_FLAGS)

# Targets
.PHONY: all engine game clean

all: format engine game

# Build engine as static library
$(BUILD_DIR)/libengine.a: $(ENGINE_OBJS)
	ar rcs $@ $^

engine: $(BUILD_DIR)/libengine.a

# Build game executable
$(BUILD_DIR)/asteroids: $(GAME_OBJS) $(BUILD_DIR)/libengine.a
	$(CC) $(CFLAGS) -o $@ $(GAME_OBJS) -L$(BUILD_DIR) -lengine

game: $(BUILD_DIR)/asteroids

# Compilation rules with proper include paths
$(BUILD_DIR)/%.o: $(ENGINE_SRC_DIR)/**/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: $(GAME_SRC_DIR)/**/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BUILD_DIR)/* $(RELEASE_DIR)/*

format:
	clang-format -i -style=Google $(shell find engine game -name '*.[ch]')

lint:
	cpplint $(shell find engine game -name '*.c')
```

### Phase 6: Update Include Paths

**Estimated Time:** 2-4 hours

**All files need include path updates:**

**Before:**
```c
#include "graphics.h"
#include "ship.h"
```

**After (in game files):**
```c
#include "engine_graphics.h"
#include "entities/ship.h"
```

**After (in engine files):**
```c
#include "graphics/graphics.h"
#include "math/geometry.h"
```

**Systematic approach:**
1. Update all `#include` statements file by file
2. Use find/replace in IDE or sed
3. Compile frequently to catch errors
4. Test after each subsystem is updated

### Phase 7: Create Engine API

**Estimated Time:** 3-4 hours

**Create public engine headers:**

**`engine/include/engine.h`**
```c
#ifndef ENGINE_H_
#define ENGINE_H_

// Include all engine subsystems
#include "engine_graphics.h"
#include "engine_audio.h"
#include "engine_input.h"
#include "engine_math.h"
#include "engine_types.h"

#endif  // ENGINE_H_
```

**`engine/include/engine_graphics.h`**
```c
#ifndef ENGINE_GRAPHICS_H_
#define ENGINE_GRAPHICS_H_

// Re-export public graphics APIs
#include "graphics/graphics.h"
#include "graphics/primitives.h"
#include "graphics/text.h"
#include "graphics/color.h"

#endif  // ENGINE_GRAPHICS_H_
```

**Similar headers for other subsystems...**

### Phase 8: Testing and Validation

**Estimated Time:** 2-3 hours

1. **Compile and test**
   ```bash
   make clean
   make all
   ./build/debug/asteroids
   ```

2. **Verify functionality**
   - Game runs correctly
   - All features work
   - No regressions
   - Performance unchanged

3. **Check build artifacts**
   ```bash
   ls -lh build/debug/
   # Should see:
   # - libengine.a (engine static library)
   # - asteroids (game executable)
   # - *.o (object files)
   ```

4. **Test engine reusability**
   - Create simple test program using only engine
   - Verify engine has no game dependencies
   - Check that linking works

### Phase 9: Documentation

**Estimated Time:** 3-4 hours

1. **Update README.md** with new structure
2. **Create docs/engine/API.md** - Engine API reference
3. **Create docs/engine/ARCHITECTURE.md** - Engine design
4. **Create docs/game/DESIGN.md** - Game design notes
5. **Add inline code documentation**

### Phase 10: Final Cleanup

**Estimated Time:** 1-2 hours

1. **Remove build artifacts** from git
   ```bash
   git rm *.o asteroids
   ```

2. **Update .gitignore**
   ```
   # Build outputs
   build/
   *.o
   *.a

   # IDE files
   .vscode/
   .idea/

   # OS files
   .DS_Store
   ```

3. **Commit restructured code**
   ```bash
   git add -A
   git commit -m "Refactor: Separate engine from game code"
   git tag v2.0-restructured
   ```

---

## Build System Updates

### Makefile Enhancements

**New features to add:**

1. **Debug and Release Builds**
   ```makefile
   debug: CFLAGS += -ggdb3 -DDEBUG
   debug: $(BUILD_DIR)/asteroids

   release: CFLAGS += -O3 -DNDEBUG
   release: $(RELEASE_DIR)/asteroids
   ```

2. **Engine as Library**
   ```makefile
   install-engine:
       cp $(BUILD_DIR)/libengine.a /usr/local/lib/
       cp -r engine/include/* /usr/local/include/engine/
   ```

3. **Dependency Tracking**
   ```makefile
   DEPS = $(ENGINE_OBJS:.o=.d) $(GAME_OBJS:.o=.d)
   -include $(DEPS)

   %.d: %.c
       $(CC) -M $(CFLAGS) $< > $@
   ```

4. **Test Target**
   ```makefile
   test:
       $(MAKE) -C tests all
       ./tests/run_all_tests
   ```

### CMake Alternative (Optional)

**For more complex builds, consider CMake:**

**`CMakeLists.txt`**
```cmake
cmake_minimum_required(VERSION 3.10)
project(Asteroids C)

set(CMAKE_C_STANDARD 99)

# Find SDL2
find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)

# Engine library
file(GLOB_RECURSE ENGINE_SOURCES engine/core/**/*.c)
add_library(engine STATIC ${ENGINE_SOURCES})
target_include_directories(engine PUBLIC engine/include engine/core)
target_link_libraries(engine SDL2::SDL2 SDL2_mixer::SDL2_mixer)

# Game executable
file(GLOB_RECURSE GAME_SOURCES game/src/**/*.c)
add_executable(asteroids ${GAME_SOURCES})
target_include_directories(asteroids PRIVATE game/include)
target_link_libraries(asteroids engine)

# Install
install(TARGETS asteroids DESTINATION bin)
install(TARGETS engine DESTINATION lib)
install(DIRECTORY engine/include/ DESTINATION include/engine)
```

---

## Benefits of New Structure

### 1. **Clear Separation**
- ✅ Engine code isolated from game code
- ✅ Easy to identify what's reusable
- ✅ Prevents accidental game-engine coupling

### 2. **Reusability**
- ✅ Engine can be used in other projects
- ✅ Compile engine as static library
- ✅ Clear public API via `engine/include/`

### 3. **Maintainability**
- ✅ Related files grouped together
- ✅ Logical organization by functionality
- ✅ Easier to find specific code

### 4. **Scalability**
- ✅ Easy to add new game entities
- ✅ Easy to add new engine features
- ✅ Clear places for new functionality

### 5. **Collaboration**
- ✅ Clear ownership of modules
- ✅ Reduced merge conflicts
- ✅ Easier code review

### 6. **Professional Structure**
- ✅ Matches industry standards
- ✅ Familiar to other developers
- ✅ Better portfolio presentation

### 7. **Build Optimization**
- ✅ Incremental compilation
- ✅ Separate debug/release builds
- ✅ Engine compiled once, linked many times

### 8. **Testing**
- ✅ Clear test organization
- ✅ Can test engine independently
- ✅ Unit tests isolated from integration tests

---

## Future Enhancements

### After Initial Restructure

1. **Create Second Game Using Engine**
   - Validates engine reusability
   - Examples: Breakout, Space Invaders, Pong
   - Identifies missing engine features

2. **Asset Pipeline**
   - JSON sprite loader
   - Asset packaging system
   - Resource hot-reloading for development

3. **Configuration System**
   - INI or JSON config files
   - Runtime config loading
   - Separate user settings from game code

4. **Plugin Architecture**
   - DLL/SO loading for game modules
   - Scripting support (Lua?)
   - Mod support

5. **Advanced Graphics**
   - Texture support (SDL_Image)
   - Sprite sheets and atlases
   - Particle systems
   - Lighting and shaders

6. **Entity Component System**
   - Generic entity management
   - Component-based architecture
   - Data-driven design

7. **Scene Management**
   - Scene graph
   - Camera system
   - Viewport management

8. **Serialization**
   - Save/load game state
   - Level editor support
   - Replay system

---

## Using the Engine in New Projects

### Example: Creating a Pong Game

**Project structure:**
```
pong_game/
├── Makefile
├── include/
│   ├── paddle.h
│   └── ball.h
├── src/
│   ├── main.c
│   ├── paddle.c
│   └── ball.c
└── lib/
    └── libengine.a  (copied from asteroids/build/debug/)
```

**Makefile for Pong:**
```makefile
CC = gcc
CFLAGS = -std=c99 -Wall -I../asteroids/engine/include $(SDL2_FLAGS)
LDFLAGS = -L./lib -lengine

SRCS = src/main.c src/paddle.c src/ball.c
OBJS = $(SRCS:.c=.o)

pong: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(OBJS) pong
```

**main.c:**
```c
#include "engine.h"  // Single include for entire engine!
#include "paddle.h"
#include "ball.h"

int main(void) {
    // Initialize engine graphics
    graphics_context_t graphics = init_graphics_context(0, 0);

    // Create game objects
    paddle_t paddle1 = create_paddle(/* ... */);
    ball_t ball = create_ball(/* ... */);

    // Game loop
    while (running) {
        clear_frame(&graphics);

        // Update game logic
        update_paddle(&paddle1);
        update_ball(&ball);

        // Render using engine functions
        render_paddle(&graphics, &paddle1);
        render_ball(&graphics, &ball);

        render_frame(&graphics);
    }

    terminate_graphics_context(&graphics);
    return 0;
}
```

**Benefits:**
- ✅ No SDL2 code in game
- ✅ Reuse all engine systems
- ✅ Focus on game logic only
- ✅ Rapid prototyping

---

## Migration Checklist

### Pre-Migration
- [ ] Backup current code (git commit + tag)
- [ ] Verify all tests pass (if any)
- [ ] Document current functionality
- [ ] Review all files and categorize (engine vs. game)

### Directory Creation
- [ ] Create `engine/` directory structure
- [ ] Create `game/` directory structure
- [ ] Create `build/`, `docs/`, `tests/` directories
- [ ] Create `tools/scripts/` directory

### Engine Migration
- [ ] Move graphics subsystem files
- [ ] Move audio subsystem files (refactor paths)
- [ ] Move input subsystem files
- [ ] Move math subsystem files
- [ ] Move utils subsystem files
- [ ] Create engine public API headers

### Game Migration
- [ ] Move entity files (ship, asteroid, bullet, etc.)
- [ ] Move stage files (intro, playing, game over)
- [ ] Move system files (collision, score)
- [ ] Move main files (main.c, game.c, settings.c)
- [ ] Refactor render.c into engine and game parts

### Asset Migration
- [ ] Move `sounds/` to `game/assets/sounds/`
- [ ] Create sprite JSON files
- [ ] Create configuration files

### Build System
- [ ] Update Makefile with new structure
- [ ] Add engine library build target
- [ ] Add debug/release build targets
- [ ] Update clean target
- [ ] Test incremental builds

### Code Updates
- [ ] Update all `#include` paths in engine files
- [ ] Update all `#include` paths in game files
- [ ] Refactor audio.c to remove hardcoded paths
- [ ] Extract sprite data from render.c to JSON
- [ ] Create sprite loader in game code

### Testing
- [ ] Compile engine library
- [ ] Compile game executable
- [ ] Run game and verify functionality
- [ ] Check for regressions
- [ ] Verify performance unchanged
- [ ] Test on both macOS and Linux (if applicable)

### Documentation
- [ ] Update README.md
- [ ] Create engine API documentation
- [ ] Create architecture documentation
- [ ] Add code comments
- [ ] Document migration process

### Cleanup
- [ ] Remove old build artifacts from git
- [ ] Update .gitignore
- [ ] Remove unused files
- [ ] Final commit and tag

### Validation
- [ ] Create simple test project using engine
- [ ] Verify engine has no game dependencies
- [ ] Check engine library size is reasonable
- [ ] Review code structure with fresh eyes

---

## Conclusion

This restructuring plan transforms the Asteroids project from a monolithic flat structure into a well-organized, reusable 2D engine with a cleanly separated game built on top.

### Key Outcomes

1. **Reusable Engine** - Can be used for other 2D games
2. **Better Organization** - Clear, logical file structure
3. **Professional Quality** - Industry-standard organization
4. **Maintainability** - Easy to navigate and modify
5. **Scalability** - Ready for growth and new features

### Estimated Total Time

- **Phase 1-3 (Move Files):** 4-6 hours
- **Phase 4 (Refactor render.c):** 2-3 hours
- **Phase 5 (Build System):** 2-3 hours
- **Phase 6 (Include Paths):** 2-4 hours
- **Phase 7 (Engine API):** 3-4 hours
- **Phase 8 (Testing):** 2-3 hours
- **Phase 9 (Documentation):** 3-4 hours
- **Phase 10 (Cleanup):** 1-2 hours

**Total:** ~20-30 hours for complete migration

### Recommended Approach

1. **Do it incrementally** - One subsystem at a time
2. **Test frequently** - Compile after each phase
3. **Commit often** - Small, atomic commits
4. **Document as you go** - Don't leave docs for last
5. **Get feedback** - Review with others mid-way

### Final Thought

This restructuring is an investment that will pay dividends:
- Future games will be faster to develop
- Engine improvements benefit all games
- Code quality and maintainability improve
- Portfolio demonstrates software engineering skills

**The current code is good. This restructuring makes it great.**

---

**End of Document**
