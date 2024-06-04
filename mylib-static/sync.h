//
// Created by HanHaocheng on 2024/4/19.
//

#ifndef MYLIB_THREAD_H
#define MYLIB_THREAD_H
#include "mylib_def.h"

#include <functional>
#include <stack>
#include <utility>

#ifdef MYLIB_MSVC
#include <windows.h>
#elif MYLIB_GUN
#include <pthread.h>
#include <semaphore.h>
#include <ucontext.h>
#endif// MYLIB_MSVC

MYLIB_SPACE_BEGIN

#ifdef MYLIB_MSVC
using tid_t = DWORD;// 线程id类型
using thread_t = HANDLE;
using mutex_t = HANDLE;
using rwlock_t = SRWLOCK;
using spinlock_t = CRITICAL_SECTION;
using semaphore_t = HANDLE;

constexpr const static int32 INF_TIME = INFINITE;

#elif MYLIB_GUN
using tid_t = uint32;// 线程id类型
using thread_t = pthread_t;
using mutex_t = pthread_mutex_t;
using rwlock_t = pthread_rwlock_t;
using spinlock_t = pthread_spinlock_t;
using semaphore_t = sem_t;
constexpr const static int32 INF_TIME = 0;
#endif// MYLIB_MSVC

/**
 * @brief 互斥锁
 */
class Mutex {
public:
  Mutex();
  ~Mutex();
  void lock();
  void unlock();

private:
  mutex_t m_mutex;
};

/**
 * @brief 自旋锁
 * */
class Spinlock {
public:
  Spinlock();
  ~Spinlock();
  void lock();
  void unlock();

private:
  spinlock_t m_spinlock;
};

/**
 * @brief 读写锁
 * */
class RWLock {
public:
  RWLock();
  ~RWLock();
  void rlock();
  void wlock();
  void unlock();

private:
  rwlock_t m_rwlock;
};

/**
 * @brief 交换锁
 */
class CASLock {
public:
  CASLock();
  ~CASLock() = default;
  void lock();
  void unlock();

private:
  volatile std::atomic_flag m_atomic_flag;
};

/**
 * @brief 信号量
 * */
class Semaphore {
public:
  explicit Semaphore(int32 init_count = 0);
  ~Semaphore();

  void wait(time_t sec = INF_TIME, int32 nanosecond = 0);
  void post();

private:
  semaphore_t m_sem;
};

#ifdef MYLIB_MSVC
using coroutine_t = LPVOID;
#elif MYLIB_GUN
using coroutine_t = ucontext_t;
#endif// MYLIB_MSVC

/**
 * @brief 协程类
 * */
class Coroutine {
public:
  using ptr = std::shared_ptr<Coroutine>;
  using size_type = size_t;
  using function_t = std::function<void(void)>;
  enum state_t
  {
    UNDEFINED = 0x0,
    READY = 0x1,
    RUNNING = 0x2,
    SUSPENDED = 0x4,
    TERMINATED = 0x8,
    DEAD = 0x10,
  };
  explicit Coroutine(Coroutine::size_type stack_size, function_t cb);
  ~Coroutine();
  void yield();
  void resume();
  // bool reset(const function_t &cb);

  static void ConvertFromThread();
  static void ConvertToThread();

private:
  static void run(Coroutine *cor);
  static void set_coroutine(coroutine_t *to);
  void create_coroutine();
  void destroy_coroutine();

private:
  cid_t m_cid;
  state_t m_state;
  char *m_stack;
  size_type m_stack_size;

  function_t m_cb;
  coroutine_t m_coroutine;

  static cid_t s_current_id;// todo id alloc

  static thread_local coroutine_t t_main_coroutine;
  static thread_local coroutine_t *t_current_coroutine;
};

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
  ~Thread();

  [[nodiscard]] tid_t thread_id() const { return m_thread_id; }
  [[nodiscard]] String thread_name() const { return m_thread_name; }

  void join();
  void detach();

private:
  static void run(Thread *thread);

  void thread_join();
  void thread_detach();
  void thread_create();

private:
  tid_t m_thread_id;
  String m_thread_name;
  std::function<void()> m_cb;

  thread_t m_thread;
  bool m_is_fiber_thread;

  //Semaphore m_sem;
  static thread_local String t_current_thread_name;
  static thread_local Thread *t_current_thread;
};

class Scheduler {
public:
  using function = std::function<void(void)>;
  void schedule(function cb);
  void schedule(Coroutine::ptr co);

private:
  std::vector<Thread::ptr> m_thread_pool;
};

MYLIB_SPACE_END
#endif//MYLIB_THREAD_H
