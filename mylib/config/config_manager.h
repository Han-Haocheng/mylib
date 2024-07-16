﻿//
// Created by HanHaocheng on 2024/6/21.
//
#ifndef MYLIB_CONFIG_MANAGER_H_
#define MYLIB_CONFIG_MANAGER_H_

#include "../core/mylib_def.h"
#include "config_value.h"

MYLIB_SPACE_BEGIN

class ConfigManager {
public:
  using ptr = std::shared_ptr<ConfigManager>;

  enum FileType
  {
    FT_YAML,
    FT_JSON,
  };

  ConfigManager(const ConfigManager &) = delete;
  ConfigManager(ConfigManager &&moved) noexcept = delete;
  ~ConfigManager();

  ConfigManager &operator=(const ConfigManager &) = delete;
  ConfigManager &operator=(ConfigManager &&moved) noexcept = delete;

  static ConfigManager::ptr GetInstance();

  bool loadFile(const String &filename);
  bool saveFile(const String &file, FileType file_type);
  //  template<class Ty,class const_ref = const Ty&>
  //  bool setConfig(const String &name, const_ref conf, const String &comment = "") {
  //  }
  template<class Ty>
  bool setConfig(String name, const Ty &conf, const String &comment = "");
  template<>
  bool setConfig<ConfigValueBase::ptr>(String name, const ConfigValueBase::ptr &conf, const String &comment);

  template<class Ty>
  typename ConfigValue<Ty>::ptr getConfig(const String &key);
  bool delConfig(const String &key);

private:
  ConfigManager();

  void analysis_yaml(const YAML::Node &rootNode);
  void analysis_json(const String &json);
  ConfigValueBase::ptr get_config_val_base(const String &key);
  bool conversion_yaml(std::ofstream &ofs);

private:
  std::unordered_map<String, ConfigValueBase::ptr, std::hash<std::string>> m_configurators;
  static ConfigManager::ptr s_config_manager;
};

//======================================================================================

template<class Ty>
bool ConfigManager::setConfig(String name, const Ty &conf, const String &comment) {
  auto fd_rt = m_configurators.find(name);
  ConfigValueBase::ptr value = std::make_shared<ConfigValue<Ty>>(name, conf, comment);
  if (fd_rt == m_configurators.end()) {
    m_configurators.emplace(name, value);
  } else {
    fd_rt->second = value;
  }
  return true;
}

template<>
bool ConfigManager::setConfig<ConfigValueBase::ptr>(String name, const ConfigValueBase::ptr &conf, const String &comment) {
  if (!conf) {
    return false;
  }
  m_configurators.emplace(name, conf);
  return true;
}

template<class Ty>
typename ConfigValue<Ty>::ptr ConfigManager::getConfig(const String &key) {
  if (key.empty()) {
    return nullptr;
  }
  auto rt = get_config_val_base(key);
  return std::dynamic_pointer_cast<ConfigValue<Ty>>(rt);
}

MYLIB_SPACE_END

#endif
