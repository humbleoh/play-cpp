#include <iostream>

#define EVAL(...)  EVAL3(__VA_ARGS__)
#define EVAL3(...) EVAL2(EVAL2(EVAL2(__VA_ARGS__)))
#define EVAL2(...) EVAL1(EVAL1(__VA_ARGS__))
#define EVAL1(...) __VA_ARGS__

#define PARENS ()

#define RECURSE_APPLY(f, ...) \
  EVAL(RECURSE_APPLY_HELPER(f, __VA_ARGS__))

// 把 RECURSE_APPLY_HELPER 展开，最后会变成嵌套 RECURSE_APPLY_HELPER_AGAIN 的形式。
// 最后通过 EVAL 不断递归求值这个嵌套形式。
#define RECURSE_APPLY_HELPER(f, a, ...) \
  f(a) __VA_OPT__(,) \
  __VA_OPT__(RECURSE_APPLY_HELPER_AGAIN PARENS (f, __VA_ARGS__))

#define RECURSE_APPLY_HELPER_AGAIN() RECURSE_APPLY_HELPER

void print(auto&& v)
{
  std::cout << v << std::endl;
}

int main(int argc, char* argv[])
{
  RECURSE_APPLY(print, 1, 2, 3, "hello world"); 
  return 0;
}
