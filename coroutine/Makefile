BUILD ?= debug
TARGET := test_coroutine
CXX := g++
CXXFLAGS := -std=c++20 -fcoroutines
LDFLAGS :=

ifeq ($(BUILD), debug)
# 为了获取完整的栈回溯
CXXFLAGS += -g -fno-omit-frame-pointer
# AddressSanitizer 是 LeakSanitizer 的超集。
# 检测内存的 LeakSanitizer 是集成在AddressSanitizer中的一个相对独立的工具，它工
# 作在检查过程的最后阶段。
CXXFLAGS += -fsanitize=address
LDFLAGS += -static-libasan -fsanitize=address
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
