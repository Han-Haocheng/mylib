#include "log_appender.h"
#include <iostream>

MYLIB_BEGIN
LogAppender::LogAppender(const Type type, const LogEvent::Level level, LogFormatter::ptr formatter)
	: m_type(type), m_level(level), m_formatter(std::move(formatter)) {}

ConsoleAppender::ConsoleAppender(const LogEvent::Level level, LogFormatter::ptr formatter)
	: LogAppender(LAT_CONSOLE, level, std::move(formatter)) {}

void ConsoleAppender::append(const LogEvent &event) {
	if (event.getType() < m_level) return;
	m_lock.lock();
	std::cout << m_formatter->format(event);
	m_lock.unlock();
}

void FileAppender::append(const LogEvent &event) {
	if (event.getType() < m_level) return;
	try_open_file();
	m_lock.lock();
	fs << m_formatter->format(event);
	m_lock.unlock();
}

FileAppender::FileAppender(String file, LogEvent::Level level, LogFormatter::ptr formatter)
	: LogAppender(LAT_FILE, level, std::move(formatter)), m_file_path(std::move(file)) {}

FileAppender::~FileAppender() {
	if (fs.is_open()) fs.close();
}
void FileAppender::try_open_file() {
	while (true) {
		if (fs.is_open()) break;
#ifdef MYLIB_LINUX
		sleep(1);
#elif MYLIB_WINDOWS
		Sleep(1);
#endif
		fs.open(m_file_path, std::ios::app);
	}
}


MYLIB_END
