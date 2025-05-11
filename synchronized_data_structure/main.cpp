#include <mutex>
#include <iostream>
#include <utility>

template<typename T>
class sync_data {
public:
  template<typename... Ts>
  sync_data(Ts&&... args)
    : m_data { std::forward<Ts>(args)... }
  {
  }

  class sync_lock {
  public:
    sync_lock(T& data, std::mutex& mutex)
      : m_data { data }
      , m_lock { mutex } // 这边会自动加锁
    {
      std::cout << "Lock" << std::endl;
    }

    ~sync_lock()
    {
      std::cout << "Unlock" << std::endl;
    }

    T* operator->() noexcept
    {
      return &m_data;
    }

    T& operator*() noexcept
    {
      return m_data;
    }

  private:
    T& m_data;
    std::lock_guard<std::mutex> m_lock;
  };

  auto lock() -> sync_lock
  {
    return { m_data, m_mutex };
  }

private:
  T m_data;
  std::mutex m_mutex;
};

int main(int argc, char* argv[])
{
  sync_data<std::string> v { "hello world" };
  auto m = v.lock();
  *m += " testing";
  std::cout << *m << std::endl;
  return 0;
}

