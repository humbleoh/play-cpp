#include <concepts>
#include <iostream>
#include <utility>

template<std::size_t... Ns>
struct integer_sequence
{
};

// <N-1, N-1, Ns...> 中的首个参数 N-1 是迭代计数，之后的参数列是数列展开。示例：
// 设 N = 4
// 迭代 4：3, 3
// 迭代 3：2, 2, 3
// 迭代 2：1, 1, 2, 3
// 迭代 1：0, 0, 1, 2, 3
// 迭代 0：把 0, 1, 2, 3 传给 integer_sequence 模版参数
template<std::size_t N, std::size_t... Ns>
struct make_integer_sequence_impl : make_integer_sequence_impl<N-1, N-1, Ns...> 
{
};

template<std::size_t... Ns>
struct make_integer_sequence_impl<0, Ns...>
{
  using type = integer_sequence<Ns...>;
};

template<std::size_t N>
using make_integer_sequence0 = typename make_integer_sequence_impl<N>::type;

template<std::size_t N>
constexpr auto make_integer_sequence1() noexcept
{
  return typename make_integer_sequence_impl<N>::type{};
}

template<std::size_t... Ns>
void print_integer_sequence(integer_sequence<Ns...>)
{
  ((std::cout << Ns << std::endl), ...);
}

int main(int argc, char* argv[])
{
  print_integer_sequence(make_integer_sequence0<5>{});
  std::cout << "==>" << std::endl;
  print_integer_sequence(make_integer_sequence1<6>());
  return 0;
}

