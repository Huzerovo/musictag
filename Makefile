PROJECT := musictag

SRC_DIR := src
BUILD_DIR := build

SOURCE := $(shell find $(SRC_DIR) -name "*.cpp")
OBJECT := $(SOURCE:$(SRC_DIR)/%=$(BUILD_DIR)/%.o)
TARGET := $(BUILD_DIR)/$(PROJECT)

CC := clang++
CC_FLAGS := -O3 -g
LD_FLAGS := $(shell taglib-config --libs)

.PHONY:	all
all: build

.PHONY: mkdir
mkdir:
	@if [ ! -d "$(BUILD_DIR)" ]; then mkdir "$(BUILD_DIR)"; fi;

.PHONY: build
build: mkdir $(TARGET)

.PHONY: clean
clean:
	rm -rf build

$(BUILD_DIR)/%.o: $(SRC_DIR)/%
	$(CC) $(CC_FLAGS) -c $< -o $@

$(TARGET): $(OBJECT)
	$(CC) -g $(LD_FLAGS) $(OBJECT) -o $@
