#include <concepts>
#include <exception>
#include <iostream>
#include <new>
#include <utility>

#include <cstddef>
#include <cstdint>

template<typename T>
class simple_allocator {
public:
  using value_type = T;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;

  T* allocate(std::size_t n)
  {
    return static_cast<T*>(malloc(n * sizeof(T)));
  }

  void deallocate(T* p, std::size_t) noexcept
  {
    free(p);
  }

  template<typename U, typename... Args>
  void construct(U* ptr, Args&&... args)
  {
    ::new(static_cast<void*>(ptr)) U(std::forward<Args>(args)...);
  }

  template<typename U>
  void destroy(U* ptr)
  {
    ptr->~U();
  }

  // 当容器需要分配与模板参数类型不同的对象时，通过 rebind 转换分配器类型。容器
  // 并不知道用户提供的分配器具体是如何实现的，可能分配器有额外的状态或策略，需
  // 要确保不同数据类型的分配器来自同一个模板，保持相同的分配策略。因此，rebind
  // 提供了一种标准化的方式，让容器能够安全地转换分配器类型
  // 场景：_List_node<T>、_Rb_tree_node<pair<const K, V>>、_Hash_node<pair<const K, V>>
  template<typename U>
  struct rebind {
    using other = simple_allocator<U>;
  };
};

int main(int argc, char* argv[])
{
  return 0;
}
