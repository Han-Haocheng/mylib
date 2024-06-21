#ifndef MYLIB_CONVERTER_H
#define MYLIB_CONVERTER_H
#include "../core/mylib_def.h"
#include "converter.h"

#include <boost/lexical_cast.hpp>

#include <forward_list>
#include <list>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

MYLIB_SPACE_BEGIN

template<typename FromType, typename ToType>
class Converter {
public:
  using from_type = FromType;
  using to_type = ToType;

  static_assert(!std::is_same<from_type, to_type>::value, "错误：模板参数转换类型相同，无需转换");

  const to_type &cast(const from_type &from) { return boost::lexical_cast<to_type>(from); }
};

template<typename Ty>
class StringConverter : private Converter<Ty, String> {
public:
  using converte_type = Ty;

  virtual String toString(const converte_type &ty) { return Converter<converte_type, String>{}.cast(ty); }
  virtual converte_type fromString(const String &ty) { return Converter<String, converte_type>{}.cast(ty); }
};

template<>
class StringConverter<String> {
public:
  using converte_type = String;

  virtual String toString(const String &ty) { return ty; }
  virtual String fromString(const String &ty) { return ty; }
};

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

//using U = YamlConverter<int>;
template<typename U>
struct IsStringConverter {

  template<typename T, typename V = void>
  struct hasToString : std::false_type {};
  template<typename T, typename V = void>
  struct hasFromString : std::false_type {};

  template<typename T>
  struct hasToString<T, std::void_t<decltype(std::declval<T>().toString(std::declval<const decltype(std::declval<T>().fromString(std::declval<const String &>())) &>()))>> : std::true_type {};

  template<typename T>
  struct hasFromString<T, std::void_t<decltype(std::declval<T>().fromString(std::declval<const String &>()))>> : std::true_type {};

  //using typ = decltype(std::declval<U>().fromString(std::declval<const String &>()));

  using type = std::integral_constant<bool, hasToString<U>::value && hasFromString<U>::value>;
  static constexpr bool value = hasToString<U>::value && hasFromString<U>::value;
};

MYLIB_SPACE_END
#endif//MYLIB_CONVERTER_H
