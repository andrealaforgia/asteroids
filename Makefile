CC = gcc

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
    PLATFORM = LINUX
    INSTALL_CMD = sudo apt-get update && sudo apt-get install -y libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev
    DEV_INSTALL_CMD = sudo apt-get update && sudo apt-get install -y cpplint clang-format
else ifeq ($(UNAME_S), Darwin)
    PLATFORM = OSX
    INSTALL_CMD = brew install sdl2 sdl2_image sdl2_mixer
    DEV_INSTALL_CMD = brew install cpplint clang-format
else
    $(error Unsupported platform)
endif

SDL2_CFLAGS := $(shell sdl2-config --cflags)
SDL2_LFLAGS := $(shell sdl2-config --libs) -lSDL2_image -lSDL2_mixer

SRCDIR = .
ENGINE_GRAPHICS_DIR = engine/core/graphics
ENGINE_MATH_DIR = engine/core/math
ENGINE_INPUT_DIR = engine/core/input
ENGINE_AUDIO_DIR = engine/core/audio
ENGINE_TIME_DIR = engine/core/time
ENGINE_UTILS_DIR = engine/core/utils
ENGINE_MEMORY_DIR = engine/core/memory
ENGINE_EVENTS_DIR = engine/core/events

GAME_ENTITIES_DIR = game/src/entities
GAME_STAGES_DIR = game/src/stages
GAME_MANAGERS_DIR = game/src/managers
GAME_COLLISION_DIR = game/src/collision
GAME_CONTROLLERS_DIR = game/src/controllers
GAME_RENDERING_DIR = game/src/rendering
GAME_AUDIO_DIR = game/src/audio
GAME_SCORING_DIR = game/src/scoring
GAME_EVENTS_DIR = game/src/events
GAME_MAIN_DIR = game/src/main

# Find all C source files in engine and game directories (root should be empty now)
SRC = $(wildcard $(ENGINE_GRAPHICS_DIR)/*.c) $(wildcard $(ENGINE_MATH_DIR)/*.c) $(wildcard $(ENGINE_INPUT_DIR)/*.c) $(wildcard $(ENGINE_AUDIO_DIR)/*.c) $(wildcard $(ENGINE_TIME_DIR)/*.c) $(wildcard $(ENGINE_UTILS_DIR)/*.c) $(wildcard $(ENGINE_MEMORY_DIR)/*.c) $(wildcard $(ENGINE_EVENTS_DIR)/*.c) \
      $(wildcard $(GAME_ENTITIES_DIR)/*.c) $(wildcard $(GAME_STAGES_DIR)/*.c) $(wildcard $(GAME_MANAGERS_DIR)/*.c) $(wildcard $(GAME_COLLISION_DIR)/*.c) $(wildcard $(GAME_CONTROLLERS_DIR)/*.c) $(wildcard $(GAME_RENDERING_DIR)/*.c) $(wildcard $(GAME_AUDIO_DIR)/*.c) $(wildcard $(GAME_SCORING_DIR)/*.c) $(wildcard $(GAME_EVENTS_DIR)/*.c) $(wildcard $(GAME_MAIN_DIR)/*.c)

HEADERS = $(wildcard $(SRCDIR)/*.h) \
          $(wildcard $(ENGINE_GRAPHICS_DIR)/*.h) $(wildcard $(ENGINE_MATH_DIR)/*.h) $(wildcard $(ENGINE_INPUT_DIR)/*.h) $(wildcard $(ENGINE_AUDIO_DIR)/*.h) $(wildcard $(ENGINE_TIME_DIR)/*.h) $(wildcard $(ENGINE_UTILS_DIR)/*.h) $(wildcard $(ENGINE_MEMORY_DIR)/*.h) $(wildcard $(ENGINE_EVENTS_DIR)/*.h) \
          $(wildcard $(GAME_ENTITIES_DIR)/*.h) $(wildcard $(GAME_STAGES_DIR)/*.h) $(wildcard $(GAME_MANAGERS_DIR)/*.h) $(wildcard $(GAME_COLLISION_DIR)/*.h) $(wildcard $(GAME_CONTROLLERS_DIR)/*.h) $(wildcard $(GAME_RENDERING_DIR)/*.h) $(wildcard $(GAME_AUDIO_DIR)/*.h) $(wildcard $(GAME_SCORING_DIR)/*.h) $(wildcard $(GAME_EVENTS_DIR)/*.h) $(wildcard $(GAME_MAIN_DIR)/*.h)

OBJ = $(SRC:.c=.o)

# Add include paths
INCLUDES = -I. \
           -I$(ENGINE_GRAPHICS_DIR) -I$(ENGINE_MATH_DIR) -I$(ENGINE_INPUT_DIR) -I$(ENGINE_AUDIO_DIR) -I$(ENGINE_TIME_DIR) -I$(ENGINE_UTILS_DIR) -I$(ENGINE_MEMORY_DIR) -I$(ENGINE_EVENTS_DIR) \
           -I$(GAME_ENTITIES_DIR) -I$(GAME_STAGES_DIR) -I$(GAME_MANAGERS_DIR) -I$(GAME_COLLISION_DIR) -I$(GAME_CONTROLLERS_DIR) -I$(GAME_RENDERING_DIR) -I$(GAME_AUDIO_DIR) -I$(GAME_SCORING_DIR) -I$(GAME_EVENTS_DIR) -I$(GAME_MAIN_DIR)

CFLAGS := -ggdb3 -Ofast --std=c99 -Wall -Wextra -pedantic-errors $(INCLUDES) $(SDL2_CFLAGS)
LFLAGS := $(SDL2_LFLAGS) -lm

TARGET = asteroids

.PHONY: all install dev_install clean lint format

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LFLAGS) 

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

install:
	$(INSTALL_CMD)

dev_install:
	$(DEV_INSTALL_CMD)

lint:
	cpplint --filter=-build/include_subdir,-legal/copyright,-runtime/threadsafe_fn $(SRC) $(HEADERS)

clean:
	rm -f $(OBJ) $(TARGET)

format:
	clang-format -i -style=Google $(SRC) $(HEADERS)

show_sdl_config:
	@echo "Checking SDL2 Configuration..."
	@which sdl2-config || echo "sdl2-config not found"
	@sdl2-config --version || echo "Failed to get SDL2 version"
	@sdl2-config --cflags || echo "Failed to get SDL2 cflags"
	@sdl2-config --libs || echo "Failed to get SDL2 libs"
	@echo "Library Paths:"
	@ldconfig -p | grep SDL || echo "No SDL libraries found in ldconfig"

