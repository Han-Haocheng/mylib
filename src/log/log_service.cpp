#include "log_service.h"

MYLIB_BEGIN

LogService::ptr LogService::m_root_logger = nullptr;

LogService::LogService(String name, const LogEvent::Level level, LogFormatter::ptr formatter)
	: m_name(std::move(name)), m_level(level), m_formatter(std::move(formatter)), m_appenders() {}

LogService::LogService(LogService &&logger) noexcept
	: m_name(std::move(logger.m_name)),
	  m_level(logger.m_level),
	  m_formatter(std::move(logger.m_formatter)),
	  m_lock(logger.m_lock),
	  m_appenders(std::move(logger.m_appenders)) {
}

LogService::LogService(const LogService &logger)
	: m_name(logger.m_name),
	  m_level(logger.m_level),
	  m_appenders(logger.m_appenders),
	  m_formatter(logger.m_formatter),
	  m_lock() {
}

LogService &LogService::operator=(LogService &&logger) noexcept {
	if (&logger == this) {
		return *this;
	}
	m_name		= std::move(logger.m_name);
	m_level		= logger.m_level;
	m_appenders = std::move(logger.m_appenders);
	m_formatter = std::move(logger.m_formatter);
	m_lock		= logger.m_lock;
	return *this;
}

LogService &LogService::operator=(const LogService &logger) {
	if (&logger == this) {
		return *this;
	}
	m_name		= logger.m_name;
	m_level		= logger.m_level;
	m_appenders = logger.m_appenders;
	m_formatter = logger.m_formatter;
	return *this;
}

void LogService::addAppender(const LogAppender::ptr &appender) {
	if (!appender) {
		return;
	}

	appender->setFormatter(m_formatter);
	m_appenders.emplace_back(appender);
}

void LogService::log(const LogEvent &event) {
	if (event.getType() < m_level) {
		return;
	}

	if (m_appenders.empty() && this != m_root_logger.get()) {
		//std::cout << "logger<" << m_name << ">appender is empty" << std::endl;
		m_root_logger->log(event);
		return;
	}

	for (auto &appender: m_appenders) {
		m_lock.lock();
		appender->append(event);
		m_lock.unlock();
	}
}

LogService::ptr LogService::RootLogger() {
	if (!m_root_logger) {
		m_root_logger = std::make_shared<LogService>(ROOT_NAME, DEF_LEVEL, LogFormatter::DEF_FORMATTER());
		m_root_logger->addAppender(ConsoleAppender::ptr{new ConsoleAppender{DEF_LEVEL, LogFormatter::DEF_FORMATTER()}});
	}
	return m_root_logger;
}
MYLIB_END
