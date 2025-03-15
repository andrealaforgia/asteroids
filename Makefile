# Compiler
CC = gcc

SDL2_FLAGS := `sdl2-config --libs --cflags` -lSDL2_mixer
RM = rm -f

# Platform detection
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
    PLATFORM = LINUX
    INSTALL_CMD = sudo apt-get install -y libsdl2-dev libsdl2-mixer-dev
    DEV_INSTALL_CMD = sudo apt-get install -y cpplint clang-format
else ifeq ($(UNAME_S), Darwin)
    PLATFORM = OSX
    INSTALL_CMD = brew install sdl2 sdl2_mixer
    DEV_INSTALL_CMD = brew install cpplint clang-format
else
    $(error Unsupported platform)
endif

# Compiler flags
CFLAGS := -ggdb3 -Ofast --std=c99 -Wall -Wextra -pedantic-errors $(SDL2_FLAGS) -lm

# Source and object files
SRCDIR = .
SRC = $(wildcard $(SRCDIR)/*.c)
HEADERS = $(wildcard $(SRCDIR)/*.h)
OBJ = $(SRC:.c=.o)

# Target
TARGET = asteroids

# Rules
.PHONY: all install dev_install clean lint format

# Ensure formatting runs before every make
all: format $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

install:
	$(INSTALL_CMD)

dev_install:
	$(DEV_INSTALL_CMD)

lint:
	cpplint --filter=-build/include_subdir,-legal/copyright,-runtime/threadsafe_fn $(SRC) $(HEADERS)

clean:
	$(RM) $(OBJ) $(TARGET)

format:
	# Run clang-format on all .c and .h files
	clang-format -i -style=Google $(SRC) $(HEADERS)
