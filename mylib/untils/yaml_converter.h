//
// Created by HanHaocheng on 2024/6/30.
//

#ifndef MYLIB_SLN_YAML_CONVERTER_H
#define MYLIB_SLN_YAML_CONVERTER_H
#include <forward_list>
#include <list>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "string_converter.h"

MYLIB_SPACE_BEGIN

template<typename Ty>
class YamlConverter : private StringConverter<Ty> {
public:
  using converte_type = Ty;

  converte_type fromString(const String &yaml) override {
    return std::move(StringConverter<Ty>::fromString(yaml));
  }

  String toString(const converte_type &ty) override {
    return StringConverter<Ty>::toString(ty);
  }
};

template<typename Ty>
class YamlConverter<std::vector<Ty>> {
public:
  using converte_type = std::vector<Ty>;

  converte_type fromString(const String &yaml) {
    YAML::Node node = YAML::Load(yaml);
    converte_type res;
    for (const auto &iter: node) {
      res.emplace_back(YamlConverter<Ty>{}.fromString(YAML::Dump(iter)));
    }
    return res;
  }

  String toString(const converte_type &ty) {
    YAML::Node node;
    for (const auto &ele: ty) {
      node.push_back(YAML::Load(YamlConverter<Ty>{}.toString(ele)));
    }

    return YAML::Dump(node);
  }
};

template<typename Ty>
class YamlConverter<std::list<Ty>> {
public:
  using converte_type = std::list<Ty>;

  converte_type fromString(const String &yaml) {
    YAML::Node node = YAML::Load(yaml);
    converte_type res;
    for (const auto &iter: node) {
      res.emplace_back(YamlConverter<Ty>{}.fromString(YAML::Dump(iter)));
    }

    return res;
  }

  String toString(const converte_type &ty) {
    YAML::Node node;
    for (auto &ele: ty) {
      node.push_back(YamlConverter<Ty>{}.toString(ele));
    }
    return YAML::Dump(node);
  }
};

template<typename Ty>
class YamlConverter<std::forward_list<Ty>> {
public:
  using converte_type = std::forward_list<Ty>;
  using value_type = typename converte_type::value_type;

  converte_type fromString(const String &yaml) {
    YAML::Node node = YAML::Load(yaml);
    converte_type res;
    for (const auto &iter: node) {
      res.emplace_front(YamlConverter<Ty>{}.fromString(YAML::Dump(iter)));
    }
    return res;
  }

  String toString(const converte_type &ty) {
    YAML::Node node;
    for (const auto &ele: ty) {
      node.push_back(YamlConverter<value_type>{}.toString(ele));
    }
    return YAML::Dump(node);
  }
};

template<typename Ty>
class YamlConverter<std::set<Ty>> {
public:
  using converte_type = std::set<Ty>;
  using value_type = typename converte_type::value_type;

  converte_type fromString(const String &yaml) {
    YAML::Node node = YAML::Load(yaml);
    converte_type res;
    for (const auto &iter: node) {
      res.emplace(YamlConverter<value_type>{}.fromString(YAML::Dump(iter)));
    }
    return res;
  }

  String toString(const converte_type &ty) {
    YAML::Node node;
    for (const auto &ele: ty) {
      node.push_back(YamlConverter<value_type>{}.toString(ele));
    }
    return YAML::Dump(node);
  }
};

template<typename Ty>
class YamlConverter<std::unordered_set<Ty>> {
public:
  using converte_type = std::unordered_set<Ty>;
  using value_type = typename converte_type::value_type;

  converte_type fromString(const String &yaml) {
    YAML::Node node = YAML::Load(yaml);
    converte_type res;
    for (const auto &iter: node) {
      res.emplace(YamlConverter<value_type>{}.fromString(YAML::Dump(iter)));
    }
    return res;
  }

  String toString(const converte_type &ty) {
    YAML::Node node;
    for (const auto &ele: ty) {
      node.push_back(YamlConverter<value_type>{}.toString(ele));
    }
    return YAML::Dump(node);
  }
};

template<typename Ty>
class YamlConverter<std::map<String, Ty>> {
public:
  using converte_type = std::map<String, Ty>;
  using value_type = typename converte_type::value_type;

  converte_type fromString(const String &yaml) {
    YAML::Node node = YAML::Load(yaml);
    converte_type res;
    for (const auto &iter: node) {
      String key = iter.first.IsScalar() ? iter.first.Scalar() : YAML::Dump(iter.first);
      value_type value = YamlConverter<value_type>{}.fromString(YAML::Dump(iter.second));
      res.emplace(key, value);
    }

    return res;
  }

  String toString(const converte_type &ty) {
    YAML::Node node;
    for (const auto &ele: ty) {
      node[ele.first] = YamlConverter<value_type>{}.toString(ele.second);
    }
    return YAML::Dump(node);
  }
};

template<typename Ty>
class YamlConverter<std::unordered_map<String, Ty>> {
public:
  using converte_type = std::unordered_map<String, Ty>;
  using value_type = typename converte_type::value_type;

  converte_type fromString(const String &yaml) {
    YAML::Node node = YAML::Load(yaml);
    converte_type res;
    for (const auto &iter: node) {
      String key = iter.first.IsScalar() ? iter.first.Scalar() : YAML::Dump(iter.first);
      value_type value = YamlConverter<value_type>{}.fromString(YAML::Dump(iter.second));
      res.emplace(key, value);
    }

    return res;
  }

  String toString(const converte_type &ty) {
    YAML::Node node;
    for (const auto &ele: ty) {
      node[ele.first] = YamlConverter<value_type>{}.toString(ele.second);
    }
    return YAML::Dump(node);
  }
};

MYLIB_SPACE_END

#endif//MYLIB_SLN_YAML_CONVERTER_H
