//
// Created by HanHaocheng on 2024/6/7.
//
#include "include.h"
#include <string>

TEST(ConfigManager, yaml_base) {
  // 创建一个 YAML 节点
  YAML::Node node = YAML::Load("[2, 3, 5, 7, 11]");
  switch (node.Type()) {
    case YAML::NodeType::Undefined:
      std::cout << "Undefined" << std::endl;
      break;
    case YAML::NodeType::Null:
      std::cout << "Null" << std::endl;
      break;
    case YAML::NodeType::Scalar:
      std::cout << "Scalar - " << YAML::Dump(node) << std::endl;
      break;
    case YAML::NodeType::Sequence:
      std::cout << node.size() << std::endl;
      for (size_t i = 0; i < node.size(); ++i) {
        std::cout << "Sequence - " << YAML::Dump(node) << std::endl;
      }

      break;
    case YAML::NodeType::Map:
      std::cout << "Map - " << YAML::Dump(node) << std::endl;
      break;
  }

  node.IsDefined();
  for (auto child: node) {
    switch (child.Type()) {
      case YAML::NodeType::Undefined:
        std::cout << "Undefined" << std::endl;
        break;
      case YAML::NodeType::Null:
        std::cout << "Null" << std::endl;
        break;
      case YAML::NodeType::Scalar:
        std::cout << "Scalar - " << YAML::Dump(child) << std::endl;
        break;
      case YAML::NodeType::Sequence:
        std::cout << "Sequence - " << YAML::Dump(child) << std::endl;
        break;
      case YAML::NodeType::Map:
        std::cout << "Map - " << YAML::Dump(child) << std::endl;
        break;
    }
  }

  // 使用 as<std::string>() 获取节点值的字符串表示
  auto value = node.as<std::vector<int>>();
  std::cout << "The value as string: " << value[0] << std::endl;

  // 使用 YAML::Dump 获取整个节点树的 YAML 格式字符串
  std::string dump = YAML::Dump(node);
  std::cout << "The node as YAML:\n"
            << dump << std::endl;
}

TEST(ConfigManager, base) {
  auto config_man = std::make_shared<MYLIB_SPACE::Configurator>();
  try {
    config_man->setConfig<int>("int1", 0x1234, "int 1");
    config_man->setConfig<std::vector<int>>("vector1", {1234, 123, 123, 12312, 31}, "vector1 1");
    //std::enable_if<mylib::CheckStringConverter<mylib::YamlConverter<int>>::value>::type;
    mylib::ConfigValue<int>::ptr t = config_man->getConfig<int>("int1");
    mylib::ConfigValue<std::vector<int>>::ptr t2 = config_man->getConfig<std::vector<int>>("vector1");

    std::cout << "int1:" << t->as() << std::endl;
    for (const auto &item: t2->as()) {
      std::cout << "vector1 : " << item << std::endl;
    }
  } catch (...) {
    std::cout << "error" << std::endl;
  }
}

TEST(ConfigManager, foreach_config) {
  auto config_man = std::make_shared<MYLIB_SPACE::Configurator>();

  config_man->setConfig<int>("int1", 1, "int 1");
  config_man->setConfig<int>("int2", 2, "int 1");
  config_man->setConfig<int>("int3", 3, "int 1");
  config_man->setConfig<int>("int4", 4, "int 1");
  config_man->setConfig<std::vector<int>>("vec", {1, 2, 3, 4}, "vec");

  for (const auto &pair: config_man->getAllConfig()) {
    std::string rt = pair.second->toString();
    std::cout << pair.first << ":"
              << rt << std::endl;
  }
}

TEST(ConfigManager, load_file) {
  /*
root:
  vec_1:
    - 1
    - 2
    - 3
  map_1:
    ele_1: 1
    ele_2: 2
*/
  auto config_man = std::make_shared<MYLIB_SPACE::Configurator>();
  config_man->setConfig<std::vector<int>>("root_1.vec_1",{0,0,0},"root.vec_1");
  config_man->setConfig<std::map<std::string, int>>("root_1.map_1",{{"ele_1",0}, {"ele_2",0}},"root.map_1");
  config_man->setConfig<std::vector<std::map<std::string, int>>>("root_1.vec_2",{
                                                                                   std::map<std::string, int>{{"ele_1", 0}, {"ele_2", 0}},
                                                                                   std::map<std::string, int>{{"ele_3", 0}, {"ele_4", 0}}
                                                                               },"root.vec_2");
  MYLIB_LOG_DEBUG(MYLIB_ROOT_LOGGER)<<">>>>>>>>>after loading file";

  for (const auto &pair: config_man->getAllConfig()) {
    std::string rt = pair.second->toString();
    std::cout << pair.first << ":"
              << rt << std::endl;
  }

  config_man->loadFile(R"(H:\Documents\program\c++\mylib\tests\test.yaml)");
  MYLIB_LOG_DEBUG(MYLIB_ROOT_LOGGER)<<">>>>>>>>>before loading file";
  for (const auto &pair: config_man->getAllConfig()) {
    std::string rt = pair.second->toString();
    std::cout << pair.first << ":"
              << rt << std::endl;
  }
}