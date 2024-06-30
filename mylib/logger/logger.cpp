#include "logger.h"

MYLIB_SPACE_BEGIN

Logger::ptr Logger::m_root_logger = nullptr;

Logger::Logger(String name, LogEvent::value_type level, LogFormatter::ptr formatter)
    : m_name(std::move(name)), m_type(level), m_appenders(), m_formatter(std::move(formatter)) {}

Logger::Logger(Logger &&logger) noexcept
    : m_name(std::move(logger.m_name)),
      m_type(logger.m_type),
      m_appenders(std::move(logger.m_appenders)),
      m_formatter(std::move(logger.m_formatter)),
      m_lock(std::move(logger.m_lock)) {
}

Logger::Logger(const Logger &logger)
    : m_name(logger.m_name),
      m_type(logger.m_type),
      m_appenders(logger.m_appenders),
      m_formatter(logger.m_formatter),
      m_lock() {
}

Logger &Logger::operator=(Logger &&logger) noexcept {
  if (&logger == this) {
    return *this;
  }
  m_name = std::move(logger.m_name);
  m_type = logger.m_type;
  m_appenders = std::move(logger.m_appenders);
  m_formatter = std::move(logger.m_formatter);
  m_lock = std::move(logger.m_lock);
  return *this;
}

Logger &Logger::operator=(const Logger &logger) {
  if (&logger == this) {
    return *this;
  }
  m_name = logger.m_name;
  m_type = logger.m_type;
  m_appenders = logger.m_appenders;
  m_formatter = logger.m_formatter;
  return *this;
}

void Logger::addAppender(const LogAppender::ptr &appender) {
  if (!appender) {
    return;
  }

  appender->setFormatter(m_formatter);
  m_appenders.emplace_back(appender);
}

void Logger::log(const LogEvent &event) {
  if (event.getType() < m_type) {
    return;
  }

  if (m_appenders.empty() && this != m_root_logger.get()) {
    //std::cout << "logger<" << m_name << ">appender is empty" << std::endl;
    m_root_logger->log(event);
    return;
  }

  m_lock.lock();
  for (auto &appender: m_appenders) {
    m_lock.lock();
    appender->append(event);
    m_lock.unlock();
  }
}

Logger::ptr Logger::RootLogger() {
  if (!m_root_logger) {
    m_root_logger = std::make_shared<Logger>(ROOT_NAME, DEF_LEVEL, LogFormatter::DEF_FORMATTER());
    m_root_logger->addAppender(ConsoleAppender::ptr{new ConsoleAppender{DEF_LEVEL, LogFormatter::DEF_FORMATTER()}});
  }
  return m_root_logger;
}
MYLIB_SPACE_END