#pragma once
#include "mylib_def.h"
#include "coroutine.h"

MYLIB_SPACE_BEGIN

#ifdef MYLIB_MSVC
using tid_t = DWORD;// 线程id类型
using thread_t = HANDLE;
#elif MYLIB_GUN
using tid_t = uint32;// 线程id类型
using thread_t = pthread_t;
#endif// MYLIB_MSVC

/**
 * @brief 线程类
 * */
class Thread {
public:
  using ptr = std::shared_ptr<Thread>;
  using wptr = std::weak_ptr<Thread>;

  static Thread *GetCurrentThread();
  static String GetCurrentThreadName();
  static tid_t GetCurrentThreadId();

  Thread(String name, std::function<void()> func, bool is_fiber_thread = false);
  Thread(Thread &&) = delete;
  Thread(const Thread &) = delete;
  ~Thread();

  Thread &operator=(Thread &&rhs) noexcept {
    if (&rhs == this) {
      return *this;
    }
    if (m_thread)
      thread_detach();
    m_use_fiber = rhs.m_use_fiber;
    m_id = rhs.m_id;
    m_name = rhs.m_name;
    m_cb = rhs.m_cb;
    m_thread = rhs.m_thread;

    return *this;
  }
  Thread &operator=(const Thread &) = delete;

  [[nodiscard]] tid_t thread_id() const { return m_id; }
  [[nodiscard]] String thread_name() const { return m_name; }

  void setName(String name) { m_name = name; }

  void join();
  void detach();

private:
  //Thread(tid_t id, thread_t thread);

  static void run(Thread *thread);
  void thread_join();
  void thread_detach();
  void thread_create();

  static thread_t thread_self() {
#ifdef MYLIB_MSVC
    return ::GetCurrentThread();
#elif MYLIB_GUN
    return pthread_self();
#endif
  }

private:
  bool m_use_fiber;
  tid_t m_id;
  String m_name;
  std::function<void()> m_cb;

  thread_t m_thread;

  //Semaphore m_sem;
  static thread_local String t_current_thread_name;
  static thread_local Thread *t_current_thread;
};


MYLIB_SPACE_END
