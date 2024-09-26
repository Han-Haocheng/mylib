//
// Created by HanHaocheng on 2024/6/28.
//

#ifndef MYLIB_SLN_SPINLOCK_H
#define MYLIB_SLN_SPINLOCK_H
#include "../base/mylib_def.h"
#ifdef MYLIB_MSVC
#include <windows.h>
#elif MYLIB_GUN
#include <pthread.h>
#endif// MYLIB_MSVC

MYLIB_BEGIN

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

MYLIB_END

#endif//MYLIB_SLN_SPINLOCK_H
