#ifndef MYLIB_LOG_EVENT_H_
#define MYLIB_LOG_EVENT_H_

#include "../convert/convert.h"
#include "../convert/type_cast.h"
#include "../core.h"


#include <memory>
#include <stdexcept>
#include <unordered_map>

MYLIB_BEGIN

class LogEvent {
public:
	using ptr = std::shared_ptr<LogEvent>;
	enum Level {
		LE_UNDEFINED = -1,
		LE_DEBUG,
		LE_INFO,
		LE_WARN,
		LE_ERROR,
	};

	explicit LogEvent(String loggerName, Level level, time_t t, DebugLocationInfo localInfo, DebugTaskInfo taskInfo);

	inline SString &msg() { return m_msg; }

	[[nodiscard]] inline const SString &msg() const { return m_msg; }
	[[nodiscard]] inline const String &getLoggerName() const { return m_logger_name; }
	[[nodiscard]] inline Level getType() const { return m_level; }
	[[nodiscard]] inline uint32 getLine() const { return m_local_info.line; }
	[[nodiscard]] inline const String &getFile() const { return m_local_info.file_name; }
	[[nodiscard]] inline const String &getFuncName() const { return m_local_info.func_name; }
	[[nodiscard]] inline const String &getClassName() const { return m_local_info.class_name; }
	[[nodiscard]] inline const time_t &getTime() const { return m_time; }
	[[nodiscard]] inline tid_t getThreadId() const { return m_task_info.thread_id; }
	[[nodiscard]] inline cid_t getCoroutineId() const { return m_task_info.coroutine_id; }
	[[nodiscard]] inline const String &getThreadName() const { return m_task_info.thread_name; }
	[[nodiscard]] inline const String &getCoroutineName() const { return m_task_info.coroutine_name; }

private:
	// 基本属性
	String m_logger_name;//日志器名
	Level m_level;		 //日志类型
	time_t m_time;		 //时间戳

	DebugTaskInfo m_task_info;
	DebugLocationInfo m_local_info;//位置信息


	// 运行信息
	SString m_msg;// 日志信息
};

template<>
struct ConversionTraits<String, LogEvent::Level, ConvertType::CT_DEFAULT> {
	using target_t = String;
	using source_t = LogEvent::Level;

	target_t convert(const source_t &source) {
		switch (source) {
			case source_t::LE_DEBUG:
				return "debug";
			case source_t::LE_INFO:
				return "info";
			case source_t::LE_WARN:
				return "warn";
			case source_t::LE_ERROR:
				return "error";
			default:
				return "unknown";
		}
	}
};

template<>
struct ConversionTraits<LogEvent::Level, String, ConvertType::CT_DEFAULT> {
	using target_t = LogEvent::Level;
	using source_t = String;

	target_t convert(const source_t &source) {
		static std::unordered_map<source_t, target_t> s_map{
#define XX(ABC, Abc, abc) \
	{"LE_" #ABC, target_t::LE_##ABC}, {#ABC, target_t::LE_##ABC}, {#Abc, target_t::LE_##ABC}, { #abc, target_t::LE_##ABC }
				XX(DEBUG, Debug, debug),
				XX(INFO, Info, info),
				XX(WARN, Warn, warn),
				XX(ERROR, Error, error),
#undef XX
		};

		MYLIB_TRY_CATCH_BEGIN
		return s_map.at(source);
		MYLIB_TRY_CATCH_END("convert error")
		return target_t::LE_UNDEFINED;
	}
};

template<>
class StringCast<LogEvent::Level> {
public:
	using Ty = LogEvent::Level;

	static String toString(const Ty &ty) {
		switch (ty) {
			case Ty::LE_DEBUG:
				return "Debug";
			case Ty::LE_INFO:
				return "Info";
			case Ty::LE_WARN:
				return "Warn";
			case Ty::LE_ERROR:
				return "Error";
			default:
				return "Unknown";
		}
	}

	static Ty fromString(const String &ty) {
		static std::unordered_map<String, Ty> s_map{
#define XX(ABC, Abc, abc) \
	{"LE_" #ABC, Ty::LE_##ABC}, {#ABC, Ty::LE_##ABC}, {#Abc, Ty::LE_##ABC}, { #abc, Ty::LE_##ABC }
				XX(DEBUG, Debug, debug),
				XX(INFO, Info, info),
				XX(WARN, Warn, warn),
				XX(ERROR, Error, error),
#undef XX
		};
		try {
			return s_map.at(ty);
		} catch (std::out_of_range &) {
			return Ty::LE_UNDEFINED;
		}
	}
};

MYLIB_END
#endif// !MYLIB_LOG_EVENT_H_
