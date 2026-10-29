CXX = g++
CXXFLAGS = -Wall -std=c++11

TARGET = producer_consumer
SRC = producer_consumer.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)