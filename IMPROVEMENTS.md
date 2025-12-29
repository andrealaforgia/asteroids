# Asteroids Refactoring Plan

**Created**: 2025-12-29
**Status**: Ready for Implementation
**Goal**: Transform the codebase from 7/10 to 9/10 through systematic refactoring

---

## Overview

This document provides a **granular, step-by-step plan** for refactoring the Asteroids game to implement design patterns, eliminate code smells, and improve architecture. Each task is sized for focused work sessions with clear acceptance criteria.

**Excluded**: Unit testing (per user preference)

**Key Metrics**:
- Current LOC: ~4,500
- Expected reduction: ~600 lines through deduplication
- New architecture LOC: ~300 lines (object pools, patterns)
- Net improvement: ~300 fewer lines, much better organized

---

## Phase 1: Quick Wins (1-2 days)

### 1.1 Extract Frame Limiter Utility
**Priority**: HIGH | **Effort**: 2-3 hours | **Impact**: Removes 90 lines of duplication

#### Files to Create:
- `engine/core/utils/frame_limiter.h`
- `engine/core/utils/frame_limiter.c`

#### Tasks:
1. [ ] **Create frame_limiter module**
   ```c
   // frame_limiter.h
   typedef struct {
     int target_fps;
     int last_frame_ticks;
     double fps_baseline;  // For delta_time normalization
   } frame_limiter_t;

   frame_limiter_t create_frame_limiter(int fps);
   double frame_limiter_wait(frame_limiter_t* limiter);
   ```

2. [ ] **Implement frame timing logic**
   - Extract timing calculation from `intro_stage.c:126-134`
   - Implement `frame_limiter_wait()` returning normalized delta_time
   - Add SDL_Delay(1) for CPU yielding

3. [ ] **Update intro_stage.c to use frame limiter**
   - Replace lines 126-155 with frame_limiter_wait()
   - Remove local timing variables
   - Verify intro stage still runs at correct FPS

4. [ ] **Update playing_stage.c to use frame limiter**
   - Replace lines 127-141 with frame_limiter_wait()
   - Remove local timing variables
   - Test gameplay timing unchanged

5. [ ] **Update game_over_stage.c to use frame limiter**
   - Replace lines 142-155 with frame_limiter_wait()
   - Remove local timing variables
   - Verify animations run correctly

**Acceptance Criteria**:
- ✅ All stages use frame_limiter
- ✅ No timing code duplication
- ✅ Gameplay feels identical
- ✅ ~90 lines removed

---

### 1.2 Replace Magic Numbers with Named Constants
**Priority**: HIGH | **Effort**: 2-3 hours | **Impact**: Better readability

#### Files to Create:
- `game/src/main/game_constants.h`

#### Files to Modify:
- `game/src/entities/ship.c`
- `game/src/stages/playing_stage.c`
- `game/src/gameplay/render.c`
- `game/src/gameplay/saucer_manager.c`
- `game/src/stages/game_over_stage.c`
- `game/src/stages/intro_stage.c`

#### Tasks:

1. [ ] **Create game_constants.h**
   ```c
   // Physics constants
   #define SHIP_BASE_SPEED 1.0
   #define SHIP_THRUST_ACCELERATION 0.625
   #define SHIP_ROTATION_STEPS 64

   // Timing constants
   #define SHIP_IMMUNITY_DURATION_MS 3000
   #define SHIP_THRUST_DURATION_MS 90
   #define SAUCER_CREATION_FREQUENCY_MS 30000
   #define SAUCER_BULLET_FIRE_INTERVAL_MS 3000
   #define SHIP_BULLET_MAX_AGE_MS 1000
   #define SAUCER_BULLET_MAX_AGE_MS 5000
   #define ACTION_TEXT_FLASH_INTERVAL_MS 750

   // Entity limits
   #define MAX_ASTEROID_COUNT 1000
   #define MAX_SHIP_BULLET_COUNT 20
   #define MAX_SAUCER_BULLET_COUNT 20
   #define MAX_SHARPNEL_COUNT 50

   // Rendering constants
   #define BULLET_PIXEL_RADIUS 2

   // Geometry constants
   #define SHIP_CANNON_POINT_INDEX 3

   // Speed constants
   #define SAUCER_BULLET_SPEED 10
   #define BULLET_SPEED 10
   ```

2. [ ] **Replace magic numbers in ship.c**
   - Line 56: `0.625` → `SHIP_THRUST_ACCELERATION`
   - Line 55: `1` → `SHIP_BASE_SPEED`
   - Line 73: `3` → `SHIP_CANNON_POINT_INDEX`
   - Add include for game_constants.h

3. [ ] **Replace magic numbers in playing_stage.c**
   - Line 63: `90` → `SHIP_THRUST_DURATION_MS`
   - Line 55: `3000` → `SHIP_IMMUNITY_DURATION_MS`
   - Add include for game_constants.h

4. [ ] **Replace magic numbers in collision_system.c**
   - Line 8: `3000` → `SHIP_IMMUNITY_DURATION_MS`
   - Add include for game_constants.h

5. [ ] **Replace magic numbers in render.c**
   - Lines with `-2` and `2` → `BULLET_PIXEL_RADIUS`
   - Add include for game_constants.h

6. [ ] **Replace magic numbers in bullet_manager.c**
   - Lines 12-13: Magic numbers → constants
   - Lines 16-18: Magic numbers → constants
   - Add include for game_constants.h

7. [ ] **Replace magic numbers in saucer_manager.c**
   - Lines 11-12: Magic numbers → constants
   - Add include for game_constants.h

**Acceptance Criteria**:
- ✅ No unexplained numeric literals remain
- ✅ All constants documented in one place
- ✅ Code compiles and runs identically

---

### 1.3 Fix Confusing Boolean Function Names
**Priority**: MEDIUM | **Effort**: 30 minutes | **Impact**: Better clarity

#### Files to Modify:
- `game/src/gameplay/collision_system.c`

#### Tasks:

1. [ ] **Rename ship_is_not_immune to ship_is_immune**
   ```c
   // Before
   static ALWAYS_INLINE bool ship_is_not_immune(ship_ptr ship) {
     return elapsed_from(ship->creation_ticks) > SHIP_IMMUNITY_DURATION_MS;
   }

   // After
   static ALWAYS_INLINE bool ship_is_immune(ship_ptr ship) {
     return elapsed_from(ship->creation_ticks) <= SHIP_IMMUNITY_DURATION_MS;
   }
   ```

2. [ ] **Update all call sites to use negation**
   - Line 17: `if (!ship_is_not_immune(ship))` → `if (ship_is_immune(ship))`
   - Line 54: Same pattern
   - Test collision detection still works

**Acceptance Criteria**:
- ✅ No double negatives in code
- ✅ Function name matches return value
- ✅ Immunity behavior unchanged

---

### 1.4 Remove Dead Code
**Priority**: LOW | **Effort**: 15 minutes | **Impact**: Cleaner code

#### Files to Modify:
- `game/src/entities/sharpnel.h`

#### Tasks:

1. [ ] **Remove unused active field from sharpnel_t**
   ```c
   // Before
   typedef struct {
     point_t position;
     double scale;
     int creation_ticks;
     bool active;  // UNUSED
   } sharpnel_t;

   // After
   typedef struct {
     point_t position;
     double scale;
     int creation_ticks;
   } sharpnel_t;
   ```

2. [ ] **Verify no references to sharpnel.active**
   - Grep codebase for `->active` or `.active`
   - Confirm removal doesn't break anything

**Acceptance Criteria**:
- ✅ Dead field removed
- ✅ Code compiles without warnings

---

### 1.5 Improve Code Comments
**Priority**: LOW | **Effort**: 1 hour | **Impact**: Better documentation

#### Files to Modify:
- `game/src/stages/playing_stage.c`

#### Tasks:

1. [ ] **Remove obvious comments that restate code**
   - Remove: `// Update all game objects`
   - Remove: `// Check all collisions`
   - Remove: `// Create asteroids if none are left`
   - Keep only comments explaining WHY, not WHAT

2. [ ] **Add useful comments for complex logic**
   - Add comment explaining delta_time normalization formula
   - Add comment explaining swap-and-pop removal strategy
   - Add comment explaining immunity flashing mechanism

**Acceptance Criteria**:
- ✅ No redundant comments
- ✅ Complex logic explained
- ✅ WHY documented, not WHAT

---

## Phase 2: Critical Architecture (3-5 days)

### 2.1 Implement Object Pool Pattern
**Priority**: CRITICAL | **Effort**: 1-2 days | **Impact**: Eliminates 400+ duplicate lines

#### Files to Create:
- `engine/core/memory/object_pool.h`
- `engine/core/memory/object_pool.c`

#### Tasks:

1. [ ] **Create object_pool module interface**
   ```c
   // object_pool.h
   typedef struct {
     void* objects;           // Contiguous array of objects
     size_t object_size;      // Size of each object in bytes
     size_t capacity;         // Maximum number of objects
     size_t* free_indices;    // Stack of available indices
     size_t free_count;       // Number of free slots
     bool* active_flags;      // Which slots are currently active
     size_t active_count;     // Number of active objects
   } object_pool_t;

   object_pool_t create_object_pool(size_t object_size, size_t capacity);
   void* pool_acquire(object_pool_t* pool, size_t* out_index);
   void pool_release(object_pool_t* pool, size_t index);
   void pool_reset(object_pool_t* pool);
   size_t pool_get_active_count(const object_pool_t* pool);
   void* pool_get_at(object_pool_t* pool, size_t index);
   bool pool_is_active(const object_pool_t* pool, size_t index);
   void pool_destroy(object_pool_t* pool);

   // Iteration helper
   typedef void (*pool_callback_t)(void* object, size_t index, void* user_data);
   void pool_foreach_active(object_pool_t* pool, pool_callback_t callback, void* user_data);
   ```

2. [ ] **Implement object pool**
   - Implement create: malloc arrays, initialize free list
   - Implement acquire: pop from free list, mark active
   - Implement release: mark inactive, push to free list
   - Implement iteration over active objects
   - Implement destroy: free all allocations

3. [ ] **Add object pool to Makefile**
   - Add to ENGINE_UTILS_DIR sources (or create MEMORY_DIR)
   - Update includes
   - Rebuild and verify compilation

**Acceptance Criteria**:
- ✅ Generic object pool compiles
- ✅ No game-specific code in pool
- ✅ Pool supports any object type
- ✅ All operations tested manually

---

### 2.2 Refactor Asteroid Manager to Use Object Pool
**Priority**: CRITICAL | **Effort**: 3-4 hours | **Impact**: Proves pool pattern

#### Files to Modify:
- `game/src/gameplay/asteroid_manager.h`
- `game/src/gameplay/asteroid_manager.c`

#### Tasks:

1. [ ] **Update asteroid_manager_t structure**
   ```c
   // Before
   static asteroid_t asteroids[MAX_ASTEROID_COUNT];
   static size_t asteroid_count = 0;

   // After (in struct)
   typedef struct {
     object_pool_t pool;
     // ... existing fields
   } asteroid_manager_t;
   ```

2. [ ] **Update init_asteroid_manager**
   ```c
   void init_asteroid_manager(...) {
     // ...
     manager->pool = create_object_pool(sizeof(asteroid_t), MAX_ASTEROID_COUNT);
   }
   ```

3. [ ] **Add cleanup function**
   ```c
   void destroy_asteroid_manager(asteroid_manager_ptr manager) {
     pool_destroy(&manager->pool);
   }
   ```

4. [ ] **Rewrite add_asteroid to use pool**
   ```c
   void add_asteroid(asteroid_manager_ptr manager, point_t position, int scale) {
     size_t index;
     asteroid_t* asteroid = (asteroid_t*)pool_acquire(&manager->pool, &index);
     if (!asteroid) {
       LOG_WARN("Asteroid pool exhausted");
       return;
     }
     *asteroid = create_asteroid(position, scale, random_color());
   }
   ```

5. [ ] **Rewrite remove_asteroid to use pool**
   ```c
   void remove_asteroid(asteroid_manager_ptr manager, size_t asteroid_index) {
     pool_release(&manager->pool, asteroid_index);
   }
   ```

6. [ ] **Update get_asteroid_count**
   ```c
   size_t get_asteroid_count(const asteroid_manager_ptr manager) {
     return pool_get_active_count(&manager->pool);
   }
   ```

7. [ ] **Update get_asteroid**
   ```c
   asteroid_ptr get_asteroid(asteroid_manager_ptr manager, size_t asteroid_index) {
     return (asteroid_ptr)pool_get_at(&manager->pool, asteroid_index);
   }
   ```

8. [ ] **Update iteration in update_asteroids**
   - Use pool_foreach_active or manual iteration over active objects
   - Ensure only active asteroids are updated

9. [ ] **Update playing_stage.c to call destroy_asteroid_manager**
   - Add cleanup when stage ends
   - Prevent memory leaks

**Acceptance Criteria**:
- ✅ Asteroid manager uses object pool
- ✅ No more manual array management
- ✅ Asteroids behave identically
- ✅ No memory leaks (verify with tools if available)

---

### 2.3 Refactor Bullet Manager to Use Object Pool
**Priority**: CRITICAL | **Effort**: 4-5 hours | **Impact**: Eliminates more duplication

#### Files to Modify:
- `game/src/gameplay/bullet_manager.h`
- `game/src/gameplay/bullet_manager.c`

#### Tasks:

1. [ ] **Update bullet_manager_t structure**
   ```c
   typedef struct {
     object_pool_t ship_bullet_pool;
     object_pool_t saucer_bullet_pool;
     // ... existing fields
   } bullet_manager_t;
   ```

2. [ ] **Update init_bullet_manager**
   - Initialize both pools
   - ship_bullet_pool: MAX_SHIP_BULLET_COUNT
   - saucer_bullet_pool: MAX_SAUCER_BULLET_COUNT

3. [ ] **Add destroy_bullet_manager**
   - Destroy both pools

4. [ ] **Refactor ship bullet functions to use pool**
   - add_ship_bullet → pool_acquire on ship_bullet_pool
   - remove_ship_bullet → pool_release
   - get_ship_bullet_count → pool_get_active_count
   - get_ship_bullet → pool_get_at
   - update_ship_bullets → iterate over active only

5. [ ] **Refactor saucer bullet functions to use pool**
   - Same pattern as ship bullets
   - Use saucer_bullet_pool

6. [ ] **Update playing_stage.c cleanup**
   - Call destroy_bullet_manager on exit

7. [ ] **Fix backward iteration for removal**
   - Ensure bullets can still be removed during iteration
   - Pool-based iteration may need different approach

**Acceptance Criteria**:
- ✅ Both bullet pools work correctly
- ✅ Bullets fire and age out properly
- ✅ No crashes when pools near capacity
- ✅ Memory cleaned up properly

---

### 2.4 Refactor Sharpnel to Use Object Pool
**Priority**: CRITICAL | **Effort**: 2-3 hours | **Impact**: Completes pool migration

#### Files to Modify:
- `game/src/entities/sharpnel.h`
- `game/src/entities/sharpnel.c`

#### Tasks:

1. [ ] **Create sharpnel_system_t (introduce struct)**
   ```c
   // sharpnel.h
   typedef struct {
     object_pool_t pool;
   } sharpnel_system_t;

   sharpnel_system_t* create_sharpnel_system(size_t max_count);
   void destroy_sharpnel_system(sharpnel_system_t* system);
   void reset_sharpnels(sharpnel_system_t* system);
   void add_sharpnel(sharpnel_system_t* system, point_t position);
   void animate_sharpnels(sharpnel_system_t* system, graphics_context_ptr ctx, double delta);
   ```

2. [ ] **Implement sharpnel_system**
   - create_sharpnel_system: malloc struct, create pool
   - destroy: free pool and struct
   - Migrate logic from global functions

3. [ ] **Remove global static sharpnel state**
   - Delete `sharpnel_t sharpnels[MAX_SHARPNEL_COUNT]`
   - Delete `size_t sharpnel_count`

4. [ ] **Update playing_stage.c**
   - Add `sharpnel_system_t* sharpnel_system` to static state
   - Initialize in init_playing_stage
   - Pass to add_sharpnel calls
   - Destroy on exit

5. [ ] **Update all call sites**
   - asteroid_manager: handle_ship_destruction passes system
   - collision_system: add_sharpnel passes system
   - Update function signatures

**Acceptance Criteria**:
- ✅ Sharpnel uses object pool
- ✅ No global sharpnel state
- ✅ Explosion effects work identically
- ✅ Memory managed properly

---

### 2.5 Eliminate Global Static State (Dependency Injection)
**Priority**: CRITICAL | **Effort**: 1-2 days | **Impact**: Enables testing and reusability

#### Phase 2.5.1: Remove Static State from Managers
**Effort**: 4-6 hours

#### Files to Modify:
- `game/src/gameplay/asteroid_manager.c`
- `game/src/gameplay/bullet_manager.c`
- `game/src/gameplay/saucer_manager.c`

#### Tasks:

1. [ ] **Remove static manager_instance pointers**
   - Delete `static asteroid_manager_t* manager_instance = NULL;` from asteroid_manager.c
   - All state should be in manager struct or pool (already done in 2.2-2.4)

2. [ ] **Remove unused parameter warnings**
   - Delete `(void)manager;` suppressions
   - Use manager parameter properly in all functions

3. [ ] **Verify all functions use manager parameter**
   - No function should touch static/global state
   - All data accessed through manager struct

**Acceptance Criteria**:
- ✅ No static arrays in managers
- ✅ No static instance pointers
- ✅ All state in manager structs

---

#### Phase 2.5.2: Remove Static State from Playing Stage
**Effort**: 6-8 hours

#### Files to Modify:
- `game/src/stages/playing_stage.c`
- `game/src/stages/playing_stage.h`

#### Tasks:

1. [ ] **Create playing_stage_state_t struct**
   ```c
   // playing_stage.h
   typedef struct {
     game_ptr game;
     graphics_context_ptr graphics_context;
     audio_context_ptr audio_context;

     asteroid_manager_t asteroid_manager;
     bullet_manager_t bullet_manager;
     saucer_manager_t saucer_manager;
     game_hud_t game_hud;
     sharpnel_system_t* sharpnel_system;

     ship_t ship;
   } playing_stage_state_t;
   ```

2. [ ] **Update init_playing_stage signature**
   ```c
   // Before
   void init_playing_stage(const game_ptr _game);

   // After
   playing_stage_state_t* init_playing_stage(game_ptr game);
   ```

3. [ ] **Update handle_playing_stage signature**
   ```c
   // Before
   game_stage_action_t handle_playing_stage(void);

   // After
   game_stage_action_t handle_playing_stage(playing_stage_state_t* state);
   ```

4. [ ] **Add cleanup function**
   ```c
   void cleanup_playing_stage(playing_stage_state_t* state);
   ```

5. [ ] **Remove all static variables from playing_stage.c**
   - Delete `static game_ptr game = NULL;`
   - Delete `static graphics_context_ptr graphics_context = NULL;`
   - Delete all static manager instances
   - Delete `static ship_t ship;`

6. [ ] **Pass state through all static helper functions**
   - Update all `static ALWAYS_INLINE` functions to take state parameter
   - Example: `void animate_ship(playing_stage_state_t* state, double delta_time)`

7. [ ] **Update main.c to use new API**
   ```c
   // In main.c:run_game
   case PLAYING: {
     playing_stage_state_t* state = init_playing_stage(game);
     game_stage_action_t action = handle_playing_stage(state);
     cleanup_playing_stage(state);
     // ... handle action
   }
   ```

**Acceptance Criteria**:
- ✅ No static variables in playing_stage.c
- ✅ State explicitly passed everywhere
- ✅ Can theoretically have multiple playing stages
- ✅ Cleanup prevents memory leaks

---

#### Phase 2.5.3: Remove Static State from Other Stages
**Effort**: 4-6 hours

#### Files to Modify:
- `game/src/stages/intro_stage.c`
- `game/src/stages/intro_stage.h`
- `game/src/stages/game_over_stage.c`
- `game/src/stages/game_over_stage.h`

#### Tasks:

1. [ ] **Apply same pattern to intro_stage**
   - Create intro_stage_state_t
   - Update init/handle/cleanup signatures
   - Remove static variables
   - Update main.c

2. [ ] **Apply same pattern to game_over_stage**
   - Create game_over_stage_state_t
   - Update init/handle/cleanup signatures
   - Remove static variables
   - Update main.c

**Acceptance Criteria**:
- ✅ No static state in any stage
- ✅ All stages follow same pattern
- ✅ Main.c manages stage lifecycles

---

### 2.6 Reduce Coupling with Context Struct
**Priority**: HIGH | **Effort**: 3-4 hours | **Impact**: Cleaner APIs

#### Files to Create:
- `game/src/main/game_context.h`

#### Files to Modify:
- All manager files

#### Tasks:

1. [ ] **Create game_context_t struct**
   ```c
   // game_context.h
   typedef struct {
     graphics_context_ptr graphics;
     audio_context_ptr audio;
     game_ptr game;
   } game_context_t;

   game_context_t create_game_context(game_ptr game);
   ```

2. [ ] **Update manager initialization signatures**
   ```c
   // Before
   void init_asteroid_manager(
     asteroid_manager_ptr manager,
     game_ptr game,
     graphics_context_ptr graphics_context,
     audio_context_ptr audio_context
   );

   // After
   void init_asteroid_manager(
     asteroid_manager_ptr manager,
     game_context_t* context
   );
   ```

3. [ ] **Update all manager structs to use context**
   ```c
   typedef struct {
     object_pool_t pool;
     game_context_t* context;  // Instead of 3 separate pointers
   } asteroid_manager_t;
   ```

4. [ ] **Apply to all managers**
   - asteroid_manager
   - bullet_manager
   - saucer_manager
   - game_hud

5. [ ] **Update all init calls in stage files**
   - Create game_context once
   - Pass to all manager inits

**Acceptance Criteria**:
- ✅ No more 4-parameter init functions
- ✅ Context passed consistently
- ✅ Easier to add new context fields

---

## Phase 3: Structural Improvements (2-3 days)

### 3.1 Reorganize gameplay/ Folder
**Priority**: MEDIUM | **Effort**: 2-3 hours | **Impact**: Better organization

#### Directory Changes:

```
Before:
game/src/gameplay/
├── asteroid_manager.c/h
├── bullet_manager.c/h
├── saucer_manager.c/h
├── collision_system.c/h
├── game_hud.c/h
├── game_audio.c/h
├── render.c/h
├── score.c/h
└── collision.c/h

After:
game/src/managers/
├── asteroid_manager.c/h
├── bullet_manager.c/h
└── saucer_manager.c/h

game/src/systems/
├── collision_system.c/h
└── collision.c/h

game/src/rendering/
├── sprites.c/h (renamed from render.c/h)
└── game_hud.c/h

game/src/audio/
└── game_audio.c/h

game/src/scoring/
└── score.c/h
```

#### Tasks:

1. [ ] **Create new directories**
   ```bash
   mkdir -p game/src/managers
   mkdir -p game/src/systems
   mkdir -p game/src/rendering
   mkdir -p game/src/audio
   mkdir -p game/src/scoring
   ```

2. [ ] **Move manager files**
   ```bash
   git mv game/src/gameplay/asteroid_manager.* game/src/managers/
   git mv game/src/gameplay/bullet_manager.* game/src/managers/
   git mv game/src/gameplay/saucer_manager.* game/src/managers/
   ```

3. [ ] **Move system files**
   ```bash
   git mv game/src/gameplay/collision_system.* game/src/systems/
   git mv game/src/gameplay/collision.* game/src/systems/
   ```

4. [ ] **Move rendering files**
   ```bash
   git mv game/src/gameplay/render.* game/src/rendering/sprites.*
   git mv game/src/gameplay/game_hud.* game/src/rendering/
   ```

5. [ ] **Move audio files**
   ```bash
   git mv game/src/gameplay/game_audio.* game/src/audio/
   ```

6. [ ] **Move scoring files**
   ```bash
   git mv game/src/gameplay/score.* game/src/scoring/
   ```

7. [ ] **Update Makefile**
   ```makefile
   GAME_MANAGERS_DIR = game/src/managers
   GAME_SYSTEMS_DIR = game/src/systems
   GAME_RENDERING_DIR = game/src/rendering
   GAME_AUDIO_DIR = game/src/audio
   GAME_SCORING_DIR = game/src/scoring

   SRC = ... \
         $(wildcard $(GAME_MANAGERS_DIR)/*.c) \
         $(wildcard $(GAME_SYSTEMS_DIR)/*.c) \
         $(wildcard $(GAME_RENDERING_DIR)/*.c) \
         $(wildcard $(GAME_AUDIO_DIR)/*.c) \
         $(wildcard $(GAME_SCORING_DIR)/*.c) \
         ...

   INCLUDES = ... \
              -I$(GAME_MANAGERS_DIR) \
              -I$(GAME_SYSTEMS_DIR) \
              -I$(GAME_RENDERING_DIR) \
              -I$(GAME_AUDIO_DIR) \
              -I$(GAME_SCORING_DIR)
   ```

8. [ ] **Update header guards**
   - `GAME_SRC_GAMEPLAY_*` → `GAME_SRC_MANAGERS_*` (for manager files)
   - `GAME_SRC_GAMEPLAY_*` → `GAME_SRC_SYSTEMS_*` (for system files)
   - etc.

9. [ ] **Delete empty gameplay/ directory**
   ```bash
   git rm -r game/src/gameplay
   ```

10. [ ] **Build and test**
    - make clean && make
    - Verify all includes resolved
    - Test game runs correctly

**Acceptance Criteria**:
- ✅ gameplay/ folder removed
- ✅ Files organized by purpose
- ✅ Clear separation of concerns
- ✅ Builds without errors

---

### 3.2 Extract Ship Controller Module
**Priority**: MEDIUM | **Effort**: 4-5 hours | **Impact**: Better SRP compliance

#### Files to Create:
- `game/src/controllers/ship_controller.h`
- `game/src/controllers/ship_controller.c`

#### Files to Modify:
- `game/src/stages/playing_stage.c`
- `game/src/entities/ship.h`
- `game/src/entities/ship.c`

#### Tasks:

1. [ ] **Design ship_controller interface**
   ```c
   // ship_controller.h
   typedef struct {
     ship_t* ship;
     graphics_context_ptr graphics_context;
     audio_context_ptr audio_context;
     bullet_manager_ptr bullet_manager;
   } ship_controller_t;

   ship_controller_t create_ship_controller(
     ship_t* ship,
     graphics_context_ptr graphics,
     audio_context_ptr audio,
     bullet_manager_ptr bullets
   );

   void ship_controller_update(ship_controller_t* controller, double delta_time);
   void ship_controller_handle_thrust(ship_controller_t* controller);
   void ship_controller_handle_fire(ship_controller_t* controller);
   void ship_controller_handle_rotate_left(ship_controller_t* controller);
   void ship_controller_handle_rotate_right(ship_controller_t* controller);
   bool ship_controller_is_destroyed(const ship_controller_t* controller);
   void ship_controller_respawn(ship_controller_t* controller, point_t position);
   ```

2. [ ] **Move ship functions from playing_stage.c**
   - Move `animate_ship()` → `ship_controller_update()`
   - Move `thrust_ship()` → `ship_controller_handle_thrust()`
   - Move `fire_ship_bullet()` → `ship_controller_handle_fire()`
   - Move ship rotation logic → rotate functions

3. [ ] **Add ship behavior to ship.c**
   ```c
   // ship.c
   void update_ship_animation(ship_t* ship, double delta_time);
   bool is_ship_immune(const ship_t* ship);
   void ship_start_thrust(ship_t* ship);
   point_t ship_get_cannon_position(const ship_t* ship);
   int ship_get_radius(const ship_t* ship);
   ```

4. [ ] **Update playing_stage.c to use controller**
   - Create ship_controller in init
   - Call controller functions instead of inline logic
   - Remove ship-specific code from playing_stage

5. [ ] **Update Makefile**
   - Add controllers directory
   - Add to includes and sources

**Acceptance Criteria**:
- ✅ All ship logic in ship.c or ship_controller.c
- ✅ playing_stage.c has no ship-specific code
- ✅ Ship behavior unchanged
- ✅ Cleaner separation of concerns

---

### 3.3 Implement State Pattern for Game Stages
**Priority**: MEDIUM | **Effort**: 6-8 hours | **Impact**: Cleaner stage management

#### Files to Create:
- `game/src/stages/stage.h`

#### Files to Modify:
- `game/src/stages/intro_stage.c`
- `game/src/stages/playing_stage.c`
- `game/src/stages/game_over_stage.c`
- `game/src/main/main.c`

#### Tasks:

1. [ ] **Define stage interface**
   ```c
   // stage.h
   typedef struct stage_t stage_t;

   struct stage_t {
     void* state;  // Stage-specific state

     void (*init)(stage_t* stage, game_ptr game);
     game_stage_action_t (*update)(stage_t* stage);
     void (*cleanup)(stage_t* stage);

     const char* name;  // For debugging
   };

   stage_t* create_intro_stage(void);
   stage_t* create_playing_stage(void);
   stage_t* create_game_over_stage(void);
   void destroy_stage(stage_t* stage);
   ```

2. [ ] **Refactor intro_stage to implement interface**
   ```c
   // intro_stage.c
   static void intro_init(stage_t* stage, game_ptr game) {
     intro_stage_state_t* state = (intro_stage_state_t*)stage->state;
     // ... existing init logic
   }

   static game_stage_action_t intro_update(stage_t* stage) {
     intro_stage_state_t* state = (intro_stage_state_t*)stage->state;
     // ... existing handle logic
   }

   static void intro_cleanup(stage_t* stage) {
     intro_stage_state_t* state = (intro_stage_state_t*)stage->state;
     // ... cleanup
     free(state);
   }

   stage_t* create_intro_stage(void) {
     stage_t* stage = malloc(sizeof(stage_t));
     stage->state = malloc(sizeof(intro_stage_state_t));
     stage->init = intro_init;
     stage->update = intro_update;
     stage->cleanup = intro_cleanup;
     stage->name = "INTRO";
     return stage;
   }
   ```

3. [ ] **Refactor playing_stage to implement interface**
   - Same pattern as intro_stage
   - Wrap existing functions in interface

4. [ ] **Refactor game_over_stage to implement interface**
   - Same pattern

5. [ ] **Update main.c to use polymorphic stages**
   ```c
   static void run_game(const game_ptr game) {
     stage_t* stages[3] = {
       create_intro_stage(),
       create_playing_stage(),
       create_game_over_stage()
     };

     int current_stage_index = 0;  // Start with intro
     stage_t* current_stage = stages[current_stage_index];
     current_stage->init(current_stage, game);

     while (true) {
       game_stage_action_t action = current_stage->update(current_stage);

       if (action == QUIT) {
         current_stage->cleanup(current_stage);
         break;
       }

       if (action == PROGRESS) {
         current_stage->cleanup(current_stage);
         current_stage_index = (current_stage_index + 1) % 3;
         current_stage = stages[current_stage_index];
         current_stage->init(current_stage, game);

         if (current_stage_index == 1) {  // Playing stage
           reset_game(game);
         }
       }
     }

     for (int i = 0; i < 3; i++) {
       destroy_stage(stages[i]);
     }
   }
   ```

6. [ ] **Remove switch statement from main.c**
   - Delete old while(true) { switch(game_stage) { ... } }
   - Verify new state machine works

**Acceptance Criteria**:
- ✅ No switch statement in main.c
- ✅ Stages implement common interface
- ✅ Easy to add new stages
- ✅ Clear stage lifecycle

---

### 3.4 Implement Observer Pattern for Game Events
**Priority**: MEDIUM | **Effort**: 1-2 days | **Impact**: Decouples systems

#### Files to Create:
- `engine/core/events/event_system.h`
- `engine/core/events/event_system.c`
- `game/src/events/game_events.h`

#### Tasks:

1. [ ] **Create generic event system in engine**
   ```c
   // event_system.h
   typedef enum {
     EVENT_TYPE_GAME_SPECIFIC_BASE = 1000  // Game can define from here
   } event_type_t;

   typedef struct {
     int type;
     void* data;
     size_t data_size;
   } event_t;

   typedef void (*event_callback_t)(const event_t* event, void* user_data);

   typedef struct {
     event_callback_t callbacks[32];  // Max subscribers per event
     void* user_data[32];
     size_t callback_count;
   } event_subscriber_list_t;

   typedef struct {
     event_subscriber_list_t subscribers[256];  // Max event types
   } event_system_t;

   event_system_t create_event_system(void);
   void subscribe(event_system_t* system, int event_type,
                  event_callback_t callback, void* user_data);
   void unsubscribe(event_system_t* system, int event_type,
                    event_callback_t callback);
   void publish(event_system_t* system, const event_t* event);
   void destroy_event_system(event_system_t* system);
   ```

2. [ ] **Define game-specific events**
   ```c
   // game_events.h
   typedef enum {
     GAME_EVENT_ASTEROID_DESTROYED = 1000,
     GAME_EVENT_SHIP_DESTROYED,
     GAME_EVENT_SAUCER_DESTROYED,
     GAME_EVENT_SHIP_FIRED,
     GAME_EVENT_SAUCER_FIRED,
     GAME_EVENT_LEVEL_COMPLETE
   } game_event_type_t;

   typedef struct {
     point_t position;
     int scale;
   } asteroid_destroyed_data_t;

   typedef struct {
     point_t position;
     bool is_big;
   } saucer_destroyed_data_t;

   // ... other event data structs
   ```

3. [ ] **Add event_system to game_context**
   ```c
   typedef struct {
     graphics_context_ptr graphics;
     audio_context_ptr audio;
     game_ptr game;
     event_system_t* events;  // NEW
   } game_context_t;
   ```

4. [ ] **Create audio event subscriber**
   ```c
   // In game_audio.c
   static void on_asteroid_destroyed(const event_t* event, void* user_data) {
     audio_context_ptr audio = (audio_context_ptr)user_data;
     asteroid_destroyed_data_t* data = (asteroid_destroyed_data_t*)event->data;

     switch (data->scale) {
       case LARGE_ASTEROID_SCALE:
         play_bang_large(audio);
         break;
       // ... other scales
     }
   }

   void subscribe_audio_events(event_system_t* events, audio_context_ptr audio) {
     subscribe(events, GAME_EVENT_ASTEROID_DESTROYED, on_asteroid_destroyed, audio);
     subscribe(events, GAME_EVENT_SHIP_DESTROYED, on_ship_destroyed, audio);
     // ... other events
   }
   ```

5. [ ] **Create scoring event subscriber**
   ```c
   // In score.c
   static void on_asteroid_destroyed(const event_t* event, void* user_data) {
     game_ptr game = (game_ptr)user_data;
     asteroid_destroyed_data_t* data = (asteroid_destroyed_data_t*)event->data;

     switch (data->scale) {
       case LARGE_ASTEROID_SCALE:
         score_large_asteroid(game);
         break;
       // ... other scales
     }
   }

   void subscribe_score_events(event_system_t* events, game_ptr game) {
     subscribe(events, GAME_EVENT_ASTEROID_DESTROYED, on_asteroid_destroyed, game);
     // ... other events
   }
   ```

6. [ ] **Update collision_system to publish events**
   ```c
   // In break_asteroid_apart
   void break_asteroid_apart(asteroid_manager_ptr manager, size_t asteroid_index) {
     asteroid_ptr asteroid = get_asteroid(manager, asteroid_index);

     // Publish event instead of directly calling audio/scoring
     asteroid_destroyed_data_t data = {
       .position = asteroid->position,
       .scale = asteroid->scale
     };
     event_t event = {
       .type = GAME_EVENT_ASTEROID_DESTROYED,
       .data = &data,
       .data_size = sizeof(data)
     };
     publish(manager->context->events, &event);

     // ... rest of asteroid breakup logic
   }
   ```

7. [ ] **Remove direct audio/scoring calls from collision code**
   - Delete play_bang_* calls
   - Delete score_* calls
   - Keep only event publishing

8. [ ] **Initialize event subscribers in playing_stage**
   ```c
   void init_playing_stage(...) {
     // ... create event system
     subscribe_audio_events(context->events, context->audio);
     subscribe_score_events(context->events, context->game);
   }
   ```

**Acceptance Criteria**:
- ✅ Event system works generically
- ✅ Audio responds to events
- ✅ Scoring responds to events
- ✅ Collision code doesn't know about audio/scoring
- ✅ Easy to add new event subscribers

---

## Phase 4: Polish & Optimization (1-2 days)

### 4.1 Reduce ALWAYS_INLINE Overuse
**Priority**: LOW | **Effort**: 2-3 hours | **Impact**: Trust compiler

#### Files to Modify:
- All .c files using ALWAYS_INLINE

#### Tasks:

1. [ ] **Identify hot path functions (profile if possible)**
   - Use profiler or educated guesses
   - Frame timing, collision detection, rendering likely hot

2. [ ] **Change ALWAYS_INLINE to inline for helpers**
   - Functions < 5 lines: keep as `static inline`
   - Functions > 5 lines: change to just `static`
   - Hot path confirmed: keep ALWAYS_INLINE

3. [ ] **Measure performance impact**
   - Run game before/after
   - Check FPS stability
   - If no regression, keep changes

**Acceptance Criteria**:
- ✅ ALWAYS_INLINE only on hot path
- ✅ No performance regression
- ✅ Compiler can optimize freely

---

### 4.2 Standardize Manager APIs
**Priority**: MEDIUM | **Effort**: 3-4 hours | **Impact**: Consistency

#### Files to Modify:
- All manager files

#### Tasks:

1. [ ] **Document API conventions**
   ```c
   // Convention:
   // - Creation: create_X_manager(params...)
   // - Destruction: destroy_X_manager(manager)
   // - Add entity: add_X(manager, ...)
   // - Remove entity: remove_X(manager, index)
   // - Get count: get_X_count(manager)
   // - Get entity: get_X(manager, index)
   // - Iteration: foreach_X(manager, callback, user_data)
   // - Update: update_X(manager, delta_time)
   ```

2. [ ] **Ensure all managers follow convention**
   - Check asteroid_manager: ✓
   - Check bullet_manager: ✓
   - Check saucer_manager: Add foreach_saucer if needed
   - Check sharpnel_system: Rename to match pattern

3. [ ] **Add missing iteration helpers**
   ```c
   void foreach_asteroid(asteroid_manager_ptr mgr,
                        void (*callback)(asteroid_ptr, void*),
                        void* user_data);
   ```

4. [ ] **Update documentation**
   - Add API contract to manager headers
   - Document expected usage patterns

**Acceptance Criteria**:
- ✅ Consistent naming across managers
- ✅ Predictable API
- ✅ Easy to learn

---

### 4.3 Add Primitive Type Safety
**Priority**: LOW | **Effort**: 2-3 hours | **Impact**: Better types

#### Files to Create:
- `engine/core/utils/types.h`

#### Tasks:

1. [ ] **Create type aliases**
   ```c
   // types.h
   typedef uint32_t timestamp_ms_t;
   typedef uint8_t rotation_index_t;  // 0-63

   typedef enum {
     ENTITY_SCALE_SMALL = 2,
     ENTITY_SCALE_MEDIUM = 3,
     ENTITY_SCALE_LARGE = 6
   } entity_scale_t;
   ```

2. [ ] **Replace raw types in ship.h**
   ```c
   // Before
   int rotation_index;
   int creation_ticks;
   int last_thrust_ticks;

   // After
   rotation_index_t rotation_index;
   timestamp_ms_t creation_ticks;
   timestamp_ms_t last_thrust_ticks;
   ```

3. [ ] **Apply to other entity types**
   - asteroid.h
   - bullet.h
   - saucer.h

4. [ ] **Update usage sites**
   - Compiler will catch type mismatches
   - Fix any issues

**Acceptance Criteria**:
- ✅ Fewer raw int/double types
- ✅ Intent clearer
- ✅ Compiler helps catch mistakes

---

### 4.4 Extract Background Asteroid Animation
**Priority**: LOW | **Effort**: 1-2 hours | **Impact**: Remove duplication

#### Files to Create:
- `game/src/rendering/background_effects.h`
- `game/src/rendering/background_effects.c`

#### Files to Modify:
- `game/src/stages/intro_stage.c`
- `game/src/stages/game_over_stage.c`

#### Tasks:

1. [ ] **Create background effects module**
   ```c
   // background_effects.h
   typedef struct {
     asteroid_t* asteroids;
     size_t count;
   } background_asteroids_t;

   background_asteroids_t create_background_asteroids(
     graphics_context_ptr ctx, size_t count);
   void update_background_asteroids(
     background_asteroids_t* bg, graphics_context_ptr ctx, double delta);
   void destroy_background_asteroids(background_asteroids_t* bg);
   ```

2. [ ] **Implement module**
   - Extract logic from intro_stage.c
   - Make generic for any count

3. [ ] **Update intro_stage to use module**
   - Replace inline asteroid array with background_asteroids_t
   - Call module functions

4. [ ] **Update game_over_stage to use module**
   - Same pattern

**Acceptance Criteria**:
- ✅ No duplicated background animation
- ✅ Both stages use same code
- ✅ Visual effects identical

---

### 4.5 Add Iterator Abstractions to Managers
**Priority**: LOW | **Effort**: 2-3 hours | **Impact**: Hide implementation

#### Files to Modify:
- All manager headers/implementations

#### Tasks:

1. [ ] **Add foreach functions to all managers**
   ```c
   // asteroid_manager.h
   typedef void (*asteroid_callback_t)(asteroid_ptr asteroid, size_t index, void* user_data);
   void foreach_active_asteroid(asteroid_manager_ptr mgr,
                                asteroid_callback_t callback,
                                void* user_data);
   ```

2. [ ] **Implement using pool_foreach_active**
   ```c
   // asteroid_manager.c
   void foreach_active_asteroid(asteroid_manager_ptr mgr,
                               asteroid_callback_t callback,
                               void* user_data) {
     pool_foreach_active(&mgr->pool, (pool_callback_t)callback, user_data);
   }
   ```

3. [ ] **Update collision_system to use iterators**
   - Replace manual loops with foreach calls
   - Hides index-based access

4. [ ] **Apply to all managers**
   - bullet_manager: foreach_active_ship_bullet, foreach_active_saucer_bullet
   - saucer_manager: May not need (single saucer)

**Acceptance Criteria**:
- ✅ Collision code uses iterators
- ✅ Manager implementation hidden
- ✅ Easier to change data structures later

---

## Phase 5: Future Enhancements (Optional)

### 5.1 Entity-Component System (ECS)
**Priority**: FUTURE | **Effort**: 1-2 weeks | **Impact**: Revolutionary

**Note**: This is a major architectural change. Only proceed after completing Phases 1-4 and evaluating if the benefits are worth the effort for your use case.

#### High-Level Plan:
1. Design component system
2. Create entity manager
3. Define components (position, velocity, renderable, etc.)
4. Create systems (movement, rendering, collision)
5. Migrate entities to components
6. Remove old entity-specific managers
7. Update stage code

**Recommendation**: Start a new branch for ECS experimentation. Don't merge until fully validated.

---

### 5.2 Abstract Renderer Interface
**Priority**: FUTURE | **Effort**: 3-4 days | **Impact**: Enables testing without SDL

#### High-Level Plan:
1. Define renderer interface (draw_line, draw_pixel, etc.)
2. Create SDL implementation
3. Create headless/mock implementation
4. Update graphics code to use interface
5. Add ability to swap renderers

**Use Case**: Test game logic without SDL initialization

---

### 5.3 Save/Load System
**Priority**: FUTURE | **Effort**: 2-3 days | **Impact**: Game state persistence

#### High-Level Plan:
1. Define serializable state
2. Implement serialize/deserialize for each manager
3. Add file I/O
4. Hook into game loop
5. Add UI for save/load

**Dependencies**: Easier after DI and object pools are in place

---

## Implementation Guidelines

### Work Session Planning

**Recommended Session Size**: 2-4 hours of focused work

**Order of Operations**:
1. Complete entire phase before moving to next
2. Within phase, complete tasks in order listed
3. Test after each task completion
4. Commit after each completed section

### Testing Strategy

After each task:
1. **Compile**: `make clean && make`
2. **Lint**: `make lint` (should pass with 0 errors)
3. **Manual test**: Run game, verify behavior unchanged
4. **Git commit**: Small, focused commits

### Rollback Plan

If a refactoring causes issues:
1. Check git status
2. Revert problematic commit: `git revert <commit>`
3. OR: Stash changes and return later: `git stash`
4. Continue with other tasks

### Documentation

Update after each phase:
- [ ] Update README.md with new architecture
- [ ] Document new patterns in code comments
- [ ] Update this file with completed status

---

## Progress Tracking

### Phase 1: Quick Wins ⬜
- [ ] 1.1 Extract Frame Limiter
- [ ] 1.2 Replace Magic Numbers
- [ ] 1.3 Fix Boolean Names
- [ ] 1.4 Remove Dead Code
- [ ] 1.5 Improve Comments

### Phase 2: Critical Architecture ⬜
- [ ] 2.1 Implement Object Pool
- [ ] 2.2 Refactor Asteroid Manager
- [ ] 2.3 Refactor Bullet Manager
- [ ] 2.4 Refactor Sharpnel
- [ ] 2.5 Eliminate Global State
  - [ ] 2.5.1 Remove from Managers
  - [ ] 2.5.2 Remove from Playing Stage
  - [ ] 2.5.3 Remove from Other Stages
- [ ] 2.6 Context Struct

### Phase 3: Structural Improvements ⬜
- [ ] 3.1 Reorganize Folders
- [ ] 3.2 Extract Ship Controller
- [ ] 3.3 State Pattern
- [ ] 3.4 Observer Pattern

### Phase 4: Polish ⬜
- [ ] 4.1 Reduce ALWAYS_INLINE
- [ ] 4.2 Standardize APIs
- [ ] 4.3 Type Safety
- [ ] 4.4 Background Effects
- [ ] 4.5 Iterator Abstractions

### Phase 5: Future ⬜
- [ ] 5.1 ECS (optional)
- [ ] 5.2 Abstract Renderer (optional)
- [ ] 5.3 Save/Load (optional)

---

## Success Metrics

### Code Quality
- ✅ No global static state
- ✅ < 5 SOLID violations (from 20+)
- ✅ No code duplication > 5 lines
- ✅ All magic numbers replaced
- ✅ 0 lint errors

### Architecture
- ✅ Clear separation of concerns
- ✅ Dependency injection throughout
- ✅ Reusable object pool
- ✅ Event-driven design
- ✅ Consistent APIs

### Maintainability
- ✅ Can add new entity in < 30 minutes
- ✅ Can add new game stage easily
- ✅ Each file < 300 lines
- ✅ Each function < 50 lines
- ✅ Clear ownership model

### Performance
- ✅ 60 FPS maintained
- ✅ No memory leaks
- ✅ Efficient entity management
- ✅ No regression from refactoring

---

## Final Notes

**Estimated Total Effort**: 2-3 weeks of focused work

**Expected Outcome**:
- Codebase rating: 7/10 → 9/10
- Line reduction: ~600 lines fewer
- Maintainability: 10x improvement
- Engine reusability: Actually achievable

**When Complete**:
- Update code-review.md with new assessment
- Celebrate! This is significant work.
- Consider writing blog post about the refactoring journey

**Questions?** Refer back to code-review.md for detailed explanations of each pattern and principle.

Good luck! 🚀
