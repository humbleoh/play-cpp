#include <deque>
#include <iostream>
#include <map>

#include <cassert>
#include <cstddef>
#include <cstdint>

#define TRACE() std::cout << __LINE__ << std::endl;

class lru_cache {
public:
  explicit lru_cache(std::size_t capacity)
    : capacity_{ capacity }
  {
  }

  bool get(int key, char& value)
  {
    auto it = cache_.find(key);
    if (it == std::end(cache_)) {
      return false;
    }
    value = it->second->second;
    return true;
  }

  bool put(int key, char value)
  {
    bool retired = false;
    auto it = cache_.find(key);
    if (it == std::end(cache_)) {
      if (fifo_queue_.size() >= capacity_) {
        auto node = fifo_queue_.front();
        fifo_queue_.pop_front();
        cache_.erase(node.first);
        retired = true;
      }
      auto nit = fifo_queue_.insert(std::end(fifo_queue_), {key, value});
      cache_.emplace(key, nit);
    } else {
      fifo_queue_.erase(it->second);
      auto nit = fifo_queue_.insert(std::end(fifo_queue_), {key, value});
      it->second = nit;
    }

    return retired;
  }

  std::size_t size()
  {
    return fifo_queue_.size();
  }

private:
  std::size_t capacity_{ 0 };
  std::size_t size_{ 0 };
  using Node = std::pair<int, char>;
  std::deque<Node> fifo_queue_;
  using NodeIterator = decltype(fifo_queue_)::iterator;
  std::map<int, NodeIterator> cache_;
};

int main(int argc, char* argv[])
{
  lru_cache cache { 5 };
  std::cout << "size: " << cache.size() << std::endl;
  char value = 'A';;
  int key = 1;
  bool rb = cache.put(key, value);
  std::cout << "put: " << rb << " key: " << key << " value: " << value << std::endl;
  std::cout << "size: " << cache.size() << std::endl;
  rb = cache.get(key, value);
  std::cout << "get: " << rb << " key: " << key << " value: " << value << std::endl;
  key = 0;
  rb = cache.get(key, value);
  std::cout << "get: " << rb << " key: " << key << " value: " << value << std::endl;
  value = 'B';;
  key = 1;
  rb = cache.put(key, value);
  std::cout << "put: " << rb << " key: " << key << " value: " << value << std::endl;
  std::cout << "size: " << cache.size() << std::endl;
  key = 1;
  rb = cache.get(key, value);
  std::cout << "get: " << rb << " key: " << key << " value: " << value << std::endl;
  value = 'Z';;
  key = 2;
  rb = cache.put(key, value);
  std::cout << "put: " << rb << " key: " << key << " value: " << value << std::endl;
  std::cout << "size: " << cache.size() << std::endl;
  value = 'Y';;
  key = 3;
  rb = cache.put(key, value);
  std::cout << "put: " << rb << " key: " << key << " value: " << value << std::endl;
  std::cout << "size: " << cache.size() << std::endl;
  value = 'X';;
  key = 4;
  rb = cache.put(key, value);
  std::cout << "put: " << rb << " key: " << key << " value: " << value << std::endl;
  std::cout << "size: " << cache.size() << std::endl;
  value = 'W';;
  key = 5;
  rb = cache.put(key, value);
  std::cout << "put: " << rb << " key: " << key << " value: " << value << std::endl;
  std::cout << "size: " << cache.size() << std::endl;
  value = 'V';;
  key = 6;
  rb = cache.put(key, value);
  std::cout << "put: " << rb << " key: " << key << " value: " << value << std::endl;
  std::cout << "size: " << cache.size() << std::endl;
  key = 1;
  rb = cache.get(key, value);
  std::cout << "get: " << rb << " key: " << key << " value: " << value << std::endl;
  value = '-';;
  key = 2;
  rb = cache.put(key, value);
  std::cout << "put: " << rb << " key: " << key << " value: " << value << std::endl;
  std::cout << "size: " << cache.size() << std::endl;
  value = 'A';;
  key = 7;
  rb = cache.put(key, value);
  std::cout << "put: " << rb << " key: " << key << " value: " << value << std::endl;
  std::cout << "size: " << cache.size() << std::endl;
  key = 3;
  rb = cache.get(key, value);
  std::cout << "get: " << rb << " key: " << key << " value: " << value << std::endl;
  return 0;
}
