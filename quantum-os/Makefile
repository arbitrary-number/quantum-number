# Quantum OS - Makefile
# 
# Revolutionary operating system kernel designed for symbolic mathematical
# computation and Quantum Number operations.
# 
# Copyright (c) 2025 Arbitrary Number Project Team
# Licensed under the Apache License, Version 2.0
# 
# Date: August 18, 2025

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -g
LDFLAGS = -lm

# Directories
KERNEL_DIR = kernel
BUILD_DIR = build
BIN_DIR = bin

# Source files
KERNEL_SOURCES = \
	$(KERNEL_DIR)/quantum_number.c \
	$(KERNEL_DIR)/memory.c \
	$(KERNEL_DIR)/process.c \
	$(KERNEL_DIR)/syscalls.c \
	$(KERNEL_DIR)/quantix_scheduler.c \
	$(KERNEL_DIR)/quantix_qfs.c \
	$(KERNEL_DIR)/quantix_qfs_ast.c \
	$(KERNEL_DIR)/kernel.c

# Object files
KERNEL_OBJECTS = $(KERNEL_SOURCES:$(KERNEL_DIR)/%.c=$(BUILD_DIR)/%.o)

# Target executable
KERNEL_TARGET = $(BIN_DIR)/quantum-os-kernel

# Debug build
DEBUG_CFLAGS = -DKERNEL_DEBUG -DQM_DEBUG -DQP_DEBUG -DQS_DEBUG -g -O0
DEBUG_TARGET = $(BIN_DIR)/quantum-os-kernel-debug

.PHONY: all clean debug install test help

# Default target
all: $(KERNEL_TARGET)

# Create directories
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Compile object files
$(BUILD_DIR)/%.o: $(KERNEL_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link kernel
$(KERNEL_TARGET): $(KERNEL_OBJECTS) | $(BIN_DIR)
	$(CC) $(KERNEL_OBJECTS) $(LDFLAGS) -o $@
	@echo "Quantum OS kernel built successfully: $@"

# Debug build
debug: CFLAGS += $(DEBUG_CFLAGS)
debug: $(DEBUG_TARGET)

$(DEBUG_TARGET): $(KERNEL_OBJECTS) | $(BIN_DIR)
	$(CC) $(KERNEL_OBJECTS) $(LDFLAGS) -o $@
	@echo "Quantum OS debug kernel built successfully: $@"

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "Build artifacts cleaned"

# Install (placeholder)
install: $(KERNEL_TARGET)
	@echo "Installing Quantum OS kernel..."
	@echo "Installation not implemented yet"

# Test the kernel
test: $(KERNEL_TARGET)
	@echo "Testing Quantum OS kernel..."
	./$(KERNEL_TARGET)

# Test debug kernel
test-debug: debug
	@echo "Testing Quantum OS debug kernel..."
	./$(DEBUG_TARGET)

# Show help
help:
	@echo "Quantum OS Build System"
	@echo "======================="
	@echo ""
	@echo "Targets:"
	@echo "  all        - Build the Quantum OS kernel (default)"
	@echo "  debug      - Build debug version with debugging symbols"
	@echo "  clean      - Remove all build artifacts"
	@echo "  test       - Build and run the kernel"
	@echo "  test-debug - Build and run the debug kernel"
	@echo "  install    - Install the kernel (not implemented)"
	@echo "  help       - Show this help message"
	@echo ""
	@echo "Build Configuration:"
	@echo "  CC         = $(CC)"
	@echo "  CFLAGS     = $(CFLAGS)"
	@echo "  LDFLAGS    = $(LDFLAGS)"
	@echo ""
	@echo "Files:"
	@echo "  Kernel     = $(KERNEL_TARGET)"
	@echo "  Debug      = $(DEBUG_TARGET)"

# Test targets
TEST_DIR = test
TEST_TARGET = $(BIN_DIR)/quantix_scheduler_test
QFS_TEST_TARGET = $(BIN_DIR)/quantix_qfs_test

test-quantix: $(TEST_TARGET)
	@echo "Running Quantix scheduler test..."
	./$(TEST_TARGET)

test-qfs: $(QFS_TEST_TARGET)
	@echo "Running Quantix QFS test..."
	./$(QFS_TEST_TARGET)

test-all: test-quantix test-qfs
	@echo "All Quantix tests completed"

$(TEST_TARGET): $(TEST_DIR)/quantix_scheduler_test.c $(KERNEL_OBJECTS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -I$(KERNEL_DIR) $< $(KERNEL_OBJECTS) $(LDFLAGS) -o $@
	@echo "Quantix scheduler test built successfully: $@"

$(QFS_TEST_TARGET): $(TEST_DIR)/quantix_qfs_test.c $(KERNEL_OBJECTS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -I$(KERNEL_DIR) $< $(KERNEL_OBJECTS) $(LDFLAGS) -o $@
	@echo "Quantix QFS test built successfully: $@"

# Dependencies
$(BUILD_DIR)/quantum_number.o: $(KERNEL_DIR)/quantum_number.h
$(BUILD_DIR)/memory.o: $(KERNEL_DIR)/memory.h $(KERNEL_DIR)/quantum_number.h
$(BUILD_DIR)/process.o: $(KERNEL_DIR)/process.h $(KERNEL_DIR)/quantum_number.h $(KERNEL_DIR)/memory.h
$(BUILD_DIR)/syscalls.o: $(KERNEL_DIR)/syscalls.h $(KERNEL_DIR)/quantum_number.h $(KERNEL_DIR)/process.h $(KERNEL_DIR)/memory.h
$(BUILD_DIR)/quantix_scheduler.o: $(KERNEL_DIR)/quantix_scheduler.h $(KERNEL_DIR)/quantum_number.h $(KERNEL_DIR)/process.h $(KERNEL_DIR)/memory.h
$(BUILD_DIR)/quantix_qfs.o: $(KERNEL_DIR)/quantix_qfs.h $(KERNEL_DIR)/quantum_number.h $(KERNEL_DIR)/memory.h
$(BUILD_DIR)/quantix_qfs_ast.o: $(KERNEL_DIR)/quantix_qfs.h $(KERNEL_DIR)/quantum_number.h $(KERNEL_DIR)/memory.h
$(BUILD_DIR)/kernel.o: $(KERNEL_DIR)/kernel.h $(KERNEL_DIR)/quantum_number.h $(KERNEL_DIR)/memory.h $(KERNEL_DIR)/process.h $(KERNEL_DIR)/syscalls.h
