//
// Created by HanHaocheng on 2024/6/15.
//

#include "exception.h"

#include <utility>

#ifdef MYLIB_WINDOWS
#include <DbgHelp.h>
#pragma comment(lib, "DbgHelp.lib")
#elif MYLIB_LINUX

#endif//MYLIB_WINDOWS

MYLIB_BEGIN

bool Exception::CaptureStackBack(std::vector<FunctionStackInfo> &out, size_t skip, size_t maxStackSize) {
#ifdef MYLIB_WINDOWS
	PVOID *pStackTrace	 = nullptr;
	PSYMBOL_INFO pSymbol = {};
	USHORT frames		 = {};
	HANDLE hCurProcess	 = {};

	auto dMaxStackSize	 = static_cast<DWORD>(maxStackSize);

	hCurProcess			 = GetCurrentProcess();
	SymInitialize(hCurProcess, nullptr, TRUE);

	pStackTrace = static_cast<PVOID *>(malloc(dMaxStackSize * sizeof(PVOID)));
	if (!pStackTrace) {
		SymCleanup(hCurProcess);
		return false;
	}

	pSymbol = static_cast<PSYMBOL_INFO>(malloc(sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(decltype(pSymbol->Name))));

	if (!pSymbol) {
		free(pStackTrace);
		SymCleanup(hCurProcess);
		return false;
	}

	frames				  = CaptureStackBackTrace(0, dMaxStackSize, pStackTrace, nullptr);
	pSymbol->MaxNameLen	  = MAX_SYM_NAME;
	pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);

	for (size_t i = skip; i < frames; i++) {
		DWORD64 displacement = 0;

		if (!SymFromAddr(hCurProcess, reinterpret_cast<DWORD64>(pStackTrace[i]), &displacement, pSymbol)) {
			out.emplace_back("unknown function", 0);
			continue;
		}

		String strFuncName = pSymbol->Name;
		out.emplace_back(strFuncName, pSymbol->Address);

		if (strFuncName == "main") break;
	}

	free(pStackTrace);
	free(pSymbol);
	SymCleanup(hCurProcess);
	return true;
#elif MYLIB_LINUX
	//todo
#endif//MYLIB_WINDOWS
}

Exception::Exception(DebugLocationInfo info, String what)
#ifdef MYLIB_MSVC
	: std::exception(what.c_str()),
#elif MYLIB_GUN
	: std::exception(),
	  m_what(std::move(what)),
#endif
	  m_info(std::move(info)),
	  m_stack_back() {
	CaptureStackBack(m_stack_back, 1);
	SString ss;
	ss << m_what << "\n";
	for (const auto &item: m_stack_back) {
		ss << "\tat (" << item.addr << ") " << item.name << "\n";
	}
	m_what.clear();
	m_what = ss.str();
}

Exception::~Exception() noexcept = default;

void Exception::show() const {
	printf("%s", m_what.c_str());
}

CString Exception::what() const noexcept {
	return m_what.c_str();
}

String Exception::what_string() const noexcept {
	return m_what;
}

MYLIB_END
