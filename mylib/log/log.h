//
// Created by HanHaocheng on 2024/5/7.
//

#ifndef MYLIB_LOG_H
#define MYLIB_LOG_H

#include "../concurrent/lock.h"
#include "../core/mylib_def.h"
#include "../untils/until.h"

#include <fstream>
#include <utility>

#define MYLIB_LOG_NAME(name) mylib::LoggerManager::GetInstance()->try_getLogger(name)

#define MYLIB_LOG_BASE(level, logger)                                       \
  if (logger && logger->getType() < MYLIB_SPACE_NAME::LogEvent::LE_##level) \
  MYLIB_SPACE_NAME::LogWarp{MYLIB_SPACE_NAME::LogEvent::LE_##level, logger, __FILE__, __func__, CLASS_NAME_, __LINE__}.msg()

#define MYLIB_LOG_DEBUG(logger) MYLIB_LOG_BASE(DEBUG, logger)
#define MYLIB_LOG_INFO(logger) MYLIB_LOG_BASE(INFO, logger)
#define MYLIB_LOG_WARN(logger) MYLIB_LOG_BASE(WARN, logger)
#define MYLIB_LOG_ERROR(logger) MYLIB_LOG_BASE(ERROR, logger)

MYLIB_SPACE_BEGIN

using LogUselock = Spinlock;

class LogEvent {
public:
  using ptr = std::shared_ptr<LogEvent>;
  enum Type
  {
    LE_UNDEFINED = -1,
    LE_DEBUG,
    LE_INFO,
    LE_WARN,
    LE_ERROR,
  };

  explicit LogEvent(String loggerName = "", String file = "", String funcName = "", String className = "", String coroutineName = "", String threadName = "UNKNOWN", Type type = LE_UNDEFINED, uint32 line = 0, time_t t = 0, tid_t thread = 0, cid_t coroutine = 0);
  inline SString &msg() { return m_msg; }
  inline const SString &msg() const { return m_msg; }
  inline const String &getLoggerName() const { return m_logger_name; }
  inline void setLoggerName(const String &mLoggerName) { m_logger_name = mLoggerName; }
  inline Type getType() const { return m_type; }
  inline void setType(Type mType) { m_type = mType; }
  inline uint32 getLine() const { return m_line; }
  inline void setLine(uint32 mLine) { m_line = mLine; }
  inline const String &getFile() const { return m_file; }
  inline void setFile(const String &mFile) { m_file = mFile; }
  inline const String &getFuncName() const { return m_func_name; }
  inline void setFuncName(const String &mFuncName) { m_func_name = mFuncName; }
  inline const String &getClassName() const { return m_class_name; }
  inline void setClassName(const String &mClassName) { m_class_name = mClassName; }
  inline const time_t &getTime() const { return m_time; }
  inline void setTime(time_t mTime) { m_time = mTime; }
  inline tid_t getThreadId() const { return m_thread_id; }
  inline void setThreadId(tid_t mThreadId) { m_thread_id = mThreadId; }
  inline cid_t getCoroutineId() const { return m_coroutine_id; }
  inline void setCoroutineId(cid_t mCoroutineId) { m_coroutine_id = mCoroutineId; }
  inline const String &getThreadName() const { return m_thread_name; }
  inline void setMThreadName(const String &mThreadName) { m_thread_name = mThreadName; }
  inline const String &getCoroutineName() const { return m_coroutine_name; }
  inline void setMCoroutineName(const String &mCoroutineName) { m_coroutine_name = mCoroutineName; }

private:
  // 输出类型
  String m_logger_name;//日志器名
  Type m_type;         //日志类型

  // 位置信息
  uint32 m_line;      //行数
  String m_file;      //文件名
  String m_func_name; //函数名
  String m_class_name;//类名

  // 异步信息
  time_t m_time;       //时间戳
  tid_t m_thread_id;   //线程id
  cid_t m_coroutine_id;//协程id
  String m_thread_name;
  String m_coroutine_name;

  // 日志信息
  SString m_msg;
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
      "[%d{%Y-%m-%d %H-%M-%S}]%T[%f:%l]%T[%N.%C - %t.%F]%T%c(%p)%T>>>>%m%n";

  using ptr = std::shared_ptr<LogFormatter>;
  using Lock = LogUselock;

  class FormatItem {
  public:
    using ptr = std::shared_ptr<FormatItem>;
    using FactoryFunc = std::function<ptr(String extendStr)>;
    explicit FormatItem(String extendStr)
        : m_extend(std::move(extendStr)) {}
    [[nodiscard]] virtual String format(const LogEvent &event) const = 0;

    constexpr static const CString FORMAT_STR = "";

    template<typename Item>
    static std::pair<String, FactoryFunc> make_format_item() {
      return std::make_pair(Item::FORMAT_STR, [](String exStr) -> LogFormatter::FormatItem::ptr {
        return std::make_shared<Item>(std::move(exStr));
      });
    }

    [[nodiscard]] const String &getExtend() const { return m_extend; }

  protected:
    String m_extend;//扩展
  };

  using ItemMap = std::unordered_map<String, FormatItem::FactoryFunc>;

public:
  explicit LogFormatter(String pattern);
  void setPattern(const String &pattern);
  String format(const LogEvent &event);
  static LogFormatter::ptr DEF_FORMATTER();

private:
  void _parse_pattern();

  Lock m_lock;
  String m_pattern;
  std::vector<FormatItem::ptr> m_items;

  static ItemMap s_item_map;
  static LogFormatter::ptr s_def_formatter;
};

class LogAppender {
public:
  constexpr static const auto DEF_LEVEL = LogEvent::Type::LE_UNDEFINED;
  using ptr = std::shared_ptr<LogAppender>;
  using Lock = LogUselock;
  inline explicit LogAppender(LogEvent::Type type = DEF_LEVEL,
                              LogFormatter::ptr formatter = nullptr)
      : m_type(type), m_formatter(std::move(formatter)) {}

  virtual void append(const LogEvent &event) = 0;
  inline void setType(LogEvent::Type type) { m_type = type; }
  inline void setFormatter(const LogFormatter::ptr &formatter) { m_formatter = formatter; }
  [[nodiscard]] inline LogEvent::Type getType() const { return m_type; }
  [[nodiscard]] inline LogFormatter::ptr getFormatter() const { return m_formatter; }

protected:
  Lock m_lock;
  LogEvent::Type m_type;
  LogFormatter::ptr m_formatter;
};

class ConsoleAppender : public LogAppender {
public:
  explicit ConsoleAppender(LogEvent::Type type, LogFormatter::ptr formatter = nullptr);
  void append(const LogEvent &event) override;
};

class FileAppender : public LogAppender {
public:
  FileAppender(String file, LogEvent::Type type, LogFormatter::ptr formatter = nullptr);
  void append(const LogEvent &event) override;
  ~FileAppender();
private:
  void try_open_file();
private:
  std::ofstream fs;
  String m_file_path;
};

#define MYLIB_LOG_ROOT_LOGGER MYLIB_SPACE_NAME::Logger::RootLogger()

class Logger {
public:
  constexpr static const CString ROOT_NAME = "root";
  constexpr static const LogEvent::Type DEF_LEVEL = LogEvent::LE_UNDEFINED;
  using ptr = std::shared_ptr<Logger>;
  using Lock = LogUselock;
  Logger(String name,
         LogEvent::Type level,
         LogFormatter::ptr formatter)
      : m_name(std::move(name)), m_type(level), m_appenders(), m_formatter(std::move(formatter)) {}

  [[nodiscard]] inline const String &getName() const { return m_name; }
  inline void setName(const String &name) { m_name = name; }
  [[nodiscard]] inline const LogFormatter::ptr &getFormatter() const { return m_formatter; }
  inline void setFormatter(const LogFormatter::ptr &formatter) { m_formatter = formatter; }
  [[nodiscard]] inline LogEvent::Type getType() const { return m_type; }
  void addAppender(const LogAppender::ptr &appender);
  void log(const LogEvent &event);
  static Logger::ptr RootLogger();

private:
  String m_name;

  LogEvent::Type m_type;
  std::vector<LogAppender::ptr> m_appenders;
  LogFormatter::ptr m_formatter;
  Lock m_lock;
  static Logger::ptr m_root_logger;
};

class LogWarp {
public:
  LogWarp(LogEvent::Type level, Logger::ptr logger, String file, String funcName, String className, uint32 line);
  inline ~LogWarp() { m_logger->log(m_event); }

  [[nodiscard]] inline SString &msg() { return m_event.msg(); }

private:
  LogEvent::Type m_level;
  LogEvent m_event;
  Logger::ptr m_logger;
};

class LoggerConfig {
};

class LoggerManager {
public:
  using ptr = std::shared_ptr<LoggerManager>;
  using Lock = LogUselock;

  static ptr GetInstance();

  void addLogger(const Logger::ptr &logger);
  Logger::ptr addLogger(const String &name, LogEvent::Type level, LogFormatter::ptr formatter);
  [[nodiscard]] Logger::ptr getLogger(const String &name);
  [[nodiscard]] Logger::ptr try_getLogger(const String &name);

private:
  void _add_logger(const Logger::ptr &logger);
  Logger::ptr _get_logger(const String &name);

private:
  Lock m_lock;
  LogFormatter::ptr m_def_formatter;
  std::unordered_map<String, Logger::ptr> m_loggers;
  static LoggerManager::ptr s_instance;
};

MYLIB_SPACE_END

#endif//MYLIB_LOG_H
