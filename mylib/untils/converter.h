#ifndef MYLIB_CONVERTER_H
#define MYLIB_CONVERTER_H
#include "../core/mylib_def.h"

MYLIB_SPACE_BEGIN

template<typename FromType, typename ToType, typename = void>
class Converter {
public:
  using from_type = FromType;
  using to_type = ToType;

  to_type cast(const from_type &from) { return boost::lexical_cast<to_type>(from); }
};

template<typename FromType, typename ToType>
class Converter<FromType, ToType, std::enable_if_t<std::is_same<FromType, ToType>::value>> {
public:
  using from_type = FromType;
  using to_type = ToType;

  to_type cast(const from_type &from) { return from; }
};

MYLIB_SPACE_END
#endif//MYLIB_CONVERTER_H
