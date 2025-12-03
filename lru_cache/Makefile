BUILD ?= debug
TARGET := test_lru_cache
CXX := g++
CXXFLAGS := -std=c++20
LDFLAGS :=

ifeq ($(BUILD), debug)
CXXFLAGS += -g -fno-omit-frame-pointer
CXXFLAGS += -fsanitize=address
#CXXFLAGS += -fsanitize=leak
LDFLAGS += -static-libasan -fsanitize=address
#LDFLAGS += -llsan -fsanitize=leak
endif

SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:.cpp=.o)
all: $(TARGET)

.PHONY: all clean

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(TARGET) $(OBJS)
