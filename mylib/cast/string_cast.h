//
// Created by HanHaocheng on 2024/6/30.
//

#ifndef MYLIB_SLN_STRING_CONVERTER_H
#define MYLIB_SLN_STRING_CONVERTER_H

#include "cast.h"

MYLIB_BEGIN

template<typename Ty>
class StringCast {
public:
  using converte_type = Ty;

  static String toString(const converte_type &ty) {
    return Cast<converte_type, String>::cast(ty);
  }

  static converte_type fromString(const String &ty) {
    return Cast<String, converte_type>::cast(ty);
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
class is_StringCast {

  template<typename T>
  using FromString = decltype(T::fromString(std::declval<const String &>()));

  template<typename T>
  using ToString = decltype(T::toString(std::declval<const FromString<T> &>()));

  template<typename T, typename V = void>
  struct hasToString : std::false_type {};
  template<typename T, typename V = void>
  struct hasFromString : std::false_type {};

  template<typename T>
  struct hasToString<T, std::void_t<ToString<T>>> : std::true_type {};

  template<typename T>
  struct hasFromString<T, std::void_t<FromString<T>>> : std::true_type {};

  //using typ = decltype(std::declval<U>().fromString(std::declval<const String &>()));
public:
  using type = std::integral_constant<bool, hasToString<U>::value && hasFromString<U>::value>;
  static constexpr bool value = hasToString<U>::value && hasFromString<U>::value;
};

template<typename Ty>
class SStringConverter {
public:
  static String toString(String);
  static Ty fromString(String);
};

MYLIB_END
#endif//MYLIB_SLN_STRING_CONVERTER_H
