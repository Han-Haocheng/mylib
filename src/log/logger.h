

#ifndef MYLIB_LOGGER_H_
#define MYLIB_LOGGER_H_

#include "log_event.h"
#include "log_formatter.h"
#include "logger_appender.h"

#include "../task/task.h"

MYLIB_BEGIN

#define MYLIB_ROOT_LOGGER MYLIB_SPACE::Logger::RootLogger()

class Logger {
public:
  constexpr static const CString ROOT_NAME = "root";
  constexpr static const LogEvent::value_type DEF_LEVEL = LogEvent::LE_UNDEFINED;
  using ptr = std::shared_ptr<Logger>;
  using Lock = Spinlock;

  Logger(String name, LogEvent::value_type level, LogFormatter::ptr formatter);
  Logger(Logger &&logger) noexcept;
  Logger(const Logger &logger);
  Logger &operator=(Logger &&logger) noexcept;
  Logger &operator=(const Logger &logger);

  [[nodiscard]] inline const String &getName() const { return m_name; }
  inline void setName(const String &name) { m_name = name; }
  [[nodiscard]] inline const LogFormatter::ptr &getFormatter() const { return m_formatter; }
  inline void setFormatter(const LogFormatter::ptr &formatter) { m_formatter = formatter; }
  [[nodiscard]] inline LogEvent::value_type getType() const { return m_type; }
  void addAppender(const LogAppender::ptr &appender);
  void log(const LogEvent &event);
  static Logger::ptr RootLogger();

  [[nodiscard]] bool formatterEmpty() const { return !m_formatter; }
  [[nodiscard]] bool appendersEmpty() const { return m_appenders.empty(); }

private:
  String m_name;

  LogEvent::value_type m_type;
  LogFormatter::ptr m_formatter;
  Lock m_lock;

  std::vector<LogAppender::ptr> m_appenders;

  static Logger::ptr m_root_logger;
};
MYLIB_END
#endif// MYLIB_LOGGER_H_
