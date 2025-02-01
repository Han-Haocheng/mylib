#include <utility>

#include "log_factory.h"

MYLIB_BEGIN

LogFactory::LogFactory(LogEvent::Level level, LogService::ptr logger, DebugLocationInfo info, DebugTaskInfo tkInfo)
	: m_level(level),
	  m_event(logger->getName(),
			  level,
			  GetCurrentTimestamp(),
			  std::move(info),
			  std::move(tkInfo)),
	  m_logger(std::move(logger)) {}
LogFactory::~LogFactory() { m_logger->log(m_event); }
SString &LogFactory::msg() { return m_event.msg(); }

MYLIB_END
