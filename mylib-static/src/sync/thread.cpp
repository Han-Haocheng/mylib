#include "thread.h"
#include "log.h"

MYLIB_SPACE_BEGIN

static const Logger::ptr s_logger = MYLIB_ROOT_LOGGER;
thread_local String Thread::t_current_thread_name = "UNKNOWN";
thread_local Thread *Thread::t_current_thread = nullptr;

Thread *Thread::GetCurrentThread() {
  if (!t_current_thread) {
    //t_current_thread = new Thread{Thread::GetCurrentThreadId(), thread_self()};
  }
  return t_current_thread;
}

String Thread::GetCurrentThreadName() {
  return t_current_thread_name;
}

tid_t Thread::GetCurrentThreadId() {
#ifdef MYLIB_MSVC
  return (tid_t)::GetCurrentThreadId();
#elif MYLIB_GUN
  return gettid();
#endif
}

Thread::Thread(String name, std::function<void()> func, bool is_fiber_thread)
    : m_name(std::move(name)),
      m_id(),
      m_cb(std::move(func)),
      m_thread(),
      m_use_fiber(is_fiber_thread) {
  if (m_name.empty())
    m_name = "UNKNOWN";

  thread_create();
}

void Thread::join() {
  if (!m_thread) return;
  thread_join();
}

void Thread::detach() {
  if (!m_thread) return;
  thread_detach();
}

Thread::~Thread() {
  if (!m_thread) return;
  thread_join();
}

void Thread::run(Thread *thread) {
  // 初始化区
  t_current_thread = thread;
  t_current_thread_name = t_current_thread->m_name;

  if (!t_current_thread->m_cb) {
    t_current_thread->m_thread = nullptr;
    t_current_thread = nullptr;
    MYLIB_LOG_WARN(s_logger)
        << "thread run, but no callback";
    return;
  }

  t_current_thread->m_id = GetCurrentThreadId();

  if (t_current_thread->m_use_fiber) {
    Coroutine::ConvertFromThread();
  }

  std::function<void()> func = nullptr;
  func.swap(t_current_thread->m_cb);

  // 功能区
  // MYLIB_LOG_DEBUG(s_logger) << "thread running";
  try {
    func();
  } catch (std::exception &e) {
    MYLIB_LOG_ERROR(s_logger)
        << "thread exception: " << e.what();
  } catch (...) {
    MYLIB_LOG_ERROR(s_logger)
        << "thread exception";
  }
  if (t_current_thread->m_use_fiber) {
    Coroutine::ConvertToThread();
  }

  // MYLIB_LOG_DEBUG(s_logger) << "thread exit";
  t_current_thread->m_thread = nullptr;
  t_current_thread = nullptr;
}

void Thread::thread_create() {
#ifdef MYLIB_MSVC
  LPTHREAD_START_ROUTINE lpStartAddress = reinterpret_cast<LPTHREAD_START_ROUTINE>(&Thread::run);
  LPVOID lpParameter = this;
  m_thread = CreateThread(
      nullptr,
      0,
      lpStartAddress,
      lpParameter,
      0,
      &m_id);

  if (!m_thread) {
    MYLIB_LOG_ERROR(s_logger)
        << "CreateThread error";
  }
#elif MYLIB_GUN
  int32 rd = pthread_create(&m_thread,
                            nullptr,
                            reinterpret_cast<void *(*) (void *)>(&Thread::run),
                            this);
  if (rd) {
    MYLIB_LOG_ERROR(s_logger)
        << "pthread_create error, rd=" << rd;
  }
#endif
}

void Thread::thread_detach() {
#ifdef MYLIB_MSVC
  if (!CloseHandle(m_thread)) {
    MYLIB_LOG_ERROR(s_logger)
        << "CloseHandle error";
  }
#elif MYLIB_GUN
  pthread_detach(m_thread);
#endif
}

void Thread::thread_join() {
#ifdef MYLIB_MSVC
  WaitForSingleObject(m_thread, INF_TIME);
  if (!TerminateThread(m_thread, 0)) {
    MYLIB_LOG_ERROR(s_logger)
        << "TerminateThread error";
  }
#elif MYLIB_GUN
  int32 rt;
  if (rt = pthread_join(m_thread, nullptr)) {
    MYLIB_LOG_ERROR(s_logger)
        << "pthread_join error, rt=" << rt;
  }
  m_thread = 0;
#endif
}
MYLIB_SPACE_END