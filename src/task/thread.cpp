#include "thread.h"

#include <windows.h>

MYLIB_BEGIN

// static const Logger::ptr s_logger = MYLIB_ROOT_LOGGER;
thread_local String Thread::t_current_thread_name = "UNKNOWN";
thread_local Thread *Thread::t_current_thread	  = nullptr;

Thread *Thread::GetCurrentThread() {
	if (!t_current_thread) {
		//t_current_thread = new Thread{Thread::GetCurrentThreadId(), self_thread()};
	}
	return t_current_thread;
}

String Thread::GetCurrentThreadName() {
	return t_current_thread_name;
}

tid_t Thread::GetCurrentThreadId() {
#ifdef MYLIB_WINDOWS
	return (tid_t)::GetCurrentThreadId();
#elif MYLIB_LINUX
	return gettid();
#endif
}

Thread::Thread(String name, std::function<void()> func)
	: m_name(std::move(name)), m_id(), m_cb(std::move(func)), m_thread() {
	if (m_name.empty())
		m_name = "UNKNOWN";
	if (!create_thread())
		MYLIB_THROW("create_thread error");
}

inline void Thread::setName(String name) { m_name = std::move(name); }

void Thread::join() {
	MYLIB_ASSERT(m_thread, "m_thread is empty");
	if (!join_thread()) MYLIB_THROW("join_thread error");
}

void Thread::detach() {
	MYLIB_ASSERT(m_thread, "m_thread is empty");
	if (!detach_thread())
		MYLIB_THROW("detach_thread error");
}

Thread::Thread(tid_t id, thread_t thread)
	: /*m_use_fiber(false),*/
	  m_id(id),
	  m_name("UNKNOWN"),
	  m_cb(nullptr),
	  m_thread(thread) {
}

Thread::~Thread() {
	if (!m_thread) return;
	try {
		join_thread();
	} catch (const Exception &e) {
		e.show();
	}
}

Thread &Thread::operator=(Thread &&rhs) noexcept {
	if (&rhs == this) {
		return *this;
	}
	if (m_thread)
		detach_thread();
	//m_use_fiber = rhs.m_use_fiber;
	m_id	 = rhs.m_id;
	m_name	 = rhs.m_name;
	m_cb	 = rhs.m_cb;
	m_thread = rhs.m_thread;
	return *this;
}

void Thread::run(Thread *thread) {
	// 初始化环境
	t_current_thread = thread;
	if (!t_current_thread) {
		MYLIB_THROW("thread is nullptr");
	}

	if (!t_current_thread->m_cb) {
		t_current_thread = nullptr;
		MYLIB_THROW("thread run, but no callback");
	}
	t_current_thread_name  = t_current_thread->m_name;
	t_current_thread->m_id = GetCurrentThreadId();

	//if (t_current_thread->m_use_fiber) {
	//  //Coroutine::ConvertFromThread();
	//}

	std::function<void()> func{std::move(t_current_thread->m_cb)};

	// 执行函数
	// MYLIB_LOG_DEBUG(s_logger) << "thread running";
	try {
		func();
	} catch (const Exception &e) {
		e.show();
	}

	//if (t_current_thread->m_use_fiber) {
	//  // Coroutine::ConvertToThread();
	//}

	// MYLIB_LOG_DEBUG(s_logger) << "thread exit";
	t_current_thread->m_thread = nullptr;
	t_current_thread		   = nullptr;
}

bool Thread::create_thread() {
#ifdef MYLIB_WINDOWS
	m_thread = CreateThread(
			nullptr,
			0,
			reinterpret_cast<LPTHREAD_START_ROUTINE>(&Thread::run),
			this,
			0,
			&m_id);
	if (!m_thread) {
		return false;
	}
#elif MYLIB_LINUX
	if (pthread_create(&m_thread, nullptr, reinterpret_cast<void *(*) (void *)>(&Thread::run), this)) {
		return false;
	}
#endif
	return true;
}

bool Thread::detach_thread() {
#ifdef MYLIB_WINDOWS
	if (!CloseHandle(m_thread)) {
		return false;
	}
#elif MYLIB_LINUX
	pthread_detach(m_thread);
#endif
	return true;
}

bool Thread::join_thread() const {
#ifdef MYLIB_WINDOWS
	WaitForSingleObject(m_thread, INFINITE);
	if (!TerminateThread(m_thread, 0)) {
		return false;
	}
#elif MYLIB_LINUX
	int32 rt;
	if (rt = pthread_join(m_thread, nullptr)) {
		return false;
	}
	m_thread = 0;
#endif
	return true;
}

HANDLE Thread::self_thread() {
#ifdef MYLIB_WINDOWS
	return ::GetCurrentThread();
#elif MYLIB_LINUX
	return pthread_self();
#endif
}
MYLIB_END
