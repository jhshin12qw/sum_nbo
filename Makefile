
CXX      = g++
CXXFLAGS = -std=c++11 -Wall -O2
TARGET   = main
SRCS     = main.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)

