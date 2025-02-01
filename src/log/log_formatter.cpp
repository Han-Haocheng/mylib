#include "log_formatter.h"

MYLIB_BEGIN

std::shared_ptr<LogFormatter::ItemMap> LogFormatter::s_item_map = nullptr;
LogFormatter::ptr LogFormatter::s_def_formatter					= nullptr;

LogFormatter::ptr LogFormatter::DEF_FORMATTER() {
	if (!s_def_formatter)
		s_def_formatter = std::make_shared<LogFormatter>(DEF_PATTERN);
	return s_def_formatter;
}

LogFormatter::LogFormatter(String pattern) : m_pattern(std::move(pattern)) {
	if (!s_item_map) {

		s_item_map = std::make_shared<ItemMap>(ItemMap{
#define XX(type) \
	std::pair<String, FormatItem::FactoryFunc>{type::FORMAT_STR, FormatItem::make_format_item<type>}
				XX(MessageFormatItem),	   //m:消息
				XX(LevelFormatItem),	   //g:日志级别
				XX(LoggerNameFormatItem),  //G:日志名称
				XX(ElapseFormatItem),	   //r:累计毫秒数
				XX(DateTimeFormatItem),	   //d:时间
				XX(FileFormatItem),		   //F:文件名
				XX(LineFormatItem),		   //l:行号
				XX(ThreadIdFormatItem),	   //t:线程id
				XX(CoroutineIdFormatItem), //c:协程id
				XX(ThreadNameFormatItem),  //T:线程名称
				XX(CoroutineNameFormatItem)//C:协程名
#undef XX
		});
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

String LogFormatter::format(const LogEvent &event) const {
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
		[[nodiscard]] bool empty() const { return pattern.empty() && extend.empty(); }
	};

	const String pattern = m_pattern;
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
		tmp_format_vec.emplace_back(String{pattern[pos++], pattern[pos]}, String{});

		// 检测是否符合扩展要求
		if (pattern[pos + 1] != '{') {
			//退出格式化模式
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
			m_items.emplace_back(new FormatItem(format.extend));
			continue;
		}
		auto fd_res = s_item_map->find(format.pattern);
		if (fd_res == s_item_map->end()) {
			m_items.emplace_back(new FormatItem(format.pattern + "{" + format.extend + "}"));
			continue;
		}
		m_items.emplace_back(fd_res->second(format.extend));
	}
}
MYLIB_END
