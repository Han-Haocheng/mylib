//
// Created by HanHaocheng on 2024/4/2.
//

#ifndef MY_PROJECT_CONFIG_MANAGER_H
#define MY_PROJECT_CONFIG_MANAGER_H
#include "converter.h"
#include "mylib_def.h"

#include <algorithm>

MYLIB_SPACE_BEGIN

class ConfigValueBase {
public:
  using ptr = std::shared_ptr<ConfigValueBase>;

  ConfigValueBase(String type_info) : m_type_name(std::move(type_info)) {}
  virtual void fromString(String string) = 0;
  [[nodiscard]] virtual String toString() const = 0;
  [[nodiscard]] const String &type_name() const { return m_type_name; }

private:
  String m_type_name;
};

template<typename ConfTy, typename CastFunc = YamlConverter<ConfTy>>
class ConfigValue : public ConfigValueBase {
public:
  using value_type = ConfTy;
  using cast_func = CastFunc;
  using ptr = std::shared_ptr<ConfigValue<ConfTy, CastFunc>>;

  static_assert(!std::is_same<nullptr_t, cast_func>::value, "static error: cast_func is nullptr");
  static_assert(!IsStringConverter<cast_func>::value, "static error: cast func need have functaions `toString()` and `fromString()`");

  explicit ConfigValue(const value_type &conf) : ConfigValueBase(typeid(value_type).name()), m_value(conf) {}
  ConfigValue(value_type &&conf) : ConfigValueBase(typeid(value_type).name()), m_value(std::move(conf)) {}
  ~ConfigValue() = default;

  const value_type &value() const { return m_value; }

  void setValue(const ConfTy &val) { m_value = val; }
  void setValue(ConfTy &&val) { m_value = std::move(val); }

  void fromString(String string) override {
    try {
      m_value = CastFunc{}.fromString(string);
    } catch (const std::exception &e) {
      // TODO: <log>error
    }
  }

  [[nodiscard]] String toString() const override {
    try {
      return CastFunc{}.toString(m_value);
    } catch (const std::exception &e) {
      // TODO: <log>error
    }
    return "";
  }

private:
  value_type m_value;
};

class Configurator {
public:
  using ptr = std::shared_ptr<Configurator>;

  template<class Ty>
  Configurator(String configName, String comment, const Ty &val)
      : m_config_name(std::move(configName)),
        m_comment(std::move(comment)),
        m_config(nullptr) {
    if constexpr (std::is_same<ConfigValueBase::ptr, Ty>::value) {
      m_config = val;
    } else if constexpr (std::is_same<nullptr_t, Ty>::value) {
      // todo error
    } else {
      m_config = std::make_shared<ConfigValue<Ty>>(val);
    }
  }

  inline void reset(const String &configStr) {
    m_config->fromString(configStr);
  }

  inline void reset(ConfigValueBase::ptr config) {
    m_config = std::move(config);
  }

  template<class Ty>
  void reset(const Ty &config) {
    try {
      if (m_config) {
        *std::dynamic_pointer_cast<Ty>(m_config) = config;
      } else {
        m_config = std::make_shared<ConfigValue<Ty>>(config);
      }
    } catch (const std::exception &) {
      //todo error
    }
  }

  template<typename ConfTy>
  const ConfTy &as() const {
    try {
      if (!m_config) return {};
      return std::dynamic_pointer_cast<ConfigValue<ConfTy>>(m_config)->value();
    } catch (const std::exception &) {
      //todo error
    }
    return {};
  }

  template<>
  [[nodiscard]] const String &as<String>() const { return {m_config->toString()}; }
  [[nodiscard]] inline const String &comment() const { return m_comment; }

  inline void setComment(String newComment) {
    m_comment.assign(std::move(newComment));
  }

private:
  String m_config_name;
  String m_comment;
  ConfigValueBase::ptr m_config;
};

class ConfiguratorManager {
public:
  using ptr = std::shared_ptr<ConfiguratorManager>;

  bool loadFile(String file){
    return false;
  }
  bool saveFile(String file){
    return false;
  }

  bool setConfigurator(String configName, String comment, Configurator::ptr conf){
    return false;
  }

  //using Ty = int;
  template<class Ty>
  bool setConfigurator(String configName, String comment, const Ty &conf) {
    auto res = m_configurators.try_emplace(configName, std::make_shared<Configurator>(configName, comment, conf));

    if (!res.second) {
      auto cur_ele = res.first->second;
      cur_ele->template reset<Ty>(conf);
      cur_ele->setComment(comment);
    }
    return true;
  }

  Configurator::ptr getConfigurator(String key){
    return nullptr;
  }

  bool addConfigurator(Configurator::ptr conf){
    return false;
  }
  bool delConfigurator(String key){
    return false;
  }

private:
  void analysis_yaml(YAML::Node rootNode){

  }
  void analysis_json(String json){

  }

private:
  std::unordered_map<String, Configurator::ptr> m_configurators;
  static ConfiguratorManager::ptr s_configurator_manager;
};

MYLIB_SPACE_END

#endif//MY_PROJECT_CONFIG_MANAGER_H
