#include "mylib_def.h"

MYLIB_BEGIN

String FormatClassName(CString fullFuncName) {
  if (fullFuncName == NULL || strlen(fullFuncName) == 0) return "";

  CString lastSpace = strrchr(fullFuncName, ' ');
  CString lastColon = strrchr(fullFuncName, ':');

  if (lastSpace == NULL || lastColon == NULL || lastSpace >= lastColon) return "";

  size_t length = lastColon - lastSpace - 1;
  return {lastSpace + 1, length};
}

MYLIB_END