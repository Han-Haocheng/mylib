//
// Created by HanHaocheng on 2024/6/28.
//

#include "spinlock.h"

MYLIB_SPACE_BEGIN

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

MYLIB_SPACE_END