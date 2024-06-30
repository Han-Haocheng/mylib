//
// Created by HanHaocheng on 2024/6/27.
//

#include "rwlock.h"

MYLIB_SPACE_BEGIN

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

MYLIB_SPACE_END