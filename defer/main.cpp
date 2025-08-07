#include <concepts>
#include <iostream>

#define FORCE_INLINE [[gnu::always_inline]] inline

// 优于基于 std::unique_ptr 的方式。采用 std::unique_ptr 需要指定非空的指针值，所
// 以多占用一个指针对象，同时析构函数被调用时会检查成员指针对象是否为空。
template<typename T>
  requires std::invocable<T> && std::copy_constructible<T>
struct Defer : T {
  FORCE_INLINE Defer(T&& callable)
    : T { callable }
  {
  }

  FORCE_INLINE ~Defer()
  {
    T::operator()();
  }
};

#define defer DEFER
#define DEFER IMPL_DEFER_HELPER(__COUNTER__)
#define IMPL_DEFER_HELPER(n) IMPL_DEFER(n)
#define IMPL_DEFER(n) Defer defer_##n = [&]() -> void

int main(int argc, char* argv[])
{
  Defer d = [](){ std::cout << "defer" << std::endl; };
  DEFER { std::cout << "defer2" << std::endl; };
  defer { std::cout << "defer3" << std::endl; };
  std::cout << "Processing" << std::endl;
  return 0;
}
