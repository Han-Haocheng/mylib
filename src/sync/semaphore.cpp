//
// Created by HanHaocheng on 2024/6/27.
//

#include "semaphore.h"

MYLIB_BEGIN

Semaphore::Semaphore(int32 init_count)
    : m_sem_count(init_count), m_sem() {
  create_semaphore();
}

Semaphore::~Semaphore() {
  destroy_semaphore();
}

void Semaphore::wait() {
  wait_semaphore();
}

void Semaphore::timedwait(time_t sec, int32 nanosecond) {
  timespec ts{sec, nanosecond};
  timedwait_semaphore(ts);
}

void Semaphore::post() {
  post_semaphore();
}

void Semaphore::create_semaphore() {
#ifdef MYLIB_MSVC
  m_sem = CreateSemaphore(nullptr, m_sem_count, m_sem_count, nullptr);
#elif MYLIB_GUN
  // 初始化信号量为1，表示资源可用
  if (sem_init(&my_semaphore, 0, m_sem_count) == -1) {
    MYLIB_THROW("create_semaphore error");
  }
#endif// MYLIB_MSVC==1
}

void Semaphore::destroy_semaphore() {
#ifdef MYLIB_MSVC
  CloseHandle(m_sem);
#elif MYLIB_GUN
  sem_destroy(&m_sem);
#endif// MYLIB_MSVC==1
}

void Semaphore::wait_semaphore() {
#ifdef MYLIB_MSVC
  if (WAIT_FAILED == WaitForSingleObject(m_sem, INFINITE))
#elif MYLIB_GUN
  if (sem_wait(&m_sem) == -1)
#endif// MYLIB_MSVC==1
  {
    MYLIB_THROW("wait_semaphore error");
  }
}

void Semaphore::timedwait_semaphore(const timespec &ts) {
#ifdef MYLIB_MSVC
  switch (WaitForSingleObject(m_sem, DWORD(ts.tv_sec * 1000 + ts.tv_nsec / 1000000))) {
    case WAIT_OBJECT_0:
      return;
    case WAIT_TIMEOUT:
      MYLIB_THROW("semaphore wait time out");
    case WAIT_FAILED:
    default:
      MYLIB_THROW("timedwait_semaphore error");
  }
#elif MYLIB_GUN
  if (sem_timedwait(&m_sem, &ts) == -1) {
    MYLIB_THROW(erron == ETIMEDOUT ? "semaphore wait time out" : "timedwait_semaphore error")
  }
#endif// MYLIB_MSVC==1
}

void Semaphore::post_semaphore() {
#ifdef MYLIB_MSVC
  ReleaseSemaphore(m_sem, 1, nullptr);
#elif MYLIB_GUN
  sem_post(&m_sem);
#endif// MYLIB_MSVC==1
}

MYLIB_END