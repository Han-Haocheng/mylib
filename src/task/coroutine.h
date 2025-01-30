#ifndef MYLIB_COROUTINE_H
#define MYLIB_COROUTINE_H
#include "../core.h"

#include <functional>

#ifdef MYLIB_WINDOWS
#include <Windows.h>
#elif MYLIB_LINUX
#include <ucontext.h>
#endif// MYLIB_MSVC

MYLIB_BEGIN

#ifdef MYLIB_WINDOWS
using coroutine_t = LPVOID;
#elif MYLIB_LINUX
using coroutine_t = ucontext_t;
#endif// MYLIB_MSVC

/**
 * @brief 协程类型
 * */
class Coroutine {
public:
	using ptr		 = std::shared_ptr<Coroutine>;
	using size_type	 = size_t;
	using function_t = std::function<void(void)>;
	enum state_t {
		COS_UNDEFINED,
		COS_READY,
		COS_RUNNING,
		COS_SUSPENDED,
		COS_TERMINATED,
		COS_DEAD,
	};
	explicit Coroutine(cid_t cid, function_t cb, size_type stack_size = 0);
	~Coroutine();
	void yield();
	void resume();

	[[nodiscard]] state_t state() const { return m_state; }

private:
	static void run(Coroutine *coroutine);
	static void set_coroutine(coroutine_t *to);
	void create_coroutine();
	void destroy_coroutine() const;

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

MYLIB_END
#endif
