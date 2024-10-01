TAGLIB := /data/data/com.termux/files/usr/lib/libtag.so
SRC_DIR := $(PWD)/src
BUILD_DIR := $(PWD)/build

#SOURCE := $(shell find $(SRC_DIR) -name "*.c")
#TARGET := $(SRC:%=$(BUILD_DIR)/%.o)

SOURCE := $(SRC_DIR)/main.cpp
TARGET := $(SOURCE:$(SRC_DIR)%.cpp=$(BUILD_DIR)%)

.PHONY:	all
all: build

.PHONY:	build
build: $(SOURCE)
	if [ ! -d "build" ]; then mkdir build; fi;
	g++ -ldl $(TAGLIB) $(SOURCE) -o $(TARGET)

.PHONY: clean
clean:
	rm -rf build
