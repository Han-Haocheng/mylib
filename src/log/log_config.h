//
// Created by HanHaocheng on 2025/2/1.
//

#ifndef MYLIB_LOG_CONFIG_H
#define MYLIB_LOG_CONFIG_H

#include "../config/ConfigValue.h"
#include "log_appender.h"
#include "log_service.h"

MYLIB_BEGIN

struct LogFormatterConfig {
	String pattern;
};

struct LogAppenderConfig {
	LogAppender::Type type;
	LogEvent::Level level;
	String path;
	LogFormatterConfig formatter;
};

struct LogServiceConfig {
	String name;
	String pattern;
	LogEvent::Level level;
	LogFormatterConfig m_formatter;
	std::vector<LogAppenderConfig> appenders;
};

class LogConfig final : ConfigValue<LogConfig> {
public:
	using super = ConfigValue<LogConfig>;
	using super::ConfigValue;


private:
	std::vector<LogServiceConfig> m_log_services;
};

template<>
struct ConversionTraits<String, LogFormatterConfig, ConvertType::CT_YAML> {
	using target_t = LogFormatterConfig;
	using source_t = String;

	static target_t convert(const source_t &src) {
		YAML::Node node = YAML::Load(src);
		target_t target{.pattern = node["pattern"].as<String>()};
		return target;
	}
};

template<>
struct ConversionTraits<LogFormatterConfig, String, ConvertType::CT_YAML> {
	using target_t = String;
	using source_t = LogFormatterConfig;

	static target_t convert(const source_t &src) {
		YAML::Node node;
		node["pattern"] = src.pattern;
		return YAML::Dump(node);
	}
};

template<>
struct ConversionTraits<String, LogAppenderConfig, ConvertType::CT_YAML> {
	using target_t = LogAppenderConfig;
	using source_t = String;

	static target_t convert(const source_t &src) {
		YAML::Node node = YAML::Load(src);
		target_t target{.pattern = node["pattern"].as<String>()};
		return target;
	}
};

template<>
struct ConversionTraits<LogAppenderConfig, String, ConvertType::CT_YAML> {
	using target_t = String;
	using source_t = LogAppenderConfig;

	static target_t convert(const source_t &src) {
		YAML::Node node;
		node["path"]	  = src.path;
		node["type"]	  = src.type;
		node["level"]	  = ConversionTraits<LogAppender::Type, String, ConvertType::CT_DEFAULT>::convert(src.type);
		node["formatter"] = ConversionTraits<LogFormatterConfig, String, ConvertType::CT_YAML>::convert(src.formatter);
		return YAML::Dump(node);
	}
};


MYLIB_END
#endif//MYLIB_LOG_CONFIG_H
