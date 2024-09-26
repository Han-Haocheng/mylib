//
// Created by HanHaocheng on 2024/5/7.
//

#ifndef MYLIB_LOG_H
#define MYLIB_LOG_H

#include "../base/mylib_def.h"
#include "log_event.h"
#include "logger.h"
#include "logger_manager.h"

#define MYLIB_LOG_NAME(name) mylib::LoggerManager::GetInstance()->try_getLogger(name)

#define MYLIB_LOG_BASE(level, logger, source_info) \
  if (logger && logger->getType() < level)         \
  MYLIB_SPACE::LogWarp{level, logger, source_info}.msg()

#define MYLIB_LOG_DEBUG(logger) MYLIB_LOG_BASE(MYLIB_SPACE::LogEvent::LE_DEBUG, logger, MYLIB_CURRENT_SOURCE_INFO)
#define MYLIB_LOG_INFO(logger) MYLIB_LOG_BASE(MYLIB_SPACE::LogEvent::LE_INFO, logger, MYLIB_CURRENT_SOURCE_INFO)
#define MYLIB_LOG_WARN(logger) MYLIB_LOG_BASE(MYLIB_SPACE::LogEvent::LE_WARN, logger, MYLIB_CURRENT_SOURCE_INFO)
#define MYLIB_LOG_ERROR(logger) MYLIB_LOG_BASE(MYLIB_SPACE::LogEvent::LE_ERROR, logger, MYLIB_CURRENT_SOURCE_INFO)

MYLIB_BEGIN

class LogWarp {
public:
  LogWarp(LogEvent::value_type level, Logger::ptr logger, SourceInfo info);
  inline ~LogWarp() { m_logger->log(m_event); }

  [[nodiscard]] inline SString &msg() { return m_event.msg(); }

private:
  LogEvent::value_type m_level;
  LogEvent m_event;
  Logger::ptr m_logger;
};

MYLIB_END

#endif//MYLIB_LOG_H
