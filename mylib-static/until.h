#pragma once
#include "mylib_def.h"
#include <ctime>
#include <iostream>

MYLIB_SPACE_BEGIN

extern "C" time_t GetCurrentTimestamp();

extern "C" constexpr char ToUpper(char c) { return c < 'a' || c > 'z' ? c : (char) (c - 32); }
extern "C" constexpr char ToLower(char c) { return c < 'A' || c > 'Z' ? c : char(c + 32); }
extern "C" constexpr size_t StringLength(CString str) {
  for (size_t a = 0;;)
    if (str[a++] == '\0') return a;
  return 0;
}

String FormatClassName(String str);

#ifdef MYLIB_MSVC
#define MYLIB_FUNC_FULL_NAME __##FUNCTION__
#elif MYLIB_GUN
#define MYLIB_FUNC_FULL_NAME __PRETTY_##FUNCTION__
#endif// MYLIB_MSVC

#define CLASS_NAME_ MYLIB_SPACE_NAME::Format##ClassName(MYLIB_FUNC_FULL_NAME)

MYLIB_SPACE_END