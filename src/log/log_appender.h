#ifndef MYLIB_LOGGER_APPENDER_H_
#define MYLIB_LOGGER_APPENDER_H_

#include "log_event.h"
#include "log_formatter.h"



#include <fstream>
#include <memory>

MYLIB_BEGIN

class LogAppender {
public:
	using ptr  = std::shared_ptr<LogAppender>;
	using Lock = Spinlock;
	enum Type {
		LAT_CONSOLE,
		LAT_FILE
	};

	MYLIB_CONSTEXPR_TYPE DEF_LEVEL = LogEvent::Level::LE_UNDEFINED;


	explicit LogAppender(Type type, LogEvent::Level level = DEF_LEVEL, LogFormatter::ptr formatter = nullptr);
	virtual ~LogAppender()					   = default;

	virtual void append(const LogEvent &event) = 0;
	void setLevel(const LogEvent::Level level) { m_level = level; }
	void setFormatter(const LogFormatter::ptr &formatter) { m_formatter = formatter; }
	[[nodiscard]] LogEvent::Level level() const { return m_level; }
	[[nodiscard]] LogFormatter::ptr formatter() const { return m_formatter; }

protected:
	Lock m_lock;

	Type m_type;
	LogEvent::Level m_level;
	LogFormatter::ptr m_formatter;
};

template<>
struct ConversionTraits<String, LogAppender::Type, ConvertType::CT_DEFAULT> {
	using target_t = String;
	using source_t = LogAppender::Type;

	static target_t convert(const source_t &type) {
		switch (type) {
			case LogAppender::Type::LAT_CONSOLE:
				return "console";
			case LogAppender::Type::LAT_FILE:
				return "file";
			default:
				return "console";
		}
	}
};

template<>
struct ConversionTraits<LogAppender::Type, String, ConvertType::CT_DEFAULT> {
	using target_t = LogAppender::Type;
	using source_t = String;

	static target_t convert(const source_t &str) {
		if (str == "console" || str == "CONSOLE") {
			return LogAppender::Type::LAT_CONSOLE;
		}
		if (str == "file" || str == "FILE") {
			return LogAppender::Type::LAT_FILE;
		}
		return LogAppender::Type::LAT_CONSOLE;
	}
};

class ConsoleAppender : public LogAppender {
public:
	explicit ConsoleAppender(LogEvent::Level level, LogFormatter::ptr formatter = nullptr);
	void append(const LogEvent &event) override;
};

class FileAppender : public LogAppender {
public:
	FileAppender(String file, LogEvent::Level level, LogFormatter::ptr formatter = nullptr);
	void append(const LogEvent &event) override;
	~FileAppender() override;

private:
	void try_open_file();

	std::ofstream fs;
	String m_file_path;
};

MYLIB_END
#endif// !MYLIB_LOGGER_APPENDER_H_
