#ifndef MYLIB_LOGGER_APPENDER_H_
#define MYLIB_LOGGER_APPENDER_H_

#include "../core/mylib_def.h"
#include "../sync/caslock.h"
#include "log_event.h"
#include "log_formatter.h"

#include <fstream>

MYLIB_SPACE_BEGIN

class LogAppender {
public:
  constexpr static const auto DEF_LEVEL = LogEvent::value_type::LE_UNDEFINED;
  using ptr = std::shared_ptr<LogAppender>;
  using Lock = Spinlock;

  explicit LogAppender(LogEvent::value_type type = DEF_LEVEL, LogFormatter::ptr formatter = nullptr);

  virtual void append(const LogEvent &event) = 0;
  inline void setType(LogEvent::value_type type) { m_type = type; }
  inline void setFormatter(const LogFormatter::ptr &formatter) { m_formatter = formatter; }
  [[nodiscard]] inline LogEvent::value_type getType() const { return m_type; }
  [[nodiscard]] inline LogFormatter::ptr getFormatter() const { return m_formatter; }

protected:
  Lock m_lock;
  LogEvent::value_type m_type;
  LogFormatter::ptr m_formatter;
};

class ConsoleAppender : public LogAppender {
public:
  explicit ConsoleAppender(LogEvent::value_type type, LogFormatter::ptr formatter = nullptr);
  void append(const LogEvent &event) override;
};

class FileAppender : public LogAppender {
public:
  FileAppender(String file, LogEvent::value_type type, LogFormatter::ptr formatter = nullptr);
  void append(const LogEvent &event) override;
  ~FileAppender();

private:
  void try_open_file();

private:
  std::ofstream fs;
  String m_file_path;
};

MYLIB_SPACE_END
#endif// !MYLIB_LOGGER_APPENDER_H_