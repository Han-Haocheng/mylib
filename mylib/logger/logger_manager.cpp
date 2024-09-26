#include "logger_manager.h"

MYLIB_BEGIN

LoggerManager::ptr LoggerManager::s_instance = nullptr;

LoggerManager::ptr LoggerManager::GetInstance() {
  if (!s_instance) {
    s_instance = std::make_shared<LoggerManager>();
    s_instance->add_logger(Logger::RootLogger());
  }
  return s_instance;
}

void LoggerManager::addLogger(const Logger::ptr &logger) {
  if (!logger || logger->getName().empty()) {
    //TODO: <log>error
    return;
  }
  m_lock.lock();
  add_logger(logger);
  m_lock.unlock();
}

Logger::ptr LoggerManager::addLogger(const String &name, LogEvent::value_type level, LogFormatter::ptr formatter) {
  if (name.empty())
    return nullptr;

  auto fmt = formatter ? std::move(formatter) : m_def_formatter;
  Logger::ptr logger = std::make_shared<Logger>(name,
                                                level == LogEvent::LE_UNDEFINED
                                                    ? Logger::DEF_LEVEL
                                                    : level,
                                                fmt);

  m_lock.lock();
  add_logger(logger);
  m_lock.unlock();
  return logger;
}

Logger::ptr LoggerManager::getLogger(const String &name) {
  if (name.empty()) {
    //TODO: <log>error
    return nullptr;
  }
  m_lock.lock();
  auto res = get_logger(name);
  m_lock.unlock();
  return res;
}

Logger::ptr LoggerManager::try_getLogger(const String &name) {
  if (name.empty()) {
    //TODO: <log>error
    return nullptr;
  }
  m_lock.lock();
  Logger::ptr log = get_logger(name);
  m_lock.unlock();
  if (!log) {
    log = std::make_shared<Logger>(name, Logger::DEF_LEVEL, LogFormatter::DEF_FORMATTER());
    m_lock.lock();
    add_logger(log);
    m_lock.unlock();
  }
  return log;
}

void LoggerManager::add_logger(const Logger::ptr &logger) {
  auto is_res = m_loggers.insert(std::make_pair(logger->getName(), logger));
  if (!is_res.second) {
    is_res.first->second = logger;
  }
}

Logger::ptr LoggerManager::get_logger(const String &name) {
  auto fd_res = m_loggers.find(name);
  if (fd_res == m_loggers.end()) {
    return nullptr;
  }
  return fd_res->second;
}
MYLIB_END