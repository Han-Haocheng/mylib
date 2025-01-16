#include "coroutine.h"
#include "../log/log.h"

MYLIB_BEGIN

static const Logger::ptr s_logger = MYLIB_ROOT_LOGGER;

thread_local coroutine_t Coroutine::t_main_coroutine;
thread_local coroutine_t *Coroutine::t_current_coroutine = nullptr;


#define MYLIB_ENUM(name, ...) \
	enum name {               \
		__VA_ARGS__           \
	};                        \
	static const CString name##_string = {#__VA_ARGS__}


template<>
class StringCast<Coroutine::state_t> {
public:
	using cast_type = Coroutine::state_t;
	using map_type	= std::unordered_map<String, cast_type>;
	using pair_t	= std::pair<String, cast_type>;

	static String toString(const cast_type &ty) {

		MYLIB_ENUM(a, a, b);
		static CString list[] = {
				"COS_UNDEFINED",
				"COS_READY",
				"COS_RUNNING",
				"COS_SUSPENDED",
				"COS_TERMINATED",
				"COS_DEAD",
		};
		constexpr const int NUM = std::size(list);

		return ty < NUM ? list[ty] : "";
	}

	static cast_type fromString(const String &ty) {
		//std::transform(ty.begin(), ty.end(), );
		try {
			return s_map.at(ty);
		} catch (const std::out_of_range &oor) {
			MYLIB_THROW(String{"fromString out of range"} + oor.what());
		} catch (...) {
			MYLIB_THROW("fromString unknown error");
		}
	}

private:
	static map_type s_map;
};

StringCast<Coroutine::state_t>::map_type StringCast<Coroutine::state_t>::s_map{
		{"0", Coroutine::COS_UNDEFINED},
		{"COS_UNDEFINED", Coroutine::COS_UNDEFINED},
		{"UNDEFINED", Coroutine::COS_UNDEFINED},
		{"cos_undefined", Coroutine::COS_UNDEFINED},
		{"undefined", Coroutine::COS_UNDEFINED},

		{"1", Coroutine::COS_READY},
		{"COS_READY", Coroutine::COS_READY},
		{"READY", Coroutine::COS_READY},
		{"cos_ready", Coroutine::COS_READY},
		{"ready", Coroutine::COS_READY},

		{"2", Coroutine::COS_RUNNING},
		{"COS_RUNNING", Coroutine::COS_RUNNING},
		{"RUNNING", Coroutine::COS_RUNNING},
		{"cos_running", Coroutine::COS_RUNNING},
		{"running", Coroutine::COS_RUNNING},

		{"4", Coroutine::COS_SUSPENDED},
		{"COS_UNDEFINED", Coroutine::COS_SUSPENDED},
		{"UNDEFINED", Coroutine::COS_SUSPENDED},
		{"cos_undefined", Coroutine::COS_SUSPENDED},
		{"undefined", Coroutine::COS_SUSPENDED},

		{"8", Coroutine::COS_TERMINATED},
		{"COS_TERMINATED", Coroutine::COS_TERMINATED},
		{"TERMINATED", Coroutine::COS_TERMINATED},
		{"cos_terminated", Coroutine::COS_TERMINATED},
		{"terminated", Coroutine::COS_TERMINATED},

		{"16", Coroutine::COS_DEAD},
		{"COS_DEAD", Coroutine::COS_DEAD},
		{"DEAD", Coroutine::COS_DEAD},
		{"cos_dead", Coroutine::COS_DEAD},
		{"dead", Coroutine::COS_DEAD},
};

Coroutine::Coroutine(cid_t cid, function_t cb, const size_type stack_size)
	: m_cid(cid),
	  m_state(COS_UNDEFINED),
	  m_stack(nullptr),
	  m_stack_size(stack_size),
	  m_cb(std::move(cb)),
	  m_coroutine() {
	if (m_cb == nullptr) {
		// todo warn
		m_cb = []() {};
	}
	create_coroutine();
	m_state = COS_READY;
}

Coroutine::~Coroutine() {
	destroy_coroutine();
	m_state = COS_DEAD;
	// todo free m_stack
	delete[] m_stack;
}

void Coroutine::yield() {
	if (m_state & ~(COS_RUNNING)) {
		//todo error
		return;
	}
	if (t_current_coroutine != &m_coroutine) {
		//todo error
		return;
	}
	m_state = COS_SUSPENDED;
	set_coroutine(&t_main_coroutine);
}

void Coroutine::resume() {
	if (m_state & ~(COS_READY | COS_SUSPENDED)) { MYLIB_THROW(String("The current state of coroutine is") + StringCast<Coroutine::state_t>::toString(m_state)); }

	if (t_current_coroutine != &t_main_coroutine) {
		//todo error
		return;
	}
	m_state = COS_READY;
	set_coroutine(&m_coroutine);
}

void Coroutine::run(Coroutine *coroutine) {
	if (!coroutine) { return; }
	Coroutine &co = *coroutine;
	function_t cb = nullptr;
	cb.swap(co.m_cb);
	co.m_state = COS_RUNNING;

	try {
		cb();
	} catch (...) {
		//todo error
	}

	co.m_state = COS_TERMINATED;
}

void Coroutine::set_coroutine(coroutine_t *to) {
	t_current_coroutine = to;
#if MYLIB_WINDOWS
	SwitchToFiber(to);
#elif MYLIB_LINUX
	swapcontext(t_current_coroutine, to);
#endif
}

void Coroutine::create_coroutine() {
#ifdef MYLIB_WINDOWS
	auto dwStackSize   = static_cast<DWORD>(m_stack_size);
	auto pStartAddress = reinterpret_cast<LPFIBER_START_ROUTINE>(&Coroutine::run);
	LPVOID lpParameter = this;

	m_coroutine		   = CreateFiber(dwStackSize, pStartAddress, lpParameter);

	if (!m_coroutine) {
		throw;//todo
	}
#elif MYLIB_LINUX
	if (-1 == getcontext(&m_coroutine)) {
		throw;//todo
	}
	m_coroutine.uc_stack.ss_sp	  = m_stack;
	m_coroutine.uc_stack.ss_size  = m_stack_size;
	m_coroutine.uc_stack.ss_flags = 0;

	makecontext(&m_coroutine,
				reinterpret_cast<void (*)()>(&Coroutine::run),
				1, this);
#endif
}

void Coroutine::destroy_coroutine() const {
#if MYLIB_MSVC
	if (m_coroutine) { DeleteFiber(m_coroutine); }
#endif
}

MYLIB_END
