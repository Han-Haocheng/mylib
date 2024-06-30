#ifndef MYLIB_DEF_H_
#define MYLIB_DEF_H_
#include <cinttypes>
#include <functional>
#include <sstream>
#include <string>

#include <boost/lexical_cast.hpp>
#include <json/json.h>
#include <yaml-cpp/yaml.h>

#define MYLIB_SPACE mylib
#define MYLIB_SPACE_BEGIN namespace MYLIB_SPACE {
#define MYLIB_SPACE_END }

//#define MYLIB_STATIC static
//#define MYLIB_THREAD thread_local
//#define MYLIB_CONST const;
//#define MYLIB_CONST_STATIC static const
// #define MYLIB_STATIC_THREAD MYLIB_STATIC MYLIB_THREAD

#ifdef _MSC_VER
#define MYLIB_MSVC _MSC_VER
#define MYLIB_FULL_FUNC_NAME __##FUNCTION__
#elif __GNUC__
#define MYLIB_GUN __GNUC__
#define MYLIB_FUNC_NAME __PRETTY_##FUNCTION__
#elif Clang
#define MYLIB_CLANG
#endif// _MSC_VER

#define MYLIB_CLASS_NAME \
  MYLIB_SPACE::FormatClassName(MYLIB_##FULL_FUNC_NAME)

#define MYLIB_LINE (unsigned int) __##LINE__
#define MYLIB_FILE_NAME __##FILE__
#define MYLIB_FUNC_NAME __##func__

#define MYLIB_CURRENT_SOURCE_INFO \
  MYLIB_##SPACE::SourceInfo { MYLIB_##LINE, MYLIB_##FILE_NAME, MYLIB_##FUNC_NAME, MYLIB_##CLASS_NAME }

MYLIB_SPACE_BEGIN

using int8 = signed char;
using int16 = signed short int;
using int32 = signed int;
using int64 = signed long long int;

using uint8 = unsigned char;
using uint16 = unsigned short int;
using uint32 = unsigned int;
using uint64 = unsigned long long int;

using float32 = float;
using float64 = double;
using CString = const char *;

using cid_t = uint32;//协程id类型
using size_t = uint64;
using weight_t = float32;

using String = std::string;
using SString = std::stringstream;

using time_t = ::time_t;//时间戳类型
using tid_t = unsigned long;

struct SourceInfo {
  uint32 line;
  String file_name;
  String func_name;
  String class_name;
};

String FormatClassName(CString fullFuncName);

MYLIB_SPACE_END

#endif// MYLIB_DEF_H_