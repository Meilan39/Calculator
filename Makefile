CC := gcc
CFLAGS := -Wall -Wextra -O2
SRC_DIR := src
OBJ_DIR := build
TARGET := calc

# Find all .c files in the source directory and its subdirectories
SRCS := $(shell find $(SRC_DIR) -type f -name "*.c")
# Convert source file paths to object file paths in the obj directory
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Default target
all: $(TARGET)

# Create the target executable by linking object files
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Compile .c files into .o files in build directory
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Ensure the obj directory exists
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean up generated files
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean
