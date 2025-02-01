#ifndef MYLIB_LOG_FORMATTER_H_
#define MYLIB_LOG_FORMATTER_H_

#include "../task.h"


#include "log_event.h"

#include <functional>

MYLIB_BEGIN

class FormatItem {
public:
	using ptr								  = std::shared_ptr<FormatItem>;
	using FactoryFunc						  = std::function<ptr(String)>;
	constexpr static const CString FORMAT_STR = "";

	explicit FormatItem(String extendStr) : m_extend(std::move(extendStr)) {}
	[[nodiscard]] virtual String format(const LogEvent &event) const { return m_extend; }

	virtual ~FormatItem() = default;

	template<typename Item>
	static ptr make_format_item(String exStr) {
		return std::make_shared<Item>(std::move(exStr));
	}

	[[nodiscard]] const String &getExtend() const { return m_extend; }

protected:
	String m_extend;//额外
};


//m:消息
//g:日志级别
//G:日志名称
//r:累计毫秒数
//d:时间
//F:文件名
//l:行号
//t:线程id
//c:协程id
//T:线程名称
//C:协程名
class LogFormatter {
public:
	constexpr static auto DEF_PATTERN =
			"[%d{%Y/%m/%d %H:%M:%S}][%f:%l][%T.%C - %t.%c] %G(%g)>>>>%m%n";

	using ptr	  = std::shared_ptr<LogFormatter>;
	using Lock	  = Spinlock;

	using ItemMap = std::unordered_map<String, FormatItem::FactoryFunc>;

	explicit LogFormatter(String pattern);

	void setPattern(const String &pattern);
	[[nodiscard]] String format(const LogEvent &event) const;

	static ptr DEF_FORMATTER();

private:
	void _parse_pattern();

	//基础属性
	String m_pattern;
	std::vector<FormatItem::ptr> m_items;

	//运行属性
	Lock m_lock;

	static std::shared_ptr<ItemMap> s_item_map;
	static ptr s_def_formatter;
};

// ================================FormatItem===========================

// 信息
class MessageFormatItem : public FormatItem {
public:
	using FormatItem::FormatItem;
	constexpr static const CString FORMAT_STR = "%m";
	[[nodiscard]] String format(const LogEvent &event) const override { return event.msg().str(); }
};

class LevelFormatItem : public FormatItem {
public:
	using FormatItem::FormatItem;
	constexpr static const CString FORMAT_STR = "%g";
	[[nodiscard]] String format(const LogEvent &event) const override { return StringCast<LogEvent::Level>::toString(event.getType()); }
};

class ElapseFormatItem : public FormatItem {
public:
	using FormatItem::FormatItem;
	constexpr static const CString FORMAT_STR = "%r";
	[[nodiscard]] String format(const LogEvent &event) const override { return "<elapse>"; }
};

class LoggerNameFormatItem : public FormatItem {
public:
	using FormatItem::FormatItem;
	constexpr static const CString FORMAT_STR = "%G";

	[[nodiscard]] String format(const LogEvent &event) const override { return event.getLoggerName(); }
};

class FileFormatItem : public FormatItem {
public:
	using FormatItem::FormatItem;
	constexpr static const CString FORMAT_STR = "%f";

	[[nodiscard]] String format(const LogEvent &event) const override { return event.getFile(); }
};

class LineFormatItem : public FormatItem {
public:
	using FormatItem::FormatItem;
	constexpr static const CString FORMAT_STR = "%l";
	[[nodiscard]] String format(const LogEvent &event) const override { return std::to_string(event.getLine()); }
};

// 异步相关
//时间
class DateTimeFormatItem : public FormatItem {
public:
	constexpr static const CString FORMAT_STR = "%d";
	explicit DateTimeFormatItem(String extendStr = "%Y-%m-%d %H-%M-%S") : FormatItem(std::move(extendStr)) {}
	[[nodiscard]] String format(const LogEvent &event) const override {
		char tmp[64];
		::tm t{};
#ifdef MYLIB_WINDOWS
		localtime_s(&t, &event.getTime());
#elif MYLIB_LINUX
		t = localtime(&event.getTime());
#endif// MYLIB_WINDOWS
		strftime(tmp, sizeof(tmp), m_extend.c_str(), &t);
		return tmp;
	}
};

// 线程id
class ThreadIdFormatItem : public FormatItem {
public:
	using FormatItem::FormatItem;
	constexpr static const CString FORMAT_STR = "%t";
	[[nodiscard]] String format(const LogEvent &event) const override { return std::to_string(event.getThreadId()); }
};

class ThreadNameFormatItem : public FormatItem {
public:
	using FormatItem::FormatItem;
	constexpr static const CString FORMAT_STR = "%T";

	[[nodiscard]] String format(const LogEvent &event) const override {
		String res = event.getThreadName();
		return res;
	}
};

// 协程
class CoroutineIdFormatItem : public FormatItem {
public:
	using FormatItem::FormatItem;
	constexpr static const CString FORMAT_STR = "%c";
	[[nodiscard]] String format(const LogEvent &event) const override { return std::to_string(event.getCoroutineId()); }
};

class CoroutineNameFormatItem : public FormatItem {
public:
	using FormatItem::FormatItem;
	constexpr static const CString FORMAT_STR = "%C";
	[[nodiscard]] String format(const LogEvent &event) const override { return event.getCoroutineName(); }
};
MYLIB_END

#endif// MYLIB_LOG_FORMATTER_H_
