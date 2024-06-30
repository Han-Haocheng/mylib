//
// Created by HanHaocheng on 2024/6/27.
//

#ifndef MYLIB_SLN_RWLOCK_H
#define MYLIB_SLN_RWLOCK_H

#include "../core/mylib_def.h"

#ifdef MYLIB_MSVC
#include <windows.h>
#elif MYLIB_GUN
#include <pthread.h>
#endif// MYLIB_MSVC

MYLIB_SPACE_BEGIN
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

MYLIB_SPACE_END

#endif//MYLIB_SLN_RWLOCK_H
