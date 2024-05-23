//
// Created by HanHaocheng on 2024/4/19.
//

#include "thread.h"
#include "../log/log.h"

MYLIB_SPACE_BEGIN

Logger::ptr Thread::s_logger = MYLIB_LOG_NAME("system");

thread_local String Thread::t_current_thread_name = "UNKNOWN";
thread_local Thread *Thread::t_current_thread = nullptr;

Thread *Thread::GetCurrentThread() {
  if (t_current_thread) {
    return t_current_thread;
  }
  return nullptr;
}

String Thread::GetCurrentThreadName() {
  return t_current_thread_name;
}

tid_t Thread::GetCurrentThreadId() {
  return gettid();
}

Thread::Thread(String name, std::function<void()> func)
    : m_thread_name(std::move(name)), m_thread_id(), m_cb(std::move(func)), m_thread() {
  if (m_thread_name.empty()) {
    m_thread_name = "UNKNOWN";
  }

  int32 rd = pthread_create(&m_thread,
                            nullptr,
                            reinterpret_cast<void *(*) (void *)>(&Thread::run),
                            this);
  if (rd) {
    MYLIB_LOG_ERROR(s_logger) << "pthread_create error, rd=" << rd;
  }
}

Thread::~Thread() {
  if (m_thread) {
    pthread_join(m_thread, nullptr);
    m_thread = 0;
  }
}

void Thread::run(Thread *thread) {
  // 初始化区
  t_current_thread = thread;
  t_current_thread_name = t_current_thread->m_thread_name;
  if (!t_current_thread->m_cb) {
    t_current_thread = nullptr;
    MYLIB_LOG_WARN(s_logger) << "thread run, but no callback";
    return;
  }

  t_current_thread->m_thread_id = GetCurrentThreadId();

  std::function<void()> func = nullptr;
  func.swap(t_current_thread->m_cb);

  // 功能区
  // MYLIB_LOG_DEBUG(s_logger) << "thread running";
  try {
    func();
  } catch (std::exception &e) {
    MYLIB_LOG_ERROR(s_logger) << "thread exception: " << e.what();
  } catch (...) {
    MYLIB_LOG_ERROR(s_logger) << "thread exception";
  }
  // MYLIB_LOG_DEBUG(s_logger) << "thread exit";
  t_current_thread = nullptr;
}

void Thread::join() {
  if (!m_thread) {
    return;
  }

  int32 rt = pthread_join(m_thread, nullptr);
  if (rt) {
    MYLIB_LOG_ERROR(s_logger) << "pthread_join error, rt=" << rt;
  }
  m_thread = 0;
}

void Thread::detach() const {
  if (m_thread) {
    pthread_detach(m_thread);
  }
}
MYLIB_SPACE_END