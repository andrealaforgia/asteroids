# Asteroids Restructuring - Execution Plan

**Goal:** Reorganize the codebase to separate reusable 2D engine from game-specific code
**Based On:** FOLDER_STRUCTURE_IMPROVEMENTS.md
**Estimated Total Time:** 20-30 hours
**Date:** 2025-12-28

---

## Overview

This plan transforms the flat 57-file structure into a hierarchical engine/game organization:
- **Engine:** Reusable 2D graphics/audio/input/math library
- **Game:** Asteroids-specific logic built on the engine
- **Benefits:** Reusability, maintainability, professional structure

---

## Prerequisites

### Before Starting

1. **Current state verification:**
   ```bash
   make clean && make
   ./asteroids --help
   # Verify everything works
   ```

2. **Create backup:**
   ```bash
   git add -A
   git commit -m "Backup before folder restructuring"
   git tag pre-restructure
   git push --tags
   ```

3. **Review current files:**
   ```bash
   ls -1 *.c *.h | wc -l  # Count files
   find . -name "*.c" -o -name "*.h" | sort
   ```

---

## Phase 1: Create Directory Structure

**Time Estimate:** 30 minutes
**Risk Level:** Low (no code changes)

### Steps

1. **Create all directories:**
   ```bash
   # Engine directories
   mkdir -p engine/core/{graphics,audio,input,math,utils}
   mkdir -p engine/include

   # Game directories
   mkdir -p game/src/{entities,stages,systems,main}
   mkdir -p game/include/{entities,stages,systems,main}
   mkdir -p game/assets/{sounds,sprites,config}

   # Supporting directories
   mkdir -p build/{debug,release}
   mkdir -p docs/{engine,game}
   mkdir -p tests/{engine,game}
   mkdir -p tools/scripts
   ```

2. **Verify structure:**
   ```bash
   tree -d -L 3
   # Should show new directory hierarchy
   ```

3. **Commit directory structure:**
   ```bash
   # Create .gitkeep files to preserve empty directories
   find engine game build docs tests tools -type d -exec touch {}/.gitkeep \;
   git add -A
   git commit -m "Create new directory structure for engine/game separation"
   ```

---

## Phase 2: Move Assets First

**Time Estimate:** 15 minutes
**Risk Level:** Low
**Why First:** Assets don't affect compilation

### Steps

1. **Move sounds directory:**
   ```bash
   git mv sounds game/assets/sounds
   ```

2. **Update audio.c references:**
   - Already using `SDL_GetBasePath()` from previous improvements
   - Change sound file paths from `sounds/` to `game/assets/sounds/`

   ```c
   // In audio.c, update:
   #define BANG_LARGE_WAV "game/assets/sounds/bangLarge.wav"
   // etc...
   ```

3. **Test:**
   ```bash
   make clean && make
   ./asteroids
   # Verify sounds still work
   ```

4. **Commit:**
   ```bash
   git add -A
   git commit -m "Move assets to game/assets/sounds directory"
   ```

---

## Phase 3: Move Engine Files - Graphics Subsystem

**Time Estimate:** 1 hour
**Risk Level:** Medium (requires include path updates)

### Files to Move

```bash
# Graphics core
git mv graphics.c engine/core/graphics/
git mv graphics.h engine/core/graphics/
git mv frame.c engine/core/graphics/
git mv frame.h engine/core/graphics/
git mv text.c engine/core/graphics/
git mv text.h engine/core/graphics/
git mv color.h engine/core/graphics/
git mv logger.h engine/core/graphics/  # Used heavily in graphics
```

### Update Includes

**In each moved file, update internal includes:**

```c
// Before:
#include "frame.h"

// After:
#include "graphics/frame.h"
```

**In files that USE graphics (game files), temporarily use:**

```c
// Temporary during migration:
#include "../engine/core/graphics/graphics.h"
```

### Update Makefile

Add engine graphics sources to compilation:

```makefile
# Add to SRCS:
GRAPHICS_SRCS = engine/core/graphics/graphics.c \
                engine/core/graphics/frame.c \
                engine/core/graphics/text.c

SRCS += $(GRAPHICS_SRCS)

# Add include path:
INCLUDES = -I. -Iengine/core
```

### Test

```bash
make clean && make
./asteroids
# Verify graphics work
```

### Commit

```bash
git add -A
git commit -m "Move graphics subsystem to engine/core/graphics"
```

---

## Phase 4: Move Engine Files - Math Subsystem

**Time Estimate:** 45 minutes
**Risk Level:** Medium

### Files to Move

```bash
git mv geometry.c engine/core/math/
git mv geometry.h engine/core/math/
git mv physics.c engine/core/math/
git mv physics.h engine/core/math/
git mv animate.c engine/core/math/
git mv animate.h engine/core/math/
git mv coords.h engine/core/math/
```

### Update Includes

Update internal cross-references in moved files:

```c
// In physics.c:
#include "math/geometry.h"
#include "math/coords.h"
```

Update game files:

```c
// Temporary:
#include "../engine/core/math/geometry.h"
```

### Update Makefile

```makefile
MATH_SRCS = engine/core/math/geometry.c \
            engine/core/math/physics.c \
            engine/core/math/animate.c

SRCS += $(MATH_SRCS)
```

### Test & Commit

```bash
make clean && make && ./asteroids
git add -A
git commit -m "Move math subsystem to engine/core/math"
```

---

## Phase 5: Move Engine Files - Input Subsystem

**Time Estimate:** 30 minutes
**Risk Level:** Low

### Files to Move

```bash
git mv keyboard.c engine/core/input/
git mv keyboard.h engine/core/input/
git mv events.c engine/core/input/
git mv events.h engine/core/input/
```

### Update & Test

Same pattern as above:
1. Update internal includes
2. Update game file includes (temporary)
3. Update Makefile
4. Test compilation and runtime
5. Commit

```bash
git add -A
git commit -m "Move input subsystem to engine/core/input"
```

---

## Phase 6: Move Engine Files - Audio Subsystem

**Time Estimate:** 1 hour
**Risk Level:** Medium (needs path refactoring)

### Files to Move

```bash
git mv audio.c engine/core/audio/
git mv audio.h engine/core/audio/
```

### Refactor Audio Paths

Already done in previous improvements! Audio now uses `SDL_GetBasePath()` and constructs paths dynamically. Just need to update the sound file name constants:

```c
// In engine/core/audio/audio.c
#define BANG_LARGE_WAV "game/assets/sounds/bangLarge.wav"
// etc...
```

### Test & Commit

```bash
make clean && make && ./asteroids
git add -A
git commit -m "Move audio subsystem to engine/core/audio"
```

---

## Phase 7: Move Engine Files - Utils Subsystem

**Time Estimate:** 30 minutes
**Risk Level:** Low

### Files to Move

```bash
git mv clock.c engine/core/utils/
git mv clock.h engine/core/utils/
git mv fps_tracker.c engine/core/utils/
git mv fps_tracker.h engine/core/utils/
git mv command_line.c engine/core/utils/
git mv command_line.h engine/core/utils/
git mv window_mode.h engine/core/utils/
git mv inline.h engine/core/utils/
```

### Note on logger.h

Already moved to graphics in Phase 3 (it's heavily used there), but could also be in utils. Choose one location.

### Test & Commit

```bash
make clean && make && ./asteroids
git add -A
git commit -m "Move utils subsystem to engine/core/utils"
```

---

## Phase 8: Move Game Files - Entities

**Time Estimate:** 1 hour
**Risk Level:** Medium

### Files to Move

For each entity, move .c to src and .h to include:

```bash
# Ship
git mv ship.c game/src/entities/
git mv ship.h game/include/entities/

# Asteroid
git mv asteroid.c game/src/entities/
git mv asteroid.h game/include/entities/

# Bullet
git mv bullet.c game/src/entities/
git mv bullet.h game/include/entities/

# Saucer
git mv saucer.c game/src/entities/
git mv saucer.h game/include/entities/

# Sharpnel
git mv sharpnel.c game/src/entities/
git mv sharpnel.h game/include/entities/
```

### Update Includes

In entity .c files:

```c
// Include own header:
#include "entities/ship.h"

// Include other entities:
#include "entities/bullet.h"

// Include engine (temporary paths):
#include "../../engine/core/graphics/graphics.h"
```

### Update Makefile

```makefile
ENTITY_SRCS = game/src/entities/ship.c \
              game/src/entities/asteroid.c \
              game/src/entities/bullet.c \
              game/src/entities/saucer.c \
              game/src/entities/sharpnel.c

SRCS += $(ENTITY_SRCS)

INCLUDES += -Igame/include
```

### Test & Commit

```bash
make clean && make && ./asteroids
git add -A
git commit -m "Move game entities to game/src/entities"
```

---

## Phase 9: Move Game Files - Stages

**Time Estimate:** 45 minutes
**Risk Level:** Medium

### Files to Move

```bash
git mv intro_stage.c game/src/stages/
git mv intro_stage.h game/include/stages/

git mv playing_stage.c game/src/stages/
git mv playing_stage.h game/include/stages/

git mv game_over_stage.c game/src/stages/
git mv game_over_stage.h game/include/stages/
```

### Update Includes

```c
#include "stages/intro_stage.h"
#include "entities/ship.h"
#include "../../engine/core/graphics/graphics.h"
```

### Test & Commit

```bash
make clean && make && ./asteroids
git add -A
git commit -m "Move game stages to game/src/stages"
```

---

## Phase 10: Move Game Files - Systems

**Time Estimate:** 1 hour
**Risk Level:** Medium (render.c needs splitting)

### Files to Move

```bash
git mv collision.c game/src/systems/
git mv collision.h game/include/systems/

git mv score.c game/src/systems/
git mv score.h game/include/systems/
```

### Handle render.c (Complex)

**Option A: Quick approach - Move as-is for now**

```bash
git mv render.c game/src/systems/
git mv render.h game/include/systems/
```

**Option B: Proper split (recommended, takes longer)**

1. Extract sprite coordinate data to JSON files
2. Create sprite loader in game
3. Move generic rendering to engine
4. Keep entity-specific rendering in game

**Recommendation:** Use Option A for initial migration, do Option B as separate refactoring later.

### Test & Commit

```bash
make clean && make && ./asteroids
git add -A
git commit -m "Move game systems to game/src/systems"
```

---

## Phase 11: Move Game Files - Main

**Time Estimate:** 30 minutes
**Risk Level:** Low

### Files to Move

```bash
git mv asteroids.c game/src/main/main.c

git mv game.c game/src/main/
git mv game.h game/include/main/

git mv game_settings.c game/src/main/
git mv game_settings.h game/include/main/
```

### Update Includes

In main.c:

```c
#include "main/game.h"
#include "main/game_settings.h"
#include "stages/intro_stage.h"
#include "../../engine/core/utils/command_line.h"
```

### Update Makefile

```makefile
MAIN_SRCS = game/src/main/main.c \
            game/src/main/game.c \
            game/src/main/game_settings.c

SRCS += $(MAIN_SRCS)
```

### Test & Commit

```bash
make clean && make && ./asteroids
git add -A
git commit -m "Move main game files to game/src/main"
```

---

## Phase 12: Create Engine Public API Headers

**Time Estimate:** 2 hours
**Risk Level:** Low (new files, no changes to existing)

### Create engine/include/engine_types.h

```c
#ifndef ENGINE_TYPES_H_
#define ENGINE_TYPES_H_

#include <stdbool.h>
#include <stdint.h>

// Forward declare commonly used types
typedef struct graphics_context graphics_context_t;
typedef struct keyboard_state keyboard_state_t;
typedef struct audio_context audio_context_t;

#endif  // ENGINE_TYPES_H_
```

### Create engine/include/engine_graphics.h

```c
#ifndef ENGINE_GRAPHICS_H_
#define ENGINE_GRAPHICS_H_

#include "engine_types.h"
#include "../core/graphics/graphics.h"
#include "../core/graphics/frame.h"
#include "../core/graphics/text.h"
#include "../core/graphics/color.h"

#endif  // ENGINE_GRAPHICS_H_
```

### Create Similar Headers

- `engine/include/engine_audio.h`
- `engine/include/engine_input.h`
- `engine/include/engine_math.h`
- `engine/include/engine_utils.h`

### Create Master Header engine/include/engine.h

```c
#ifndef ENGINE_H_
#define ENGINE_H_

// Include all engine subsystems
#include "engine_types.h"
#include "engine_graphics.h"
#include "engine_audio.h"
#include "engine_input.h"
#include "engine_math.h"
#include "engine_utils.h"

// Engine version
#define ENGINE_VERSION_MAJOR 1
#define ENGINE_VERSION_MINOR 0
#define ENGINE_VERSION_PATCH 0

#endif  // ENGINE_H_
```

### Commit

```bash
git add engine/include/
git commit -m "Create engine public API headers"
```

---

## Phase 13: Update All Include Paths

**Time Estimate:** 3-4 hours
**Risk Level:** High (affects all files, easy to break)

### Strategy

1. **Do one subsystem at a time**
2. **Compile after each change**
3. **Test game after each subsystem**

### Engine Files - Update to Use Relative Paths

In `engine/core/graphics/graphics.c`:

```c
// Old temporary includes:
#include "frame.h"
#include "text.h"

// New proper includes:
#include "graphics/frame.h"
#include "graphics/text.h"
#include "math/geometry.h"  // Cross-subsystem
#include "utils/clock.h"    // Cross-subsystem
```

### Game Files - Update to Use Public API

In `game/src/entities/ship.c`:

```c
// Old:
#include "ship.h"
#include "../../engine/core/graphics/graphics.h"

// New:
#include "entities/ship.h"
#include "engine_graphics.h"
#include "engine_math.h"
```

### Update Makefile Include Paths

```makefile
INCLUDES = -Iengine/include \
           -Iengine/core \
           -Igame/include
```

### Systematic Approach

```bash
# Update engine files first
find engine/core -name "*.c" -o -name "*.h"
# Edit each file's includes

# Test compilation
make clean && make

# Update game files
find game/src -name "*.c"
find game/include -name "*.h"
# Edit each file's includes

# Test compilation and runtime
make clean && make && ./asteroids
```

### Commit Frequently

```bash
git add -A
git commit -m "Update include paths for engine graphics subsystem"
# Repeat for each subsystem
```

---

## Phase 14: Build Engine as Static Library

**Time Estimate:** 2 hours
**Risk Level:** Medium

### Create New Makefile Structure

**Save current Makefile as Makefile.old**

**Create new Makefile:**

```makefile
# Platform detection
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    # macOS
    SDL2_FLAGS = $(shell sdl2-config --cflags)
    SDL2_LIBS = $(shell sdl2-config --libs) -lSDL2_mixer
else ifeq ($(UNAME_S),Linux)
    # Linux
    SDL2_FLAGS = $(shell pkg-config --cflags sdl2)
    SDL2_LIBS = $(shell pkg-config --libs sdl2) -lSDL2_mixer
endif

# Directories
ENGINE_SRC_DIR = engine/core
ENGINE_INC_DIR = engine/include
GAME_SRC_DIR = game/src
GAME_INC_DIR = game/include
BUILD_DIR = build/debug
OBJ_DIR = $(BUILD_DIR)/obj

# Compiler flags
CC = gcc
CFLAGS = -ggdb3 -Ofast --std=c99 -Wall -Wextra -pedantic-errors
INCLUDES = -I$(ENGINE_INC_DIR) -I$(ENGINE_SRC_DIR) -I$(GAME_INC_DIR) $(SDL2_FLAGS)

# Find all source files
ENGINE_SRCS = $(shell find $(ENGINE_SRC_DIR) -name '*.c')
GAME_SRCS = $(shell find $(GAME_SRC_DIR) -name '*.c')

# Object files
ENGINE_OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(notdir $(ENGINE_SRCS)))
GAME_OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(notdir $(GAME_SRCS)))

# Targets
TARGET = $(BUILD_DIR)/asteroids
ENGINE_LIB = $(BUILD_DIR)/libengine.a

.PHONY: all clean engine game run

all: engine game

# Build engine static library
$(ENGINE_LIB): $(ENGINE_OBJS)
	@mkdir -p $(BUILD_DIR)
	ar rcs $@ $^
	@echo "Engine library built: $@"

engine: $(ENGINE_LIB)

# Build game executable
$(TARGET): $(GAME_OBJS) $(ENGINE_LIB)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $(GAME_OBJS) -L$(BUILD_DIR) -lengine $(SDL2_LIBS) -lm
	@echo "Game built: $@"

game: $(TARGET)

# Pattern rules for compiling
$(OBJ_DIR)/%.o: $(ENGINE_SRC_DIR)/**/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

$(OBJ_DIR)/%.o: $(GAME_SRC_DIR)/**/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

# Utility targets
clean:
	rm -rf $(BUILD_DIR)/*

run: $(TARGET)
	$(TARGET)

# Install engine for reuse
install-engine: $(ENGINE_LIB)
	sudo cp $(ENGINE_LIB) /usr/local/lib/
	sudo mkdir -p /usr/local/include/asteroids-engine
	sudo cp -r $(ENGINE_INC_DIR)/* /usr/local/include/asteroids-engine/
	@echo "Engine installed to /usr/local"
```

### Test Build

```bash
make clean
make engine
# Should create build/debug/libengine.a

make game
# Should create build/debug/asteroids

make run
# Should run the game
```

### Verify Library Contents

```bash
ar -t build/debug/libengine.a
# Should list all engine object files
```

### Commit

```bash
git add Makefile
git commit -m "Build engine as static library and link game against it"
```

---

## Phase 15: Update .gitignore

**Time Estimate:** 15 minutes
**Risk Level:** Low

### Create/Update .gitignore

```gitignore
# Build outputs
build/
*.o
*.a
*.dSYM/

# Executables
asteroids
*.out

# IDE files
.vscode/
.idea/
*.swp
*.swo
*~

# OS files
.DS_Store
Thumbs.db

# Backup files
*.bak
*.old

# Compiled Python files (if any tools)
__pycache__/
*.pyc

# Log files
*.log

# Keep structure
!build/.gitkeep
!docs/.gitkeep
!tests/.gitkeep
```

### Remove Build Artifacts from Git

```bash
git rm -r --cached *.o asteroids 2>/dev/null || true
git add .gitignore
git commit -m "Update .gitignore for new build structure"
```

---

## Phase 16: Documentation

**Time Estimate:** 3-4 hours
**Risk Level:** Low

### Update README.md

Add section explaining new structure:

```markdown
## Project Structure

```
asteroids/
├── engine/              # Reusable 2D engine
│   ├── core/           # Engine implementation
│   │   ├── graphics/   # Rendering system
│   │   ├── audio/      # Sound system
│   │   ├── input/      # Input handling
│   │   ├── math/       # Math and physics
│   │   └── utils/      # Utilities
│   └── include/        # Public engine API
├── game/               # Asteroids game
│   ├── src/           # Game source code
│   ├── include/       # Game headers
│   └── assets/        # Game assets
└── build/             # Compiled outputs
```

### Create docs/engine/API.md

Document the engine's public API with examples.

### Create docs/engine/ARCHITECTURE.md

Explain engine design and subsystems.

### Create docs/game/DEVELOPMENT.md

Game-specific development notes.

### Commit

```bash
git add README.md docs/
git commit -m "Update documentation for new structure"
```

---

## Phase 17: Testing & Validation

**Time Estimate:** 2-3 hours
**Risk Level:** Low

### Test Checklist

- [ ] Game compiles without warnings
- [ ] Game runs correctly
- [ ] All features work (ship control, shooting, scoring)
- [ ] Audio works (all sound effects play)
- [ ] Graphics render correctly
- [ ] F11 fullscreen toggle works
- [ ] S key sound toggle works
- [ ] --vsync option works
- [ ] --volume option works
- [ ] --show-fps works
- [ ] --window-mode options work
- [ ] Game over screen works
- [ ] Intro screen works

### Performance Check

```bash
# Run with FPS display
./build/debug/asteroids --show-fps

# Should maintain 60 FPS or higher
```

### Memory Check (if valgrind available)

```bash
valgrind --leak-check=full ./build/debug/asteroids
# Play for a minute, then quit
# Check for memory leaks
```

### Cross-Platform Test

If you have access to Linux or another Mac:

```bash
git push
# On other machine:
git pull
make clean && make
./build/debug/asteroids
```

---

## Phase 18: Create Release Build

**Time Estimate:** 30 minutes
**Risk Level:** Low

### Add Release Target to Makefile

```makefile
RELEASE_BUILD_DIR = build/release
RELEASE_TARGET = $(RELEASE_BUILD_DIR)/asteroids
RELEASE_CFLAGS = -O3 -DNDEBUG --std=c99 -Wall -Wextra -pedantic-errors

release: CFLAGS = $(RELEASE_CFLAGS)
release: BUILD_DIR = $(RELEASE_BUILD_DIR)
release: OBJ_DIR = $(RELEASE_BUILD_DIR)/obj
release: TARGET = $(RELEASE_TARGET)
release: ENGINE_LIB = $(RELEASE_BUILD_DIR)/libengine.a
release: all
	strip $(RELEASE_TARGET)
	@echo "Release build complete: $(RELEASE_TARGET)"
```

### Build and Test

```bash
make release
ls -lh build/release/asteroids
./build/release/asteroids
```

### Commit

```bash
git add Makefile
git commit -m "Add release build target"
```

---

## Phase 19: Final Cleanup

**Time Estimate:** 1 hour
**Risk Level:** Low

### Remove Old Files

Check for any remaining files in root:

```bash
ls -1 *.c *.h 2>/dev/null
# Should be empty or only Makefile-related
```

If any remain, verify they're moved and remove:

```bash
git rm <old_file>
```

### Clean Up Tools

```bash
# Move any remaining scripts
git mv clint.sh tools/scripts/ 2>/dev/null || true
git mv hlint.sh tools/scripts/ 2>/dev/null || true
```

### Verify Directory Structure

```bash
tree -L 3 -I 'build'
# Should match proposed structure
```

### Commit

```bash
git add -A
git commit -m "Final cleanup: remove old files and organize tools"
```

---

## Phase 20: Tag and Document

**Time Estimate:** 30 minutes
**Risk Level:** Low

### Create Completion Tag

```bash
git tag -a v2.0-restructured -m "Complete engine/game restructuring"
git push origin v2.0-restructured
```

### Update CHANGELOG.md

Add section:

```markdown
## [2.0.0] - 2025-12-28 - Restructuring

### Changed
- **Major Restructuring**: Separated reusable 2D engine from game code
- Engine moved to `engine/core/` with public API in `engine/include/`
- Game moved to `game/src/` and `game/include/`
- Assets organized in `game/assets/`
- Build outputs moved to `build/debug/` and `build/release/`

### Added
- Engine static library (`libengine.a`)
- Public engine API headers
- Separate debug and release build targets
- Comprehensive documentation in `docs/`

### Technical
- Engine can now be reused in other projects
- Clean separation of concerns
- Professional project structure
- ~20 hours of restructuring work
```

### Commit

```bash
git add CHANGELOG.md
git commit -m "Update CHANGELOG for v2.0 restructuring"
git push
```

---

## Validation: Create Test Project

**Time Estimate:** 2 hours
**Optional but Recommended**

### Create Simple Test Using Engine

Create `../pong/` directory:

```c
// pong/main.c
#include "engine.h"

int main(void) {
    graphics_context_t gfx = init_graphics_context(0, 0, WINDOWED, false);

    while (true) {
        clear_frame(&gfx);

        // Draw a rectangle using engine
        draw_rect(&gfx, 100, 100, 50, 100, COLOR_WHITE);

        render_frame(&gfx);

        if (/* ESC pressed */) break;
    }

    terminate_graphics_context(&gfx);
    return 0;
}
```

### Makefile for Pong

```makefile
ASTEROIDS_DIR = ../asteroids_20251228
ENGINE_LIB = $(ASTEROIDS_DIR)/build/debug/libengine.a
ENGINE_INC = $(ASTEROIDS_DIR)/engine/include

CC = gcc
CFLAGS = -std=c99 -I$(ENGINE_INC) $(shell sdl2-config --cflags)
LDFLAGS = -L$(ASTEROIDS_DIR)/build/debug -lengine $(shell sdl2-config --libs) -lSDL2_mixer -lm

pong: main.o
	$(CC) -o $@ $< $(LDFLAGS)

main.o: main.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o pong
```

### Test

```bash
cd ../pong
make
./pong
# Should display a white rectangle
```

This validates that the engine is truly reusable!

---

## Troubleshooting Guide

### Issue: Compilation Errors After Moving Files

**Solution:**
1. Check include paths in affected file
2. Verify file actually exists at new location
3. Check Makefile has correct source directories
4. Run `make clean` and rebuild

### Issue: Undefined Reference Errors During Linking

**Solution:**
1. Verify engine library built: `ls -l build/debug/libengine.a`
2. Check Makefile links against engine: `-lengine`
3. Verify all engine sources compiled: `ar -t build/debug/libengine.a`

### Issue: Runtime Errors (Segfaults, etc.)

**Solution:**
1. Run with debugger: `gdb build/debug/asteroids`
2. Check for NULL pointers in moved code
3. Verify SDL paths correct for assets
4. Check logger.h is accessible everywhere needed

### Issue: Can't Find Assets (Sounds Missing)

**Solution:**
1. Verify `game/assets/sounds/` contains WAV files
2. Check audio.c uses correct paths
3. Run from project root, not build/ directory
4. Check `SDL_GetBasePath()` returns correct path

---

## Success Criteria

### ✅ Restructuring Complete When:

1. [ ] All files moved to new locations
2. [ ] Game compiles without errors or warnings
3. [ ] Game runs with all features working
4. [ ] Engine builds as static library
5. [ ] All include paths updated
6. [ ] Documentation updated
7. [ ] Build system working (debug and release)
8. [ ] .gitignore updated
9. [ ] All tests pass
10. [ ] Git history clean (good commit messages)
11. [ ] Tagged with version 2.0
12. [ ] CHANGELOG updated
13. [ ] (Optional) Test project using engine works

---

## Post-Restructuring Next Steps

### Immediate Follow-ups

1. **Create unit tests** in `tests/engine/` and `tests/game/`
2. **Extract sprite data** from render.c to JSON
3. **Document engine API** with usage examples
4. **Create second game** using engine (validates reusability)

### Future Enhancements

1. Add texture support (SDL_Image)
2. Create sprite loading system
3. Add particle effects
4. Implement scene management
5. Add configuration file support
6. Create level editor

---

## Time Tracking Template

Use this to track actual time spent:

| Phase | Estimated | Actual | Notes |
|-------|-----------|--------|-------|
| 1. Directory Structure | 30m | | |
| 2. Move Assets | 15m | | |
| 3. Graphics Subsystem | 1h | | |
| 4. Math Subsystem | 45m | | |
| 5. Input Subsystem | 30m | | |
| 6. Audio Subsystem | 1h | | |
| 7. Utils Subsystem | 30m | | |
| 8. Game Entities | 1h | | |
| 9. Game Stages | 45m | | |
| 10. Game Systems | 1h | | |
| 11. Game Main | 30m | | |
| 12. Engine API Headers | 2h | | |
| 13. Update Include Paths | 3-4h | | |
| 14. Build System | 2h | | |
| 15. .gitignore | 15m | | |
| 16. Documentation | 3-4h | | |
| 17. Testing | 2-3h | | |
| 18. Release Build | 30m | | |
| 19. Cleanup | 1h | | |
| 20. Tag & Document | 30m | | |
| **TOTAL** | **20-30h** | | |

---

## Conclusion

This plan provides a step-by-step path to transform your Asteroids project into a professionally structured codebase with a reusable 2D engine. Take it one phase at a time, commit frequently, and test after each change.

**Remember:**
- ✅ Backup before starting
- ✅ One subsystem at a time
- ✅ Compile and test frequently
- ✅ Commit after each successful phase
- ✅ Don't rush - quality over speed

**Good luck!** 🚀
