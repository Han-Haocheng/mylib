//
// Created by HanHaocheng on 2024/4/21.
//

#ifndef MYPROJECT_SEMAPHORE_H
#define MYPROJECT_SEMAPHORE_H

#include "../core/mylib_def.h"
#include <semaphore.h>

MYLIB_SPACE_BEGIN

class Semaphore {
public:
  explicit Semaphore(int init_count = 0);
  ~Semaphore();

  void wait();
  void wait(time_t sec, int64 nanosecond = 0);
  void post();

private:
  sem_t m_sem;
};

MYLIB_SPACE_END
#endif//MYPROJECT_SEMAPHORE_H
