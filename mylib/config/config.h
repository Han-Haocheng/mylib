//
// Created by HanHaocheng on 2024/4/2.
//

#ifndef MY_PROJECT_CONFIG_MANAGER_H
#define MY_PROJECT_CONFIG_MANAGER_H
#include "../converter/ymal_converter.h"
#include "../core/mylib_def.h"

#include <algorithm>

MYLIB_SPACE_BEGIN

class ConfigValueBase {
public:
  using ptr = std::shared_ptr<ConfigValueBase>;

  // ConfigValueBase(String name, String comment, String type_name);
  // ~ConfigValueBase() = default;
  //  [[nodiscard]] inline String name() const { return m_name; }
  //  [[nodiscard]] inline String comment() const { return m_comment; }
  //  [[nodiscard]] inline String typeName() const { return m_type_name; }

  virtual void fromString(String string) = 0;
  [[nodiscard]] virtual String toString() const = 0;

  //protected:
  //  String m_type_name;
  //  String m_config_str;
  //
  //private:
  //  String m_name;
  //  String m_comment;
};

template<typename ConfTy,
         typename CastFunc = YamlConverter<ConfTy>,
         typename = std::enable_if<is_converter<CastFunc>::value>>
class ConfigValue : public ConfigValueBase {
public:
  using Type = ConfTy;
  using ptr = std::shared_ptr<ConfigValue<ConfTy, CastFunc>>;

  //  ConfigValue(String name, const Type &conf, String comment)
  //      : ConfigValueBase(name, comment, typeid(Type).name()), m_value(conf) {
  //       m_config_str = CastFunc{}.toString(m_value);
  //  }

  explicit ConfigValue(const Type &conf)
      : m_value(conf) {
  }

  ~ConfigValue() = default;

  const Type &val() const { return m_value; }
  
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
  Type m_value;
};

class Configurator {
public:
  using ptr = std::shared_ptr<Configurator>;
  
  Configurator(String configName, String comment, ConfigValueBase::ptr config=nullptr);

  inline void reset(const String &config) {
    m_config->fromString(config);
  }

  inline void reset(ConfigValueBase::ptr config) {
    m_config = std::move(config);
  }

  template<class ConfTy>
  void reset(const ConfTy &config) {
    if (!m_config)
      m_config = std::make_shared<ConfigValue<ConfTy>>(config);
    *m_config = config;
  }

  template<typename ConfTy>
  const ConfTy &as() const {
    typename ConfigValue<ConfTy>::ptr res =
        std::dynamic_pointer_cast<ConfigValue<ConfTy>>(m_config);
    if (!res) {
      return {};
    }
    return res->config();
  }

  template<>
  [[nodiscard]] const String &as<String>() const { return {m_config->toString()}; }

private:
  String m_config_name;
  String m_comment;
  ConfigValueBase::ptr m_config;
};

class ConfiguratorManager {
public:
  using ptr = std::shared_ptr<ConfiguratorManager>;

  void loadFile(String file);
  void saveFile(String file);

  bool setConfigurator(Configurator::ptr conf);
  Configurator::ptr getConfigurator(String key);
  
  bool addConfigurator(Configurator::ptr conf);
  bool delConfigurator(String key);
private:

  void analysis_yaml(YAML::Node rootNode);
  void analysis_json(String json);

private:
  std::unordered_map<String, Configurator::ptr> m_configurators;
  static ConfiguratorManager::ptr s_configurator_manager;
};

//class ConfigManager {
//public:
//  constexpr static const CString DEF_CONFIG_FILE = "config.yaml";
//  using ptr = std::shared_ptr<ConfigManager>;
//
//  enum ConfigFileType : int8
//  {
//    CFT_YAML,
//    CFT_JSON,
//  };
//
//  explicit ConfigManager()
//      : m_config_file("config.yml"), m_configs() {}
//  explicit ConfigManager(String confPath) : m_config_file(std::move(confPath)), m_configs() {}
//  ~ConfigManager() = default;
//
//  void setConfigFile(const String &path);
//
//  template<typename ConfTy>
//  const ConfTy &getConfig(const String &key) const;
//
//  template<typename ConfTy>
//  ConfTy &getConfig(const String &key);
//
//  template<typename ConfTy>
//  void setConfig(String key, ConfTy &&conf, String comment = "");
//
//  void showAllConfigs();
//  bool load();
//  bool load(const String &file);
//  bool save(ConfigFileType type);
//
//private:
//  ConfigValueBase::ptr get_config_base(const String &key) const;
//
//  bool load_from_yaml() noexcept;
//  bool load_from_json() noexcept;
//  void save_to_yaml() noexcept;
//  void save_to_json() noexcept;
//
//  YAML::Node convert_to_yaml();
//
//  void recursive_load_yaml_func(const YAML::Node &node, String current_node_name);
//
//private:
//  //ConfigFileType m_config_file_type;
//  String m_config_file;
//  std::unordered_map<std::string, ConfigValueBase::ptr> m_configs;
//
//  static ConfigManager::ptr s_manager;
//};
//
//template<typename ConfTy>
//const ConfTy &ConfigManager::getConfig(const String &key) const {
//  auto conf_val = get_config_base(key);
//  if (!conf_val || conf_val->typeName() != typeid(ConfTy).name()) {
//    return ConfTy{};
//  }
//  return *dynamic_cast<ConfTy *>(conf_val.get());
//}
//
//template<typename ConfTy>
//ConfTy &ConfigManager::getConfig(const String &key) {
//  auto conf_val = get_config_base(key);
//  if (!conf_val || conf_val->typeName() != typeid(ConfTy).name()) {
//    return {};
//  }
//  return *dynamic_cast<ConfTy *>(conf_val.get());
//}
//
//template<typename ConfTy>
//void ConfigManager::setConfig(String key, ConfTy &&conf, String comment) {
//  if (key.empty()) {
//    return;
//  }
//
//  auto conf_val = get_config_base(key);
//  if (conf_val) {
//    conf_val.reset(new ConfigValue<ConfTy>{key, std::forward<ConfTy>(conf), comment});
//    return;
//  }
//  std::transform(key.begin(), key.end(), key.begin(), ::tolower);
//  conf_val = std::make_shared<ConfigValue<ConfTy>>(key, std::forward<ConfTy>(conf), std::move(comment));
//  m_configs[key] = conf_val;
//}

MYLIB_SPACE_END

#endif//MY_PROJECT_CONFIG_MANAGER_H
