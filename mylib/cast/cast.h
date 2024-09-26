#ifndef MYLIB_CONVERTER_H
#define MYLIB_CONVERTER_H
#include "../base/mylib_def.h"

#include <boost/lexical_cast.hpp>

MYLIB_BEGIN

template<typename FromType, typename ToType, typename = void>
class Cast {
public:
  using from_type = FromType;
  using to_type = ToType;

  static to_type cast(const from_type &from) { return boost::lexical_cast<to_type>(from); }
};

template<typename FromType, typename ToType>
class Cast<FromType, ToType, std::enable_if_t<std::is_same<FromType, ToType>::value>> {
public:
  using from_type = FromType;
  using to_type = ToType;

  static to_type cast(const from_type &from) { return from; }
};

//template<class FromType, class ToType>
//ToType type_cast(const FromType &from) { return boost::lexical_cast<ToType>(from); }

// template<class FromType, class ToType>
//struct type_cast {
//  using from_type = int;
//  using to_type = String;
//
//  static to_type operator()(const from_type&from) {return boost::lexical_cast<to_type>(from); }
//};

MYLIB_END
#endif//MYLIB_CONVERTER_H
