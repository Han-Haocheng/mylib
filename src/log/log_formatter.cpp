#include "log_formatter.h"

MYLIB_BEGIN

// 字符串
class StringFormatItem : public FormatItem {
public:
	constexpr static const CString FORMAT_STR = "";
	explicit StringFormatItem(String extendStr) : FormatItem(std::move(extendStr)) {}
	[[nodiscard]] String format(const LogEvent &event) const override { return m_extend; }
};

class MessageFormatItem : public FormatItem {
public:
	constexpr static const CString FORMAT_STR = "%m";
	explicit MessageFormatItem(String extendStr) : FormatItem(std::move(extendStr)) {}
	[[nodiscard]] String format(const LogEvent &event) const override { return event.msg().str(); }
};

class TypeFormatItem : public FormatItem {
public:
	constexpr static const CString FORMAT_STR = "%p";
	explicit TypeFormatItem(String extendStr) : FormatItem(std::move(extendStr)) {}
	[[nodiscard]] String format(const LogEvent &event) const override { return StringCast<LogEvent::value_type>::toString(event.getType()); }
};

class ElapseFormatItem : public FormatItem {
public:
	constexpr static const CString FORMAT_STR = "%r";
	explicit ElapseFormatItem(String extendStr) : FormatItem(std::move(extendStr)) {}
	[[nodiscard]] String format(const LogEvent &event) const override { return "<elapse>"; }
};

class LoggerNameFormatItem : public FormatItem {
public:
	constexpr static const CString FORMAT_STR = "%c";
	explicit LoggerNameFormatItem(String extendStr) : FormatItem(std::move(extendStr)) {}
	[[nodiscard]] String format(const LogEvent &event) const override { return event.getLoggerName(); }
};

class FileFormatItem : public FormatItem {
public:
	constexpr static const CString FORMAT_STR = "%f";
	explicit FileFormatItem(String extendStr) : FormatItem(std::move(extendStr)) {}
	[[nodiscard]] String format(const LogEvent &event) const override { return event.getFile(); }
};

class LineFormatItem : public FormatItem {
public:
	constexpr static const CString FORMAT_STR = "%l";
	explicit LineFormatItem(String extendStr) : FormatItem(std::move(extendStr)) {}
	[[nodiscard]] String format(const LogEvent &event) const override { return std::to_string(event.getLine()); }
};

class NewLineFormatItem : public FormatItem {
public:
	constexpr static const CString FORMAT_STR = "%n";
	explicit NewLineFormatItem(String extendStr) : FormatItem(std::move(extendStr)) {}
	[[nodiscard]] String format(const LogEvent &event) const override { return "\n"; }
};

class TabFormatItem : public FormatItem {
public:
	constexpr static const CString FORMAT_STR = "%T";
	explicit TabFormatItem(String extendStr) : FormatItem(std::move(extendStr)) {}
	[[nodiscard]] String format(const LogEvent &event) const override { return "\t"; }
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
	constexpr static const CString FORMAT_STR = "%t";
	explicit ThreadIdFormatItem(String extendStr) : FormatItem(std::move(extendStr)) {}
	[[nodiscard]] String format(const LogEvent &event) const override { return std::to_string(event.getThreadId()); }
};

class ThreadNameFormatItem : public FormatItem {
public:
	constexpr static const CString FORMAT_STR = "%N";
	explicit ThreadNameFormatItem(String extendStr) : FormatItem(std::move(extendStr)) {}
	[[nodiscard]] String format(const LogEvent &event) const override {
		String res = event.getThreadName();
		return res;
	}
};

// 协程
class CoroutineIdFormatItem : public FormatItem {
public:
	constexpr static const CString FORMAT_STR = "%F";
	explicit CoroutineIdFormatItem(String extendStr) : FormatItem(std::move(extendStr)) {}
	[[nodiscard]] String format(const LogEvent &event) const override { return std::to_string(event.getCoroutineId()); }
};

class CoroutineNameFormatItem : public FormatItem {
public:
	constexpr static const CString FORMAT_STR = "%C";
	explicit CoroutineNameFormatItem(String extendStr) : FormatItem(std::move(extendStr)) {}
	[[nodiscard]] String format(const LogEvent &event) const override { return event.getCoroutineName(); }
};

std::shared_ptr<LogFormatter::ItemMap> LogFormatter::s_item_map = nullptr;
LogFormatter::ptr LogFormatter::s_def_formatter					= nullptr;

LogFormatter::ptr LogFormatter::DEF_FORMATTER() {
	if (!s_def_formatter)
		s_def_formatter = std::make_shared<LogFormatter>(DEF_PATTERN);
	return s_def_formatter;
}

template<const char *str>
class Message {
public:
	Message() : length{std::strlen(str)} {
	}

	constexpr const char *s() {
		return str;
	}

private:
	std::size_t length{0};
};

extern const char hello[] = "external linkage"; // external linkage
const char hello11[]	  = "internal linkage!";// internal linkage

// 位置相关
// 文件
// 行数
//
//%d{%Y-%m-%d %H-%M-%S}[%f:%l]%T%p%T%c%T%t%T%m%n
LogFormatter::LogFormatter(mylib::String pattern) : m_pattern(std::move(pattern)) {
	if (!s_item_map) {
		s_item_map = std::make_shared<ItemMap>(
				ItemMap{FormatItem::make_format_item<MessageFormatItem>(),	  //m:消息
						FormatItem::make_format_item<TypeFormatItem>(),		  //p:日志级别
						FormatItem::make_format_item<ElapseFormatItem>(),	  //r:累计毫秒数
						FormatItem::make_format_item<LoggerNameFormatItem>(), //c:日志名称
						FormatItem::make_format_item<ThreadIdFormatItem>(),	  //t:线程id
						FormatItem::make_format_item<NewLineFormatItem>(),	  //n:换行
						FormatItem::make_format_item<DateTimeFormatItem>(),	  //d:时间
						FormatItem::make_format_item<FileFormatItem>(),		  //f:文件名
						FormatItem::make_format_item<LineFormatItem>(),		  //l:行号
						FormatItem::make_format_item<TabFormatItem>(),		  //T:Tab
						FormatItem::make_format_item<CoroutineIdFormatItem>(),//F:协程id
						FormatItem::make_format_item<ThreadNameFormatItem>(), //N:线程名称
						FormatItem::make_format_item<CoroutineNameFormatItem>()});
	}
	m_lock.lock();
	_parse_pattern();
	m_lock.unlock();
}

void LogFormatter::setPattern(const String &pattern) {
	m_pattern = pattern;
	m_lock.lock();
	_parse_pattern();
	m_lock.unlock();
}
String LogFormatter::format(const LogEvent &event) {
	SString res{};
	for (const auto &item: m_items) {
		String str = item->format(event);
		res << str;
	}
	return res.str();
}

void LogFormatter::_parse_pattern() {
	struct TmpFormat {
		String pattern;
		String extend;
		TmpFormat() = default;
		TmpFormat(String pattern, String extend) : pattern(std::move(pattern)), extend(std::move(extend)) {}
		[[nodiscard]] bool empty() const { return pattern.empty() && extend.empty(); };
	};

	String pattern = m_pattern;
	std::vector<TmpFormat> tmp_format_vec;
	TmpFormat tmp_format;

	for (size_t pos = 0; pos < m_pattern.size(); ++pos) {
		if (pattern[pos] != '%') {
			tmp_format.extend += pattern[pos];
			continue;
		}

		//检测复合格式化要求
		if (pos + 1 == m_pattern.size()) {
			tmp_format.extend += "%";
			continue;
		}
		if (!tmp_format.empty()) {
			tmp_format_vec.emplace_back(tmp_format);
			tmp_format = {};
		}

		//进入格式化模式
		tmp_format_vec.emplace_back(
				String{pattern[pos++], pattern[pos]}, String{});

		// 检测是否符合扩展要求
		if (pattern[pos + 1] != '{') {
			//推出格式化模式
			continue;
		}

		auto &back_tf = tmp_format_vec.back();
		auto ex_end	  = pattern.find_first_of('}', pos + 2);
		if (ex_end == std::string::npos) {
			// 如果是扩展未闭合，则进入普通模式
			//pos = end_pos;
			continue;
		}
		back_tf.extend = pattern.substr(pos + 2, ex_end - pos - 2);
		pos			   = ex_end;
	}
	if (!tmp_format.empty()) {
		tmp_format_vec.emplace_back(tmp_format);
	}

	for (const auto &format: tmp_format_vec) {
		if (format.pattern.empty()) {
			m_items.emplace_back(new StringFormatItem(format.extend));
			continue;
		}
		auto fd_res = s_item_map->find(format.pattern);
		if (fd_res == s_item_map->end()) {
			m_items.emplace_back(new StringFormatItem(format.pattern + "{" + format.extend + "}"));
      continue;
    }
    m_items.emplace_back(fd_res->second(format.extend));
  }
}
MYLIB_END
