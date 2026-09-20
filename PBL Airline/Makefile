CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
SRC = src/main.cpp src/Flight.cpp src/Passenger.cpp src/Ticket.cpp src/Airline.cpp src/Menu.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = skylink_app

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET) $(TARGET).exe

.PHONY: all clean
