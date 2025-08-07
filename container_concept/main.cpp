#include <iostream>
#include <stack>
#include <vector>
#include <utility>

template<typename Container>
void print_container(const Container& container)
{
  std::cout << ">>" << __func__ << std::endl;
  for (auto&& v: container)
  {
    std::cout << v << std::endl;
  }
}

template<typename Container,
  typename = decltype(std::declval<Container>().begin(), std::declval<Container>().end())>
void print_container2(const Container& container)
{
  std::cout << ">>" << __func__ << std::endl;
  for (auto&& v: container)
  {
    std::cout << v << std::endl;
  }
}

template<typename T>
concept ContainerConcept = requires(T v) {
  { v.begin() } -> std::input_or_output_iterator;
  { v.end() } -> std::input_or_output_iterator;
};

template<ContainerConcept Container>
void print_container3(const Container& container)
{
  std::cout << ">>" << __func__ << std::endl;
  for (auto&& v: container)
  {
    std::cout << v << std::endl;
  }
}

int main(int argc, char* argv[])
{
  std::vector<int> c = { 1, 2, 3, 4, 5 };
  print_container(c);
  std::stack<int> s;
  //print_container(s); // 长长的错误信息
  print_container2(c);
  //print_container2(s); // 短短的错误信息
  print_container3(c);
  //print_container3(s); // 短短且清晰的错误信息
  return 0;
}
