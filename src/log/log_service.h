

#ifndef MYLIB_LOGGER_H_
#define MYLIB_LOGGER_H_

#include "log_appender.h"
#include "log_event.h"
#include "log_formatter.h"


MYLIB_BEGIN

#define MYLIB_ROOT_LOGGER MYLIB_SPACE::LogService::RootLogger()

class LogService {
public:
	constexpr static auto ROOT_NAME		   = "root";
	constexpr static LogEvent::Level DEF_LEVEL = LogEvent::LE_UNDEFINED;
	using ptr								   = std::shared_ptr<LogService>;
	using Lock								   = Spinlock;

	LogService(String name, LogEvent::Level level, LogFormatter::ptr formatter);
	LogService(LogService &&logger) noexcept;
	LogService(const LogService &logger);
	LogService &operator=(LogService &&logger) noexcept;
	LogService &operator=(const LogService &logger);

	[[nodiscard]] const String &getName() const { return m_name; }
	void setName(const String &name) { m_name = name; }
	[[nodiscard]] const LogFormatter::ptr &getFormatter() const { return m_formatter; }
	void setFormatter(const LogFormatter::ptr &formatter) { m_formatter = formatter; }
	[[nodiscard]] LogEvent::Level getType() const { return m_level; }
	void addAppender(const LogAppender::ptr &appender);
	void log(const LogEvent &event);
	static ptr RootLogger();

	[[nodiscard]] bool formatterEmpty() const { return !m_formatter; }
	[[nodiscard]] bool appendersEmpty() const { return m_appenders.empty(); }

private:
	String m_name;

	LogEvent::Level m_level;
	LogFormatter::ptr m_formatter;
	std::vector<LogAppender::ptr> m_appenders;


	Lock m_lock;
	static ptr m_root_logger;
};
MYLIB_END
#endif// MYLIB_LOGGER_H_
