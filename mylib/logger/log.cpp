//
// Created by HanHaocheng on 2024/5/7.
//

#include "log.h"

#include "../async/coroutine.h"
#include "../async/thread.h"
#include "../untils/until.h"
#include <iostream>
#include <memory>
#include <utility>

MYLIB_BEGIN

LogWarp::LogWarp(LogEvent::value_type level, Logger::ptr logger, SourceInfo info)
    : m_level(level),
      m_logger(std::move(logger)),
      m_event(info, logger->getName(), Thread::GetCurrentThreadName(), "UNKNOWN",
              m_level, GetCurrentTimestamp(), Thread::GetCurrentThreadId(), 1) {
}

MYLIB_END
