#include "config.h"

#include <exception>
#include <fstream>
#include <functional>
#include <iostream>

MYLIB_SPACE_BEGIN

// ConfigValueBase::ConfigValueBase(String name, String comment, String type_name)
//     : m_type_name(std::move(type_name)), m_name(std::move(name)), m_comment(std::move(comment)) {
// }
// 
// void ConfigManager::setConfigFile(const String &path) {
//   m_config_file = path.empty() ? DEF_CONFIG_FILE : path;
// }
// 
// void ConfigManager::showAllConfigs() {
//   for (auto &conf_pair: m_configs) {
//     std::cout << conf_pair.first << " : " << conf_pair.second->toString() << "\n";
//   }
// }
// 
// bool ConfigManager::load() {
//   return load_from_yaml() || load_from_json();
// }
// 
// bool ConfigManager::load(const String &file) {
//   setConfigFile(file);
//   return load();
// }
// 
// bool ConfigManager::save(ConfigFileType type) {
//   switch (type) {
//     case CFT_YAML:
//       save_to_yaml();
//       break;
//     case CFT_JSON:
//       save_to_json();
//       break;
//   }
//   return true;
// }
// 
// ConfigValueBase::ptr ConfigManager::get_config_base(const String &key) const {
//   auto fd_res = m_configs.find(key);
//   if (fd_res == m_configs.end()) {
//     return nullptr;
//   }
//   return fd_res->second;
// }
// 
// bool ConfigManager::load_from_yaml() noexcept {
//   try {
//     YAML::Node root_node = YAML::LoadFile(m_config_file);
//     recursive_load_yaml_func(root_node, "");
//     return true;
//   } catch (const YAML::BadFile &bf) {
//     //TODO: <exception>
//   } catch (...) {
//   }
//   return false;
// }
// bool ConfigManager::load_from_json() noexcept {
//   return false;
// }
// 
// void ConfigManager::save_to_yaml() noexcept {
//   try {
//     std::ofstream ofs{m_config_file};
//     if (!ofs.is_open())
//       throw;//todo:<exception>
//     ofs << convert_to_yaml();
//     ofs.close();
//   } catch (...) {
//     //todo: <exception>
//   }
// }
// void ConfigManager::save_to_json() noexcept {
// }
// 
// YAML::Node ConfigManager::convert_to_yaml() {
//   return YAML::Node();
// }
// 
// void ConfigManager::recursive_load_yaml_func(const YAML::Node &node, String current_node_name) {
//   if (!current_node_name.empty()) {
//     current_node_name += ".";
//   }
// 
//   std::cout << YAML::Dump(node) << std::endl;
// 
//   if (!node.IsMap()) {
//     return;
//   }
// 
//   for (auto &child_node: node) {
//     String child_name = child_node.first.Scalar();
//     if (child_name.empty())
//       continue;
//     std::transform(child_name.begin(), child_name.end(), child_name.begin(), ::tolower);
//     if (child_name.find_first_not_of("abcdefghikjlmnopqrstuvwxyz_012345678") != std::string::npos) {
//       //TODO: <log>
//       continue;
//     }
// 
//     String key = current_node_name + child_name;
//     ConfigValueBase::ptr var = get_config_base(key);
// 
//     if (!var) { continue; }
// 
//     if (child_node.second.IsScalar()) {
//       var->fromString(child_node.second.Scalar());
//     } else {
//       String val_str = YAML::Dump(child_node.second);
//       var->fromString(val_str);
//       recursive_load_yaml_func(child_node.second, key);
//     }
//   }
// }



MYLIB_SPACE_END
