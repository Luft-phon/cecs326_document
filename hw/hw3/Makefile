CXX = g++
CXXFLAGS = -Wall -std=c++17

TARGET = scheduler

all:
	$(CXX) $(CXXFLAGS) cpu_scheduler.cpp -o $(TARGET)

run: all
	./$(TARGET) processes2.txt

clean:
	rm -f $(TARGET)