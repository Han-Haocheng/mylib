#pragma once
#include "../core/mylib_def.h"
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

MYLIB_SPACE_END
