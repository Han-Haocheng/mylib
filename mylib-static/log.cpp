//
// Created by HanHaocheng on 2024/5/7.
//

#include "log.h"
#include "converter.h"
#include "sync.h"

#include <iostream>
#include <memory>
#include <utility>

MYLIB_SPACE_BEGIN
LogEvent::LogEvent(String loggerName, String file, String funcName, String className, String threadName, String coroutineName, value_type type, uint32 line, time_t t, tid_t thread, cid_t coroutine)
    : m_logger_name(std::move(loggerName)),
      m_type(type),
      m_line(line),
      m_file(std::move(file)),
      m_func_name(std::move(funcName)),
      m_class_name(std::move(className)),
      m_time(t),
      m_thread_id(thread),
      m_coroutine_id(coroutine),
      m_thread_name(std::move(threadName)),
      m_coroutine_name(std::move(coroutineName)) {
}

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
#ifdef MYLIB_GUN
    sleep(1);
#elif MYLIB_MSVC
    Sleep(1);
#endif
    fs.open(m_file_path, std::ios::app);
  }
}

template<>
class StringConverter<LogEvent::value_type> {
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

// 字符串
class StringFormatItem : public LogFormatter::FormatItem {
public:
  constexpr static const CString FORMAT_STR = "";
  explicit StringFormatItem(String extendStr) : FormatItem(std::move(extendStr)) {}
  [[nodiscard]] String format(const LogEvent &event) const override { return m_extend; }
};

class MessageFormatItem : public LogFormatter::FormatItem {
public:
  constexpr static const CString FORMAT_STR = "%m";
  explicit MessageFormatItem(String extendStr) : FormatItem(std::move(extendStr)) {}
  [[nodiscard]] String format(const LogEvent &event) const override { return event.msg().str(); }
};

class TypeFormatItem : public LogFormatter::FormatItem {
public:
  constexpr static const CString FORMAT_STR = "%p";
  explicit TypeFormatItem(String extendStr) : FormatItem(std::move(extendStr)) {}
  [[nodiscard]] String format(const LogEvent &event) const override { return mylib::StringConverter<mylib::LogEvent::value_type>::toString(event.getType()); }
};

class ElapseFormatItem : public LogFormatter::FormatItem {
public:
  constexpr static const CString FORMAT_STR = "%r";
  explicit ElapseFormatItem(String extendStr) : FormatItem(std::move(extendStr)) {}
  [[nodiscard]] String format(const LogEvent &event) const override { return "<elapse>"; }
};

class LoggerNameFormatItem : public LogFormatter::FormatItem {
public:
  constexpr static const CString FORMAT_STR = "%c";
  explicit LoggerNameFormatItem(String extendStr) : FormatItem(std::move(extendStr)) {}
  [[nodiscard]] String format(const LogEvent &event) const override { return event.getLoggerName(); }
};

class FileFormatItem : public LogFormatter::FormatItem {
public:
  constexpr static const CString FORMAT_STR = "%f";
  explicit FileFormatItem(String extendStr) : FormatItem(std::move(extendStr)) {}
  [[nodiscard]] String format(const LogEvent &event) const override { return event.getFile(); }
};

class LineFormatItem : public LogFormatter::FormatItem {
public:
  constexpr static const CString FORMAT_STR = "%l";
  explicit LineFormatItem(String extendStr) : FormatItem(std::move(extendStr)) {}
  [[nodiscard]] String format(const LogEvent &event) const override { return std::to_string(event.getLine()); }
};

class NewLineFormatItem : public LogFormatter::FormatItem {
public:
  constexpr static const CString FORMAT_STR = "%n";
  explicit NewLineFormatItem(String extendStr) : FormatItem(std::move(extendStr)) {}
  [[nodiscard]] String format(const LogEvent &event) const override { return "\n"; }
};

class TabFormatItem : public LogFormatter::FormatItem {
public:
  constexpr static const CString FORMAT_STR = "%T";
  explicit TabFormatItem(String extendStr) : FormatItem(std::move(extendStr)) {}
  [[nodiscard]] String format(const LogEvent &event) const override { return "\t"; }
};

// 异步相关
//时间
class DateTimeFormatItem : public LogFormatter::FormatItem {
public:
  constexpr static const CString FORMAT_STR = "%d";
  explicit DateTimeFormatItem(String extendStr = "%Y-%m-%d %H-%M-%S") : FormatItem(std::move(extendStr)) {}
  [[nodiscard]] String format(const LogEvent &event) const override {
    char tmp[64];
    ::tm t;
#ifdef MYLIB_MSVC
    localtime_s(&t, &event.getTime());
#elif MYLIB_GUN
    t = localtime(&event.getTime());
#endif// MYLIB_WINDOW
    strftime(tmp, sizeof(tmp), m_extend.c_str(), &t);

    return tmp;
  }
};

// 线程id
class ThreadIdFormatItem : public LogFormatter::FormatItem {
public:
  constexpr static const CString FORMAT_STR = "%t";
  explicit ThreadIdFormatItem(String extendStr) : FormatItem(std::move(extendStr)) {}
  [[nodiscard]] String format(const LogEvent &event) const override { return std::to_string(event.getThreadId()); }
};

class ThreadNameFormatItem : public LogFormatter::FormatItem {
public:
  constexpr static const CString FORMAT_STR = "%N";
  explicit ThreadNameFormatItem(String extendStr) : FormatItem(std::move(extendStr)) {}
  [[nodiscard]] String format(const LogEvent &event) const override {
    String res = event.getThreadName();
    return res;
  }
};

// 协程
class CoroutineIdFormatItem : public LogFormatter::FormatItem {
public:
  constexpr static const CString FORMAT_STR = "%F";
  explicit CoroutineIdFormatItem(String extendStr) : FormatItem(std::move(extendStr)) {}
  [[nodiscard]] String format(const LogEvent &event) const override { return std::to_string(event.getCoroutineId()); }
};
class CoroutineNameFormatItem : public LogFormatter::FormatItem {
public:
  constexpr static const CString FORMAT_STR = "%C";
  explicit CoroutineNameFormatItem(String extendStr) : FormatItem(std::move(extendStr)) {}
  [[nodiscard]] String format(const LogEvent &event) const override { return event.getCoroutineName(); }
};

LogFormatter::ItemMap LogFormatter::s_item_map = {};
LogFormatter::ptr LogFormatter::s_def_formatter = nullptr;

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

extern const char hello[] = "external linkage";// external linkage
const char hello11[] = "internal linkage!";    // internal linkage

// 位置相关
// 文件
// 行数
//
//%d{%Y-%m-%d %H-%M-%S}[%f:%l]%T%p%T%c%T%t%T%m%n
LogFormatter::LogFormatter(mylib::String pattern) : m_pattern(std::move(pattern)) {
  if (s_item_map.empty()) {
    s_item_map = ItemMap{
        FormatItem::make_format_item<MessageFormatItem>(),    //m:消息
        FormatItem::make_format_item<TypeFormatItem>(),       //p:日志级别
        FormatItem::make_format_item<ElapseFormatItem>(),     //r:累计毫秒数
        FormatItem::make_format_item<LoggerNameFormatItem>(), //c:日志名称
        FormatItem::make_format_item<ThreadIdFormatItem>(),   //t:线程id
        FormatItem::make_format_item<NewLineFormatItem>(),    //n:换行
        FormatItem::make_format_item<DateTimeFormatItem>(),   //d:时间
        FormatItem::make_format_item<FileFormatItem>(),       //f:文件名
        FormatItem::make_format_item<LineFormatItem>(),       //l:行号
        FormatItem::make_format_item<TabFormatItem>(),        //T:Tab
        FormatItem::make_format_item<CoroutineIdFormatItem>(),//F:协程id
        FormatItem::make_format_item<ThreadNameFormatItem>(), //N:线程名称
        FormatItem::make_format_item<CoroutineNameFormatItem>(),
    };
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
    auto ex_end = pattern.find_first_of('}', pos + 2);
    if (ex_end == std::string::npos) {
      // 如果是扩展未闭合，则进入普通模式
      //pos = end_pos;
      continue;
    }
    back_tf.extend = pattern.substr(pos + 2, ex_end - pos - 2);
    pos = ex_end;
  }
  if (!tmp_format.empty()) {
    tmp_format_vec.emplace_back(tmp_format);
  }

  for (const auto &format: tmp_format_vec) {
    if (format.pattern.empty()) {
      m_items.emplace_back(new StringFormatItem(format.extend));
      continue;
    }
    auto fd_res = s_item_map.find(format.pattern);
    if (fd_res == s_item_map.end()) {
      m_items.emplace_back(new StringFormatItem(format.pattern + "{" + format.extend + "}"));
      continue;
    }
    m_items.emplace_back(fd_res->second(format.extend));
  }
}

Logger::ptr Logger::m_root_logger = nullptr;

void Logger::addAppender(const LogAppender::ptr &appender) {
  if (!appender) {
    return;
  }

  appender->setFormatter(m_formatter);
  m_appenders.emplace_back(appender);
}

void Logger::log(const LogEvent &event) {
  if (event.getType() < m_type) {
    return;
  }

  if (m_appenders.empty() && this != m_root_logger.get()) {
    //std::cout << "logger<" << m_name << ">appender is empty" << std::endl;
    m_root_logger->log(event);
    return;
  }
  m_lock.lock();
  for (auto &appender: m_appenders) {
    m_lock.lock();
    appender->append(event);
    m_lock.unlock();
  }
}

Logger::ptr Logger::RootLogger() {
  if (!m_root_logger) {
    m_root_logger = std::make_shared<Logger>(
        ROOT_NAME,
        DEF_LEVEL,
        LogFormatter::DEF_FORMATTER());
    m_root_logger->addAppender(ConsoleAppender::ptr{new ConsoleAppender{DEF_LEVEL,
                                                                        LogFormatter::DEF_FORMATTER()}});
  }
  return m_root_logger;
}

LoggerManager::ptr LoggerManager::s_instance = nullptr;

LoggerManager::ptr LoggerManager::GetInstance() {
  if (!s_instance) {
    s_instance = std::make_shared<LoggerManager>();
    s_instance->_add_logger(Logger::RootLogger());
  }
  return s_instance;
}

void LoggerManager::addLogger(const Logger::ptr &logger) {
  if (!logger || logger->getName().empty()) {
    //TODO: <log>error
    return;
  }
  m_lock.lock();
  _add_logger(logger);
  m_lock.unlock();
}

Logger::ptr LoggerManager::addLogger(const String &name, LogEvent::value_type level, LogFormatter::ptr formatter) {
  if (name.empty())
    return nullptr;

  auto fmt = formatter ? std::move(formatter) : m_def_formatter;
  Logger::ptr logger = std::make_shared<Logger>(name,
                                                level == LogEvent::LE_UNDEFINED
                                                    ? Logger::DEF_LEVEL
                                                    : level,
                                                fmt);

  m_lock.lock();
  _add_logger(logger);
  m_lock.unlock();
  return logger;
}

Logger::ptr LoggerManager::getLogger(const String &name) {
  if (name.empty()) {
    //TODO: <log>error
    return nullptr;
  }
  m_lock.lock();
  auto res = _get_logger(name);
  m_lock.unlock();
  return res;
}

Logger::ptr LoggerManager::try_getLogger(const String &name) {
  if (name.empty()) {
    //TODO: <log>error
    return nullptr;
  }
  m_lock.lock();
  Logger::ptr log = _get_logger(name);
  m_lock.unlock();
  if (!log) {
    log = std::make_shared<Logger>(name, Logger::DEF_LEVEL, LogFormatter::DEF_FORMATTER());
    m_lock.lock();
    _add_logger(log);
    m_lock.unlock();
  }
  return log;
}

void LoggerManager::_add_logger(const Logger::ptr &logger) {
  auto is_res = m_loggers.insert(std::make_pair(logger->getName(), logger));
  if (!is_res.second) {
    is_res.first->second = logger;
  }
}

Logger::ptr LoggerManager::_get_logger(const String &name) {
  auto fd_res = m_loggers.find(name);
  if (fd_res == m_loggers.end()) {
    return nullptr;
  }
  return fd_res->second;
}

LogWarp::LogWarp(LogEvent::value_type level,
                 Logger::ptr logger,
                 String file,
                 String funcName,
                 String className, int32 line)
    : m_level(level),
      m_logger(std::move(logger)),
      m_event("logger_name",
              std::move(file),
              std::move(funcName),
              std::move(className),
              Thread::GetCurrentThreadName(),
              "UNKNOWN",
              m_level,
              line,
              GetCurrentTimestamp(),
              Thread::GetCurrentThreadId(),
              1) {
  m_event.setLoggerName(m_logger->getName());
}

MYLIB_SPACE_END
