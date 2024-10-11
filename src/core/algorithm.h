#pragma once

#include "define.h"

#ifdef MYLIB_MSVC
#define MYLIB_FULL_FUNC_NAME __##FUNCTION__
#elif MYLIB_GUN
#define MYLIB_FUNC_NAME __PRETTY_##FUNCTION__
#elif MYLIB_CLANG
#define MYLIB_CLANG
#endif// _MSC_VER

#define MYLIB_CLASS_NAME \
  MYLIB_SPACE::FormatClassName(MYLIB_##FULL_FUNC_NAME)

#define MYLIB_LINE (unsigned int) __##LINE__
#define MYLIB_FILE_NAME __##FILE__
#define MYLIB_FUNC_NAME __##func__

#define MYLIB_CURRENT_SOURCE_INFO \
  MYLIB_SPACE::SourceInfo { MYLIB_##LINE, MYLIB_##FILE_NAME, MYLIB_##FUNC_NAME, MYLIB_##CLASS_NAME }


MYLIB_BEGIN

struct SourceInfo {
  uint32 line;
  String file_name;
  String func_name;
  String class_name;
};

String FormatClassName(CString fullFuncName);

extern "C" time_t GetCurrentTimestamp();
extern "C" constexpr char ToUpper(char c) { return c < 'a' || c > 'z' ? c : (char) (c - 32); }
extern "C" constexpr char ToLower(char c) { return c < 'A' || c > 'Z' ? c : char(c + 32); }
extern "C" constexpr size_t StringLength(CString str) {
  size_t a = 0;
  while (str[a++] != '\0')
    ;
  return a;
}

MYLIB_END