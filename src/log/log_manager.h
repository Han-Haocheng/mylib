#ifndef MYLIB_LOGGER_MANAGER_H
#define MYLIB_LOGGER_MANAGER_H

#include "log_config.h"
#include "log_event.h"
#include "log_factory.h"
#include "log_formatter.h"
#include "log_service.h"


#include <memory>


#define MYLIB_LOG_NAME(name) mylib::LoggerManager::GetInstance()->try_getLogger(name)

#define MYLIB_LOG_BASE(level, logger, location, task) \
	if (logger && logger->getType() < level)          \
	MYLIB_SPACE::LogFactory{level, logger, location, task}.msg()

#define MYLIB_LOG_DEBUG(logger) MYLIB_LOG_BASE(MYLIB_SPACE::LogEvent::LE_DEBUG, logger, MYLIB_DEBUG_LOCATION_INFO, MYLIB_DEBUG_TASK_INFO)
#define MYLIB_LOG_INFO(logger) MYLIB_LOG_BASE(MYLIB_SPACE::LogEvent::LE_INFO, logger, MYLIB_DEBUG_LOCATION_INFO, MYLIB_DEBUG_TASK_INFO)
#define MYLIB_LOG_WARN(logger) MYLIB_LOG_BASE(MYLIB_SPACE::LogEvent::LE_WARN, logger, MYLIB_DEBUG_LOCATION_INFO, MYLIB_DEBUG_TASK_INFO)
#define MYLIB_LOG_ERROR(logger) MYLIB_LOG_BASE(MYLIB_SPACE::LogEvent::LE_ERROR, logger, MYLIB_DEBUG_LOCATION_INFO, MYLIB_DEBUG_TASK_INFO)

MYLIB_BEGIN

class LoggerManager : public Singleton<LoggerManager> {
	friend class Singleton<LoggerManager>;

public:
	using ptr  = std::shared_ptr<LoggerManager>;
	using Lock = Spinlock;

private:
	LoggerManager();

	void add_logger(const LogService::ptr &logger);
	LogService::ptr get_logger(const String &name);

public:
	void addLogger(const LogService::ptr &logger);
	LogService::ptr addLogger(const String &name, LogEvent::Level level, LogFormatter::ptr formatter);
	[[nodiscard]] LogService::ptr logger(const String &name);
	[[nodiscard]] LogService::ptr try_getLogger(const String &name);


private:
	Lock m_lock;
	Config<LogConfig>::ptr m_config;
	LogFormatter::ptr m_def_formatter;
	std::unordered_map<String, LogService::ptr> m_loggers;
};


MYLIB_END

#endif// !MYLIB_LOGGER_MANAGER_H
