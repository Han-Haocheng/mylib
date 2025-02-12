//
// Created by HanHaocheng on 2025/2/1.
//

#ifndef MYLIB_LOG_CONFIG_H
#define MYLIB_LOG_CONFIG_H

#include "log_appender.h"
#include "log_service.h"

MYLIB_BEGIN

struct LogFormatterConfig {
	String pattern;

	LogFormatter::ptr create() {
		LogFormatter::ptr formatter = pattern.empty() ? LogFormatter::DEF_FORMATTER() : std::make_shared<LogFormatter>(pattern);
		return formatter;
	}
};

struct LogAppenderConfig {
	LogAppender::Type type = static_cast<LogAppender::Type>(-1);
	LogEvent::Level level  = LogEvent::LE_UNDEFINED;
	String pattern;
	String path;

	LogAppender::ptr create() {
		if (level == LogEvent::LE_UNDEFINED) {
			MYLIB_THROW("config error: level is undefined.");
		}
		LogFormatter::ptr formatter = pattern.empty() ? LogFormatter::DEF_FORMATTER() : std::make_shared<LogFormatter>(pattern);

		switch (type) {
			case LogAppender::LAT_CONSOLE:
				return std::make_shared<ConsoleAppender>(level, formatter);
			case LogAppender::LAT_FILE:
				if (path.empty()) {
					MYLIB_THROW("config error: path is empty.");
				}
				return std::make_shared<FileAppender>(path, level, formatter);
		}
		return nullptr;
	}
};

struct LogServiceConfig {
	String name;
	String pattern;
	LogEvent::Level level = LogEvent::LE_UNDEFINED;
	std::vector<LogAppenderConfig> appenders;

	LogService::ptr create() {
		if (level == LogEvent::LE_UNDEFINED) {
			MYLIB_THROW("config error: level is undefined.");
		}
		if (name.empty()) {
			MYLIB_THROW("config error: name is empty.");
		}

		LogFormatter::ptr formatter = pattern.empty() ? LogFormatter::DEF_FORMATTER() : std::make_shared<LogFormatter>(pattern);
		auto service				= std::make_shared<LogService>(name, level, formatter);
		for (auto &appender: appenders) {
			service->addAppender(appender.create());
		}
		return service;
	}
};

struct LogConfig {
	String pattern;
	std::map<String, LogServiceConfig> m_value;

	std::vector<LogService::ptr> services() {
		std::vector<LogService::ptr> res;
		res.reserve(m_value.size());
		for (auto &[name, service]: m_value) {
			service.name = name;
			res.push_back(service.create());
		}
		return res;
	}

	LogFormatter::ptr formatter() {
		if (pattern.empty()) {
			return LogFormatter::DEF_FORMATTER();
		}
		return std::make_shared<LogFormatter>(pattern);
	}
};


//======================= Log转换特征 ===============================
template<>
struct ConversionTraits<String, LogFormatterConfig, ConvertType::CT_YAML> {
	using target_t = String;
	using source_t = LogFormatterConfig;

	static target_t convert(const source_t &src) {
		YAML::Node node;
		node["pattern"] = src.pattern;
		return YAML::Dump(node);
	}
};

template<>
struct ConversionTraits<LogFormatterConfig, String, ConvertType::CT_YAML> {
	using target_t = LogFormatterConfig;
	using source_t = String;

	static target_t convert(const source_t &src) {
		YAML::Node node = YAML::Load(src);
		target_t target{node["pattern"].as<String>()};
		return target;
	}
};

template<>
struct ConversionTraits<String, LogAppenderConfig, ConvertType::CT_YAML> {
	using target_t = String;
	using source_t = LogAppenderConfig;

	static target_t convert(const source_t &src) {
		YAML::Node node;
		node["type"]	= YAML::Load(ToStringBase<LogAppender::Type>(src.type));
		node["level"]	= YAML::Load(ToStringBase<LogEvent::Level>(src.level));
		node["pattern"] = src.pattern;
		node["path"]	= src.path;
		return YAML::Dump(node);
	}
};

template<>
struct ConversionTraits<LogAppenderConfig, String, ConvertType::CT_YAML> {
	using target_t = LogAppenderConfig;
	using source_t = String;

	static target_t convert(const source_t &src) {
		YAML::Node node = YAML::Load(src);
		target_t target;
		target.level   = FromStringBase<LogEvent::Level>(node["level"].as<String>());
		target.type	   = FromStringBase<LogAppender::Type>(node["type"].as<String>());
		target.pattern = node["pattern"].as<String>();
		target.path	   = node["path"].as<String>();
		return target;
	}
};

template<>
struct ConversionTraits<String, LogServiceConfig, ConvertType::CT_YAML> {
	using target_t = String;
	using source_t = LogServiceConfig;

	static target_t convert(const source_t &src) {
		YAML::Node node;
		node["level"]	= ToStringBase<LogEvent::Level>(src.level);
		node["pattern"] = src.pattern;
		for (const auto &appender: src.appenders) {
			node["appenders"].push_back(ToYamlBase<LogAppenderConfig>(appender));
		}
		return YAML::Dump(node);
	}
};

template<>
struct ConversionTraits<LogServiceConfig, String, ConvertType::CT_YAML> {
	using target_t = LogServiceConfig;
	using source_t = String;

	static target_t convert(const source_t &src) {
		YAML::Node node = YAML::Load(src);
		target_t target;
		target.pattern = node["pattern"].as<String>();
		target.level   = FromStringBase<LogEvent::Level>(node["level"].as<String>());
		for (const auto &appender: node["appenders"]) {
			target.appenders.push_back(FromYamlBase<LogAppenderConfig>(YAML::Dump(appender)));
		}
		return target;
	}
};


template<>
struct ConversionTraits<String, LogConfig, ConvertType::CT_YAML> {
	using target_t = String;
	using source_t = LogConfig;

	static target_t convert(const source_t &src) {
		YAML::Node node;
		node["pattern"] = src.pattern;
		node["loggers"] = ToYamlBase<std::map<String, LogServiceConfig>>(src.m_value);
		return YAML::Dump(node);
	}
};

template<>
struct ConversionTraits<LogConfig, String, ConvertType::CT_YAML> {
	using target_t = LogConfig;
	using source_t = String;

	static target_t convert(const source_t &src) {
		YAML::Node node = YAML::Load(src);
		target_t target;
		target.pattern = node["pattern"].as<String>();
		for (const auto &logger: node["loggers"]) {
			target.m_value[logger.first.as<String>()] = FromYamlBase<LogServiceConfig>(YAML::Dump(logger.second));
		}
		return target;
	}
};


MYLIB_END
#endif//MYLIB_LOG_CONFIG_H
