# compiler
CXX = g++
CXXFLAGS = -std=c++17 -g -Wall -I./core

# source files
SRC = $(wildcard *.cpp) \
      $(wildcard core/*.cpp) \
      $(wildcard market/*.cpp) \
      $(wildcard user/*.cpp) \
      $(wildcard trading/*.cpp) \
      $(wildcard wallet/*.cpp)

# build folder
BUILD = build

# object files (in build folder, preserve relative path)
OBJ = $(patsubst %.cpp,$(BUILD)/%.o,$(SRC))

# executable
TARGET = $(BUILD)/main

# default rule
all: $(TARGET)

# link object files to create executable
$(TARGET): $(OBJ)
	@mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) -o $@ $^

# compile .cpp to .o in build folder
$(BUILD)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# clean objects and exe
clean:
	rm -rf $(BUILD)/*
