#include <concepts>
#include <iostream>

template<typename T>
  requires std::invocable<T> && std::copy_constructible<T>
struct Defer : T {
  Defer(T&& callable)
    : T { callable }
  {
  }

  ~Defer()
  {
    T::operator()();
  }
};

int main(int argc, char* argv[])
{
  Defer d = [](){ std::cout << "defer" << std::endl; };
  std::cout << "Processing" << std::endl;
  return 0;
}
