#ifndef MYLIB_DEF_H_
#define MYLIB_DEF_H_
#include <cinttypes>
#include <memory>
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
#define MYLIB_WINDOWS _WIN32
#elif linux
#define MYLIB_LINUX linux
#endif// _WIN32

// 用于判断包含的包
#define MYLIB_LOG_LIB 0x1
#define MYLIB_CONFIG_LIB 0x2
#define MYLIB_EXCEPTION_LIB 0x4
#define MYLIB_THREAD_LIB 0x8
#define MYLIB_SCHEDULE_LIB 0x10


#if _DEBUG
#define MYLIB_DEBUG 0x1
#else
#define MYLIB_RELEASE 0x1
#endif// _DEBUG

#define MYLIB_CONSTEXPR_TYPE constexpr static auto

MYLIB_BEGIN

using int8	   = signed char;
using int16	   = signed short int;
using int32	   = signed int;
using int64	   = signed long long int;

using uint8	   = unsigned char;
using uint16   = unsigned short int;
using uint32   = unsigned int;
using uint64   = unsigned long long int;

using float32  = float;
using float64  = double;
using CString  = const char *;

using cid_t	   = uint32;//协程id类型
using size_t   = uint64;
using weight_t = float32;

using String   = std::string;
using SString  = std::stringstream;

using time_t   = ::time_t;//时间戳类型
using tid_t	   = unsigned long;

// tid_t thread_id;	  //线程id
// cid_t coroutine_id;	  //协程id
// String thread_name;	  //线程名
// String coroutine_name;//协程名

thread_local static tid_t CURRENT_THREAD_ID		  = 0;
thread_local static cid_t CURRENT_COROUTINE_ID	  = 0;
thread_local static String CURRENT_THREAD_NAME	  = "";
thread_local static String CURRENT_COROUTINE_NAME = "";


template<class Ty>
class Singleton {
public:
	using ptr = std::shared_ptr<Ty>;
	static ptr getInstance() {
		if (m_instance == nullptr) {
			m_instance = new Ty();
		}
	}

private:
	static Ty *m_instance;
};

MYLIB_END

#endif// MYLIB_DEF_H_
