//
// Created by HanHaocheng on 2024/4/2.
//

#ifndef MY_PROJECT_CONFIG_MANAGER_H
#define MY_PROJECT_CONFIG_MANAGER_H
#include "converter.h"
#include "mylib_def.h"

#include <algorithm>
#include <utility>

MYLIB_SPACE_BEGIN

class ConfigValueBase {
public:
  using ptr = std::shared_ptr<ConfigValueBase>;

  explicit ConfigValueBase(String name, String comment, String type_name);
  virtual ~ConfigValueBase() = default;

  virtual void fromString(String string) = 0;
  [[nodiscard]] virtual String toString() const = 0;
  [[nodiscard]] const String &type_name() const { return m_type_name; }

  [[nodiscard]] const String &comment() const { return m_comment; }
  [[nodiscard]] const String &name() const { return m_name; }

  void setComment(const String &mComment) { m_comment = mComment; }
  void setName(const String &mConfigName) { m_name = mConfigName; }

private:
  String m_name;
  String m_comment;
  String m_type_name;
};

template<typename ConfTy, typename CastFunc = YamlConverter<ConfTy>>
class ConfigValue : public ConfigValueBase {
public:
  static_assert(!std::is_same<nullptr_t, CastFunc>::value, "static error: cast_func is nullptr");
  static_assert(IsStringConverter<CastFunc>::value, "static error:cast_func don't have toString and fromString");

  using value_type = ConfTy;
  using cast_func = YamlConverter<value_type>;
  using ptr = std::shared_ptr<ConfigValue<value_type, cast_func>>;

  explicit ConfigValue(String name, const value_type &val, String comment)
      : ConfigValueBase(std::move(name), std::move(comment), typeid(value_type).name()), m_value(val) {}

  explicit ConfigValue(String name, value_type &&conf, String comment)
      : ConfigValueBase(std::move(name), std::move(comment), typeid(value_type).name()), m_value(std::move(conf)) {}

  ~ConfigValue() override = default;

  void setValue(const value_type &val) { m_value = val; }
  void setValue(value_type &&val) { m_value = val; }
  value_type &value() { return m_value; }
  [[nodiscard]] const value_type &value() const { return m_value; }

  template<class AsType = value_type>
  [[nodiscard]] const AsType &as() const {
    if constexpr (std::is_same<value_type, AsType>::value) {
      return m_value;
    } else if constexpr (std::is_same<std::string, AsType>::value) {
      return cast_func{}.toString(m_value);
    }
    //static_assert(false, "static error: unknown error");
  }

  template<class AsType, class Converter = Converter<value_type, AsType>, typename = std::enable_if<!std::is_same<AsType, value_type>::value>::type>
  [[nodiscard]] const AsType &as() const {
    return Converter{}.cast(m_value);
  }

  [[nodiscard]] String toString() const override { return {cast_func{}.toString(m_value)}; }
  void fromString(String string) override { m_value = cast_func{}.fromString(string); }

private:
  value_type m_value;
};

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

#endif//MY_PROJECT_CONFIG_MANAGER_H
