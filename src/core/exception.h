//
// Created by HanHaocheng on 2024/6/15.
//

#ifndef MYLIB_SLN_EXCEPTION_H
#define MYLIB_SLN_EXCEPTION_H

#include "algorithm.h"
#include "define.h"


#include <utility>
#include <vector>

//#include "../logger/log.h"

#define MYLIB_LINE (unsigned int) (__LINE__)
#define MYLIB_FILE_NAME (const char *) (__FILE__)
#define MYLIB_CLASS_NAME (MYLIB_SPACE::FormatClassName(MYLIB_FUNC_FULL_NAME))
#define MYLIB_FUNC_NAME (const char *) (__func__)


#define MYLIB_DEBUG_LOCATION_INFO \
	MYLIB_SPACE::DebugLocationInfo { (MYLIB_LINE), (MYLIB_FILE_NAME), (MYLIB_FUNC_NAME), (MYLIB_CLASS_NAME) }

#define MYLIB_DEBUG_TASK_INFO \
	MYLIB_SPACE::DebugTaskInfo { MYLIB_SPACE::CURRENT_THREAD_ID, MYLIB_SPACE::CURRENT_COROUTINE_ID, MYLIB_SPACE::CURRENT_THREAD_NAME, MYLIB_SPACE::CURRENT_COROUTINE_NAME }

#define MYLIB_THROW(what) \
	throw MYLIB_SPACE::Exception((MYLIB_DEBUG_LOCATION_INFO), (what))

#if MYLIB_DEBUG
#define MYLIB_ASSERT(condition, what) \
	if (!condition) MYLIB_SPACE::Exception((MYLIB_DEBUG_LOCATION_INFO), (what)).show(), exit(-1);
#elif MYLIB_RELEASE
#define MYLIB_ASSERT(a, what) ;
#endif

#define MYLIB_TRY_THROW(condition, what) \
	if (condition) MYLIB_THROW(what)
#define MYLIB_TRY_CATCH_BEGIN try

#define MYLIB_CATCH(type, body) \
	catch (const type &e) {     \
		body                    \
	}

#define MYLIB_TRY_CATCH_END(error_type)                  \
	catch (const MYLIB_SPACE::Exception &e) {            \
		e.show();                                        \
	}                                                    \
	catch (const std::exception &e) {                    \
		MYLIB_THROW(String(error_type ": ") + e.what()); \
	}                                                    \
	catch (...) {                                        \
		MYLIB_THROW(error_type ": unknow");              \
	}


MYLIB_BEGIN

struct DebugLocationInfo {
	uint32 line;	  //行数
	String file_name; //文件名
	String func_name; //函数名
	String class_name;//类型名
};

struct DebugTaskInfo {
	tid_t thread_id;	  //线程id
	cid_t coroutine_id;	  //协程id
	String thread_name;	  //线程名
	String coroutine_name;//协程名
};

struct FunctionStackInfo {
	String name;
	uint64 addr;

	FunctionStackInfo() : addr(0) {}
	FunctionStackInfo(String name, const uint64 addr) : name(std::move(name)), addr(addr) {}
};

class Exception : public std::exception {
public:
	/**
   * @brief 捕获堆栈信息
   * @param out 输出的堆栈信息
   * @param skip 跳过的步数
   * @param maxStackSize 堆栈最大大小
   * @return 是否成功
   */
	static bool CaptureStackBack(std::vector<FunctionStackInfo> &out, size_t skip, size_t maxStackSize = 64);

	explicit Exception(DebugLocationInfo info, String what);
	~Exception() noexcept override;

	Exception(Exception &&e) noexcept				  = default;
	Exception(const Exception &e) noexcept			  = default;

	Exception &operator=(Exception &&e) noexcept	  = default;
	Exception &operator=(const Exception &e) noexcept = default;

	[[nodiscard]] const std::vector<FunctionStackInfo> &stackBack() const { return m_stack_back; }
	[[nodiscard]] const DebugLocationInfo &info() const { return m_info; }
	[[nodiscard]] CString what() const noexcept override;
	[[nodiscard]] String what_string() const noexcept;

	void show() const;

private:
	String m_what;
	DebugLocationInfo m_info;
	std::vector<FunctionStackInfo> m_stack_back;
};

MYLIB_END
#endif//MYLIB_SLN_EXCEPTION_H
