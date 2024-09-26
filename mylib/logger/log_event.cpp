#include "log_event.h"

MYLIB_BEGIN
LogEvent::LogEvent(SourceInfo localInfo,
                   String loggerName,
                   String coroutineName,
                   String threadName,
                   LogEvent::value_type type,
                   time_t t,
                   tid_t thread,
                   cid_t coroutine)
    : m_logger_name(std::move(loggerName)),
      m_type(type),
      m_local_info(std::move(localInfo)),
      m_time(t),
      m_thread_id(thread),
      m_coroutine_id(coroutine),
      m_thread_name(std::move(threadName)),
      m_coroutine_name(std::move(coroutineName)) {
}

MYLIB_END
