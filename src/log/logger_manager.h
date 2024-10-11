#ifndef MYLIB_LOGGER_MANAGER_H
#define MYLIB_LOGGER_MANAGER_H

#include "logger.h"

MYLIB_BEGIN

class LoggerManager {
public:
  using ptr = std::shared_ptr<LoggerManager>;
  using Lock = Spinlock;

  static ptr GetInstance();

  void addLogger(const Logger::ptr &logger);
  Logger::ptr addLogger(const String &name, LogEvent::value_type level, LogFormatter::ptr formatter);
  [[nodiscard]] Logger::ptr getLogger(const String &name);
  [[nodiscard]] Logger::ptr try_getLogger(const String &name);

private:
  void add_logger(const Logger::ptr &logger);
  Logger::ptr get_logger(const String &name);

private:
  Lock m_lock;
  LogFormatter::ptr m_def_formatter;
  std::unordered_map<String, Logger::ptr> m_loggers;
  static LoggerManager::ptr s_instance;
};

class LoggerConfig {
public:
private:
};

MYLIB_END

#endif// !MYLIB_LOGGER_MANAGER_H
