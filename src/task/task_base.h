
#include "../core/core.h"
#include "../log/log.h"
#ifdef MYLIB_WINDOWS
#include <Windows.h>
#elif MYLIB_LINUX
#include <ucontext.h>
#endif// MYLIB_MSVC