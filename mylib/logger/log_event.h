#ifndef MYLIB_LOG_EVENT_H_
#define MYLIB_LOG_EVENT_H_
#include "../core/mylib_def.h"

MYLIB_SPACE_BEGIN

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
  // 输出类型
  String m_logger_name;//日志器名
  value_type m_type;   //日志类型

  // 位置信息
  SourceInfo m_local_info;

  time_t m_time;//时间戳

  // 异步信息
  tid_t m_thread_id;   //线程id
  cid_t m_coroutine_id;//协程id
  String m_thread_name;
  String m_coroutine_name;

  // 日志信息
  SString m_msg;
};

MYLIB_SPACE_END
#endif// !MYLIB_LOG_EVENT_H_
