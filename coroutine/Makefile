.PHONY: all clean

TARGET := test_coroutine
CXX := g++
CXXFLAGS := -std=c++20 -fcoroutines
LDFLAGS :=

SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:.cpp=.o)
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(TARGET) $(OBJS)
