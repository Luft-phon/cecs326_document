CXX = g++
CXXFLAGS = -Wall -std=c++11

TARGET = process_tree
SRC = process_tree.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)