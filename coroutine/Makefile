.PHONY: all clean

TARGET := test_coroutine
CXX := g++
CXXFLAGS := -std=c++20 -fcoroutines

all: $(TARGET)

$(TARGET):
	$(CXX) $(CXXFLAGS) main.cpp -o $@

clean:
	rm -rf $(TARGET)
