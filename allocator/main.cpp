#include <iostream>
#include <map>
#include <mutex>
#include <new>

#include <cassert>
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

class heap_tracker {
public:
  static heap_tracker& get_instance()
  {
    static heap_tracker singleton;
    return singleton;
  }

  std::size_t size_allocated() const noexcept
  {
    return m_total_allocated;
  }

  void increase_size_allocated(void* p, std::size_t size)
  {
    std::lock_guard<std::mutex> lk { m_mutex };
    assert(m_map_size_track.find(p) == std::end(m_map_size_track));
    m_map_size_track[p] = size;
    m_total_allocated += size;
  }

  void decrease_size_allocated(void* p, std::size_t size = 0)
  {
    assert(m_total_allocated >= size);
    std::lock_guard<std::mutex> lk { m_mutex };
    assert(m_map_size_track.find(p) != std::end(m_map_size_track));
    assert(size == 0 || m_map_size_track[p] == size);
    m_total_allocated -= m_map_size_track[p];
    m_map_size_track.erase(p);;
  }

  heap_tracker(const heap_tracker&) = delete;
  heap_tracker(heap_tracker&&) = delete;
  heap_tracker& operator=(const heap_tracker&) = delete;
  heap_tracker& operator=(heap_tracker&&) = delete;

private:
  heap_tracker() = default;
  ~heap_tracker() = default;

private:
  using map_ptr_to_size = std::map<void*, std::size_t,
    std::less<void*>, simple_allocator<std::pair<void* const, std::size_t>>>;
  map_ptr_to_size m_map_size_track;
  std::size_t m_total_allocated = 0u;
  std::mutex m_mutex;
};

void* operator new(std::size_t size)
{
  auto& ht = heap_tracker::get_instance();
  auto p = malloc(size);
  ht.increase_size_allocated(p, size);
  return p;
}

void* operator new(std::size_t size, const std::nothrow_t& tag)
{
  auto& ht = heap_tracker::get_instance();
  auto p = malloc(size);
  ht.increase_size_allocated(p, size);
  return p;
}

void operator delete(void* p)
{
  auto& ht = heap_tracker::get_instance();
  ht.decrease_size_allocated(p);
  free(p);
}


void operator delete(void* p, std::size_t size)
{
  auto& ht = heap_tracker::get_instance();
  ht.decrease_size_allocated(p, size);
  free(p);
}

int main(int argc, char* argv[])
{
  auto& ht = heap_tracker::get_instance();
  std::cout << ht.size_allocated() << std::endl;
  auto p1 = new int32_t { 100 };
  std::cout << ht.size_allocated() << std::endl;
  auto p2 = new int64_t { 200 };
  std::cout << ht.size_allocated() << std::endl;
  delete p1;
  std::cout << ht.size_allocated() << std::endl;
  delete p2;
  std::cout << ht.size_allocated() << std::endl;
  return 0;
}
