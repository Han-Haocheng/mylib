//
// Created by HanHaocheng on 2024/4/20.
//

#include "until.h"
#include "../sync/sync.h"

MYLIB_SPACE_BEGIN

time_t GetCurrentTimestamp() { return time(nullptr); }

String FormatClassName(String str) {
  if (str.empty()) return "";
  size_t pos = str.find_last_of(' ') + 1;
  return str.substr(pos, str.find_last_of(':') - pos - 1).c_str();
}

MYLIB_SPACE_END
