
#ifndef MYLIB_LOG_LOGFACTORY_H_
#define MYLIB_LOG_LOGFACTORY_H_


#include "log_service.h"



MYLIB_BEGIN

class LogFactory {
public:
	LogFactory(LogEvent::Level level, LogService::ptr logger, DebugLocationInfo info, DebugTaskInfo tkInfo);
	~LogFactory();

	[[nodiscard]] SString &msg();

private:
	LogEvent::Level m_level;
	LogEvent m_event;
	LogService::ptr m_logger;
};

MYLIB_END

#endif// !MYLIB_LOG_LOGFACTORY_H_
