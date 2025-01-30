#ifndef MYLIB_LOGGER_MANAGER_H
#define MYLIB_LOGGER_MANAGER_H

#include "../task/thread.h"
#include "logger.h"

#include <memory>
#include <utility>


#define MYLIB_LOG_NAME(name) mylib::LoggerManager::GetInstance()->try_getLogger(name)

#define MYLIB_LOG_BASE(level, logger, source_info) \
	if (logger && logger->getType() < level)       \
	MYLIB_SPACE::LogFactory{level, logger, source_info}.msg()

#define MYLIB_LOG_DEBUG(logger) MYLIB_LOG_BASE(MYLIB_SPACE::LogEvent::LE_DEBUG, logger, MYLIB_CURRENT_SOURCE_INFO)
#define MYLIB_LOG_INFO(logger) MYLIB_LOG_BASE(MYLIB_SPACE::LogEvent::LE_INFO, logger, MYLIB_CURRENT_SOURCE_INFO)
#define MYLIB_LOG_WARN(logger) MYLIB_LOG_BASE(MYLIB_SPACE::LogEvent::LE_WARN, logger, MYLIB_CURRENT_SOURCE_INFO)
#define MYLIB_LOG_ERROR(logger) MYLIB_LOG_BASE(MYLIB_SPACE::LogEvent::LE_ERROR, logger, MYLIB_CURRENT_SOURCE_INFO)

MYLIB_BEGIN

class LoggerManager {
public:
	using ptr  = std::shared_ptr<LoggerManager>;
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

class LogFactory {
public:
	LogFactory(LogEvent::value_type level, Logger::ptr logger, SourceInfo info)
		: m_level(level),
		  m_event(std::move(info),
				  logger->getName(),
				  Thread::GetCurrentThreadName(),
				  "UNKNOWN",
				  m_level, GetCurrentTimestamp(), Thread::GetCurrentThreadId(), 1),
		  m_logger(std::move(logger)) {}
	~LogFactory() { m_logger->log(m_event); }

	[[nodiscard]] SString &msg() { return m_event.msg(); }

private:
	LogEvent::value_type m_level;
	LogEvent m_event;
	Logger::ptr m_logger;
};

class LoggerConfig {
public:
private:
};

MYLIB_END

#endif// !MYLIB_LOGGER_MANAGER_H
