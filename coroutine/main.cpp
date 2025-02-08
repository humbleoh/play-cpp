#include <concepts>
#include <coroutine>
#include <exception>
#include <iostream>
#include <utility>

struct future_type {
  std::coroutine_handle<> handle;

  void resume()
  {
    handle();
  }

  struct promise_type {
    future_type get_return_object()
    {
      std::cout << __PRETTY_FUNCTION__ << std::endl;
      return future_type { *this };
    }

    std::suspend_always initial_suspend()
    {
      std::cout << __PRETTY_FUNCTION__ << std::endl;
      return {};
    }

    std::suspend_never final_suspend() noexcept
    {
      std::cout << __PRETTY_FUNCTION__ << std::endl;
      return {};
    }

    void unhandled_exception()
    {
      std::cout << __PRETTY_FUNCTION__ << std::endl;
    }
  };

  explicit future_type(promise_type& p)
    : handle { std::coroutine_handle<promise_type>::from_promise(p) }
  {
      std::cout << __PRETTY_FUNCTION__ << std::endl;
  }

  future_type(future_type&& c)
    : handle { std::exchange(c.handle, nullptr) }
  {
      std::cout << __PRETTY_FUNCTION__ << std::endl;
  }
};

struct awaiter {
  bool await_ready() const noexcept
  {
    std::cout << __PRETTY_FUNCTION__ << " " << this << std::endl;
    return false;
  }

  void await_suspend(std::coroutine_handle<> h)
  {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }

  void await_resume() const noexcept
  {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }

  awaiter()
  {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }
  int i;
};

future_type counter(int terminal)
{
  awaiter a;
  for (int i = 0; i < terminal; ++i) {
    co_await awaiter{};
    std::cout << "counter seed=" << terminal << " " << i << std::endl;
  }
}

int main(int argc, char* argv[])
{
  auto h = counter(2);
  std::cout << "> start" << std::endl;
  h.resume();
  std::cout << "> resume1" << std::endl;
  h.resume();
  std::cout << "> resume2" << std::endl;
  h.resume();
  return 0;
}
