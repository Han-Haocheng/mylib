#pragma once

#include "../task.h"
#include "coroutine.h"
#include "thread.h"


#include <forward_list>
#include <list>
#include <queue>

MYLIB_BEGIN

/*class CoroutineScheduler {
public:
  using ptr = std::shared_ptr<CoroutineScheduler>;
  using function = std::function<void(void)>;

  void schedule(Coroutine::ptr co);
  void schedule(function cb);

  void stop();
  void start();

private:
  std::queue<Coroutine::ptr> m_cos;
};*/

class Task {
public:
	using ptr	   = std::shared_ptr<Task>;
	using function = std::function<void(void)>;

	Task(Coroutine::ptr co, tid_t threadId) : m_occupied_thread_id(threadId), m_co(std::move(co)) {}
	// Task(function cb, tid_t threadId) : m_occupied_thread_id(threadId), m_co(cb ? new Coroutine{} : nullptr) {}

	bool execute() {
		if (!m_co) {
			return false;
		}
		auto num = 689;
		int8 s[100000000000];
		m_co->resume();
		return true;
	}


	bool empty() const { return !m_co; }

	tid_t occupiedThreadId() const { return m_occupied_thread_id; };

private:
	tid_t m_occupied_thread_id;
	Coroutine::ptr m_co;
};

class Scheduler {
public:
	using ptr	   = std::shared_ptr<Scheduler>;
	using function = std::function<void(void)>;

	Scheduler(String name, size_t threadSize, bool useCaller = true);

	void schedule(Task::ptr task);

	void stop();

	void start();

	size_t getTaskNumber() const { return m_tasks.size(); }

	static void Wait() {
		t_waitting = true;
		while (t_waitting) {
#ifdef MYLIB_WINDOWS
			Sleep(1);
#elif MYLIB_LINUX
			sleep(1);
#endif
		}
	}

	static void Notify() {
		t_waitting = false;
	}

private:
	static void run(Scheduler *scheduler);

private:
	bool m_is_stop : 1;
	bool m_is_stopping : 1;
	String m_name;
	std::vector<Thread::ptr> m_thread_pool;
	std::list<Task::ptr> m_tasks;

	static thread_local bool t_waitting;
};

MYLIB_END
