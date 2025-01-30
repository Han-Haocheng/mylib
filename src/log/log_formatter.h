#ifndef MYLIB_LOG_FORMATTER_H_
#define MYLIB_LOG_FORMATTER_H_

#include "../task.h"


#include "log_event.h"

#include <functional>

MYLIB_BEGIN

class FormatItem {
public:
	using ptr		  = std::shared_ptr<FormatItem>;
	using FactoryFunc = std::function<ptr(String extendStr)>;

	explicit FormatItem(String extendStr)
		: m_extend(std::move(extendStr)) {}
	[[nodiscard]] virtual String format(const LogEvent &event) const = 0;

	virtual ~FormatItem()											 = default;
	constexpr static const CString FORMAT_STR						 = "";

	template<typename Item>
	static std::pair<String, FactoryFunc> make_format_item() {
		return std::make_pair(Item::FORMAT_STR, [](String exStr) -> FormatItem::ptr {
			return std::make_shared<Item>(std::move(exStr));
		});
	}

	[[nodiscard]] const String &getExtend() const { return m_extend; }

protected:
	String m_extend;//额外
};

//m:消息
//p:日志级别
//r:累计毫秒数
//c:日志名称
//t:线程id
//n:换行
//d:时间
//f:文件名
//l:行号
//T:Tab
//F:协程id
//N:线程名称
class LogFormatter {
public:
	constexpr static const CString DEF_PATTERN =
			"[%d{%Y/%m/%d %H:%M:%S}][%f:%l][%N.%C - %t.%F] %c(%p)%T>>>>%m%n";

	using ptr	  = std::shared_ptr<LogFormatter>;
	using Lock	  = Spinlock;

	using ItemMap = std::unordered_map<String, FormatItem::FactoryFunc>;

	explicit LogFormatter(String pattern);

	void setPattern(const String &pattern);
	String format(const LogEvent &event);

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

MYLIB_END

#endif// MYLIB_LOG_FORMATTER_H_
