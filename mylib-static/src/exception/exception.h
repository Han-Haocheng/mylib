//
// Created by HanHaocheng on 2024/6/15.
//

#ifndef MYLIB_SLN_EXCEPTION_H
#define MYLIB_SLN_EXCEPTION_H

#include "../core/mylib_def.h"
#include "../log/log.h"

#include <utility>
#include <windows.h>

#include <dbghelp.h>

#define MYLIB_THROW_BASE(level, logger, what) \
  MYLIB_SPACE::Exception { logger, level, MYLIB_CURRENT_SOURCE_INFO, what }

#define MYLIB_THROW_ERROR(logger, what) MYLIB_THROW_BASE(MYLIB_SPACE::LogEvent::LE_ERROR, logger, what)
#define MYLIB_THROW_WARN(logger, what) MYLIB_THROW_BASE(MYLIB_SPACE::LogEvent::LE_WARN, logger, what)
#define MYLIB_THROW_INFO(logger, what) MYLIB_THROW_BASE(MYLIB_SPACE::LogEvent::LE_INFO, logger, what)
#define MYLIB_THROW_DEBUG(logger, what) MYLIB_THROW_BASE(MYLIB_SPACE::LogEvent::LE_DEBUG, logger, what)

MYLIB_SPACE_BEGIN

struct FunctionInfo {
  String name;
  uint64 addr;

  FunctionInfo() = default;
  FunctionInfo(const String &name, const uint64 &addr);
};

class Exception : std::exception {
public:
  explicit Exception(Logger::ptr logger, LogEvent::value_type level, const SourceInfo &info, String what);

  Exception(Exception &&e) noexcept = default;
  Exception(const Exception &e) noexcept = default;
  Exception &operator=(Exception &&e) noexcept = default;
  Exception &operator=(const Exception &e) noexcept = default;

  [[nodiscard]] const char *what() const override { return m_what.c_str(); }

  void show_stack();

  static bool CaptureStackBack(std::vector<FunctionInfo> &out, size_t skip, size_t maxStackSize = 64);

private:
  Logger::ptr m_logger;
  LogEvent::value_type m_level;
  SourceInfo m_info;
  String m_what;
  std::vector<FunctionInfo> m_stack_back;
};

MYLIB_SPACE_END

#endif//MYLIB_SLN_EXCEPTION_H
