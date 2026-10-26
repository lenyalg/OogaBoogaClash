CC = gcc
RAYLIB_INC = ../../raylib/src
RAYLIB_LIB = ../../raylib/src
# Project include dir (for common.h, sprite_manager.h, etc.)
INCLUDE_DIR = include

# Add project include directory and Raylib includes
# Add source directories to include search so headers in src/* are found
CFLAGS = -I$(INCLUDE_DIR) -Isrc -Isrc/graphics/menu -Isrc/graphics/gamegraphics -Isrc/gameplay \
	-Isrc/utils -I$(RAYLIB_INC) -Wall -Wextra -O2 -Isrc/graphics/hud

# Linker flags (remove erroneous -lcommon)
LDFLAGS = -L$(RAYLIB_LIB) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
## Collect all source files under src but exclude example programs with their own main (menudebut.c)
SRCS = $(shell find src -name '*.c' -print)
TARGET = mon_jeu.out

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(SRCS) $(CFLAGS) $(LDFLAGS) -o $(TARGET)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)
