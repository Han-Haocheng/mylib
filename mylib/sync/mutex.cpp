//
// Created by HanHaocheng on 2024/6/28.
//

#include "mutex.h"
#include "../exception/exception.h"
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

MYLIB_END