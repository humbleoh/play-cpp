#include <concepts>
#include <coroutine>
#include <exception>
#include <iostream>
#include <utility>

class task_ctrlblk;

class task {
public:
  using promise_type = task_ctrlblk;

  task() = default;
  task(task_ctrlblk& p);

  bool await_ready();
  std::suspend_always await_suspend(std::coroutine_handle<task> handle);
  int await_resume();

private:
  std::coroutine_handle<> m_handle;
};

class task_ctrlblk {
public:
  task get_return_object();
  std::suspend_always initial_suspend();
  std::suspend_always final_suspend();
  void unhandled_exception();
};

class awaiter {
public:
  awaiter();
  bool await_ready();
  std::suspend_always await_suspend(std::coroutine_handle<task> handle);
  int await_resume();
private:
  task_ctrlblk& m_task_ctrlblk;
};

int main(int argc, char* argv[])
{
  return 0;
}
