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
  void schedule();

private:
  std::coroutine_handle<promise_type> m_handle;
};

class task_ctrlblk {
public:
  task get_return_object();
  std::suspend_always initial_suspend();
  // final_suspend() 必须是 noexcept
  std::suspend_always final_suspend() noexcept;
  void unhandled_exception();
  void return_value(int v);

private:
  friend class task;
  friend class awaiter_counter;

  enum class awaiting_state : std::uint8_t {
    READING = 0u,
    WRITING = 1u,
    WAITING = 3u,
    IDLE    = 254u,
    RUNNING = 255u
  };

  using params_meta_type = union {
    std::size_t len_data;
    std::size_t timestamp_start;
    std::size_t counter;
  };

  using params_type = union {
    const void* ptr_data_in;
    void* ptr_data_out;
    std::uint32_t timeout_threshold;
    std::uint32_t counter_threshold;
  };

  params_meta_type m_meta;
  params_type m_params;
  awaiting_state m_state = awaiting_state::IDLE;
  int m_status = -1;
  int m_value = 0;
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

// awaiter 对象就是一个异步操作，构建 awaiter 对象时传入操作参数。
// 然后在 await_suspend(.) 中，又把操作参数传入给承诺体对象并把协程句柄注册到调
// 度器中。操作结束后，调度器会把操作结果写进承诺体对象，并恢复协程运行。
class awaiter_counter {
public:
  awaiter_counter() = default;
  bool await_ready();
  void await_suspend(std::coroutine_handle<task_ctrlblk> handle);
  int await_resume();

private:
  std::coroutine_handle<task_ctrlblk> m_handle;
};

bool awaiter_counter::await_ready()
{
  return false;
}

void awaiter_counter::await_suspend(std::coroutine_handle<task_ctrlblk> handle)
{
  m_handle = handle;
  auto&& p = m_handle.promise();
  p.m_status = 1;
  p.m_state = task_ctrlblk::awaiting_state::WAITING;
  p.m_meta.counter = 0;
  p.m_params.counter_threshold = 4;
}

int awaiter_counter::await_resume()
{
  auto&& p = m_handle.promise();
  return p.m_status;
}
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

void task_ctrlblk::return_value(int v)
{
  m_value = v;
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

void task::schedule()
{
  auto&& p = m_handle.promise();
  if (p.m_state == task_ctrlblk::awaiting_state::WAITING) {
    p.m_meta.counter++;
    if (p.m_meta.counter >= p.m_params.counter_threshold) {
      std::cout << "> Triggered timeri\n";
      m_handle.resume();
    }
  }
}

bool task::await_ready()
{
  return false;
}

void task::await_suspend(std::coroutine_handle<task_ctrlblk> handle)
{
  // 启动当下内层协程直至被挂起
  m_handle.resume();
  // 恢复外层协程
  handle.resume();
}

int task::await_resume()
{
  auto&& p = m_handle.promise();
  return p.m_value;
}

task test_func()
{
  int i = 0;
  for (;;) {
    std::cout << "iterate: " << i++ << std::endl;
    int rc = co_await awaiter_counter{};
    std::cout << "> co_await rc: " << rc << std::endl;
  }
}

task test_inner(int i)
{
  co_return i + 1000; 
}

task test_nested()
{
  int i = 0;
  for (;;) {
    int rc = co_await test_inner(i++);
    std::cout << "co_await task: " << rc << std::endl;
    co_await awaiter {};
  }
}

int main(int argc, char* argv[])
{
#if 0
  auto c =  test_func();
  c.resume();
  for (int i = 0; i < 4; ++i) {
    c.schedule();
  }
#endif
  auto c = test_nested();
  c.resume();
  c.resume();
  c.resume();
  c.resume();
  return 0;
}
