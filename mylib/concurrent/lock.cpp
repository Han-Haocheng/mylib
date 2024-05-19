//
// Created by HanHaocheng on 2024/4/21.
//

#include "lock.h"

MYLIB_SPACE_BEGIN

Mutex::Mutex() : m_mutex() {
  pthread_mutex_init(&m_mutex, nullptr);
}
Mutex::~Mutex() { pthread_mutex_destroy(&m_mutex); }
void Mutex::lock() {
  pthread_mutex_lock(&m_mutex);
}
void Mutex::unlock() {
  pthread_mutex_unlock(&m_mutex);
}

MYLIB_SPACE_END
mylib::RWLock::RWLock() : m_rwlock() {
  pthread_rwlock_init(&m_rwlock, nullptr);
}
mylib::RWLock::~RWLock() { pthread_rwlock_destroy(&m_rwlock); }
void mylib::RWLock::rdlock() {
  pthread_rwlock_rdlock(&m_rwlock);
}
void mylib::RWLock::wrlock() {
  pthread_rwlock_wrlock(&m_rwlock);
}
void mylib::RWLock::unlock() {
  pthread_rwlock_unlock(&m_rwlock);
}
mylib::CASLock::CASLock() : m_atomic_flag() { m_atomic_flag.clear(); }
void mylib::CASLock::lock() {
  while (std::atomic_flag_test_and_set_explicit(&m_atomic_flag, std::memory_order_acquire))
    ;
}
void mylib::CASLock::unlock() {
  std::atomic_flag_clear_explicit(&m_atomic_flag, std::memory_order_release);
}
mylib::Spinlock::Spinlock() : m_spinlock(0) { pthread_spin_init(&m_spinlock, 0); }
mylib::Spinlock::~Spinlock() { pthread_spin_destroy(&m_spinlock); }
void mylib::Spinlock::lock() { pthread_spin_lock(&m_spinlock); }
void mylib::Spinlock::unlock() { pthread_spin_unlock(&m_spinlock); }
