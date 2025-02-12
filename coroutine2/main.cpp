#include <concepts>
#include <coroutine>
#include <exception>
#include <iostream>
#include <utility>

#include <cstddef>
#include <cstdint>

class task_ctrlblk;

class task {
public:
  using promise_type = task_ctrlblk;

  task() = default;
  task(task_ctrlblk& p);
  ~task();

  bool await_ready();
  void await_suspend(std::coroutine_handle<task_ctrlblk> handle);
  int await_resume();

  void resume();

private:
  std::coroutine_handle<promise_type> m_handle;
};

class task_ctrlblk {
public:
  task get_return_object();
  std::suspend_always initial_suspend();
  std::suspend_always final_suspend() noexcept;
  void unhandled_exception();
};

class awaiter {
public:
  awaiter() = default;
  bool await_ready();
  void await_suspend(std::coroutine_handle<task_ctrlblk> handle);
  int await_resume();

private:
  std::coroutine_handle<task_ctrlblk> m_handle;
};

bool awaiter::await_ready()
{
  return false;
}

void awaiter::await_suspend(std::coroutine_handle<task_ctrlblk> handle)
{
  m_handle = handle;
}

int awaiter::await_resume()
{
  return -1;
}

task task_ctrlblk::get_return_object()
{
  return task { *this };
}

std::suspend_always task_ctrlblk::initial_suspend()
{
  return {};
}

std::suspend_always task_ctrlblk::final_suspend() noexcept
{
  return {};
}

void task_ctrlblk::unhandled_exception()
{
}

task::task(task_ctrlblk& p)
  : m_handle { std::coroutine_handle<task_ctrlblk>::from_promise(p) }
{
}

task::~task()
{
  m_handle.destroy();
}

void task::resume()
{
  m_handle.resume();
}

bool task::await_ready()
{
  return false;
}

void task::await_suspend(std::coroutine_handle<task_ctrlblk> handle)
{
  m_handle.resume();
  handle.resume();
}

int task::await_resume()
{
  return -2;
}

task test_func()
{
  int i = 0;
  for (;;) {
    std::cout << "iterate: " << i++ << std::endl;
    int rc = co_await awaiter{};
    std::cout << "> co_await rc: " << rc << std::endl;
  }
}

int main(int argc, char* argv[])
{
  auto c =  test_func();
  c.resume();
  c.resume();
  c.resume();
  return 0;
}
