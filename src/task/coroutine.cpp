#include "coroutine.h"

MYLIB_BEGIN

static const LogService::ptr s_logger = MYLIB_ROOT_LOGGER;

thread_local coroutine_t Coroutine::t_main_coroutine;
thread_local coroutine_t *Coroutine::t_current_coroutine = nullptr;


#define MYLIB_ENUM(name, ...) \
	enum name {               \
		__VA_ARGS__           \
	};                        \
	static const CString name##_string = {#__VA_ARGS__}


template<>
struct ConversionTraits<String, Coroutine::state_t, ConvertType::CT_DEFAULT> {
	using target_t = String;
	using source_t = Coroutine::state_t;

	static target_t convert(const source_t &ty) {
		static CString list[] = {
				"undefined",
				"ready",
				"running",
				"suspended",
				"terminated",
				"dead",
		};

		constexpr const int NUM = std::size(list);

		return ty < NUM ? list[ty] : "";
	}
};


template<>
struct ConversionTraits<Coroutine::state_t, String, ConvertType::CT_DEFAULT> {
	using target_t = Coroutine::state_t;
	using source_t = String;

	static target_t convert(const source_t &ty) {
		std::map<String, Coroutine::state_t> s_map{
#define XX(num, name, NAME) \
	{#num, Coroutine::COS_##NAME}, {"COS_" #NAME, Coroutine::COS_##NAME}, {#name, Coroutine::COS_##NAME}
				XX(0, undefined, UNDEFINED),
				XX(1, ready, READY),
				XX(2, running, RUNNING),
				XX(4, suspended, SUSPENDED),
				XX(8, terminated, TERMINATED),
				XX(16, dead, DEAD),
#undef XX
		};
		return s_map.at(ty);
	}
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
	if (m_state & ~(COS_READY | COS_SUSPENDED)) {
		MYLIB_THROW(String("The current state of coroutine is") + ToString<Coroutine::state_t>(m_state));
	}

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
