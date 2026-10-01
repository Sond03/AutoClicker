CXX = g++
CXXFLAGS = -Wall -std=c++26 $(shell pkg-config --cflags libevdev)
LDFLAGS = $(shell pkg-config --libs libevdev)

TARGET = autoclicker-cli
SRC = autoclicker-cli.cpp

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
