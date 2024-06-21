#include "coroutine.h"
#include "log.h"

MYLIB_SPACE_BEGIN

static const Logger::ptr s_logger = MYLIB_ROOT_LOGGER;

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

Coroutine::Coroutine(function_t cb, Coroutine::size_type stack_size)
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

void Coroutine::run(Coroutine *coroutine) {
  if (!coroutine) {
    return;
  }
  Coroutine &co = *coroutine;
  function_t cb = nullptr;
  cb.swap(co.m_cb);
  co.m_state = RUNNING;

  try {
    cb();
  } catch (...) {
    //todo error
  }

  co.m_state = TERMINATED;
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
  auto dwStackSize = static_cast<DWORD>(m_stack_size);
  auto pStartAddress = reinterpret_cast<LPFIBER_START_ROUTINE>(&Coroutine::run);
  LPVOID lpParameter = this;

  m_coroutine = CreateFiber(dwStackSize, pStartAddress, lpParameter);

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

MYLIB_SPACE_END
