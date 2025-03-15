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

SDL2_CFLAGS := `sdl2-config --cflags`
SDL2_LFLAGS := `sdl2-config --libs` -lSDL2_mixer

CFLAGS := -ggdb3 -Ofast --std=c99 -Wall -Wextra -pedantic-errors $(SDL2_CFLAGS) 
LFLAGS := $(SDL2_LFLAGS) -lm

SRCDIR = .
SRC = $(wildcard $(SRCDIR)/*.c)
HEADERS = $(wildcard $(SRCDIR)/*.h)
OBJ = $(SRC:.c=.o)

TARGET = asteroids

.PHONY: all install dev_install clean lint format

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LFLAGS) -o $@ $^

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
