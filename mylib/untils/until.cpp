//
// Created by HanHaocheng on 2024/4/20.
//

#include "until.h"
#include "../concurrent/thread.h"

MYLIB_SPACE_BEGIN

time_t GetCurrentTimestamp() { return time(nullptr); }

CString FormatClassName(mylib::CString str) {
  if (!str) {
    return "";
  }
  String tmpStr = str;
  size_t pos = tmpStr.find_last_of(' ') + 1, n = tmpStr.find_last_of(':') - pos - 1;
  return tmpStr.substr(pos, n).c_str();
}

MYLIB_SPACE_END
