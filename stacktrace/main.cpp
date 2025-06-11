#include <format>
#include <iostream>
#include <stdexcept>
#include <stacktrace>

#define DIAG_ASSERT(cond, fmt, ...) \
  do { \
    if (cond) break; \
    throw stack_runtime_error { \
      std::stacktrace::current(), \
      std::format(fmt __VA_OPT__(,) __VA_ARGS__) }; \
  } while (0)

#define DIAG_ASSERT2(cond, fmt, ...) \
  do { \
    if (!(cond)) \
      diag_throw(std::stacktrace::current(), \
        std::format(fmt __VA_OPT__(,) __VA_ARGS__)); \
  } while (0)

class stack_runtime_error : public std::runtime_error {
public:
  stack_runtime_error(const std::stacktrace& st, const std::string& msg)
    : std::runtime_error { msg + "\n" + std::to_string(st) }
  {
  }
};

constexpr void diag_throw(const std::stacktrace& st, const std::string& msg)
{
  throw stack_runtime_error { st, msg };
}

void bar()
{
  //DIAG_ASSERT(false, "test: {}", "hello world");
  DIAG_ASSERT2(false, "test2: {}", "hello world");
}

void fun()
{
  bar();
}

int main(int argc, char* argv[])
{
  try {
    fun();
  } catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
  }

  return 0;
}

