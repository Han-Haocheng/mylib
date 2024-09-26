//
// Created by HanHaocheng on 2024/4/19.
//

#ifndef MYLIB_THREAD_H
#define MYLIB_THREAD_H

#include "../base/mylib_def.h"
#include <utility>

MYLIB_BEGIN
/**
 * @brief 交换锁
 */
class CASLock {
public:
  CASLock();
  ~CASLock() = default;
  void lock();
  void unlock();

private:
  volatile std::atomic_flag m_atomic_flag;
};
MYLIB_END

#endif//MYLIB_THREAD_H
