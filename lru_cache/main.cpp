#include <deque>
#include <iostream>
#include <map>
#include <sstream>

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
    upsert(it->second->first, it->second->second);
    return true;
  }

  bool put(int key, char value)
  {
    bool retired = false;
    if (fifo_queue_.size() >= capacity_) {
      int key_retired;
      retired = retire(key_retired);;
    }

    upsert(key, value);
    return retired;
  }

  bool retire(int& key)
  {
    if (fifo_queue_.empty()) {
      return false;
    }

    auto node = fifo_queue_.front();
    fifo_queue_.pop_front();
    cache_.erase(node.first);
    key = node.first;
    return true;
  }

  bool upsert(int key, char value)
  {
    auto it = cache_.find(key);
    if (it == std::end(cache_)) {
      auto nit = fifo_queue_.insert(std::end(fifo_queue_), {key, value});
      cache_.emplace(key, nit);
      return true;
    } else {
      // 可否优化，即复用迭代器？
      fifo_queue_.erase(it->second);
      auto nit = fifo_queue_.insert(std::end(fifo_queue_), {key, value});
      it->second = nit;
      return false; 
    }
  }

  std::size_t size()
  {
    return fifo_queue_.size();
  }

  std::string to_string()
  {
    std::stringstream os;
    os << "cache=>";
    for (auto p: fifo_queue_) {
      os << "{" << p.first << "," << p.second << "},";
    }

    return os.str();
  }

private:
  std::size_t capacity_{ 0 };
  std::size_t size_{ 0 };
  using Node = std::pair<int, char>;
  // 如何高效操纵 fifo_queue_ 的迭代器，比如迭代器有没有办法出队后再重新入队并
  // 且可以复用迭代器。
  std::deque<Node> fifo_queue_;
  using NodeIterator = decltype(fifo_queue_)::iterator;
  // 访问模式：(map: key -> queue::iterator) -> (queue::iterator -> value)
  std::map<int, NodeIterator> cache_;
};

int main(int argc, char* argv[])
{
  lru_cache cache { 5 };
  std::cout << "size: " << cache.size() << std::endl;
  char value = 'A';;
  int key = 1;
  bool rb = cache.put(key, value);
  assert(!rb);
  assert(cache.size() == 1);
  std::cout << "put: " << rb << " key: " << key << " value: " << value << std::endl;
  std::cout << "size: " << cache.size() << std::endl;
  std::cout << cache.to_string() << std::endl;

  key = 1;
  rb = cache.get(key, value);
  std::cout << "get: " << rb << " key: " << key << " value: " << value << std::endl;
  std::cout << cache.to_string() << std::endl;
  assert(rb);
  assert(value == 'A');

  key = 0;
  rb = cache.get(key, value);
  std::cout << "get: " << rb << " key: " << key << " value: " << value << std::endl;
  std::cout << cache.to_string() << std::endl;
  assert(!rb);

  value = 'B';;
  key = 1;
  rb = cache.put(key, value);
  std::cout << "put: " << rb << " key: " << key << " value: " << value << std::endl;
  std::cout << "size: " << cache.size() << std::endl;
  std::cout << cache.to_string() << std::endl;
  assert(!rb);
  assert(cache.size() == 1);

  key = 1;
  rb = cache.get(key, value);
  std::cout << "get: " << rb << " key: " << key << " value: " << value << std::endl;
  std::cout << cache.to_string() << std::endl;
  assert(rb);
  assert(value == 'B');

  value = 'Z';;
  key = 2;
  rb = cache.put(key, value);
  std::cout << "put: " << rb << " key: " << key << " value: " << value << std::endl;
  std::cout << "size: " << cache.size() << std::endl;
  std::cout << cache.to_string() << std::endl;
  assert(!rb);
  assert(cache.size() == 2);

  value = 'Y';;
  key = 3;
  rb = cache.put(key, value);
  std::cout << "put: " << rb << " key: " << key << " value: " << value << std::endl;
  std::cout << "size: " << cache.size() << std::endl;
  std::cout << cache.to_string() << std::endl;
  assert(!rb);
  assert(cache.size() == 3);

  value = 'X';;
  key = 4;
  rb = cache.put(key, value);
  std::cout << "put: " << rb << " key: " << key << " value: " << value << std::endl;
  std::cout << "size: " << cache.size() << std::endl;
  std::cout << cache.to_string() << std::endl;
  assert(!rb);
  assert(cache.size() == 4);

  value = 'W';;
  key = 5;
  rb = cache.put(key, value);
  std::cout << "put: " << rb << " key: " << key << " value: " << value << std::endl;
  std::cout << "size: " << cache.size() << std::endl;
  std::cout << cache.to_string() << std::endl;
  assert(!rb);
  assert(cache.size() == 5);

  value = 'V';;
  key = 6;
  rb = cache.put(key, value);
  std::cout << "put: " << rb << " key: " << key << " value: " << value << std::endl;
  std::cout << "size: " << cache.size() << std::endl;
  std::cout << cache.to_string() << std::endl;
  assert(rb);
  assert(cache.size() == 5);

  key = 1;
  rb = cache.get(key, value);
  std::cout << "get: " << rb << " key: " << key << " value: " << value << std::endl;
  std::cout << cache.to_string() << std::endl;
  assert(!rb);

  value = '-';;
  key = 2;
  rb = cache.put(key, value);
  std::cout << "put: " << rb << " key: " << key << " value: " << value << std::endl;
  std::cout << "size: " << cache.size() << std::endl;
  std::cout << cache.to_string() << std::endl;
  assert(rb);
  assert(cache.size() == 5);

  value = 'A';;
  key = 7;
  rb = cache.put(key, value);
  std::cout << "put: " << rb << " key: " << key << " value: " << value << std::endl;
  std::cout << "size: " << cache.size() << std::endl;
  std::cout << cache.to_string() << std::endl;
  assert(rb);
  assert(cache.size() == 5);

  key = 3;
  rb = cache.get(key, value);
  std::cout << "get: " << rb << " key: " << key << " value: " << value << std::endl;
  std::cout << cache.to_string() << std::endl;
  assert(!rb);

  key = 4;
  rb = cache.get(key, value);
  std::cout << "get: " << rb << " key: " << key << " value: " << value << std::endl;
  std::cout << cache.to_string() << std::endl;
  assert(rb);
  assert(value == 'X');

  value = '=';
  key = 8;
  rb = cache.put(key, value);
  std::cout << "put: " << rb << " key: " << key << " value: " << value << std::endl;
  std::cout << "size: " << cache.size() << std::endl;
  std::cout << cache.to_string() << std::endl;
  assert(rb);
  assert(cache.size() == 5);

  key = 5;
  rb = cache.get(key, value);
  std::cout << "get: " << rb << " key: " << key << " value: " << value << std::endl;
  std::cout << cache.to_string() << std::endl;
  assert(!rb);
  assert(cache.size() == 5);

  return 0;
}
