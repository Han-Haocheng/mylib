#include "config_manager.h"

#include <fstream>
#include <stack>
#include <utility>

#include <json/json.h>

MYLIB_BEGIN

bool Configurator::loadFile(const String &filename) {
  try {
    // 查找最后一个点（`.`）字符的位置
    size_t dotpos = filename.find_last_of('.');

    // 如果点不存在，或者点是字符串的最后一个字符，则返回空字符串
    if (dotpos == std::string::npos || dotpos == filename.length() - 1) { return false; }

    // 获取点之后的所有字符，即文件扩展名
    String ftype = filename.substr(dotpos + 1);

    if (ftype == "yml" || ftype == "yaml") {
      const YAML::Node node = YAML::LoadFile(filename);
      analysis_yaml(node);
      return true;
    }

    if (ftype == "json" || ftype == "jsn") {
      Json::Reader reader;
      analysis_json(filename);
      return true;
    }
  } catch (const YAML::BadFile &e) {

  }
  return false;
}

bool Configurator::saveFile(const String &file, FileType file_type) {
  if (file.empty()) { return false; }
  std::ofstream ofs{file, std::ios::out};
  if (!ofs.is_open()) {
    //todo error
    return false;
  }
  switch (file_type) {
    case FT_YAML:
      conversion_yaml(ofs);
      return true;
    case FT_JSON:
      // conversion_json(ofs);
      ofs << "";
      return true;
  }
  return false;
}

bool Configurator::delConfig(const String &key) {
  if (key.empty()) {
    return false;
  }

  const auto fd_rt = m_configurators.find(key);
  if (fd_rt == m_configurators.end()) {
    return false;
  }
  m_configurators.erase(fd_rt);
  return true;
}

void Configurator::analysis_yaml(const YAML::Node &rootNode) {
  struct AnalysisInfo {
    YAML::Node node;
    String base_key;
    AnalysisInfo();
    AnalysisInfo(const YAML::Node &node, String baseKey) : node(node), base_key(std::move(baseKey)) {}
  };
  std::stack<AnalysisInfo> node_stack;
  node_stack.emplace(rootNode, "");

  do {
    AnalysisInfo top = node_stack.top();
    node_stack.pop();

    if (top.node.IsMap()) {
      // 处理图
      for (const auto &child_node: top.node) {
        auto key = (top.base_key.empty() ? "" : top.base_key + ".") + child_node.first.as<String>();
        auto val_str = YAML::Dump(child_node.second);

        auto rt = get_config_val_base(key);
        node_stack.emplace(child_node.second, key);
        if (rt)
          rt->fromString(val_str);
      }
    }
  } while (!node_stack.empty());
}

void Configurator::analysis_json(const String &json) {}

ConfigValueBase::ptr Configurator::get_config_val_base(const String &key) {
  auto fd_rt = m_configurators.find(key);
  if (fd_rt == m_configurators.end()) { return nullptr; }
  return fd_rt->second;
}

bool Configurator::conversion_yaml(std::ofstream &ofs) {
  YAML::Node out;
  for (const auto &[key, val]: m_configurators) { out[key] = YAML::Load(val->toString()); }
  ofs << out;
  return true;
}

Configurator::~Configurator() {}

Configurator::Configurator() : m_configurators() {}

MYLIB_END
