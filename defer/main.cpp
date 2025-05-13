#include <concepts>
#include <iostream>

#define FORCE_INLINE [[gnu::always_inline]] inline

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
