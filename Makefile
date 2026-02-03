
CXX := clang++
SRC_DIR   := .
BUILD_DIR := build
GEN_DIR   := grpc/generated
TEST_DIR  := tests
TEST_OUT  := temp
TEST_SRCS := $(wildcard $(TEST_DIR)/*.cc)
TEST_BINS := $(patsubst $(TEST_DIR)/%.cc,$(TEST_OUT)/%,$(TEST_SRCS))
TARGET 	  := $(BUILD_DIR)/agent
SOURCES   := unity.cc

INCLUDES  := -I$(SRC_DIR) -I$(GEN_DIR)
INCLUDES  += $(shell pkg-config --cflags grpc++)

CXXFLAGS := \
	-std=c++17 \
	-g \
	-O0 \
	-Wno-writable-strings\
	$(INCLUDES)
#	-Wall \
#	-Wextra \
#	-Wno-unused-function \

LDFLAGS := \
	$(shell pkg-config --libs grpc++) \
	-pthread \
	-lgrpc++ \
	-lgrpc \
	-lprotobuf \
	-labsl_base \
	-labsl_synchronization \
	-labsl_strings \
	-ldbus-1 \
	-lsystemd \
	-lssl \
	-lcrypto \
	-lgrpc++_reflection


all: $(TARGET)

run: $(TARGET)
	./$(TARGET)

$(TARGET): $(SOURCES)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(TEST_OUT)/%: $(TEST_DIR)/%.cc
	mkdir -p $(TEST_OUT)
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

test: $(TEST_BINS)
	@set -e; \
	for t in $(TEST_BINS); do \
		echo "[TEST] $$t"; \
		$$t; \
	done

clean:
	rm -rf $(BUILD_DIR) $(TEST_OUT)

.PHONY: all run test clean
