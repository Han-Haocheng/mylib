//
// Created by HanHaocheng on 2024/6/27.
//

#ifndef MYLIB_SLN_SEMAPHORE_H
#define MYLIB_SLN_SEMAPHORE_H

#include "../core/mylib_def.h"

#ifdef MYLIB_MSVC
#include <ctime>
#include <windows.h>
#elif MYLIB_GUN
#include <semaphore.h>
#endif// MYLIB_MSVC

MYLIB_SPACE_BEGIN

#if MYLIB_MSVC
using semaphore_t = HANDLE;
constexpr const static int32 INF_TIME = INFINITE;
#elif MYLIB_GUN
using semaphore_t = sem_t;
constexpr const static int32 INF_TIME = 0;
#endif

/**
 * @brief 信号量
 * */
class Semaphore {
public:
  explicit Semaphore(int32 init_count = 0);
  ~Semaphore();

  Semaphore(Semaphore &&) = delete;
  Semaphore(const Semaphore &) = delete;

  Semaphore &operator=(Semaphore &&) = delete;
  Semaphore &operator=(const Semaphore &) = delete;

  void wait();
  void timedwait(time_t sec, int32 nanosecond);
  void post();

private:
  void create_semaphore();
  void destroy_semaphore();
  void wait_semaphore();

  void timedwait_semaphore(const timespec &ts);
  void post_semaphore();

private:
  int32 m_sem_count;
  semaphore_t m_sem;
};

MYLIB_SPACE_END

#endif//MYLIB_SLN_SEMAPHORE_H
