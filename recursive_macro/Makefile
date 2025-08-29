BUILD ?= debug
TARGET := a.out
CXX := g++
CXXFLAGS := -std=c++20
# 预处理标志
CPPFLAGS :=
LDFLAGS :=

ifeq ($(BUILD), debug)
# 为了获取完整的栈回溯
CXXFLAGS += -g -fno-omit-frame-pointer
# AddressSanitizer 是 LeakSanitizer 的超集。
# 检测内存的 LeakSanitizer 是集成在 AddressSanitizer 中的一个相对独立的工具，它
# 工作在检查过程的最后阶段。
CXXFLAGS += -fsanitize=address
LDFLAGS += -static-libasan
else
# assert() 开关
CXXFLAGS += -DNDEBUG
endif

SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:.cpp=.o)
all: $(TARGET)

.PHONY: all clean pp

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

pp: main.cpp
	$(CXX) -E $< -o $<.pp

clean:
	rm -rf $(TARGET) $(OBJS) main.cpp.pp
