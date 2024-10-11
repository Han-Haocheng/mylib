#include "scheduler.h"

#include <memory>
#include <utility>
MYLIB_BEGIN

Scheduler::Scheduler(String name, size_t threadSize, bool useCaller)
    : m_is_stop(false),
      m_is_stopping(false),
      m_name(std::move(name)),
      m_thread_pool(),
      m_tasks() {
  size_t thread_size = threadSize;
  if (useCaller) {
    thread_size--;
  }
  m_thread_pool.assign(thread_size, nullptr);
}

void Scheduler::schedule(Task::ptr task) {
  if (!task) {
    return;
  }

  if (task->empty()) {
    return;
  }

  m_tasks.emplace_front(task);
}

void Scheduler::stop() {
  m_is_stop = true;
  Scheduler::Notify();
  for (auto &th: m_thread_pool) {
    th->join();
  }
}

void Scheduler::start() {
  size_t i = 0;
  m_is_stop = false;
  for (auto &th: m_thread_pool) {
    th = std::make_shared<Thread>(
        m_name + "_" + std::to_string(i++), [this] { run(this); }, true);
  }
}

void Scheduler::run(Scheduler *scheduler) {
  if (!scheduler) {
    return;
  }

  tid_t thread_id = Thread::GetCurrentThreadId();
  auto &sc = *scheduler;
  auto &tasks = sc.m_tasks;

  while (!sc.m_is_stop) {
    if (tasks.empty()) {
      Scheduler::Wait();
    }

    bool is_get_task = false;
    auto iter = tasks.begin();
    for (; iter != tasks.end(); ++iter) {
      if ((*iter)->occupiedThreadId() != 0 && (*iter)->occupiedThreadId() != thread_id) {
        Scheduler::Notify();
        continue;
      }
      is_get_task = true;
    }

    if (!is_get_task) {
      continue;
    }
    Task::ptr task_p = *iter;
    tasks.erase(iter);
    if (!task_p) {
      continue;
    }
    task_p->execute();
  }
}

MYLIB_END
