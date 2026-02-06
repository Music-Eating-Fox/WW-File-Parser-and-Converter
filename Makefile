.PHONY: all, build

COMPILER            := gcc
SOURCE_FILES        := ./src/*.c
BUILD_NAME          := get_stuff
BUILD_LOCATION      := ./bin
INCLUDE_LOCATIONS   := -I/opt/homebrew/include
LIBRARY_LOCATIONS   := -L/opt/homebrew/lib
LIBRARIES           := 
FRAMEWORK_LOCATIONS := 
FRAMEWORKS          := 
COMPILER_FLAGS      := # -std=c2x

all: build

build:
	@$(COMPILER) $(SOURCE_FILES) -o $(BUILD_LOCATION)/$(BUILD_NAME) $(INCLUDE_LOCATIONS) $(LIBRARY_LOCATIONS) $(LIBRARIES) $(FRAMEWORK_LOCATIONS) $(FRAMEWORKS) $(COMPILER_FLAGS)

# run:
# 	@$(BUILD_LOCATION)/$(BUILD_NAME)