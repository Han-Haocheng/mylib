//
// Created by HanHaocheng on 2024/6/28.
//

#include "mutex.h"

MYLIB_BEGIN

Mutex::Mutex() : m_is_locked(false), m_mutex() { create_mutex(); }
Mutex::~Mutex() { destroy_mutex(); }

void Mutex::lock() { lock_mutex(); }
bool Mutex::try_lock() { return !m_is_locked && (lock_mutex(), true); }
void Mutex::unlock() {
  if (!m_is_locked) return;
  unlock_mutex();
}

void Mutex::create_mutex() {
#ifdef MYLIB_MSVC
  m_mutex = CreateMutex(nullptr, FALSE, nullptr);
  if (!m_mutex) {
    MYLIB_THROW("create_mutex error");
  }
#elif MYLIB_GUN
  pthread_mutex_init(&m_mutex, nullptr);
#endif// MYLIB_MSVC==1
}
void Mutex::destroy_mutex() {
#ifdef MYLIB_MSVC
  if (CloseHandle(m_mutex) != TRUE) {
    MYLIB_THROW("destroy_mutex error");
  }
#elif MYLIB_GUN
  pthread_mutex_destroy(&m_mutex);
#endif// MYLIB_MSVC==1
}
void Mutex::lock_mutex() {
#ifdef MYLIB_MSVC
  WaitForSingleObject(m_mutex, INFINITE);
#elif MYLIB_GUN
  pthread_mutex_lock(&m_mutex);
#endif// MYLIB_MSVC==1
  m_is_locked = true;
}
void Mutex::unlock_mutex() {
#ifdef MYLIB_MSVC
  if (ReleaseMutex(m_mutex)) {
    MYLIB_THROW("unlock_mutex error");
  }
#elif MYLIB_GUN
  pthread_mutex_unlock(&m_mutex);
#endif// MYLIB_MSVC==1
  m_is_locked = false;
}



Spinlock::Spinlock() : m_spinlock() { create_spinlock(); }
Spinlock::~Spinlock() { destroy_spinlock(); }

bool Spinlock::try_lock() { return try_lock_spinlock(); }
void Spinlock::lock() { lock_spinlock(); }
void Spinlock::unlock() { unlock_spinlock(); }

void Spinlock::create_spinlock() {
#ifdef MYLIB_MSVC
  InitializeCriticalSectionAndSpinCount(&m_spinlock, -1);
#elif MYLIB_GUN
  pthread_spin_init(&m_spinlock, 0);
#endif// MYLIB_MSVC==1
}
void Spinlock::destroy_spinlock() {
#ifdef MYLIB_MSVC
  DeleteCriticalSection(&m_spinlock);
#elif MYLIB_GUN
  pthread_spin_destroy(&m_spinlock);
#endif// MYLIB_MSVC==1
}
bool Spinlock::try_lock_spinlock() {
#ifdef MYLIB_MSVC
  return TryEnterCriticalSection(&m_spinlock);
#elif MYLIB_GUN
  return !pthread_spin_trylock(&m_spinlock);
#endif// MYLIB_MSVC==1
}
void Spinlock::lock_spinlock() {
#ifdef MYLIB_MSVC
  EnterCriticalSection(&m_spinlock);
#elif MYLIB_GUN
  pthread_spin_lock(&m_spinlock);
#endif// MYLIB_MSVC==1
}
bool Spinlock::unlock_spinlock() {
#ifdef MYLIB_MSVC
  LeaveCriticalSection(&m_spinlock);
#elif MYLIB_GUN
  pthread_spin_unlock(&m_spinlock);
#endif// MYLIB_MSVC==1
  return true;
}



RWLock::RWLock() : m_state(RWS_IDLE), m_rwlock() { create_rwlock(); }
RWLock::~RWLock() { destroy_rwlock(); }

void RWLock::rdLock() { read_lock(); }
void RWLock::wrLock() { write_lock(); }
bool RWLock::try_rdLock() { return m_state == RWS_WRITE && (read_lock(), true); }
bool RWLock::try_wrLock() { return m_state == RWS_WRITE && (write_lock(), false); }

void RWLock::unlock() {
  if (m_state == RWS_READ) {
    read_unlock();
  } else if (m_state == RWS_WRITE) {
    write_unlock();
  }
}

void RWLock::create_rwlock() {
#ifdef MYLIB_MSVC
  InitializeSRWLock(&m_rwlock);
#elif MYLIB_GUN
  pthread_rwlock_init(&m_rwlock, nullptr);
#endif// MYLIB_MSVC==1
}

void RWLock::destroy_rwlock() {
#ifdef MYLIB_MSVC
#elif MYLIB_GUN
  pthread_rwlock_destroy(&m_rwlock);
#endif// MYLIB_MSVC==1
}

void RWLock::read_lock() {
#ifdef MYLIB_MSVC
  AcquireSRWLockShared(&m_rwlock);
#elif MYLIB_GUN
  pthread_rwlock_rdlock(&m_rwlock);
#endif// MYLIB_MSVC==1
  m_state = RWS_READ;
}

void RWLock::write_lock() {
#ifdef MYLIB_MSVC
  AcquireSRWLockExclusive(&m_rwlock);
#elif MYLIB_GUN
  pthread_rwlock_wrlock(&m_rwlock);
#endif// MYLIB_MSVC==1
  m_state = RWS_WRITE;
}

void RWLock::read_unlock() {
#ifdef MYLIB_MSVC
  ReleaseSRWLockShared(&m_rwlock);
#elif MYLIB_GUN
  pthread_rwlock_unlock(&m_rwlock);
#endif// MYLIB_MSVC==1
  m_state = RWS_IDLE;
}

void RWLock::write_unlock() {
#ifdef MYLIB_MSVC
  ReleaseSRWLockExclusive(&m_rwlock);
#elif MYLIB_GUN
  pthread_rwlock_unlock(&m_rwlock);
#endif// MYLIB_MSVC==1
  m_state = RWS_IDLE;
}


CASLock::CASLock() : m_atomic_flag() { m_atomic_flag.clear(); }

void CASLock::lock() {
  while (std::atomic_flag_test_and_set_explicit(&m_atomic_flag, std::memory_order_acquire))
    Sleep(0);
}

void CASLock::unlock() {
  std::atomic_flag_clear_explicit(&m_atomic_flag, std::memory_order_release);
}

MYLIB_END