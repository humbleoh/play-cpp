#include <iostream>
#include <tuple>
#include <variant>

int main(int argc, char* argv[])
{
  volatile std::tuple<int, bool> v1;
  volatile std::tuple<int> v2;
  volatile std::variant<int, void> v3;
  return 0;
}
