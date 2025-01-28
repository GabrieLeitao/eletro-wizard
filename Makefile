# Compiler and linker
CXX = g++
CC = gcc

# Include directories
INCLUDE_DIR = ./include  # This is where glad.h is located
CXXFLAGS = -std=c++11 -Wall -I$(INCLUDE_DIR)   # For C++ files
CCFLAGS = -I$(INCLUDE_DIR)  # For C files like glad.c

# Libraries
LIBS = -lglfw -lGL -lm

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*.c)

# Object files
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(OBJ_FILES))

# Executable
EXEC = $(BIN_DIR)/main

# Targets
all: $(EXEC)

# Link object files into the final executable
$(EXEC): $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)  # Ensure bin directory exists
	$(CXX) $(OBJ_FILES) -o $(EXEC) $(LIBS)

# Compile .cpp files to .o files in the obj directory
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)  # Ensure obj directory exists
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile .c files to .o files in the obj directory
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)  # Ensure obj directory exists
	$(CC) $(CCFLAGS) -c $< -o $@

# Clean all build artifacts
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Run the executable
run: $(EXEC)
	./$(EXEC)
