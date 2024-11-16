//
// Created by HanHaocheng on 2024/6/21.
//
#ifndef MYLIB_CONFIG_MANAGER_H_
#define MYLIB_CONFIG_MANAGER_H_

#include "config_value.h"

#include <unordered_map>

MYLIB_BEGIN

class Configurator {
public:
  using ptr = std::shared_ptr<Configurator>;

  enum FileType
  {
    FT_YAML,
    FT_JSON,
  };

  Configurator();
  Configurator(const Configurator &) = delete;
  Configurator(Configurator &&moved) noexcept = delete;
  ~Configurator();

  Configurator &operator=(const Configurator &) = delete;
  Configurator &operator=(Configurator &&moved) noexcept = delete;

  bool loadFile(const String &filename);
  bool saveFile(const String &file, FileType file_type = FT_JSON);

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

  [[nodiscard]] const auto &getAllConfig() const { return m_configurators; }

private:
  void analysis_yaml(const YAML::Node &rootNode);
  void analysis_json(const String &json);
  ConfigValueBase::ptr get_config_val_base(const String &key);
  bool conversion_yaml(std::ofstream &ofs);

  std::unordered_map<String, ConfigValueBase::ptr> m_configurators;
};

//======================================================================================

template<class Ty>
bool Configurator::setConfig(String name, const Ty &conf, const String &comment) {
  const auto fd_rt = m_configurators.find(name);
  ConfigValueBase::ptr value = std::make_shared<ConfigValue<Ty>>(name, conf, comment);
  if (fd_rt == m_configurators.end()) {
    m_configurators.emplace(name, value);
  } else {
    fd_rt->second = value;
  }
  return true;
}

template<>
inline bool Configurator::setConfig<ConfigValueBase::ptr>(String name, const ConfigValueBase::ptr &conf, const String &comment) {
  if (!conf) {
    return false;
  }
  m_configurators.emplace(std::move(name), conf);
  return true;
}

template<class Ty>
typename ConfigValue<Ty>::ptr Configurator::getConfig(const String &key) {
  if (key.empty()) {
    return nullptr;
  }
  auto rt = get_config_val_base(key);
  return std::dynamic_pointer_cast<ConfigValue<Ty>>(rt);
}

MYLIB_END

#endif
