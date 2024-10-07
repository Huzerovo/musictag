#
# 仅适用于使用GCC编译器编译
#

SRC_DIR := $(PWD)/src
BUILD_DIR := $(PWD)/build

#SOURCE := $(shell find $(SRC_DIR) -name "*.c")
#TARGET := $(SRC:%=$(BUILD_DIR)/%.o)

SOURCE := $(SRC_DIR)/main.cpp
TARGET := $(SOURCE:$(SRC_DIR)%.cpp=$(BUILD_DIR)%)

CC := clang++
COMPILER_FLAGS := -O3 $(shell taglib-config --libs)

.PHONY:	all
all: build

$(TARGET): $(SOURCE)
	@if [ ! -d "build" ]; then mkdir build; fi;
	$(CC) $(COMPILER_FLAGS) $(SOURCE) -o $(TARGET)

.PHONY: build
build: $(TARGET)

.PHONY: clean
clean:
	rm -rf build
