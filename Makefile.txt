# Compiler Configuration
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
TARGET = arcade_os

# Source definitions
SRCS = main.c player_manager.c machine_manager.c ui_renderer.c persistence_manager.c
OBJS = $(SRCS:.c=.o)
# Simple dependency tracking
DEPS = core_types.h persistence.h

.PHONY: all clean rebuild debug

# Default build target
all: $(TARGET)

# Linking phase
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Explicit Compilation Rule
%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

debug: CFLAGS = -Wall -Wextra -std=c99 -g -O0
debug: clean $(TARGET)

# Clean up build artifacts
clean:
	rm -f $(OBJS) $(TARGET) *.bin *.csv

# Rebuild from scratch
rebuild: clean all