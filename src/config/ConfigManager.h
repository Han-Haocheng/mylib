//
// Created by HanHaocheng on 2024/6/21.
//
#ifndef MYLIB_CONFIG_MANAGER_H_
#define MYLIB_CONFIG_MANAGER_H_

#include "Config.h"

#include <unordered_map>

MYLIB_BEGIN

class ConfigManager : public Singleton<ConfigManager> {
public:
	using ptr = std::shared_ptr<ConfigManager>;

	enum FileType {
		FT_YAML,
		FT_JSON,
	};

	ConfigManager();
	ConfigManager(const ConfigManager &)		  = delete;
	ConfigManager(ConfigManager &&moved) noexcept = delete;
	~ConfigManager();

	ConfigManager &operator=(const ConfigManager &)			 = delete;
	ConfigManager &operator=(ConfigManager &&moved) noexcept = delete;

	bool loadFile(const String &filename);
	bool saveFile(const String &file, FileType file_type = FT_JSON);

	void setConfig(ConfigValueBasic::ptr conf);
	ConfigValueBasic::ptr getConfig(const String &key);

	template<class Ty>
	void setConfig(String name, const Ty &conf, const String &comment = "") {
		MYLIB_TRY_THROW(name.empty(), "config error: name do not exist or is empty");

		MYLIB_TRY_CATCH_BEGIN {
			setConfig(name, std::make_shared<Config<Ty>>(std::move(name), conf, comment));
		}
		MYLIB_TRY_CATCH_END("config error")
	}

	template<class Ty>
	typename Config<Ty>::ptr getConfig(const String &key) {
		MYLIB_TRY_THROW(key.empty(), "config error: key is empty when get config.");

		MYLIB_TRY_CATCH_BEGIN { return std::dynamic_pointer_cast<Config<Ty>>(getConfig(key)); }
		MYLIB_TRY_CATCH_END("config error")
		return nullptr;
	}

	bool delConfig(const String &key);
	[[nodiscard]] const auto &getAllConfig() const { return m_configurators; }

private:
	void analysis_yaml(const YAML::Node &rootNode);
	void analysis_json(const String &json);
	bool conversion_yaml(std::ofstream &ofs);

	std::unordered_map<String, ConfigValueBasic::ptr> m_configurators;
};

MYLIB_END

#endif
