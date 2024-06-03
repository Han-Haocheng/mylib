//
// Created by HanHaocheng on 2024/4/19.
//

#include "sync.h"

#include <utility>
#include "log.h"

MYLIB_SPACE_BEGIN

static Logger::ptr s_logger = MYLIB_LOG_NAME("sys.sync");

thread_local String Thread::t_current_thread_name = "UNKNOWN";
thread_local Thread *Thread::t_current_thread = nullptr;

Mutex::Mutex() : m_mutex() {
#ifdef MYLIB_MSVC
  m_mutex = CreateMutex(nullptr, FALSE, nullptr);
  if (!m_mutex) {
    //todo error
  }
#elif MYLIB_GUN
  pthread_mutex_init(&m_mutex, nullptr);
#endif// MYLIB_MSVC==1
}

Mutex::~Mutex() {
#ifdef MYLIB_MSVC
  if (CloseHandle(m_mutex) != TRUE) {
    //todo error
  }
#elif MYLIB_GUN
  pthread_mutex_destroy(&m_mutex);
#endif// MYLIB_MSVC==1
}

void Mutex::lock() {
#ifdef MYLIB_MSVC
  WaitForSingleObject(m_mutex, INFINITE);
#elif MYLIB_GUN
  pthread_mutex_lock(&m_mutex);
#endif// MYLIB_MSVC==1
}
void Mutex::unlock() {
#ifdef MYLIB_MSVC
  if (ReleaseMutex(m_mutex)) {
    //todo error
  }
#elif MYLIB_GUN
  pthread_mutex_unlock(&m_mutex);
#endif// MYLIB_MSVC==1
}

CASLock::CASLock() : m_atomic_flag() { m_atomic_flag.clear(); }

void CASLock::lock() {
  while (std::atomic_flag_test_and_set_explicit(
      &m_atomic_flag,
      std::memory_order_acquire))
    ;
}

void CASLock::unlock() {
  std::atomic_flag_clear_explicit(
      &m_atomic_flag,
      std::memory_order_release);
}

Spinlock::Spinlock() : m_spinlock() {
#ifdef MYLIB_MSVC
  InitializeCriticalSectionAndSpinCount(&m_spinlock, -1);
#elif MYLIB_GUN
  pthread_spin_init(&m_spinlock, 0);
#endif// MYLIB_MSVC==1
}
Spinlock::~Spinlock() {
#ifdef MYLIB_MSVC
  DeleteCriticalSection(&m_spinlock);
#elif MYLIB_GUN
  pthread_spin_destroy(&m_spinlock);
#endif// MYLIB_MSVC==1
}
void Spinlock::lock() {
#ifdef MYLIB_MSVC
  while (TryEnterCriticalSection(&m_spinlock) == 0) Sleep(0);
#elif MYLIB_GUN
  pthread_spin_lock(&m_spinlock);
#endif// MYLIB_MSVC==1
}
void Spinlock::unlock() {
#ifdef MYLIB_MSVC
  LeaveCriticalSection(&m_spinlock);
#elif MYLIB_GUN
  pthread_spin_unlock(&m_spinlock);
#endif// MYLIB_MSVC==1
}

RWLock::RWLock() : m_rwlock() {
#ifdef MYLIB_MSVC
  InitializeSRWLock(&m_rwlock);
#elif MYLIB_GUN
  pthread_rwlock_init(&m_rwlock, nullptr);
#endif// MYLIB_MSVC==1
}

RWLock::~RWLock() {

#ifdef MYLIB_MSVC
#elif MYLIB_GUN
  pthread_rwlock_destroy(&m_rwlock);
#endif// MYLIB_MSVC==1
}

void RWLock::rlock() {
#ifdef MYLIB_MSVC
  AcquireSRWLockShared(&m_rwlock);
#elif MYLIB_GUN
  pthread_rwlock_rdlock(&m_rwlock);
#endif// MYLIB_MSVC==1
}

void RWLock::wlock() {
#ifdef MYLIB_MSVC
  AcquireSRWLockExclusive(&m_rwlock);
#elif MYLIB_GUN
  pthread_rwlock_wrlock(&m_rwlock);
#endif// MYLIB_MSVC==1
}

void RWLock::unlock() {
#ifdef MYLIB_MSVC
  ReleaseSRWLockExclusive(&m_rwlock);
  ReleaseSRWLockShared(&m_rwlock);
#elif MYLIB_GUN
  pthread_rwlock_unlock(&m_rwlock);
#endif// MYLIB_MSVC==1
}

Semaphore::Semaphore(int32 init_count) : m_sem() {
#ifdef MYLIB_MSVC
  m_sem = CreateSemaphore(nullptr,
                          init_count,
                          init_count,
                          nullptr);
#elif MYLIB_GUN
  sem_init(&m_sem, 0, init_count);
#endif// MYLIB_MSVC==1
}

Semaphore::~Semaphore() {
#ifdef MYLIB_MSVC
  CloseHandle(m_sem);
#elif MYLIB_GUN
  sem_destroy(&m_sem);
#endif// MYLIB_MSVC==1
}

void Semaphore::wait(time_t sec, int32 nanosecond) {
  timespec ts{sec, nanosecond};
#ifdef MYLIB_MSVC
  WaitForSingleObject(m_sem,
                      ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
#elif MYLIB_GUN
  if (sec == INF_TIME) {
    sem_wait(&m_sem);
  } else {
    sem_timedwait(&m_sem, &ts);
  }
#endif// MYLIB_MSVC==1
}
void Semaphore::post() {
#ifdef MYLIB_MSVC
  ReleaseSemaphore(m_sem, 1, nullptr);
#elif MYLIB_GUN
  sem_post(&m_sem);
#endif// MYLIB_MSVC==1
}

cid_t Coroutine::s_current_id = 0;
thread_local coroutine_t Coroutine::t_main_coroutine;
thread_local coroutine_t *Coroutine::t_current_coroutine = nullptr;

void Coroutine::ConvertToThread() {
#ifdef MYLIB_GUN
  getcontext(&t_main_coroutine);
  t_main_coroutine.uc_stack.ss_sp = nullptr;
  t_main_coroutine.uc_stack.ss_size = 0;
  t_main_coroutine.uc_stack.ss_flags = 0;
  makecontext(&t_main_coroutine, nullptr, 0);
#elif MYLIB_MSVC
  if (!ConvertFiberToThread()) {
    MYLIB_LOG_ERROR(s_logger)
        << "ConvertFiberToThread error";
  }
#endif
}

void Coroutine::ConvertFromThread() {
#ifdef MYLIB_MSVC
  t_main_coroutine = ConvertThreadToFiber(nullptr);
#endif
}

Coroutine::Coroutine(Coroutine::size_type stack_size, function_t cb)
    : m_cid(s_current_id++),
      m_state(UNDEFINED),
      m_stack(nullptr),
      m_stack_size(stack_size),
      m_cb(std::move(cb)),
      m_coroutine() {
  if (m_cb == nullptr) {
    // todo warn
    m_cb = []() {};
  }
  create_coroutine();
  m_state = READY;
}

Coroutine::~Coroutine() {
  destroy_coroutine();
  // todo free m_stack
  delete[] m_stack;
}

void Coroutine::yield() {
  if (m_state & ~(RUNNING)) {
    //todo error
    return;
  }
  if (t_current_coroutine != &m_coroutine) {
    //todo error
    return;
  }
  m_state = SUSPENDED;
  set_coroutine(&t_main_coroutine);
}

void Coroutine::resume() {
  if (m_state & ~(READY | SUSPENDED)) {
    //todo error
    return;
  }
  if (t_current_coroutine != &t_main_coroutine) {
    //todo error
    return;
  }
  m_state = READY;
  set_coroutine(&m_coroutine);
}

void Coroutine::run(Coroutine *from) {
  if (t_current_coroutine != &from->m_coroutine) {
    //todo error
  }

  Coroutine *co = from;
  function_t cb = nullptr;
  cb.swap(co->m_cb);

  from->m_state = RUNNING;

  try {
    cb();
  } catch (...) {
    //todo error
  }
  from->m_state = TERMINATED;
}

void Coroutine::set_coroutine(coroutine_t *to) {
  t_current_coroutine = to;
#if MYLIB_MSVC
  SwitchToFiber(to);
#elif MYLIB_GUN
  swapcontext(t_current_coroutine, to);
#endif
}

void Coroutine::create_coroutine() {
#if MYLIB_MSVC
  m_coroutine = CreateFiber(m_stack_size,
                            reinterpret_cast<LPFIBER_START_ROUTINE>(&Coroutine::run),
                            t_main_coroutine);
  if (!m_coroutine) {
    throw;//todo
  }
#elif MYLIB_GUN
  if (-1 == getcontext(&m_coroutine)) {
    throw;//todo
  }

  m_coroutine.uc_stack.ss_sp = m_stack;
  m_coroutine.uc_stack.ss_size = m_stack_size;
  m_coroutine.uc_stack.ss_flags = 0;

  makecontext(&m_coroutine,
              reinterpret_cast<void (*)()>(&Coroutine::run),
              1, this);
#endif
}

void Coroutine::destroy_coroutine() {
#if MYLIB_MSVC
  if (m_coroutine) {
    DeleteFiber(m_coroutine);
  }
#endif
}

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
#ifdef MYLIB_MSVC
  return ::GetCurrentThreadId();
#elif MYLIB_GUN
  return gettid();
#endif
}

Thread::Thread(String name, std::function<void()> func, bool is_fiber_thread)
    : m_thread_name(std::move(name)),
      m_thread_id(),
      m_cb(std::move(func)),
      m_thread(),
      m_is_fiber_thread(is_fiber_thread) {
  if (m_thread_name.empty())
    m_thread_name = "UNKNOWN";

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
  t_current_thread_name = t_current_thread->m_thread_name;

  if (!t_current_thread->m_cb) {
    t_current_thread = nullptr;
    MYLIB_LOG_WARN(s_logger)
        << "thread run, but no callback";
    return;
  }

  t_current_thread->m_thread_id = GetCurrentThreadId();

  if (t_current_thread->m_is_fiber_thread) {
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
  if (t_current_thread->m_is_fiber_thread) {
    Coroutine::ConvertToThread();
  }

  // MYLIB_LOG_DEBUG(s_logger) << "thread exit";
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
      &m_thread_id);

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