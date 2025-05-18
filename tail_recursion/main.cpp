#include <cstdint>
#include <iostream>

std::uint64_t factorial_tailrecur_impl(std::uint64_t n, std::uint64_t res)
{
  if (n == 0)
    return res;

  // Tail recursion is here!!!
  // Tail recursion optimization happens at and above -O2 optimization level.
  return factorial_tailrecur_impl(n - 1, n * res);
}

std::uint64_t factorial_tailrecur(std::uint64_t n)
{
  return factorial_tailrecur_impl(n, 1);
}

std::uint64_t factorial_nontailrecur(std::uint64_t n)
{
  if (n == 0)
    return 1;

  // Although the last operation is not the recursive call, it is
  // treated as a tail recursiion by the compiler, and is optimized
  // away at and above -O2 optimization level.
  return n * factorial_nontailrecur(n - 1);
}

int main(int argc, char* argv[])
{
  std::cout << factorial_tailrecur(4) << std::endl;
  std::cout << factorial_nontailrecur(4) << std::endl;
  return 0;
}
