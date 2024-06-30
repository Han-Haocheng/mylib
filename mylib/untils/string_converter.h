//
// Created by HanHaocheng on 2024/6/30.
//

#ifndef MYLIB_SLN_STRING_CONVERTER_H
#define MYLIB_SLN_STRING_CONVERTER_H

#include "converter.h"

MYLIB_SPACE_BEGIN

template<typename Ty>
class StringConverter {
public:
  using converte_type = Ty;

  virtual String toString(const converte_type &ty) {
    return Converter<converte_type, String>{}.cast(ty);
  }
  virtual converte_type fromString(const String &ty) {
    return Converter<String, converte_type>{}.cast(ty);
  }
};

//template<typename Ty>
//class StringConverter<Ty> {
//public:
//  using converte_type = Ty;
//
//  virtual String toString(const converte_type &ty) { return ty; }
//  virtual converte_type fromString(const String &ty) { return ty; }
//};


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
#endif//MYLIB_SLN_STRING_CONVERTER_H
