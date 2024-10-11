#ifndef MYLIB_DEF_H_
#define MYLIB_DEF_H_
#include <cinttypes>
#include <sstream>
#include <string>

#define MYLIB_SPACE mylib
#define MYLIB_BEGIN namespace MYLIB_SPACE {
#define MYLIB_END }

//#define MYLIB_STATIC static
//#define MYLIB_THREAD thread_local
//#define MYLIB_CONST const;
//#define MYLIB_CONST_STATIC static const
// #define MYLIB_STATIC_THREAD MYLIB_STATIC MYLIB_THREAD


// 用于判断编译器类型
#ifdef _MSC_VER 
#define MYLIB_MSVC _MSC_VER
#elif __GNUC__ 
#define MYLIB_GUN __GNUC__
#elif Clang 
#define MYLIB_CLANG
#endif// _MSC_VER

// 用于判断系统类型
#ifdef _WIN32
#define MYLIB_WINDOWS
#elif linux
#define MYLIB_LINUX
#endif// _WIN32

// 用于判断包含的包
#define MYLIB_LOG_LIB       0x1
#define MYLIB_CONFIG_LIB    0x2
#define MYLIB_EXCEPTION_LIB 0x4
#define MYLIB_THREAD_LIB    0x8
#define MYLIB_SCHEDULE_LIB  0x10
#define USE_
#ifdef _DEBUG
#define MYLIB_DEBUG
#else
#define MYLIB_RELEASE
#endif// MYLIB


MYLIB_BEGIN

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



MYLIB_END

#endif// MYLIB_DEF_H_