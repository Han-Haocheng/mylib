#ifndef MYLIB_LOG_EVENT_H_
#define MYLIB_LOG_EVENT_H_

#include "../core/core.h"

#include <memory>
#include <stdexcept>
#include <unordered_map>

MYLIB_BEGIN

class LogEvent {
public:
  using ptr = std::shared_ptr<LogEvent>;
  enum value_type
  {
    LE_UNDEFINED = -1,
    LE_DEBUG,
    LE_INFO,
    LE_WARN,
    LE_ERROR,
  };

  explicit LogEvent(SourceInfo localInfo, String loggerName, String coroutineName, String threadName, value_type type, time_t t, tid_t thread, cid_t coroutine);

  inline SString &msg() { return m_msg; }

  [[nodiscard]] inline const SString &msg() const { return m_msg; }
  [[nodiscard]] inline const String &getLoggerName() const { return m_logger_name; }
  [[nodiscard]] inline value_type getType() const { return m_type; }
  [[nodiscard]] inline uint32 getLine() const { return m_local_info.line; }
  [[nodiscard]] inline const String &getFile() const { return m_local_info.file_name; }
  [[nodiscard]] inline const String &getFuncName() const { return m_local_info.func_name; }
  [[nodiscard]] inline const String &getClassName() const { return m_local_info.class_name; }
  [[nodiscard]] inline const time_t &getTime() const { return m_time; }
  [[nodiscard]] inline tid_t getThreadId() const { return m_thread_id; }
  [[nodiscard]] inline cid_t getCoroutineId() const { return m_coroutine_id; }
  [[nodiscard]] inline const String &getThreadName() const { return m_thread_name; }
  [[nodiscard]] inline const String &getCoroutineName() const { return m_coroutine_name; }

private:
  // 基本属性
  String m_logger_name;   //日志器名
  value_type m_type;      //日志类型
  SourceInfo m_local_info;//位置信息
  time_t m_time;          //时间戳
  tid_t m_thread_id;      //线程id
  cid_t m_coroutine_id;   //协程id
  String m_thread_name;   //线程名
  String m_coroutine_name;//协程名

  // 运行信息
  SString m_msg;// 日志信息
};

template<>
class StringCast<LogEvent::value_type> {
public:
  using Ty = LogEvent::value_type;

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
  {"LE_" #ABC, Ty::LE_##ABC}, {#ABC, Ty::LE_##ABC}, {#Abc, Ty::LE_##ABC}, {#abc, Ty::LE_##ABC}
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
