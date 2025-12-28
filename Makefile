CC = gcc

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
    PLATFORM = LINUX
    INSTALL_CMD = sudo apt-get update && sudo apt-get install -y libsdl2-dev libsdl2-mixer-dev
    DEV_INSTALL_CMD = sudo apt-get update && sudo apt-get install -y cpplint clang-format
else ifeq ($(UNAME_S), Darwin)
    PLATFORM = OSX
    INSTALL_CMD = brew install sdl2 sdl2_mixer
    DEV_INSTALL_CMD = brew install cpplint clang-format
else
    $(error Unsupported platform)
endif

SDL2_CFLAGS := $(shell sdl2-config --cflags)
SDL2_LFLAGS := $(shell sdl2-config --libs) -lSDL2_mixer

SRCDIR = .
ENGINE_GRAPHICS_DIR = engine/core/graphics
ENGINE_MATH_DIR = engine/core/math
ENGINE_INPUT_DIR = engine/core/input
ENGINE_AUDIO_DIR = engine/core/audio
ENGINE_UTILS_DIR = engine/core/utils

GAME_ENTITIES_DIR = game/src/entities
GAME_STAGES_DIR = game/src/stages
GAME_INCLUDE_DIR = game/include

# Find all C source files in root, engine, and game directories
SRC = $(wildcard $(SRCDIR)/*.c) \
      $(wildcard $(ENGINE_GRAPHICS_DIR)/*.c) $(wildcard $(ENGINE_MATH_DIR)/*.c) $(wildcard $(ENGINE_INPUT_DIR)/*.c) $(wildcard $(ENGINE_AUDIO_DIR)/*.c) $(wildcard $(ENGINE_UTILS_DIR)/*.c) \
      $(wildcard $(GAME_ENTITIES_DIR)/*.c) $(wildcard $(GAME_STAGES_DIR)/*.c)

HEADERS = $(wildcard $(SRCDIR)/*.h) \
          $(wildcard $(ENGINE_GRAPHICS_DIR)/*.h) $(wildcard $(ENGINE_MATH_DIR)/*.h) $(wildcard $(ENGINE_INPUT_DIR)/*.h) $(wildcard $(ENGINE_AUDIO_DIR)/*.h) $(wildcard $(ENGINE_UTILS_DIR)/*.h) \
          $(wildcard $(GAME_INCLUDE_DIR)/*/*.h)

OBJ = $(SRC:.c=.o)

# Add include paths
INCLUDES = -I. \
           -I$(ENGINE_GRAPHICS_DIR) -I$(ENGINE_MATH_DIR) -I$(ENGINE_INPUT_DIR) -I$(ENGINE_AUDIO_DIR) -I$(ENGINE_UTILS_DIR) \
           -I$(GAME_INCLUDE_DIR) -I$(GAME_INCLUDE_DIR)/entities -I$(GAME_INCLUDE_DIR)/stages -I$(GAME_INCLUDE_DIR)/systems -I$(GAME_INCLUDE_DIR)/main

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

