# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++11 -I./include -g $(shell pkg-config --cflags gtk+-3.0) -I/usr/include/gtk-3.0

# Directories
SRC_DIR = src
OBJ_DIR = build
BIN_DIR = build
DATA_DIR = data

# Files
SOURCES = $(wildcard $(SRC_DIR)/**/*.cpp)
SOURCES += $(SRC_DIR)/main.cpp
SOURCES += $(SRC_DIR)/GUI.cpp
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
EXEC = GKompress 

# Targets
all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXEC) $(shell pkg-config --cflags --libs gtk+-3.0)

# Object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)  # Create object file directory if it doesn't exist
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(EXEC) && rm -rf $(OBJ_DIR)/* && rm -f $(DATA_DIR)/output/* 

# Run the program (optional)
run: $(EXEC)
	./$(EXEC)
