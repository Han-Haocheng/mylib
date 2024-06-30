//
// Created by HanHaocheng on 2024/6/28.
//

#ifndef MYLIB_SLN_MUTEX_H
#define MYLIB_SLN_MUTEX_H

#include "../core/mylib_def.h"

#ifdef MYLIB_MSVC
#include <windows.h>
#elif MYLIB_GUN
#include <pthread.h>
#endif// MYLIB_MSVC

MYLIB_SPACE_BEGIN

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

MYLIB_SPACE_END
#endif//MYLIB_SLN_MUTEX_H
