#pragma once
#include "../core/mylib_def.h"
#include <ctime>
#include <iostream>

#ifdef MYLIB_MSVC
#define MYLIB_FULL_FUNC_NAME __##FUNCTION__
#elif MYLIB_GUN
#define MYLIB_FUNC_NAME __PRETTY_##FUNCTION__
#endif// MYLIB_MSVC

#define MYLIB_CLASS_NAME MYLIB_SPACE::FormatClassName(MYLIB_##FULL_FUNC_NAME)
#define MYLIB_LINE (unsigned int) __##LINE__
#define MYLIB_FILE_NAME __##FILE__
#define MYLIB_FUNC_NAME __##func__

#define MYLIB_CURRENT_SOURCE_INFO \
  MYLIB_##SPACE::SourceInfo { MYLIB_##LINE, MYLIB_##FILE_NAME, MYLIB_##FUNC_NAME, MYLIB_##CLASS_NAME }

#define MYLIB_INIT_SOURCE_INFO(sourceInfo) \
  sourceInfo = MYLIB_##CURRENT_SOURCE_INFO

MYLIB_SPACE_BEGIN

struct SourceInfo {
  uint32 line;
  String file_name;
  String func_name;
  String class_name;
};

extern "C" time_t GetCurrentTimestamp();

extern "C" constexpr char ToUpper(char c) { return c < 'a' || c > 'z' ? c : (char) (c - 32); }
extern "C" constexpr char ToLower(char c) { return c < 'A' || c > 'Z' ? c : char(c + 32); }
extern "C" constexpr size_t StringLength(CString str) {
  for (size_t a = 0;;)
    if (str[a++] == '\0') return a;
  return 0;
}

String FormatClassName(String str);

MYLIB_SPACE_END
