#include "log_event.h"

MYLIB_BEGIN
LogEvent::LogEvent(String loggerName, Level level, time_t t, DebugLocationInfo localInfo, DebugTaskInfo taskInfo)
	: m_logger_name(std::move(loggerName)),
	  m_level(level),
	  m_local_info(std::move(localInfo)),
	  m_time(t),
	  m_task_info(std::move(taskInfo)) {
}

MYLIB_END
