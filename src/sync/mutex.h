//
// Created by HanHaocheng on 2024/6/28.
//

#ifndef MYLIB_SLN_MUTEX_H
#define MYLIB_SLN_MUTEX_H

#include "../core/core.h"

#ifdef MYLIB_MSVC
#include <windows.h>
#elif MYLIB_GUN
#include <pthread.h>
#endif// MYLIB_MSVC

MYLIB_BEGIN

#ifdef MYLIB_MSVC
using mutex_t = HANDLE;
#elif MYLIB_GUN
using mutex_t = pthread_mutex_t;
#endif// MYLIB_MSVC

/**
 * @brief 互斥锁
 */
class Mutex {
public:
  Mutex();
  ~Mutex();
  void lock();
  bool try_lock();
  void unlock();

private:
  void create_mutex();
  void destroy_mutex();
  void lock_mutex();
  void unlock_mutex();

private:
  bool m_is_locked;
  mutex_t m_mutex;
};


#ifdef MYLIB_MSVC
using spinlock_t = CRITICAL_SECTION;
#elif MYLIB_GUN
using spinlock_t = pthread_spinlock_t;
#endif// MYLIB_MSVC

/**
 * @brief 自旋锁
 * */
class Spinlock {
public:
  Spinlock();
  ~Spinlock();
  void lock();
  bool try_lock();
  void unlock();

private:
  void create_spinlock();
  void destroy_spinlock();
  bool try_lock_spinlock();
  void lock_spinlock();
  bool unlock_spinlock();

private:
  spinlock_t m_spinlock;
};



#ifdef MYLIB_MSVC
using rwlock_t = SRWLOCK;
#elif MYLIB_GUN
using rwlock_t = pthread_rwlock_t;
#endif// MYLIB_MSVC

/**
 * @brief 读写锁
 * */
class RWLock {
public:
  RWLock();
  ~RWLock();
  void rdLock();
  void wrLock();
  bool try_rdLock();
  bool try_wrLock();
  void unlock();

private:
  void create_rwlock();
  void destroy_rwlock();
  void read_lock();
  void write_lock();
  void read_unlock();
  void write_unlock();

private:
  enum State
  {
    RWS_IDLE = 0x1,
    RWS_READ = 0x2,
    RWS_WRITE = 0x4,
  };

  State m_state;
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

MYLIB_END
#endif//MYLIB_SLN_MUTEX_H
