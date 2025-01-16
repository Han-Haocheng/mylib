#include "logger_appender.h"
#include <iostream>

MYLIB_BEGIN

void ConsoleAppender::append(const LogEvent &event) {
	if (event.getType() < m_type) return;
	m_lock.lock();
	std::cout << m_formatter->format(event);
	m_lock.unlock();
}

ConsoleAppender::ConsoleAppender(LogEvent::value_type type, LogFormatter::ptr formatter)
	: LogAppender(type, std::move(formatter)) {}

void FileAppender::append(const LogEvent &event) {
	if (event.getType() < m_type) return;
	try_open_file();
	m_lock.lock();
	fs << m_formatter->format(event);
	m_lock.unlock();
}

FileAppender::FileAppender(String file, LogEvent::value_type type, LogFormatter::ptr formatter) : LogAppender(type, std::move(formatter)), m_file_path(std::move(file)) {}
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

LogAppender::LogAppender(LogEvent::value_type type, LogFormatter::ptr formatter)
	: m_type(type), m_formatter(std::move(formatter)) {}

MYLIB_END
