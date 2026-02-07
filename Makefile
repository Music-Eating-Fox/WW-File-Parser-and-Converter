.PHONY: all, build

ifeq ($(OS), Windows_NT)

COMPILER            := gcc
SOURCE_FILES        := ./src/*.c
BUILD_NAME          := get_stuff.exe
BUILD_LOCATION      := ./bin
INCLUDE_LOCATIONS   := 
LIBRARY_LOCATIONS   := 
LIBRARIES           := -lws2_32
FRAMEWORK_LOCATIONS := 
FRAMEWORKS          := 
COMPILER_FLAGS      := 

$(info Windows)

else

COMPILER            := gcc
SOURCE_FILES        := ./src/*.c
BUILD_NAME          := get_stuff
BUILD_LOCATION      := ./bin
INCLUDE_LOCATIONS   := -I/opt/homebrew/include
LIBRARY_LOCATIONS   := -L/opt/homebrew/lib
LIBRARIES           := 
FRAMEWORK_LOCATIONS := 
FRAMEWORKS          := 
COMPILER_FLAGS      := -std=c2x

$(info Unix)

endif

all: build

scaffold:
	mkdir -p bin
	mkdir -p bin/files
	mkdir -p bin/files/bms
	mkdir -p bin/files/midi
	mkdir -p bin/files/txt
	mkdir -p data

build:
	@$(COMPILER) $(SOURCE_FILES) -o $(BUILD_LOCATION)/$(BUILD_NAME) $(INCLUDE_LOCATIONS) $(LIBRARY_LOCATIONS) $(LIBRARIES) $(FRAMEWORK_LOCATIONS) $(FRAMEWORKS) $(COMPILER_FLAGS)

# run:
# 	@$(BUILD_LOCATION)/$(BUILD_NAME)