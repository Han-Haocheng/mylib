#ifndef MYLIB_COROUTINE_H
#define MYLIB_COROUTINE_H
#include "../core/mylib_def.h"

#ifdef MYLIB_MSVC
#include <Windows.h>
#elif MYLIB_GUN
#include <ucontext.h>
#endif// MYLIB_MSVC

MYLIB_SPACE_BEGIN

#ifdef MYLIB_MSVC
using coroutine_t = LPVOID;
#elif MYLIB_GUN
using coroutine_t = ucontext_t;
#endif// MYLIB_MSVC

/**
 * @brief 协程类型
 * */
class Coroutine {
public:
  using ptr = std::shared_ptr<Coroutine>;
  using size_type = size_t;
  using function_t = std::function<void(void)>;
  enum state_t
  {
    COS_UNDEFINED = 0x0,
    COS_READY = 0x1,
    COS_RUNNING = 0x2,
    COS_SUSPENDED = 0x4,
    COS_TERMINATED = 0x8,
    COS_DEAD = 0x16,
  };
  explicit Coroutine(cid_t cid, function_t cb, size_type stack_size = 0);
  ~Coroutine();
  void yield();
  void resume();

  // bool reset(const function_t &cb);

  //  static void ConvertFromThread();
  //  static void ConvertToThread();

  [[nodiscard]] state_t state() const { return m_state; }

private:
  static void run(Coroutine *coroutine);
  static void set_coroutine(coroutine_t *to);
  void create_coroutine();
  void destroy_coroutine();

private:
  cid_t m_cid;
  state_t m_state;
  uint8 *m_stack;
  size_type m_stack_size;
  function_t m_cb;

  coroutine_t m_coroutine;

  static thread_local coroutine_t t_main_coroutine;
  static thread_local coroutine_t *t_current_coroutine;
};

MYLIB_SPACE_END
#endif