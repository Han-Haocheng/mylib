//
// Created by HanHaocheng on 2024/4/19.
//

#ifndef MYLIB_THREAD_H
#define MYLIB_THREAD_H
#include <pthread.h>
#include <utility>

#include "../core/mylib_def.h"

// #include "semaphore.h"

MYLIB_SPACE_BEGIN

class Logger;

class Thread {
public:
  using ptr = std::shared_ptr<Thread>;
  using wptr = std::weak_ptr<Thread>;

  static Thread *GetCurrentThread();
  static String GetCurrentThreadName();
  static tid_t GetCurrentThreadId();

  Thread(String name, std::function<void()> func);
  ~Thread();

  [[nodiscard]] tid_t thread_id() const { return m_thread_id; }
  [[nodiscard]] String thread_name() const { return m_thread_name; }

  void join();
  void detach() const;

private:
  static void run(Thread *thread);

private:
  tid_t m_thread_id;
  String m_thread_name;
  std::function<void()> m_cb;
  pthread_t m_thread;

  //Semaphore m_sem;
  thread_local static String t_current_thread_name;
  thread_local static Thread *t_current_thread;

  static std::shared_ptr<Logger> s_logger;
};

MYLIB_SPACE_END
#endif//MYLIB_THREAD_H
