//
// Created by HanHaocheng on 2024/4/21.
//

#include "semaphore.h"
MYLIB_SPACE_BEGIN
Semaphore::Semaphore(int init_count) : m_sem() {
  sem_init(&m_sem, 0, init_count);
}
Semaphore::~Semaphore() {
  sem_destroy(&m_sem);
}
void Semaphore::wait() {
  sem_wait(&m_sem);
}
void Semaphore::wait(time_t sec, int64 nanosecond) {
  timespec ts{sec, nanosecond};
  sem_timedwait(&m_sem, &ts);
}
void Semaphore::post() {
  sem_post(&m_sem);
}

MYLIB_SPACE_END