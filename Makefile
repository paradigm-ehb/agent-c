# Toolchain
CC := clang

# Paths
SRC_DIR := ../agent-c
AGENT_SRC := ./agent/agent_core.cc
BUILD_DIR := build
TARGET := agent

# gRPC flags (resolved at make time)
GRPC_CFLAGS := $(shell pkg-config --cflags grpc++)
GRPC_LIBS   := $(shell pkg-config --libs grpc++)

# Flags
CFLAGS := -I$(SRC_DIR) -Wall $(GRPC_CFLAGS)
LDFLAGS := -lm $(GRPC_LIBS)

# Default target
.PHONY: all
all: $(BUILD_DIR)/$(TARGET)

# Build rule
$(BUILD_DIR)/$(TARGET): $(AGENT_SRC)
	@mkdir -p $(BUILD_DIR)
	@echo "[build] $(TARGET)"
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@

# Run target
.PHONY: run
run: all
	@echo "[run] ./$(BUILD_DIR)/$(TARGET)"
	@./$(BUILD_DIR)/$(TARGET)

# Clean
.PHONY: clean
clean:
	@echo "[clean]"
	rm -rf $(BUILD_DIR)
