#ifndef MYLIB_CONVERTER_H
#define MYLIB_CONVERTER_H
#include "mylib_def.h"
#include <forward_list>
#include <list>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <boost/lexical_cast.hpp>

#include "converter.h"
MYLIB_SPACE_BEGIN

template<typename FromType, typename ToType>
class Converter {
public:
  using from_type = FromType;
  using to_type = ToType;

  static_assert(!std::is_same<from_type, to_type>::value, "错误：模板参数转换类型相同，无需转换");

  const to_type &cast(const from_type &from) {
    return boost::lexical_cast<to_type>(from);
  }
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
  Ty fromString(const String &yaml) override {
    return std::move(StringConverter<Ty>::fromString(yaml));
  }

  String toString(const Ty &ty) override {
    return StringConverter<Ty>::toString(ty);
  }
};

template<typename Ty>
class YamlConverter<std::vector<Ty>> {
public:
  using CastTy = std::vector<Ty>;
  CastTy fromString(const String &yaml) {
    YAML::Node node = YAML::Load(yaml);
    CastTy castVal;
    for (const auto &iter: node) {
      castVal.emplace_back(YamlConverter<Ty>{}.fromString(YAML::Dump(iter)));
    }
    return castVal;
  }

  String toString(const CastTy &ty) {
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
  using CastTy = std::list<Ty>;

  CastTy fromString(const String &yaml) {
    YAML::Node node = YAML::Load(yaml);
    CastTy castVal;
    for (const auto &iter: node) {
      castVal.emplace_back(YamlConverter<Ty>{}.fromString(YAML::Dump(iter)));
    }

    return castVal;
  }

  String toString(const CastTy &ty) {
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
  using CastTy = std::forward_list<Ty>;

  CastTy fromString(const String &yaml) {
    YAML::Node node = YAML::Load(yaml);
    CastTy castVal;
    for (const auto &iter: node) {
      castVal.emplace_front(YamlConverter<Ty>{}.fromString(YAML::Dump(iter)));
    }

    return castVal;
  }

  String toString(const CastTy &ty) {
    YAML::Node node;
    for (const auto &ele: ty) {
      node.push_back(YamlConverter<Ty>{}.toString(ele));
    }
    return YAML::Dump(node);
  }
};

template<typename Ty>
class YamlConverter<std::set<Ty>> {
public:
  using CastTy = std::set<Ty>;

  CastTy fromString(const String &yaml) {
    YAML::Node node = YAML::Load(yaml);
    CastTy castVal;
    for (const auto &iter: node) {
      castVal.emplace(YamlConverter<Ty>{}.fromString(YAML::Dump(iter)));
    }

    return castVal;
  }

  String toString(const CastTy &ty) {
    YAML::Node node;
    for (const auto &ele: ty) {
      node.push_back(YamlConverter<Ty>{}.toString(ele));
    }
    return YAML::Dump(node);
  }
};

template<typename Ty>
class YamlConverter<std::unordered_set<Ty>> {
public:
  using CastTy = std::unordered_set<Ty>;

  CastTy fromString(const String &yaml) {
    YAML::Node node = YAML::Load(yaml);
    CastTy castVal;
    for (const auto &iter: node) {
      castVal.emplace(YamlConverter<Ty>{}.fromString(YAML::Dump(iter)));
    }

    return castVal;
  }

  String toString(const CastTy &ty) {
    YAML::Node node;
    for (const auto &ele: ty) {
      node.push_back(YamlConverter<Ty>{}.toString(ele));
    }
    return YAML::Dump(node);
  }
};

template<typename Ty>
class YamlConverter<std::map<String, Ty>> {
public:
  using CastTy = std::map<String, Ty>;

  CastTy fromString(const String &yaml) {
    YAML::Node node = YAML::Load(yaml);
    CastTy castVal;
    for (const auto &iter: node) {
      std::pair<String, Ty> a{iter.first.IsScalar() ? iter.first.Scalar() : YAML::Dump(iter.first),
                              YamlConverter<Ty>{}.fromString(YAML::Dump(iter.second))};

      castVal.emplace(a);
    }

    return castVal;
  }

  String toString(const CastTy &ty) {
    YAML::Node node;
    for (const auto &ele: ty) {
      node[ele.first] = YamlConverter<Ty>{}.toString(ele.second);
    }
    return YAML::Dump(node);
  }
};

template<typename Ty>
class YamlConverter<std::unordered_map<String, Ty>> {
public:
  using CastTy = std::unordered_map<String, Ty>;

  CastTy fromString(const String &yaml) {
    YAML::Node node = YAML::Load(yaml);
    CastTy castVal;
    for (const auto &iter: node) {
      std::pair<String, Ty> a{iter.first.IsScalar() ? iter.first.Scalar() : YAML::Dump(iter.first),
                              YamlConverter<Ty>{}.fromString(YAML::Dump(iter.second))};

      castVal.emplace(a);
    }

    return castVal;
  }

  String toString(const CastTy &ty) {
    YAML::Node node;
    for (const auto &ele: ty) {
      node[ele.first] = YamlConverter<Ty>{}.toString(ele.second);
    }
    return YAML::Dump(node);
  }
};

// using U = YamlConverter<String>;
template<typename U>
struct IsStringConverter {

  template<typename>
  using void_t = void;

  template<typename T, typename V = void>
  struct hasToString
      : std::false_type {};

  template<typename T>
  struct hasToString<T, void_t<decltype(T::toString)>>
      : std::true_type {};

  template<typename T, typename V = void>
  struct hasFromString
      : std::false_type {};

  template<typename T>
  struct hasFromString<T, void_t<decltype(T::fromString)>>
      : std::true_type {};

  static constexpr bool value = hasToString<U>::value && hasFromString<U>::value;
};

MYLIB_SPACE_END
#endif//MYLIB_CONVERTER_H
