//
// Created by HanHaocheng on 2024/4/19.
//

#include "caslock.h"


#ifdef MYLIB_MSVC
#include <Windows.h>
#elif MYLIB_GUN
#include <pthread.h>
#endif// MYLIB_MSVC
MYLIB_SPACE_BEGIN

CASLock::CASLock() : m_atomic_flag() { m_atomic_flag.clear(); }

void CASLock::lock() {
  while (std::atomic_flag_test_and_set_explicit(&m_atomic_flag, std::memory_order_acquire))
    Sleep(0);

}

void CASLock::unlock() {
  std::atomic_flag_clear_explicit(&m_atomic_flag, std::memory_order_release);
}

MYLIB_SPACE_END