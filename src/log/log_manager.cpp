#include "log_manager.h"

#include "../config/ConfigManager.h"

MYLIB_BEGIN

LoggerManager::LoggerManager() {
	add_logger(LogService::RootLogger());
	m_config = ConfigManager::GetInstance()->getConfig<LogConfig>("log");
}

void LoggerManager::addLogger(const LogService::ptr &logger) {
	if (!logger || logger->getName().empty()) {
		//TODO: <log>error
		return;
	}
	m_lock.lock();
	add_logger(logger);
	m_lock.unlock();
}

LogService::ptr LoggerManager::addLogger(const String &name, LogEvent::Level level, LogFormatter::ptr formatter) {
	if (name.empty())
		return nullptr;

	auto fmt			   = formatter ? std::move(formatter) : m_def_formatter;
	LogService::ptr logger = std::make_shared<LogService>(name,
														  level == LogEvent::LE_UNDEFINED
																  ? LogService::DEF_LEVEL
																  : level,
														  fmt);

	m_lock.lock();
	add_logger(logger);
	m_lock.unlock();
	return logger;
}

LogService::ptr LoggerManager::logger(const String &name) {
	if (name.empty()) {
		//TODO: <log>error
		return nullptr;
	}
	m_lock.lock();
	auto res = get_logger(name);
	m_lock.unlock();
	return res;
}

LogService::ptr LoggerManager::try_getLogger(const String &name) {
	if (name.empty()) {
		//TODO: <log>error
		return nullptr;
	}
	m_lock.lock();
	LogService::ptr log = get_logger(name);
	m_lock.unlock();
	if (!log) {
		log = std::make_shared<LogService>(name, LogService::DEF_LEVEL, LogFormatter::DEF_FORMATTER());
		m_lock.lock();
		add_logger(log);
		m_lock.unlock();
	}
	return log;
}

void LoggerManager::add_logger(const LogService::ptr &logger) {
	auto is_res = m_loggers.insert(std::make_pair(logger->getName(), logger));
	if (!is_res.second) {
		is_res.first->second = logger;
	}
}

LogService::ptr LoggerManager::get_logger(const String &name) {
	auto fd_res = m_loggers.find(name);
	if (fd_res == m_loggers.end()) {
		return nullptr;
	}
	return fd_res->second;
}
MYLIB_END
