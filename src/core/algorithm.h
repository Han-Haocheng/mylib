#pragma once

#include "define.h"

#ifdef MYLIB_MSVC
#define MYLIB_FUNC_FULL_NAME __##FUNCTION__
#elif MYLIB_GUN
#define MYLIB_FUNC_FULL_NAME __PRETTY_##FUNCTION__
#elif MYLIB_CLANG
#define MYLIB_CLANG
#endif// MYLIB_MSVC

MYLIB_BEGIN


String FormatClassName(CString fullFuncName);
extern "C" time_t GetCurrentTimestamp();
extern "C" constexpr char ToUpper(char c) { return c < 'a' || c > 'z' ? c : (char) (c - 32); }
extern "C" constexpr char ToLower(char c) { return c < 'A' || c > 'Z' ? c : char(c + 32); }
extern "C" constexpr size_t StringLength(CString str) {
	size_t a = 0;
	while (str[a++] != '\0');
	return a;
}

MYLIB_END
