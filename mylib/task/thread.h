#ifndef MYLIB_THREAD_H_
#define MYLIB_THREAD_H_

#include "../core/core.h"
#include <functional>

#ifdef MYLIB_MSVC
#include <windows.h>
#endif// MYLIB_MSVC

MYLIB_BEGIN

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

  Thread(String name, std::function<void()> func);
  Thread(Thread &&) = delete;
  Thread(const Thread &) = delete;
  ~Thread();

  Thread &operator=(Thread &&rhs) noexcept;
  Thread &operator=(const Thread &) = delete;

  [[nodiscard]] tid_t thread_id() const { return m_id; }
  [[nodiscard]] String thread_name() const { return m_name; }

  void setName(String name);

  void join();
  void detach();

private:
  Thread(tid_t id, thread_t thread);

  static void run(Thread *thread);
  bool join_thread() const;
  bool detach_thread();
  bool create_thread();

  static thread_t self_thread();

private:
  tid_t m_id;
  String m_name;
  std::function<void()> m_cb;

  thread_t m_thread;

  //Semaphore m_sem;
  static thread_local String t_current_thread_name;
  static thread_local Thread *t_current_thread;
};

MYLIB_END
#endif// !MYLIB_THREAD_H_
