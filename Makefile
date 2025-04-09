CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I.
TARGET = main
OBJS = main.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

main.o: main.cpp matrix.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

clean:
	rm -f $(OBJS) $(TARGET)