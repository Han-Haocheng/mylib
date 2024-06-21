//
// Created by HanHaocheng on 2024/4/19.
//

#include "sync.h"

#include "../log/log.h"
#include <utility>

MYLIB_SPACE_BEGIN

static Logger::ptr s_logger = MYLIB_ROOT_LOGGER;

Mutex::Mutex() : m_mutex() {
#ifdef MYLIB_MSVC
  m_mutex = CreateMutex(nullptr, FALSE, nullptr);
  if (!m_mutex) {
    //todo error
  }
#elif MYLIB_GUN
  pthread_mutex_init(&m_mutex, nullptr);
#endif// MYLIB_MSVC==1
}

Mutex::~Mutex() {
#ifdef MYLIB_MSVC
  if (CloseHandle(m_mutex) != TRUE) {
    //todo error
  }
#elif MYLIB_GUN
  pthread_mutex_destroy(&m_mutex);
#endif// MYLIB_MSVC==1
}

void Mutex::lock() {
#ifdef MYLIB_MSVC
  WaitForSingleObject(m_mutex, INFINITE);
#elif MYLIB_GUN
  pthread_mutex_lock(&m_mutex);
#endif// MYLIB_MSVC==1
}
void Mutex::unlock() {
#ifdef MYLIB_MSVC
  if (ReleaseMutex(m_mutex)) {
    //todo error
  }
#elif MYLIB_GUN
  pthread_mutex_unlock(&m_mutex);
#endif// MYLIB_MSVC==1
}

CASLock::CASLock() : m_atomic_flag() { m_atomic_flag.clear(); }

void CASLock::lock() {
  while (std::atomic_flag_test_and_set_explicit(
      &m_atomic_flag,
      std::memory_order_acquire))
    ;
}

void CASLock::unlock() {
  std::atomic_flag_clear_explicit(
      &m_atomic_flag,
      std::memory_order_release);
}

Spinlock::Spinlock() : m_spinlock() {
#ifdef MYLIB_MSVC
  InitializeCriticalSectionAndSpinCount(&m_spinlock, -1);
#elif MYLIB_GUN
  pthread_spin_init(&m_spinlock, 0);
#endif// MYLIB_MSVC==1
}
Spinlock::~Spinlock() {
#ifdef MYLIB_MSVC
  DeleteCriticalSection(&m_spinlock);
#elif MYLIB_GUN
  pthread_spin_destroy(&m_spinlock);
#endif// MYLIB_MSVC==1
}
void Spinlock::lock() {
#ifdef MYLIB_MSVC
  while (TryEnterCriticalSection(&m_spinlock) == 0) Sleep(0);
#elif MYLIB_GUN
  pthread_spin_lock(&m_spinlock);
#endif// MYLIB_MSVC==1
}
void Spinlock::unlock() {
#ifdef MYLIB_MSVC
  LeaveCriticalSection(&m_spinlock);
#elif MYLIB_GUN
  pthread_spin_unlock(&m_spinlock);
#endif// MYLIB_MSVC==1
}

RWLock::RWLock() : m_rwlock() {
#ifdef MYLIB_MSVC
  InitializeSRWLock(&m_rwlock);
#elif MYLIB_GUN
  pthread_rwlock_init(&m_rwlock, nullptr);
#endif// MYLIB_MSVC==1
}

RWLock::~RWLock() {

#ifdef MYLIB_MSVC
#elif MYLIB_GUN
  pthread_rwlock_destroy(&m_rwlock);
#endif// MYLIB_MSVC==1
}

void RWLock::rlock() {
#ifdef MYLIB_MSVC
  AcquireSRWLockShared(&m_rwlock);
#elif MYLIB_GUN
  pthread_rwlock_rdlock(&m_rwlock);
#endif// MYLIB_MSVC==1
}

void RWLock::wlock() {
#ifdef MYLIB_MSVC
  AcquireSRWLockExclusive(&m_rwlock);
#elif MYLIB_GUN
  pthread_rwlock_wrlock(&m_rwlock);
#endif// MYLIB_MSVC==1
}

void RWLock::unlock() {
#ifdef MYLIB_MSVC
  ReleaseSRWLockExclusive(&m_rwlock);
  ReleaseSRWLockShared(&m_rwlock);
#elif MYLIB_GUN
  pthread_rwlock_unlock(&m_rwlock);
#endif// MYLIB_MSVC==1
}

Semaphore::Semaphore(int32 init_count) : m_sem() {
#ifdef MYLIB_MSVC
  m_sem = CreateSemaphore(nullptr,
                          init_count,
                          init_count,
                          nullptr);
#elif MYLIB_GUN
  sem_init(&m_sem, 0, init_count);
#endif// MYLIB_MSVC==1
}

Semaphore::~Semaphore() {
#ifdef MYLIB_MSVC
  CloseHandle(m_sem);
#elif MYLIB_GUN
  sem_destroy(&m_sem);
#endif// MYLIB_MSVC==1
}

void Semaphore::wait(time_t sec, int32 nanosecond) {
  timespec ts{sec, nanosecond};
#ifdef MYLIB_MSVC
  WaitForSingleObject(m_sem,
                      ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
#elif MYLIB_GUN
  if (sec == INF_TIME) {
    sem_wait(&m_sem);
  } else {
    sem_timedwait(&m_sem, &ts);
  }
#endif// MYLIB_MSVC==1
}
void Semaphore::post() {
#ifdef MYLIB_MSVC
  ReleaseSemaphore(m_sem, 1, nullptr);
#elif MYLIB_GUN
  sem_post(&m_sem);
#endif// MYLIB_MSVC==1
}

MYLIB_SPACE_END