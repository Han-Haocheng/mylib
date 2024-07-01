//
// Created by HanHaocheng on 2024/6/7.
//
#include "include.h"


TEST(ConfigManager, base) {
  auto config_man = MYLIB_SPACE::ConfigManager::GetInstance();
  try {
    config_man->setConfig<int>("int1", 0x1234, "int 1");
    config_man->setConfig<std::vector<int>>("vector1", {1234, 123, 123, 12312, 31}, "vector1 1");
    //std::enable_if<mylib::IsStringConverter<mylib::YamlConverter<int>>::value>::type;
    mylib::ConfigValue<int>::ptr t = config_man->getConfig<int>("int1");
    mylib::ConfigValue<std::vector<int>>::ptr t2 = config_man->getConfig<std::vector<int>>("vector1");

    std::cout << "int1:" << t->as() << std::endl;
    for (const auto &item: t2->as()) {
      std::cout << "vector1:-" << item << std::endl;
    }
  } catch (...) {
    std::cout << "error" << std::endl;
  }
}