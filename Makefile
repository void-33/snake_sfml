CXX = g++
CXXFLAGS = -I"./include"
LDFLAGS = -L"./lib" -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -mwindows

SRC_DIR = src
OBJ_DIR = bin
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
TARGET = $(OBJ_DIR)/app

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -f $(OBJ) $(TARGET)

