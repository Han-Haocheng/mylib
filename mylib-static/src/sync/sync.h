//
// Created by HanHaocheng on 2024/4/19.
//

#ifndef MYLIB_THREAD_H
#define MYLIB_THREAD_H
#include "../core/mylib_def.h"

#include <functional>
#include <stack>
#include <utility>

#ifdef MYLIB_MSVC
#include <windows.h>
#elif MYLIB_GUN
#include <pthread.h>
#include <semaphore.h>
#endif// MYLIB_MSVC

MYLIB_SPACE_BEGIN

#ifdef MYLIB_MSVC
using mutex_t = HANDLE;
using rwlock_t = SRWLOCK;
using spinlock_t = CRITICAL_SECTION;
using semaphore_t = HANDLE;

constexpr const static int32 INF_TIME = INFINITE;

#elif MYLIB_GUN
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

MYLIB_SPACE_END
#endif//MYLIB_THREAD_H
